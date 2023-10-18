#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_RESOURCES 2

sem_t resource_semaphores[NUM_RESOURCES];

void* access_resource(void* arg) {
    int resource_id = *((int*)arg);

    sem_wait(&resource_semaphores[resource_id]); // Wait for the resource to become available
    printf("Thread accessed resource %d\n", resource_id);

    // Simulate some work on the resource
    sleep(2);

    sem_post(&resource_semaphores[resource_id]); // Release the resource

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_RESOURCES];
    int thread_args[NUM_RESOURCES] = {0, 1}; // Resource IDs

    // Initialize the semaphores for each resource
    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_init(&resource_semaphores[i], 0, 1); // Initial value of 1
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        pthread_create(&threads[i], NULL, access_resource, &thread_args[i]);
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphores
    for (int i = 0; i < NUM_RESOURCES; i++) {
        sem_destroy(&resource_semaphores[i]);
    }

    return 0;
}

