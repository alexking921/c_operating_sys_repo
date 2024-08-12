#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	
    //How many processes are created? 
	if(fork() && fork()){
        fork();
    }
    printf("Hello world!");

	return 0;
}