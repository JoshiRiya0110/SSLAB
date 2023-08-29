#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define BUF_SIZE 10
int main(int argc, char* argv[])
{
    ssize_t numRead;
    char buf[BUF_SIZE];

    if(argc!=2)
	printf("Wrong Arguments");

    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
    if(fd == -1)
	perror("Error : " );

    while((numRead = read(fd , buf , BUF_SIZE))>0)
	printf("%s\n",buf);

    if(numRead == -1)
	perror("Error ");

    return 0;
}
