#include <pthread.h>
#include <stdio.h>

/* 
EXERCISE: Matrix Multi w threads
    - Add pthread_barrier code to the template code in matmult.c
    - Ensure that the “verification” thread does not run until all “multiplier” threads have hit the barrier
    - You should get “SUCCESS: Matrices matched” once you implemented correctly

*/



/*
//Create a barrier object
pthread_barrier_t barrier; 

//  Function:   pthread_barrier_init
//  Purpose:    initializes a pthread barrier object
//  Params:     barrier --> Address to pthread_barrier_t to initialized
//              attr    --> Barrier attributes
//              count   --> Number of threads to wait for until unblocking
//  Returns:    0 on success [anything else on failure]
int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned count); 

//  Function:   pthread_barrier_destroy
//  Purpose:    destroys a pthread barrier object
//  Params:     barrier --> Address to pthread_barrier_t to destroy
//  Returns:    0 on success [anything else on failure]
int pthread_barrier_destroy(pthread_barrier_t *restrict barrier); 


// ****THIS IS THE EQUIV OF THE checkin() function of lect barrier ****
//  Function:   pthread_barrier_wait
//  Purpose:    forces a thread to wait until "count" threads have signalled at the barrier
//  Params:     barrier --> Address to pthread_barrier_t to wait for
//  Returns:    0 on success [anything else on failure]
int pthread_barrier_wait(pthread_barrier_t *restrict barrier);
*/


pthread_barrier_t barrier;

void * thread_func(void* arg){
    int id = *(int*)arg;
    printf("I am thread [%d], PRE-BARRIER\n", id);
    pthread_barrier_wait(&barrier);
    printf("I am thread [%d], POST-BARRIER\n", id);
}

int main(int argc, char **argv){ 
    pthread_t thread1;
    pthread_t thread2;
    int thread_ids[2] = {1, 2};

    pthread_barrier_init(&barrier, NULL, 2);

    pthread_create(&thread1, NULL, thread_func, &thread_ids[0]);
    pthread_create(&thread2, NULL, thread_func, &thread_ids[1]);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_barrier_destroy(&barrier);
}