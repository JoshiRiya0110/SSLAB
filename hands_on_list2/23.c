#include<unistd.h>
#include<stdio.h>

int main()
{
   long PIPE_BUF, OPEN_MAX;
   PIPE_BUF = pathconf(".", _PC_PIPE_BUF);
   OPEN_MAX = sysconf(_SC_OPEN_MAX);

   printf("Pipe buffer = %ld\n",PIPE_BUF);
   printf("Maximum number of files that can be open = %ld\n",OPEN_MAX);

   return 0;
}
