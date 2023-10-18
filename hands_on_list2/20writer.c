#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#define BUF_SIZE 100

int main()
{
   const char *fifoFile = "/home/riya/hands_on_list2/tempfifo20";

   if(mknod(fifoFile, 0777, 0) == -1)
   {
	perror("mknod");
	return 0;
   }

   int fd = open(fifoFile, O_WRONLY);
   if(fd == -1)
   {
	perror("Error in opening the fifo file\n");
	return 0;
   }

   //message to be written to fifo file

   fflush(stdout);
   char input[BUF_SIZE];
   printf("Enter the message\n");
   scanf("%s",input);

   fflush(stdout);
   int count = write(fd, input, BUF_SIZE);
   if(count == -1)
   {
	perror("Error while writing to fifo file");
	return 0;
   }
   
   close(fd);
 
   return 0;

}
