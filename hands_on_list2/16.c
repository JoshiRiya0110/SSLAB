#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define BUF_SIZE 40
int main()
{
   int pipe1[2]; //pipe for parent to child
   int pipe2[2]; //pipe for child to parent

   if(pipe(pipe1) == -1 || pipe(pipe2) == -1)
   {
	perror("Pipe creation failed");
	return 0;
   }

   pid_t pid = fork();

   if(pid == -1)
   {
	perror("Fork Failed");
	return 0;
   }

   if(pid == 0)
   {
	//child process

	close(pipe1[1]); //closing write end of pipe1
	close(pipe2[0]); //closing read end of pipe2

	//reading data from parent

	char output[BUF_SIZE];
        read(pipe1[0], output, BUF_SIZE-1);
	int n = strlen(output);

	//processing data from parent
	
	for(int i=0 ; i<n/2 ; i++)
	{
	   output[i] = 'R';
	}

	//sending proccessed data back to parent

	write(pipe2[1], output, BUF_SIZE-1);

	close(pipe1[0]);
	close(pipe2[1]);

   }
   else
   {
	//parent process

	close(pipe1[0]); //closing read end of pipe1
	close(pipe2[1]); //closing write end of pipe2


	//sending data to child

	char input[BUF_SIZE];
	printf("Enter the data to be send to child through parent\n");
	scanf("%s",input);

	write(pipe1[1], input, BUF_SIZE-1);


	//receing and printing child's response
     
	read(pipe2[0], input, BUF_SIZE-1);
	printf("Data received by parent through child : %s\n", input);
	
	close(pipe1[1]); //closing write end of pipe1
	close(pipe2[0]); //closing read end of pipe2
   }
   
   return 0;
}
