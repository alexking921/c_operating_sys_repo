#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	
	int nProcess = strtol(argv[1], NULL, 0);
	pid_t wpid;
	int status = 0;
	int i;
	for(i = 0; i < nProcess; i ++){
		pid_t pid;
		if((pid =fork()) == 0) {
			printf("%d\n", getpid());

			if (execl("/bin/echo", "/bin/echo", "hello", "there", NULL) < 0) {
				printf("ERROR: execl failed\n");
				exit(1);
			}
		} else {
			// alternate way to wait for child processes
			// In this case a child is created only when previous child exits
			// wait(NULL);
		}
	}

	while ((wpid = wait(&status)) > 0);

	// or
	// for(i = 0; i < nProcess; i++)
	// 	wait(NULL);

	pid_t pid = fork();
	if(pid == 0){
		char *args[] = {"ptime", NULL};
		if (execv(*args, args) < 0) {
			printf("ERROR: execv failed\n");
			exit(1);
		}
	}
	wait(NULL);

	return 0;
}