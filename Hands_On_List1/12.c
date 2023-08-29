#include<stdio.h>
#include<fcntl.h>
int main(int argc,char* argv[])
{
    if(argc != 2)
	printf("Wrong Arguments");
    int fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1)
    {
	perror("Error : ");
    }

    int flags = fcntl(fd,F_GETFL);    
    if(flags == -1)
    {
	perror("Error: ");
    }
    int accessMode = flags & O_ACCMODE;
    if(accessMode == O_WRONLY)
	printf("File is writable\n");
    else if(accessMode == O_RDWR)
	printf("File is both readable and writable\n");
    else if(accessMode == O_RDONLY)
	printf("File is readable\n");
    return 0;
}
