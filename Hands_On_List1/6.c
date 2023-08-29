#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#define BUF_SIZE 20

int main(int argc, char* argv[])
{  
   char buf[BUF_SIZE];
   if(argc != 2)
   {
	printf("Wrong Arguments");
   }
   int numRead = read(STDIN_FILENO, buf, BUF_SIZE);
   if(numRead == -1)
	perror("Error: ");
   buf[numRead] = '\0';
   int numWrite = write(STDOUT_FILENO, buf, numRead);
   if(numWrite == -1)
	perror("Error: ");
   return 0;
}
