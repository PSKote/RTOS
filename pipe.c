/*
** pipe.c -------
** C program that collects the output and pipes it as input into another. 
** Without using standard |  operator
*/

#include <stdio.h>	/* standard I/O routines.               */
#include <stdlib.h>	/* exit(), etc.                         */
#include <unistd.h>	/* fork(), etc.                         */

int main(void)
{
    int pfds[2];

    pipe(pfds);

    if (!fork()) 
    {
        close(1);       /* close normal stdout */
        dup(pfds[1]);   /* make stdout same as pfds[1] */
        close(pfds[0]); /* we don't need this */
	execlp("sh", "sh", "-c", "ls -l *.jpg", NULL);
    } 
    else 
    {
        close(0);       /* close normal stdin */
        dup(pfds[0]);   /* make stdin same as pfds[0] */
        close(pfds[1]); /* we don't need this */
        execlp("wc", "wc", "-l", NULL);        
    }

    return 0;
}


