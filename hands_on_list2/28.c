#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msg_id;
    struct msqid_ds msq_info;

    // Generate the same key used by the message queue
    key = ftok(".", 'e');

    // Get the message queue identifier
    msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Display the current permissions
    if (msgctl(msg_id, IPC_STAT, &msq_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(EXIT_FAILURE);
    }

    printf("Current permissions: %o\n", msq_info.msg_perm.mode);

    // Change the permissions (e.g., set to 0644)
    msq_info.msg_perm.mode = 0644;

    // Update the permissions
    if (msgctl(msg_id, IPC_SET, &msq_info) == -1) {
        perror("msgctl IPC_SET");
        exit(EXIT_FAILURE);
    }

    printf("New permissions: %o\n", msq_info.msg_perm.mode);

    return 0;
}

