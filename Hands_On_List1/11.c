#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define BUF_SIZE 20
int main(int argc, char * argv[])
{
    char buf1[30];
    int numRead;
    if(argc!=2)
    {
	perror("Error : " );
    }
    int fd1 = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    int fd2 = dup(fd1);
    if((numRead = read(fd1, buf1, BUF_SIZE))>0) 
    {
	printf("Printing the file content using fd1 \n");
        printf("%s\n",buf1);
    }
    else
    {
	perror("Error :");
    }

    off_t offset = lseek(fd1, 0, SEEK_SET);

    if((numRead = read(fd2, buf1, BUF_SIZE))>0)
    {
	printf("Printing the file content using fd2 \n");
	printf("%s\n",buf1);
    } 
    else
    {
	printf("Error: %s\n", strerror(errno));
    }
    return 0;
}
