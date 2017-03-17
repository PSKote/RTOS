/*
** server.c ---------
** this receives message from client that is connected to it.
** while execution give port number as command line argument.
*/

#include <stdio.h>	/* standard I/O routines.               */
#include <stdlib.h>	/* rand(), macros, etc.                 */
#include <errno.h>	/* defines integer value for errorno	*/
#include <string.h>	/* handling string operations 		*/
#include <sys/types.h>	/* various type definitions.            */
#include <sys/socket.h>	/* sockaddr structure			*/
#include <netinet/in.h>	/* htons()				*/
#include <arpa/inet.h>	/* defines internet operation		*/
#include <unistd.h>	/* fork()                               */
#include <signal.h>	/* handle signals			*/

#define MAXDATASIZE 100	/* maximum data length 			*/
#define BACKLOG 5	/* clients waiting is queue		*/

void 
my_handler_for_sigint(int signumber)
{
  	char ans[2];
  	if (signumber == SIGINT)
  	{
    		printf("received SIGINT\n");
    		printf("Program received a CTRL-C\n");
    		printf("Terminate Y/N : "); 
    		scanf("%s", ans);
    		if (strcmp(ans,"Y") == 0)
    		{
       			printf("Existing ....\n");
       			exit(0); 
    		}
    		else
    		{
       			printf("Continung ..\n");
    		}
  	}
}

int main(int argc, char *argv[])
{

   	/* Registering the Signal handler */
  	if (signal(SIGINT, my_handler_for_sigint) == SIG_ERR)
	{
      		printf("\ncan't catch SIGINT\n");
	}

	int s, s2, t, len;
	struct sockaddr_in server, client;
	char str[100];
	struct sigaction sa;

	/* creating socket stream */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port=htons(atoi(argv[1]));		/* port number		  */                      
	server.sin_addr.s_addr=htonl(INADDR_ANY); 	/* Connect to any address */

	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == -1) {
		perror("bind");
		exit(1);
	}

	/* listen to 5 clients */
	if (listen(s, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}

	for(;;) 
	{
		int done, n;
		printf("Waiting for a connection\n");

		/* accept connection fron client */
		t = sizeof(client);
		if ((s2 = accept(s, (struct sockaddr *)&client, &t)) == -1) {
			perror("accept");
			exit(1);
		}
		printf("Connected. Ready to receive messages\n");

		/* receiving message from client */
		done = 0;
		do 
		  {
			n = recv(s2, str, 100, 0);
			if (n <= 0) 
			{
				if (n < 0) 
					perror("recv");
				done = 1;
			}
			if (!done)
			{
				printf("%s", str);
			}

		  } 
		while (!done);

		close(s2);
	}
	return 0;
}
