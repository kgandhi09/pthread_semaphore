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

void* thread_routine( void* ) {
    printf("Test from thread routine\n");

    //returning null void*
    void* ret = NULL;
    return ret;
}

int main(int argc, char *argv[]) {
    // Create a thread
    pthread_t thread1, thread2;
    if (pthread_create(&thread1, NULL, &thread_routine, NULL) != 0) printf("Failure to create a new thread\n"); 
    if (pthread_create(&thread2, NULL, &thread_routine, NULL) != 0) printf("Failure to create a new thread\n"); 

    // Join the thread
    if(pthread_join(thread1, NULL) != 0) printf("Failure to call thread wait for termination\n");
    if(pthread_join(thread2, NULL) != 0) printf("Failure to call thread wait for termination\n");

    return 0;
}
