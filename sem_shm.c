/* 
** sem_shm.c -----------
** 1. Re-write the sample program which uses Shared memory with  parent and child created out of fork()
**
** 2. Implement Semaphore for the preventing accidental over write while reading in progress
*/

#include <stdio.h>	 /* standard I/O routines.               */
#include <sys/types.h>   /* various type definitions.            */
#include <sys/ipc.h>     /* general SysV IPC structures          */
#include <sys/shm.h>	 /* semaphore functions and structs.     */
#include <sys/sem.h>	 /* shared memory functions and structs. */
#include <unistd.h>	 /* fork()                               */
#include <stdlib.h>	 /* rand()                               */
#include <signal.h>	 /* handling signals			 */
#include <string.h>	 /* handling string operations 		 */


#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int shmid;		/* shared memory id */
char *data;		/* data to be stored in the shared memory */

void 
my_handler_for_sigint(int signumber)
{
  	if (signumber == SIGINT)
  	{
   		printf("received SIGINT\n");
    		printf("Program received a CTRL-C\n"); 
       		printf("Existing ....\n");
	
    		/* detach from the segment: */
    		if (shmdt(data) == -1) 
		{
        		perror("shmdt");
        		exit(1);
    		}

    		/* remove it: */
    		if (shmctl(shmid, IPC_RMID, NULL) == -1) 
		{
        		perror("shmctl");
    	    		exit(1);
    		}
       		exit(0); 
  	}
}

void
sem_lock(int semid)
{
    	/* structure for semaphore operations.   */
    	struct sembuf sem_op;

    	/* wait on the semaphore, unless it's value is non-negative. */
    	sem_op.sem_num = 0;
    	sem_op.sem_op = -1;
    	sem_op.sem_flg = 0;
    	semop(semid, &sem_op, 1);
}

void
sem_unlock(int semid)
{
    	/* structure for semaphore operations.   */
    	struct sembuf sem_op;

    	/* signal the semaphore - increase its value by one. */
    	sem_op.sem_num = 0;
    	sem_op.sem_op = 1;   
    	sem_op.sem_flg = 0;
    	semop(semid, &sem_op, 1);
}

int main(int argc, char *argv[])
{

   	/* Registering the Signal handler */
  	if (signal(SIGINT, my_handler_for_sigint) == SIG_ERR)
	{
      		printf("\ncan't catch SIGINT\n");
	}

	union semun               /* semaphore value, for semctl().     */
	{
        	int val;
                struct semid_ds *buf;
                ushort * array;
        } sem_val;

	key_t key;
    	int semid;			/* semaphore ID */
	pid_t pid;		        /* PID of child process. */
	char datastr[100];		/* string to be stored in shared memory */


	/* make the key: */
	if ((key = ftok("sem_shm.c", 'R')) == -1) 
	{
	        perror("ftok");
	        exit(1);
	}

	/* create a semaphore */
	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    	if (semid == -1) 
	{
		perror("semget");
		exit(1);
    	}

	/* intialize the first (and single) semaphore in our set to '1'. */
	sem_val.val = 1;
    	if (semctl(semid, 0, IPC_RMID, sem_val) == -1) 
	{
        	perror("semctl");
        	exit(1);
    	}

	/* connect to (and possibly create) the segment: */
	shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
	if (shmid == -1) 
	{
	        perror("shmget");
	        exit(1);
	}

	/* attach to the segment to get a pointer to it: */
	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1)) 
	{
	        perror("shmat");
	        exit(1);
	}

	while(1)
	{
		/* fork-off a child process */
    		pid = fork();

		if (pid == -1)
		{
			perror("fork");
	    		exit(1);
		}
		else if (pid > 0)
		{
			sem_lock(semid);	/* lock the shared memory before writing */
			printf("\n**Enter a string:** \n");
			fgets(datastr, 100, stdin);
			printf("writing to segment: \"%s\"\n", datastr);
        		strncpy(data, datastr, SHM_SIZE);
			printf("Segment is written\n");
			sem_unlock(semid);	/* unlock the shared memory after writing */
		}
		else
		{
			sem_lock(semid);	/* lock the shared memory before reading */
			printf("Segment contains: %s\n", data);
			sem_unlock(semid);	/* unlock the shared memory after reading */
			exit(0);
		}
	}

    	return 0;

}
