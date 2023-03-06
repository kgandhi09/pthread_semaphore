#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Defining macro for number of threads
#define NUM_THREADS 8

//                                                                                    Shared Memory
// 
//                           ┌───────────┬────────────┬───────────┬────────────┬────────────┬────────────┬────────────┬────────────┬─────────┬────────┐
//                           │           │            │           │            │            │            │            │            │         │        │
//                           │           │            │           │            │            │            │            │            │         │        │
//                           │           │            │           │            │            │            │            │            │         │        │
// Producer   ──────────────>│           │            │           │            │            │            │            │            │         │        ├────────>   Consumer
//                           │           │            │           │            │            │            │            │            │         │        │
//                           │           │            │           │            │            │            │            │            │         │        │
//                           └───────────┴────────────┴───────────┴────────────┴────────────┴────────────┴────────────┴────────────┴─────────┴────────┘
// 

// shared memory array for publisher subscriber
int shm_queue[10] = {-1};
int queue_size = 0;

// Declare threads
pthread_t threads[NUM_THREADS];

// Declare semaphores
sem_t shm_queue_empty;
sem_t shm_queue_full;

// mutex 
pthread_mutex_t mutex_SHM;

// print the shared memory array
void print_shm_queue(char* role) {
    printf("[%s] Shared Memory Queue: ", role);
    for (int i = 0; i < queue_size; i++) {
        printf("%d ", shm_queue[i]);
    }
    printf("\n");
}

// Routine for publish thread
void *publish(void *arg) {
    int index = *(int *)arg;
  
    while(1){
        int shm_val = rand() % 100;
        
        // Wait until there is at least one slot is empty in shm_queue
        sem_wait(&shm_queue_empty);

        // Lock the mutex
        pthread_mutex_lock(&mutex_SHM);

        shm_queue[queue_size] = shm_val;
        queue_size++;
        
        printf("[Publisher] queue size: %d\n", queue_size);
       
        // Unlock the mutex
        pthread_mutex_unlock(&mutex_SHM);

        sem_post(&shm_queue_full);
    }
    return nullptr;
}

// Routine for subscribe thread
void *subscribe(void *arg) {
    int index = *(int*)arg;

    while(1){
        sem_wait(&shm_queue_full);

        // Lock the mutex
        pthread_mutex_lock(&mutex_SHM);
        
        int consumed_val = shm_queue[queue_size-1];
        queue_size--;

        printf("[Subscriber] queue size: %d\n", queue_size);

        // Unock the mutex
        pthread_mutex_unlock(&mutex_SHM);
        
        sem_post(&shm_queue_empty);
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    
    // Initializing mutex
    pthread_mutex_init(&mutex_SHM, NULL);

    // Initializing semaphores
    sem_init(&shm_queue_empty, 0, 10);
    sem_init(&shm_queue_full, 0, 0);

    // Creating first thread as producer (publisher) and all other following to
    // be consumers (subscribers)
    for (int i = 0; i < NUM_THREADS; i++) {

        // passing index as arg
        int *a = (int *)malloc(sizeof(int));
        *a = i;

        // publisher
        if (i == 0) {
            pthread_create(&threads[i], NULL, &publish, a);
        } else {
            pthread_create(&threads[i], NULL, &subscribe, a);
        }
    }

    // Joining the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&shm_queue_empty);
    sem_destroy(&shm_queue_full);

    // Destroy mutex buffer
    pthread_mutex_destroy(&mutex_SHM);

    return 0;
}
