#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    int shared_val = 5;
    int * shared_dynamic = (int*) malloc(sizeof(int));
    *shared_dynamic = 5;

    pid_t pid = fork();
    pid_t terminated_pid;
    
    if (pid > 0) {

        //Waiting for child    
        terminated_pid = wait(NULL);
        

        //What would be the value of shared_val and shared_memory? 
        
        printf("In parent, Shared val : %d \n",shared_val);
        printf("In parent, Shared memory : %d \n", shared_dynamic);


    } else if (pid == 0) {
        
        shared_val += 10;
        *shared_dynamic = (*shared_dynamic) + 5;

        printf("In child, Shared val : %d \n",shared_val);
        printf("In child, Shared memory : %d \n", shared_dynamic);

    } else {

        printf("Failure creating child process (error number: %d)\n", errno);
        
    }

    return 0;
}