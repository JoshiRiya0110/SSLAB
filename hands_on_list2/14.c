#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
   int pipefd[2];
   char string[50];
   char buf[50];
   
   if(pipe(pipefd) == -1)
   {
	perror("Error of pipe: ");
	return 0;
   }
   
   scanf("%s", string);
   write(pipefd[1], string, strlen(string) + 1);
   
   read(pipefd[0], buf, strlen(string) + 1); 
   printf("%s\n",buf);

   return 0;
}
