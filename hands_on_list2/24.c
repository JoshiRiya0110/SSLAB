#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>

int main()
{
   key_t key;
   int msgqid;

   key = ftok(".", 'A');
   if(key == -1)
   {
	perror("Error in ftok ");
	return 0;
   }

   msgqid = msgget(key, IPC_CREAT | 0777);
   if(msgqid == -1)
   {
	perror("Error in msgget");
	return 0;
   }

   printf("Message queue key : %d\n", key);
   printf("Message queue ID  : %d\n", msgqid);

   return 0;
}
