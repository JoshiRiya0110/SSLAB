#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
int main() 
{
    sem_t binary_semaphore;
    sem_init(&binary_semaphore, 0, 1);
    printf("Binary semaphore created and initialized to 1.\n");
    sem_destroy(&binary_semaphore);
    printf("Semaphore destroyed.\n");
    return 0;
}

