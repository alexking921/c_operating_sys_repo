#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 10
#define NUMITEMS 100

static pthread_mutex_t buffer_access 	= PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t buffer_not_empty 	= PTHREAD_COND_INITIALIZER;
static pthread_cond_t buffer_not_full 	= PTHREAD_COND_INITIALIZER;

static int buffer[BUFSIZE];
static int in 				= 0;
static int out 				= 0;
static int count 			= 0;

/*
 * Inserts a number into the buffer
*/
void * producer(void *arg)
{
	int item, i;
	//produce items randomly
	for(i = 0; i < NUMITEMS; i++) {
		//Get the lock to start working on the buffer
		pthread_mutex_lock(&buffer_access);

		printf("Producer: Buffer Full Check, busy wait?\n");
		while (count == BUFSIZE) {
			//Buffer full, release lock and sleep until buffer not full
			printf("Producer: full Buffer, sleeping until buffer_not_full condition signal\n");
			pthread_cond_wait(&buffer_not_full, &buffer_access); // I'm going to sleep. Wake me up ONLY if there's a 'buffer_not_full' signal  (aka the CV), I'm releasing the lock too bc I'll be sleeping anyway so someone else take it. 
		};

		//We have the lock and we know the buffer is not full
		printf("Producer: buffer_not_full signal received\n");

		//Add an item into the buffer and release lock
		item 		= rand();
		buffer[in] 	= item;
		in = (in + 1) % BUFSIZE;
		count++;
		printf("Produced item: %10d\tBuffer count: %d\n", item, count);

		//Signal that the buffer is not empty since we produced an item into it
		pthread_cond_signal(&buffer_not_empty);		
		pthread_mutex_unlock(&buffer_access);
	}

	return NULL;
}



















/*
 * Extracts a number from the buffer
*/
void * consumer(void *arg)
{
	int item, i;
	for(i = 0; i < NUMITEMS; i++) {
		//Get the lock to start working on the buffer
		pthread_mutex_lock(&buffer_access);

		printf("Consumer: Buffer Empty Check, busy wait?\n");
		while (count == 0) {
			//Buffer empty, release lock and sleep until buffer not empty
			printf("Consumer: Empty Buffer, sleeping until buffer_not_empty condition signal\n");
			pthread_cond_wait(&buffer_not_empty, &buffer_access);
		}

		//We have the lock and we know the buffer is not empty
		printf("Consumer: buffer_not_empty signal received\n");

		//Remove item from buffer and unlock the lock
		item = buffer[out];
		out = (out + 1) % BUFSIZE;
		count--;
		printf("Consumed item: %10d\tBuffer count: %d\n", item, count);

		//Signal that the buffer is not full since we consumed an item from it
		pthread_cond_signal(&buffer_not_full);		
		pthread_mutex_unlock(&buffer_access);
	}

	return NULL;
}





















int main(int argc, char **argv){
	
	pthread_t prod;
	pthread_t cons;

	//Create consumer threads
	if(pthread_create(&cons, NULL, consumer, NULL)!=0){
		fprintf(stderr,"Error creating consumer thread\n");
		exit(1);
	}
	
	//Let consumer run for a second to demo the wasted CPU of busy wait
	sleep(1);

	//Create producer threads
	if(pthread_create(&prod, NULL, producer, NULL)!=0){
		fprintf(stderr,"Error creating producer thread\n");
		pthread_join(cons,NULL);
		exit(1);
	}

	//Join threads
	if(pthread_join(cons,NULL)!=0)
		fprintf(stderr,"Error joining consumer thread\n");
	if(pthread_join(prod,NULL)!=0)
		fprintf(stderr,"Error joining producer thread\n");

	return 0;
}




//This program contains two threads T1 and T2, and has a serious flaw. 
//What is it? How could you fix it? Hint: there are numerous ways to 
//fix this such that x and y are both incremented by 3 (you cannot change 
//the assignment statements in T1 or T2).

// ANSWER: 
// Depending on the order of the context switches between threads, they both will be waiting on 
//each other to release their locks, so the program won't be able to move further. 
// If T1 grabs the Xlock and then it switches to T2 which grabs the Ylock, adds +2 to y, and then tries to
// grab the Xlock, it wont be able to, so it will have to wait. Then a context switch back to T1 where after
// in increments x+2 it tries to grab the Ylock, which it cant until T2 drops it. 

int x=0, y=0;
pthread_mutex_t xlock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t ylock = PTHREAD_MUTEX_INITIALIZER;


// T1:
pthread_mutex_lock (&xlock);
x = x + 1;
pthread_mutex_lock (&ylock); 
y = y + 1; pthread_mutex_unlock (&ylock); 
pthread_mutex_unlock (&xlock);

 
//T2:
pthread_mutex_lock (&ylock);
y = y + 2;
pthread_mutex_lock (&xlock);
x = x + 2;
pthread_mutex_unlock (&xlock);
pthread_mutex_unlock (&ylock);












// int x=0, y=0;
// pthread_mutex_t xlock = PTHREAD_MUTEX_INITIALIZER; 
// pthread_mutex_t ylock = PTHREAD_MUTEX_INITIALIZER;


// // T1:
// pthread_mutex_lock (&xlock);
// x = x + 1;
// pthread_mutex_lock (&ylock); 
// y = y + 1; pthread_mutex_unlock (&ylock); 
// pthread_mutex_unlock (&xlock);

 
// //T2:
// pthread_mutex_lock (&ylock);
// y = y + 2;
// pthread_mutex_lock (&xlock);
// x = x + 2;
// pthread_mutex_unlock (&xlock);
// pthread_mutex_unlock (&ylock);





















































// SIGNALS PROBLEM :

// Problem 3: Signals (25 Pts)
// Write a main program that receives SIGALRM signals. When it receives an exact number of SIGALRM signals (LOW), it prints a warning message, 
// when it receives an exact number of SIGALRM signals (HIGH), it terminates the program. Do not worry about how or when the SIGALRM signals get sent.
// Hints: (i) you may find a global variable useful and (ii) avoid races. Do NOT worry about ANY errors.
#define LOW 3
#define HIGH 10

int totalSignals;
int killVal;
struct sigaction act;

// global declarations if any
void alarm_handler (int signo) {
	totalSignals = totalSignals + 1;

	if(totalSignals == LOW){
		printf("WARNING LOW");
	}
	else if(totalSignals == HIGH){
		printf("WARNING HIGH");
		// TERMINATE HERE (so need to allow a signal in or use unblockable signal)
		killVal = 1;
		raise(SIGKILL);
	}

}


void main () {
// set up signal handler for SIGALRM
	killVal = 0; 
    act.sa_handler = alarm_handler;
    act.sa_flags = 0; 
    sigfillset(&act.sa_mask);
	sigaction(SIGALRM, &act, NULL); 
     // assume main runs for a while ...
	// and a timer generates alarms ...(no need to write this part)


	// experimental solution
	if(killVal == 1){
		exit(1);
	}

}































// int main(){
//     numChildrenRemaining = N; 

//     // setup structs for sigaction call on SIGINT 
//     act.sa_handler = myFunc;
//     act.sa_flags = 0; 
//     sigfillset(&act.sa_mask);

//     // setup struct for the other sigaction call(to handle the SIGCHLD)
//     actChild.sa_handler = myChldFunc;
//     actChild.sa_flags = 0;
//     sigfillset(&actChild.sa_mask);


//     sigaction(SIGCHLD, &actChild, NULL); // prepping the SIGCHLD handler before any children are created, just to be safe

//     // This function creates N children
//     create_children(N);


//     sigaction(SIGINT, &act, numChildrenRemaining); // now ^C (aka SIGINT) signals are caught
//     // while children are running, no ^C

//     while(numChildrenRemaining > 0){
//         // parent waiting...
//         // wait();
//         sleep(1);
//         // sigwait(SIGCHLD);
//         pause();
//     }
//     // now we know all children have returned

//     // Hint: setting the handler function in the call to sigaction to SIG_DFL re-establishes the default 
//     // signal behavior. Do NOT worry about ANY errors.
//     act.sa_handler = SIG_DFL;
//     sigaction(SIGINT, &act, NULL); 
//     // now SIGINT will go back to its default action



//     // make sure all children are done
//     // rest of main code ... can allow ^C now

//     return 0;



// }












































































//Problem 4: Synchronization (25 Pts)
//Recall the Barrier construct discussed in lab and lecture. Each thread calls checkin at the barrier. This function blocks each thread until the last thread calls checkin, and at that point, all threads are released. Do NOT worry about ANY errors. Write checkin.

typedef struct {
    int n;  // number of threads to synchronize
    int num_ci; // number that have checked in
    lock L;
    condition CV;
    } Barrier;

void init (Barrier *B, int num) {
    B->n = num;
    B->num_ci = 0;
}



// Question: Why is it possible to use an IF statement instead of a while loop for blocking on the CV?
// Because we know the thread can't change the state (in this case 'B->num_ci).
// Spurious wake ups would be a bigger problem if B->num_ci was being updated after wakeup


void checkin (Barrier *B) {
	lock(B->L); // grab lock
	B->num_ci++; // incrementing 'num threads at barrier'
	while(B->num_ci < B->n){ // while the number of threads at the barrier is less than the num threads we want to synch...
		wait(&B->CV), &(B->L)); // wait on the CV, and release the lock in the meanwhile
		// wait(CV, L)  // --not sure how imp syntax is with this answer
	}
	signalBroadcast(B->CV); // wake up all threads at the barrier 
	
	unlock(B->L); // unlock
}









