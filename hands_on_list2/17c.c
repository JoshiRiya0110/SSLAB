#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main() {
    int pipefd[2];
    pipe(pipefd);

    if(!fork()) 
    {
	//child process
	close(STDOUT_FILENO);
        fcntl(pipefd[1], F_DUPFD, STDOUT_FILENO);
        close(pipefd[0]);  // Close the read end in the child process
        execlp("ls", "ls", "-l", NULL);
    } 
    else 
    {
	//parent process
	close(STDIN_FILENO);
        fcntl(pipefd[0], F_DUPFD, STDIN_FILENO);
        close(pipefd[1]);  // Close the write end in the parent process
        execlp("wc", "wc", NULL);
    }

    return 0;
}

