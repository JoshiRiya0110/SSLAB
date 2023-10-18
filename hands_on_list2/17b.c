#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() 
{
	int pipe_ls_wc[2];
    	pipe(pipe_ls_wc);
    	if(!fork()) 
	{
        	dup2(pipe_ls_wc[1], STDOUT_FILENO);
        	close(pipe_ls_wc[0]);  // Close the read end in the child process
        	execlp("ls", "ls", "-l", NULL);
    	} 
	else 
	{
        	dup2(pipe_ls_wc[0], STDIN_FILENO);
        	close(pipe_ls_wc[1]);  // Close the write end in the parent process
        	execlp("wc", "wc", NULL);
    	}
	return 0;
}

