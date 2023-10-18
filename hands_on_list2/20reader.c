#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#define BUF_SIZE 100

int main(int argc, char* argv[])
{
   const char *fifoFile = "/home/riya/hands_on_list2/tempfifo20";

   int fd = open(fifoFile, O_RDONLY);
   if(fd == -1)
   {
	perror("Error in opening the fifo file\n");
	return 0;
   }

   //message to be written to fifo file

   char output[BUF_SIZE];

   int count = read(fd, output, BUF_SIZE);
   if(count == -1)
   {
	perror("Error while reading from fifo file");
	return 0;
   }
   
   output[count] = '\0';
   printf("Received message : %s\n",output);
   
   //close fifo file
   close(fd);
 
   return 0;

}
