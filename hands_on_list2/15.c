#include <sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define BUF_SIZE 40

int main()
{
   int pipefd[2];
   if(pipe(pipefd)==-1)
   {
	perror("Error of pipe ");
	return 0;
   }
   
   int val = fork();

   if(val == -1)
   {
	perror("Fork failed");
	return 0;
   }

   char input[BUF_SIZE];
   char output[BUF_SIZE];

   if(val != 0)   
   {
	close(pipefd[0]);
	printf("Writing on pipe through parent process\n");
	fflush(stdin);
	scanf("%s", input);
	write(pipefd[1], input, strlen(input) + 1);
	close(pipefd[1]);
   }
   else
   {
	close(pipefd[1]);
	read(pipefd[0], output, BUF_SIZE);
	printf("Reading from pipe through child process\n");
        printf("%s\n",output);
	close(pipefd[0]);
   }
   
   return 0;
}
