#include<stdbool.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<time.h>
#include<sys/stat.h>
int main(int argc , char* argv[])
{
    struct stat sb;
    bool link;
    link = (argc == 2) && strcmp(argv[1],"-l") == 0;
    if(link)
    {
	if(lstat(argv[1],&sb) == -1)
	      perror("lstat");		    
    }
    else
    {
        if(stat(argv[1],&sb) == -1)
              perror("stat");
    }
    
    printf("File Type:  ");
    switch(sb.st_mode & S_IFMT)
    {
	case S_IFREG: printf("regular file\n");		break;
	case S_IFDIR: printf("directory\n"); 		break;
	case S_IFCHR: printf("character device\n");	break;
	case S_IFBLK: printf("block device\n");		break;
	case S_IFLNK: printf("symnolic (soft) link\n"); break;
	case S_IFIFO: printf("FIFO or pipe\n");		break;
	case S_IFSOCK: printf("Socket\n");		break;
	default:      printf("unknown file type\n");	break;
    }
    return 0;
}
