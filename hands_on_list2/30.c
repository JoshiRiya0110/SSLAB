#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SHM_KEY 12345
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shmaddr;

    // Create the shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment for writing
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Write data to shared memory
    strcpy(shmaddr, "Hello, shared memory!");

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Attach the shared memory segment with read-only permission
    shmaddr = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shmaddr == (char *)(-1)) {
        perror("shmat (read-only)");
        exit(1);
    }

    printf("Read-only data: %s\n", shmaddr);

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}

