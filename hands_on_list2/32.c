#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_TICKETS 100
int ticket_counter = 0;

// Declare a semaphore
sem_t ticket_semaphore;

void* generate_ticket(void* arg) {
    int* thread_id = (int*)arg;

    for (int i = 0; i < 10; i++) {
        // Wait until the semaphore is available
        sem_wait(&ticket_semaphore);

        // Generate and print a ticket number
        printf("Thread %d generated ticket #%d\n", *thread_id, ticket_counter);
        ticket_counter++;

        // Release the semaphore
        sem_post(&ticket_semaphore);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3]; // Create 3 threads
    int thread_ids[3] = {1, 2, 3};

    // Initialize the semaphore
    sem_init(&ticket_semaphore, 0, 1); // Initial value of 1

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, generate_ticket, &thread_ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&ticket_semaphore);

    return 0;
}

