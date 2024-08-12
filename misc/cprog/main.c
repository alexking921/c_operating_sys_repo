#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    
    int n = 5; 
    pid_t childpid = 0;
    int i;
   
    for (i = 0; i < n; i++){
        //printf("INSIDE the LOOP (should happen 5 times) \n");
        fflush(stdout);

       
       
      childpid = fork();
       
      if (childpid == 0){
        
        // instructions for the child here
        childpid = getpid();
        printf("I'm a child process w/ ID: %d \n", childpid);
        fflush(stdout);
        exit(0);
      }
      wait(NULL);
      
    }
    //wait()
    
    fprintf(stderr, "i:%d  process ID:%ld  parent ID:%ld  child ID:%ld\n", i, (long)getpid(), (long)getppid(), (long)childpid);
    return 0; 
    
}