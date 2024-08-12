#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	
	// take 'n' from the input and convert to integer
	// You can use strtol()
	int n; 
	int i;
	n = atoi(argv[1]);
	pid_t childpid = 0;


   for (i = 0; i < n; i++){

	   childpid = fork();

	   if(childpid == 0){
		   // these are instructions for child 
		   childpid = getpid();
		   printf("%d \n", getpid());
		   fflush(stdout);
		   execl("/bin/echo", "/bin/echo", "hello there", NULL); 
	   }

   }

   while(wait(NULL) > 0);
   // wait for ALL children?


   int last_id = fork();
   if(last_id == 0){
	   // child instructions:
	   char *args[] = {"/Users/alexking/4061 Lab Docs/lab3  4061/lab_activity/ptime", NULL};
	   execv(*args, args);
   }

	wait(NULL);


	return 0;
}