#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
	int a = symlink("file1","file1SL");
	if(a<0)
	       {perror("Failed");return 1;}

	int b = link("file1","file1HL");
	if(b<0)
		{perror("Failed");return 1;}

	int c = mknod("file1FIFO",S_IFIFO,0);
	if(c<0)
		{perror("Failed");return 1;}
	return 0;
}
