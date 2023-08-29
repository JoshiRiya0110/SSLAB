#include<stdio.h>
#include<stdlib.h>
#include<sys/select.h>

int main(void)
{
   int retval;
   fd_set rfds;
   struct timeval tv;
   
   FD_ZERO(&rfds);
   FD_SET(0, &rfds);

   tv.tv_sec = 5;
   tv.tv_usec = 0;

   retval = select(1, &rfds, NULL, NULL, &tv);
   if(retval == -1)
	perror("Error:");
   else if(retval)
	printf("Data is available now.\n");
   else
	printf("No data within 5 seconds.\n");
   return 0;
}
