#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#define BUF_SIZE 100
#define N 5

int main()
{
   char *fifoFile = "/home/riya/hands_on_list2/tempfifo21";
   if(mkfifo(fifoFile, 0777) == -1)
   {
	perror("Error in mkfifo ");
	return 0;
   }
   
   char writeBuf[BUF_SIZE];
   char readBuf[BUF_SIZE];
   
   for(int i=1; i<=N; i++)
   {
	int fd = open(fifoFile, O_WRONLY);
	if(fd == -1)
	{
	   perror("Error in opening file");
	   return 0;
	}
	
	scanf("%s",writeBuf);
	if(write(fd, writeBuf, BUF_SIZE) == -1)
	{
	   perror("Error in write");
	   return 0;
	}
	close(fd);

	fd = open(fifoFile, O_RDONLY);
	if(fd == -1)
	{
	   perror("Error in opening file");
	   return 0;
	}

	if(read(fd, readBuf, BUF_SIZE) == -1)
	{
	   perror("Error in read");
	   return 0;
	}

	printf("User 2: %s\n", readBuf);
	close(fd);
   }
  
   return 0;
}
