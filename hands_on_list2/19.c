#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
   int val1 = mknod("myfifo1",S_IFIFO | O_RDWR,0);
   if(val1 == -1)
   {
	perror("Error of mknod\n");
   }
   else
   {
	printf("myfifo1 created successfully\n");
   }

   int val2 = mkfifo("myfifo2",O_RDWR);
   if(val2 == -1)
   {
	perror("Error of mkfifo\n");
   }
   else
   {
	printf("myfifo2 created successfully\n");
   }

   return 0;

}	
