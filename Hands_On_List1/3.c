#include<stdio.h>
#include <fcntl.h>
#include<errno.h>
#include<string.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
	printf("Give filepath as the only argument");
	exit(0);
    }
    int  fd = creat(argv[1], S_IRUSR | S_IWUSR);
    if(fd==-1)
	printf("Error: %s\n",strerror(errno));
    else
	printf("File Descriptor Value Is %d\n",fd);
    close(fd);
    return 0;
}
