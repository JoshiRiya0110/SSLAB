#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main()
{
   key_t key;
   int msgid;
   struct msgbuf msg;

   key = ftok(".",'C');
   if(key == -1)
   {
	perror("Error of ftok");
	return 0;
   }

   msgid = msgget(key, 0777 | IPC_CREAT);
   if(msgid == -1)
   {
	perror("Error of msgget");
	return 0;
   }

   printf("Enter the message: ");
   scanf("%s",msg.mtext);

   msg.mtype = 1;

   int val = msgsnd(msgid, &msg, strlen(msg.text) , 0);
   if(val == -1)
   {
	perror("Error in msgsnd");
	return 0;
   }

   system("ipcs -q");

   return 0;
}
