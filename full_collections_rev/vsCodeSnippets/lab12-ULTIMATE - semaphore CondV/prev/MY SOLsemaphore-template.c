/*
Breakout Number: 
king1493
Members: Alex King

*/

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
#include<sys/time.h>

long semTotal = 0;

struct buffer {

	int vals[100];
	int index;
};

struct semBuffer {

	struct buffer* q;
	sem_t* psem;
	sem_t* csem;
	pthread_mutex_t* mutex;
};

void insert(struct buffer* q, int val) {

	q->vals[q->index] = val;
	++q->index;
}

int delete(struct buffer* q) {

	--q->index;
	int val = q->vals[q->index];
	return val;
}









// TODO: Insert code to use a semaphore.
void* semProducer(void* arg) {

	// Random delay. DO NOT REMOVE!
	usleep(rand() % 1000);

	struct semBuffer* sq = (struct semBuffer*) arg;
	
	static int in = 0;
	++in;

	sem_wait(sq->psem);

	pthread_mutex_lock(sq->mutex);
	// Add an element to the queue.
	insert(sq->q, in);
	pthread_mutex_unlock(sq->mutex);

	sem_post(sq->csem);


}






// TODO: Insert code to use a semaphore.
void* semConsumer(void* arg) {

	// Random delay. DO NOT REMOVE!
	usleep(rand() % 1000);

	struct semBuffer* sq = (struct semBuffer*) arg;


	sem_wait(sq->csem);
	pthread_mutex_lock(sq->mutex);
	// Reove an element from the queue.
	semTotal += delete(sq->q);
	pthread_mutex_unlock(sq->mutex);

	sem_post(sq->psem);
}







int main(int argc, char** argv) {

	if (argc != NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// Seed the random generator.
	srand(time(NULL));

	// Create threads.
	pthread_t semPool[100];

	struct timeval start;
	gettimeofday(&start, NULL);

	// Create semaphore controlled task queue.
	struct semBuffer* sq = (struct semBuffer*) malloc(sizeof(struct semBuffer));

	sq->q = (struct buffer*) malloc(sizeof(struct buffer));
	sq->q->index=0;
	sq->psem = (sem_t*) malloc(sizeof(sem_t));
	sq->csem = (sem_t*) malloc(sizeof(sem_t));
	sq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

	// TODO: Initialize semaphores 
	sem_init(sq->psem, 0, 50);
	sem_init(sq->csem, 0, 0);

	pthread_mutex_init(sq->mutex, NULL);

	// Creating producers and consumers
	for (int i=0; i < 50; ++i) {

		pthread_create(&semPool[i], NULL, semProducer, (void*) sq);
		pthread_create(&semPool[50 + i], NULL, semConsumer, (void*) sq);
	}

	for (int i=0; i < 100; ++i) pthread_join(semPool[i], NULL); //wait for all the threads to be finished

	struct timeval end;
	gettimeofday(&end, NULL);

	// Validating the results
	printf("Sem Test: \nTotal of buffer  = %ld\n", semTotal);
	printf("Time (in us) to complete = %ld\n", ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec));
}
