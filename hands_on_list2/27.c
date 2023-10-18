#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msg_id;
    struct msg_buffer msg;

    // Generate the same key used by the receiver program
    key = ftok(".", 'D');

    // Get the message queue identifier
    msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Determine whether to send or receive a message
    printf("Enter 's' to send a message or 'r' to receive a message: ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 's')
    {
        // Prepare the message to send
        msg.msg_type = 1; // Message type should match the receiver program
        printf("Enter the message to send: ");
        scanf(" %s\n", msg.msg_text);

        // Send the message to the queue
        if (msgsnd(msg_id, &msg, sizeof(msg), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Message sent to the queue: %s\n", msg.msg_text);
    }
    else if (choice == 'r')
    {
	printf("Enter the choice of the flag i.e either 0 or I: ");
	char ch;
        scanf("%c\n",&ch);

	if(ch == '0')
	{		 
            // Receive a message from the queue
       	    if(msgrcv(msg_id, &msg, sizeof(msg), 1, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
	    }
        }
	else
        {
             // Receive a message from the queue
             if(msgrcv(msg_id, &msg, sizeof(msg), 1, IPC_NOWAIT) == -1) {
             perror("msgrcv");
             exit(EXIT_FAILURE);
	     }
	}

        printf("Received message from the queue: %s\n", msg.msg_text);
    }
    else
    {
        printf("Invalid choice. Please enter 's' to send or 'r' to receive.\n");
    }

    return 0;
}

