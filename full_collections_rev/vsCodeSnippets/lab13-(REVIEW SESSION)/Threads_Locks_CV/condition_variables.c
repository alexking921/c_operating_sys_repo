#define NUM_ARGS 0

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

long condTotal = 0;

struct queue {

	int vals[100];
	int index; //indicates the next available spot
};

struct condQueue {

	struct queue* q;
	pthread_cond_t* cond;
	pthread_mutex_t* mutex;
};



void insert(struct queue* q, int val) {

	q->vals[q->index] = val;
	++q->index;
}

int delete(struct queue* q) {

	--q->index;
	int val = q->vals[q->index];
	return val;
}

// TODO: Insert code to use a condition variable.
void *condProducer(void* arg) {

	// Random delay. DO NOT REMOVE!
	usleep(rand() % 1000);

	struct condQueue* cq = (struct condQueue*) arg;

	pthread_mutex_lock(cq->mutex);

	// Counter.
	static int in = 0;
	++in;

	// Add an element to the queue.
	insert(cq->q, in);

	pthread_cond_signal(cq->cond); //to send signal to any waiting consumer
	pthread_mutex_unlock(cq->mutex);
}

// TODO: Insert code to use a condition variable.
void *condConsumer(void* arg) {

	// Random delay. DO NOT REMOVE!
	usleep(rand() % 1000);

	struct condQueue* cq = (struct condQueue*) arg;

	pthread_mutex_lock(cq->mutex);
	while (cq->q->index < 1) pthread_cond_wait(cq->cond, cq->mutex);//wait if buffer is empty

	// Remove an element from the queue.
	condTotal += delete(cq->q);

	pthread_mutex_unlock(cq->mutex);
}

int main(int argc, char** argv) {

	if (argc != NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// Seed the random generator.
	srand(time(NULL));

	// Create threads.
	pthread_t condPool[100];

	struct timeval start;
	gettimeofday(&start, NULL);

	// Create the cond variable controlled task queue.
	struct condQueue* cq = (struct condQueue*) malloc(sizeof(struct condQueue));
	cq->q = (struct queue*) malloc(sizeof(struct queue));
	cq->q->index=0;

	// Allocate memory and initialize condition variable and mutex
	cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_cond_init(cq->cond, NULL);
	pthread_mutex_init(cq->mutex, NULL);

	// Launch them.
	for (int i=0; i < 50; ++i) {

		pthread_create(&condPool[i], NULL, condProducer, (void*) cq); //start 50 producer threads
		pthread_create(&condPool[50 + i], NULL, condConsumer, (void*) cq); //start 50 consumer threads
	}

	for (int i=0; i < 100; ++i) pthread_join(condPool[i], NULL); //wait for all the threads to be finished

	struct timeval end;
	gettimeofday(&end, NULL);

	printf("Cond Test: \nTotal of buffer = %ld\n", condTotal);
	printf("Time (in us) to run = %ld\n\n", ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec));

	}
