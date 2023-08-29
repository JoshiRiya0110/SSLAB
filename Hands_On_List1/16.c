#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char* argv[])
{
    struct flock lock;
    if(argc != 3)
    {
	printf("Arguments Missing\n");
    }

    int fd = open(argv[1], O_RDWR);

    if(fd == -1)
    {
	perror("Error : ");
        return 0;
    }
    lock.l_type = (argv[2][0]=='w')?F_WRLCK:F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    printf("Before entering the cs\n");
    fcntl(fd,F_SETLKW,&lock);
    printf("Inside the cs\n");
    printf("Press enter to unlock");
    getchar();
    printf("file unlocked");
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    printf("END\n");
    return 0;
}
