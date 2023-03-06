#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 12

int dice_val[NUM_THREADS];                    // shared array to store dice values after each thread rolls dice
int status[NUM_THREADS] = {0};                // status of every thread after rolling dice -- either WINNER (1) or LOSER (0)

pthread_barrier_t dice_val_barrier;
pthread_barrier_t status_set_barrier;

// Thread routine for each thread to roll dice
void* roll_dice(void* arg){
    
    int index = *(int*)arg;
 
    // Roll the dice and store the random number
    dice_val[index] = (rand() % 6 ) + 1;
 
    // Raise the barrier until all dice rolling threads arrive here 
    pthread_barrier_wait(&dice_val_barrier);
    pthread_barrier_wait(&status_set_barrier);
    
    // print if won or lost from the status arr
    if(status[index] == 0) {
        printf("Dice Value: %d, I lost :(\n", dice_val[index]);
    }
    else {
        printf("Dice Value: %d, I won :)\n", dice_val[index]);
    }
 
    free(arg);
    return nullptr;
} 

int main(int argc, char* argv[]){

    // Declare threads
    pthread_t threads[NUM_THREADS];

    // Initialize the pthread barriers
    pthread_barrier_init(&dice_val_barrier, NULL, NUM_THREADS + 1);
    pthread_barrier_init(&status_set_barrier, NULL, NUM_THREADS + 1);

    // Create threads
    for(int i = 0; i < NUM_THREADS; i++){
        int* a = (int*)malloc(sizeof(int));
        *a = i;
        if(pthread_create(&threads[i], NULL, &roll_dice, a) != 0) printf("Failure to create %d thread\n", i);
    }

    pthread_barrier_wait(&dice_val_barrier);

    //Calculate the winner
    int max = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if(dice_val[i] > max){
            max = dice_val[i]; 
        }
    }

    // Change the status
    for(int i = 0; i < NUM_THREADS; i++){
        if (dice_val[i] == max) status[i] = 1;
    }

    pthread_barrier_wait(&status_set_barrier);

    // Join the threads
    for(int i = 0; i < NUM_THREADS; i++){
        if(pthread_join(threads[i], NULL) != 0) printf("Failure to join %d thread\n", i);
    }

    // Destroy the pthread barriers
    pthread_barrier_destroy(&dice_val_barrier);
    pthread_barrier_destroy(&status_set_barrier);

    return 0;
}
