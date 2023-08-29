#include<stdio.h>
int main(int argc, char* argv[])
{
    int fd1 = open(argv[1], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fd2 = open(argv[2], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fd3 = open(argv[3], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fd3 = open(argv[4], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fd4 = open(argv[5], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fd5 = open(argv[6], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    for(;;);
}
