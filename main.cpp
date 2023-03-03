// Author: Kushal Gandhi
// Date: 2023-02-28
// Description: This is a test program
//             to test the thread creation
//             in C++.
//
// Compile: g++ -o main main.cpp -lpthread
// Run: ./main

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shared_arr[10] = {1,2,3,4,5,6,7,8,9,10};
pthread_mutex_t thread_mutex;

void* thread_routine( void* arg) {
    int shared_arr_idx = *(int*)arg;
    int size_shared_arr = (sizeof(shared_arr)/sizeof(shared_arr[0])/2);
    int sum_elems_per_thread = 0;

    for(int i = 0; i < size_shared_arr; i++){
        sum_elems_per_thread += shared_arr[shared_arr_idx*size_shared_arr + i];
    }

    //returning the argument as void pointer
    *(int*)arg = sum_elems_per_thread;
    return arg;
}

int main(int argc, char *argv[]) {

    pthread_t threads[2];

    // Initialize the thread mutex without any attributes
    pthread_mutex_init(&thread_mutex, NULL);
    
    // Create two threads in a for loop
    for(int i = 0; i < sizeof(threads)/sizeof(threads[0]); i++){
        int *a = (int*)malloc(sizeof(int));
        *a = i;
        //Passing the the number of thread as argument to the thread
        pthread_create(&threads[i], NULL, &thread_routine, a);
    }

    int global_sum = 0;

    // Join the threads 
    for(int i = 0; i < sizeof(threads)/sizeof(threads[0]); i++){
        void* thread_return_value;
        pthread_join(threads[i], &thread_return_value);
        global_sum += *(int*)thread_return_value;
    }

    printf("Global sum calculated by two threads: %d\n", global_sum);

    // Destroy the mutex
    pthread_mutex_destroy(&thread_mutex);

    return 0;
}
