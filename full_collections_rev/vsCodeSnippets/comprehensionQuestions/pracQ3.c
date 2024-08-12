/* 
QUESTION: 
    The SIGINT signal indicates that a ^C is sent to your process. The default action is to terminate the process.
    If your program has children, then they will become orphans. We will prevent this. Your main program will 
    call create_children which will create N children as given to you, and you will modify the main program to
    prevent a ^C from terminating it if children are still running. If a ^C is hit while children are running,
    you should print: NOT YET: “k children still running” (where k is the number of children still running).
    ^C should work as usual after all children are finished.
    Hint: setting the handler function in the call to sigaction to SIG_DFL re-establishes the default 
    signal behavior. Do NOT worry about ANY errors.

*/
    
// MY IDEA-
// create handler for SIGCHLD and every time we get a SIGCHLD signal, we decrement numChildrenRemaining,
// then once numChildrenRemaining <= 0, we know all children have exited and can do myFunc =  
// SIGCHLD : child sends SIG

#include <stdio.h>
#include <signal.h>
#define N 10;

// int numSigChldSignals; we could also simply count the num of SIGCHLD calls until it == N
int numChildrenRemaining; 
struct sigaction act;
struct sigaction actChild;  // this is to handle the SIGCHLD


void create_children(int n){
    // function to create N children
}

void myFunc(int num){
    // I dont think we have access to 'numChildrenRemaining' so that's the arg we should pass to myFunct, right?
    printf("No ^C bc there's a total of %d children remaining", num);
}

void myChldFunc(int num){
    numChildrenRemaining = (numChildrenRemaining - 1);
}



int main(){
    numChildrenRemaining = N; 

    // setup structs for sigaction call on SIGINT 
    act.sa_handler = myFunc;
    act.sa_flags = 0; 
    sigfillset(&act.sa_mask);

    // setup struct for the other sigaction call(to handle the SIGCHLD)
    actChild.sa_handler = myChldFunc;
    actChild.sa_flags = 0;
    sigfillset(&actChild.sa_mask);


    sigaction(SIGCHLD, &actChild, NULL); // prepping the SIGCHLD handler before any children are created, just to be safe

    // This function creates N children
    create_children(N);


    sigaction(SIGINT, &act, numChildrenRemaining); // now ^C (aka SIGINT) signals are caught
    // while children are running, no ^C

    while(numChildrenRemaining > 0){
        // parent waiting...
        // wait();
        sleep(1);
        // sigwait(SIGCHLD);
        pause();
    }
    // now we know all children have returned

    // Hint: setting the handler function in the call to sigaction to SIG_DFL re-establishes the default 
    // signal behavior. Do NOT worry about ANY errors.
    act.sa_handler = SIG_DFL;
    sigaction(SIGINT, &act, NULL); 
    // now SIGINT will go back to its default action



    // make sure all children are done
    // rest of main code ... can allow ^C now

    return 0;



}










































/****************************************************************************************
  * ANOTHER SOLUTION WHERE I JUST USE A GLOBAL VARIABLE INSTEAD OF PASSING ANY ARGS TO THE myFunc() which sorta complicates the code more than it might have to 
*****************************************************************************************/
 























