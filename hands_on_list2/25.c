#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h>
#include<unistd.h>

int main()
{
   //create a message queue
   key_t key = ftok(".",'B');
   int msgid = msgget(key, IPC_CREAT | 0777);

   if(msgid == -1)
   {
	perror("Error of msgget");
	return 0;
   }

   struct msqid_ds msg_info;

   //getting info about message queue
   int val = msgctl(msgid, IPC_STAT, &msg_info);
   if(val == -1)
   {
	perror("Error of msgctl ");
	return 0;
   }

   printf("Access Permission: %o\n", msg_info.msg_perm.mode);

   printf("UID: %d\n", msg_info.msg_perm.uid);
   printf("GID: %d\n", msg_info.msg_perm.gid);

   printf("Time of last message sent: %s", ctime(&msg_info.msg_stime));
   printf("Time of last message received: %s", ctime(&msg_info.msg_rtime));

   printf("Time of last change: %s", ctime(&msg_info.msg_ctime));

   printf("Size of queue: %lu bytes\n", msg_info.msg_cbytes);

   printf("Number of messages: %lu\n", msg_info.msg_qnum);

   printf("Maximum number of bytes allowed: %lu\n", msg_info.msg_qbytes);

   printf("PID of msgsnd: %d\n", msg_info.msg_lspid);
   printf("PID of msgrcv: %d\n", msg_info.msg_lrpid);

   //closing the message queue
   msgctl(msgid,IPC_RMID,NULL);

   return 0;
}
