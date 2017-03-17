/*
** client.c --------------
** this is used to send message to the server to which is connected.
** send the IP address and the port number of the server to connect.
** once connected type your message.
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

	int s, t, len;
	struct sockaddr_in client;
	char str[MAXDATASIZE];		/* buffer for message */

	/* creating socket stream */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}
	printf("Welcome to My Chat messenger\n");
	client.sin_family=AF_INET;
        client.sin_port=htons(atoi(argv[2]));		/* port address */
	client.sin_addr.s_addr=inet_addr(argv[1]);	/* IP address   */

	if (connect(s, (struct sockaddr *)&client, sizeof(client)) == -1) 
	{
		perror("connect");
		exit(1);
	}
	printf("Start chatting with %s\n", argv[1]);

	/* to send the message */
	while(1)
	{
		printf("> ");
		fgets(str, MAXDATASIZE, stdin);
		if (send(s, str, strlen(str), 0) == -1) 
		{
			perror("send");
			exit(1);
		}
	}
	close(s);
	return 0;
}
