#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int ALARMcount;
struct sigaction act;

void AlarmSignalHandler(int sig) {
	
	//if another alarm occurs while in signal handler, ignore it
	signal(SIGALRM, SIG_IGN); 
 
	//increment alarm counter
	ALARMcount++;
	printf("Woke up for the %d time\n",ALARMcount);

	//if count < 3 call alarm again	
	if(ALARMcount< 3){
	  alarm(5);
	  sigaction(SIGALRM,&act,NULL);
	}
	else{
    		exit(0);
    	}
}

int main(int argc, char*argv[]){
    // struct sigaction act;

    //initialize counter
    ALARMcount = 0;

    act.sa_handler=AlarmSignalHandler;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    //install signal handler for SIGALRM
 
    //call alarm(5) 
    alarm(5);
    
    while(1){}
    return 0;
} 
