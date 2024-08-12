/* 
QUESTION: 
Processes A and B wish to exchange messages (“hello” and “bye”). 
However, this program has a fatal flaw. 
What is it and how can it be easily fixed? 
(Hint: The problem is not with the size of messages nor with IPC_CREAT).


CODE OF PROCESS A 
mymsg_t m1 = {15, “hello”}, m2; int mid;
key_t key = 100;
mid = msgget (key,
           IPC_CREAT| 0777);
msgrcv (mid, (void *)&m2,
     sizeof(mymsg_t)-
     sizeof(long), 20, 0);
msgsnd (mid, (void *)&m1,
     sizeof(mymsg_t)-
     sizeof(long), 0);



CODE OF PROCESS B
mymsg_t m1 = {20, “bye”}, m2;
int mid;
key_t key = 100;
mid = msgget (key,
     IPC_CREAT| 0777);
msgrcv (mid, (void *)&m2,
     sizeof (mymsg_t)-
     sizeof (long), 15, 0);
msgsnd (mid, (void *)&m1,
     sizeof(mymsg_t)-
     sizeof(long)), 0);




*/