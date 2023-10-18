#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int main() {
    key_t key;
    int semid;
    key = ftok(".", 'E');
    semid = semget(key, 1, 0666 | IPC_CREAT);
    union semun 
    {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } argument;
    argument.val = 5;  // Set the initial value
    semctl(semid, 0, SETVAL, argument);
    printf("Counting semaphore created and initialized with value %d.\n", argument.val);
    return 0;
}

