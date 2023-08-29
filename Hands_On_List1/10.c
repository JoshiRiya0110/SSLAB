#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define BUF_SIZE 10
int main(int argc, char* argv[])
{
    char buf1[10] = "abcdefghij";
    char buf2[20] ;
    int numRead;
    int j=0;
    int count = 0;

    if(argc != 2)
	perror("Error1: ");

    int fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1)
	perror("Error2: ");

    int num = write(fd, buf1, BUF_SIZE);
    if(num == -1)
    {
	perror("Error3 : ");
    }

    off_t offset = lseek(fd, (BUF_SIZE + 1), SEEK_CUR);
    if(offset == -1)
    {
	perror("Error4: ");
    }
    else
    {
	printf("%ld\n",offset);
    }
    
    int num_written = write(fd, buf1, BUF_SIZE);
    if(num_written == -1)
    {
	perror("Error5 : ");
    }
    close(fd);

    int fd1 = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if((numRead = read(fd1, buf2, 2 * BUF_SIZE))>0)
    {
	printf("%s\n",buf2);
        for(j=0 ; j<numRead ; j++)
        {
		if(buf2[j] == 'a')
                {
			count++;
              }	
	}
	printf("Total %d a's are present",count); 
    }
    else
    {
	perror("Error :");
    }
    
    return 0;
}

