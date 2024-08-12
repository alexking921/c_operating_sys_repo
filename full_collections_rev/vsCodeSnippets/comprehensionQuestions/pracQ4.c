#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
/* 
QUESTION: 
    Modify the reader-writer lock example to help waiting writers. When the number of current 
    readers exceeds R, then block readers until the lock becomes free. Modify the appropriate 
    functions. Do NOT worry about ANY errors.

    ** THE CODE GIVEN FOR THIS QUESTION SEEMS TO BE BORDERLINE PSEUDO CODE so VS editor not as helpful in pointing out errors

*/


// SETUP CODE FOR READER/ WRITER 
lock L;
condition CV;
// 0 unheld; -1 writer has it; > 0 readers have it int num_readers = 0;
int num_readers = 0; 
int R;



// READER-LOCK // SOLUTION IDEA 1
acquire_rlock () {
  lock (&L);

  if(num_readers > R){ // MY SOLUTION ATTEMPT
    // block readers   // MY SOLUTION ATTEMPT
    wait(&CV, &L);           // MY SOLUTION ATTEMPT
  }                   // MY SOLUTION ATTEMPT
  else{               // MY SOLUTION ATTEMPT
    while (num_readers < 0){
      wait (&CV, &L);
    }
    num_readers++;
    unlock (&L);
  }
}




// READER-LOCK // SOLUTION IDEA 2
acquire_rlock () {
  lock (&L);
  while (num_readers < 0 || num_readers > R){ // MY SOLUTION ATTEMPT #2
    wait (&CV, &L);
  }
  num_readers++;
  unlock (&L);
}









// WRITER-LOCK
acquire_wlock () { // this does not change!
  lock (&L);
  while (num_readers != 0)
    wait (&CV, &L);
  num_readers = -1;
  unlock (&L);
}





// RELEASE (same for both READER and WRITER)
release(){
  lock (&L);
  if(num_readers == -1){
    num_readers = 0;
  }
  else{
    num_readers--;
  }
  if(num_readers == 0) {
    broadcast (&CV, &L);
  }
  unlock (&L);
}





int main(){
    









    return 0;
}