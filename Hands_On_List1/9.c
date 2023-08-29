#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

int main(int argc, char* argv[])
{
    struct stat fi;
    if(argc != 2)
	printf("Wrong Arguments");
    stat(argv[1], &fi);
    printf("Device ID: %ju\n",(uintmax_t)fi.st_dev );
    return 0;
}
