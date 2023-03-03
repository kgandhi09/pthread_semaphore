#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int fuel_tank = 0;

// Mutex to protect the fuel tank
pthread_mutex_t fuel_tank_mutex;

// Condition var to signal and broadcast fuel tank val
pthread_cond_t fuel_tank_cond;

void* fill_fuel(void* arg){


    // Setting the upper limit of fuel tank to 150
    while ( fuel_tank < 150) {
        pthread_mutex_lock(&fuel_tank_mutex);
        // Fill the fuel tank
        fuel_tank += 10;
        printf("Filled the tank with total fuel: %d\n", fuel_tank);

        pthread_mutex_unlock(&fuel_tank_mutex);
        // Signal the user that we have filled the fuel with some val
        pthread_cond_signal(&fuel_tank_cond);
        sleep(1);
    }


    return nullptr; 
}

void* use_fuel(void* arg){
   
    pthread_mutex_lock(&fuel_tank_mutex);

    // Setting the lower limit to be used from the tank to 10
    while (fuel_tank < 40) {
        printf("Waiting for the fuel\n"); 
        // Wait for the fuel to be filled by another thread 
        pthread_cond_wait(&fuel_tank_cond, &fuel_tank_mutex);
    }

    fuel_tank -= 40;
    printf("Got the fuel, using it!\n");

    pthread_mutex_unlock(&fuel_tank_mutex);

    return nullptr; 
}

int main(int argc, char* argv[]){
    
    // Make two threads 
    pthread_t threads[2];

    // Initializing the fuel tank mutex
    pthread_mutex_init(&fuel_tank_mutex, NULL);

    // Initialize the pthread condition 
    pthread_cond_init(&fuel_tank_cond, NULL);

    // Creating thread for fuel filling in the tank
    pthread_create(&threads[0], NULL, &fill_fuel, NULL);
    
    // Creating thread to use the fuel from the tank
    pthread_create(&threads[1], NULL, &use_fuel, NULL);

    // Joining both the threads
    for(int i =0; i < 2; i++){
        pthread_join(threads[i], NULL);
    }

    // Destroy the pthread condition
    pthread_cond_destroy(&fuel_tank_cond);

    // Destroy the mutex
    pthread_mutex_destroy(&fuel_tank_mutex);

    return 0;
}

