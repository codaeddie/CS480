/*
 * The code is not part of the real application, and just used to 
 * illustrate the bounded-buffer problem using Semaphore and/or mutexes. 
 * Detailed requirements please refer to the assignment documentation.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include "bbuffer.h"

// You may need to define more variables here
buffer_t buffer;
pthread_t consumer_tid[CONSUMERS];
pthread_t producer_tid[PRODUCERS];

sem_t empty;
sem_t full;
sem_t print_sem;
pthread_mutex_t mutex;

void initilization()
{
    sem_init(&empty, 0, BUFFER_SIZE); // initially, empty slots = BUFFER_SIZE
    sem_init(&full, 0, 0); // initially, full slots = 0
    sem_init(&print_sem, 0, 1); // initialize print_sem to 1
    pthread_mutex_init(&mutex, NULL);
}

int insert_item(int item, long int id)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer.value[buffer.next_in] = item;
    sem_wait(&print_sem);
    printf("producer %ld: inserted item %d into buffer index %d\n", id, item, buffer.next_in);
    sem_post(&print_sem);
    buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return 0;
}

int remove_item(int *item, long int id)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    *item = buffer.value[buffer.next_out];
    sem_wait(&print_sem);
    printf("consumer %ld: removed item %d from buffer index %d\n", id, *item, buffer.next_out);
    sem_post(&print_sem);
    buffer.next_out = (buffer.next_out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return 0;
}

void * producer(void *param)
{
    int item, i;
    long int id = (long int)param;

    sem_wait(&print_sem);
    printf("producer %ld started!\n", id);
    sem_post(&print_sem);

    i = PRODUCER_ITERATIONS;
    while (i--) {
        sleep(rand() % 3);

        item = rand() % 10000;
        if (insert_item(item, id))
            printf("Error while inserting to buffer\n");
    }

    initilization();
    pthread_exit(0);
}

void * consumer(void *param)
{
    int item, i;
    long int id = (long int)param;

    sem_wait(&print_sem);
    printf("consumer %ld started!\n", id);
    sem_post(&print_sem);

    i = CONSUMER_ITERATIONS;
    while (i--) {
        sleep(rand() % 3);

        if (remove_item(&item, id))
            printf("Error while removing from buffer\n");
    }

    initilization();
    pthread_exit(0);
}

void cleanup() {
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&print_sem);
    pthread_mutex_destroy(&mutex);
}