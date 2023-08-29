#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
	printf("Argument Missing\n");
    }
    int fd = open(argv[1], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd==-1)
    {
	perror("error ");
    }
    else
    {
	printf("File opened Successfully\n");
    }
    close(fd);
    return 0;
}
