

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

/****************************************************************************************
  * 2 DIFF WAYS TO 
*****************************************************************************************/


/****************************************************************************************
  * // trying to get threads to print in order of their ID
  * // ringInefficentVersion() uses BROADCAST 
  * and the one AFTER is the GOOD ONE which uses ARRAY OF CONDV
*****************************************************************************************/







/****************************************************************************************
  * THIS IS THE INEFFICIENT SOL :(
*****************************************************************************************/

int numThreads = 10; // hardcoded for simplicity

// trying to get threads to print in order of their ID
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV = PTHREAD_COND_INITIALIZER;

int turn = 0; 
void* ringNotEfficentVersion(int my_id){
    while(1){
        pthread_mutex_lock(&lock);
        if(turn == my_id){
            printf("Im thread number: %d", my_id);
            turn = (turn + 1) % numThreads;
            pthread_cond_broadcast(&CV); // here's the INEFFICIENCY PART
        }
        else{
            pthread_cond_wait(&CV, &lock);
        }

        pthread_mutex_unlock(&lock);

    } // end of while

} // end of ring()



















/****************************************************************************************
  * THIS IS THE EFFICIENT, GOOD SOL :(
*****************************************************************************************/





numThreads = 10; // hardcoded for simplicity
int const MAX = 10; 
// trying to get threads to print in order of their ID
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV[MAX] = PTHREAD_COND_INITIALIZER; // ARRAY OF COND VARS instead of just 1

// this ARRAY of CONDVariables allows EACH thread to have it's own CondVar 
// and each is WAITING on its own, so then the current thread (the thread whos turn is it)
// is able to wake up ONLY the thread whos next -- by cleverly using 'pthread_cond_signal'
// instead of 'pthread_cond_broadcast' and then we use the INDEX of the CV ARRAY to pick what
// CV we need to pass as the arg -- this is ALL DONE ON LINE 110


int turn = 0; 
void* ringEfficientVersion(int my_id){
    while(1){
        pthread_mutex_lock(&lock);
        if(turn == my_id){
            printf("Im thread number: %d", my_id);
            turn = (turn + 1) % numThreads;
            pthread_cond_signal(&CV[turn]); // IMPROVEMENT HERE
        }
        else{
            pthread_cond_wait(&CV[my_id], &lock); // IMPROVEMENT HERE
        }

        pthread_mutex_unlock(&lock);

    } // end of while

} // end of ring()












































