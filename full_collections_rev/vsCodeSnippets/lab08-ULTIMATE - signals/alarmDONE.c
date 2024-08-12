#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// EXERCISE: 
// - Write a signal handler that handles SIGALRM and prints “Woke up for the # time”
// - Call alarm(5) three times then exit

// LINE NUMBERS MARKED W THE RED DOTS ARE CODE ADDED AS PART OF SOLUTION

int ALARMcount;
struct sigaction act; 						// SOLUTION

void AlarmSignalHandler(int sig) {

	//if another alarm occurs while in signal handler, ignore it
	signal(SIGALRM, SIG_IGN);

	//increment alarm counter, & print "Woke up for the # time"
	ALARMcount++; 							// SOLUTION
	printf("Woke up for the %d time\n",ALARMcount); // SOLUTION
	

	//if count < 3 call alarm again
	if(ALARMcount < 3){
	  alarm(5); // SOLUTION
	  sigaction(SIGALRM, &act, NULL); 		// SOLUTION
	}
	else {
		exit(0);
  }

	//reinstall the signal handler --// the solution ignored this part here so idk
}





int main(int argc, char*argv[]){

	//initialize counter
	ALARMcount = 0;

	//install signal handler for SIGALRM
	act.sa_handler = AlarmSignalHandler; 	// SOLUTION
	act.sa_flags = 0; 						// SOLUTION
	sigemptyset(&act.sa_mask); 				// SOLUTION
	sigaction(SIGALRM, &act, NULL); 		// SOLUTION

	//call alarm(5)
	alarm(5); 								// SOLUTION

	while(1){};
	return 0;
}
