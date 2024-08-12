#include "utils.h"

char *getChunkData(int mapperID){
	static int entryTime = 0;
	static int msgqueue;

	// one time opening of message queue using static variables
	if(entryTime == 0) {
		key_t key = ftok("test", 65);
		if((msgqueue = msgget(key, 0666 | IPC_CREAT))<0){
			printf("error");
			exit(1);
		}
	}
	entryTime = 1;


	// receive chunk from the master
	struct msgBuffer chunk;
	memset(chunk.msgText, '\0', MSGSIZE);
	
	if(msgrcv(msgqueue, (void*) &chunk, sizeof(chunk.msgText), mapperID, 0) < 0){
		printf("Error occured for CHUNKRECV: %s\n", strerror(errno));
		exit(0);
	}
	
	if(strcmp(chunk.msgText, "END") == 0){
		// send ack to master and then return NULL
		struct msgBuffer chunk;
		memset(chunk.msgText, '\0', MSGSIZE);
		strcpy(chunk.msgText, "ACK");
		chunk.msgType = ENDTYPE;

		int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
		if(ret == -1){
			printf("Error occured for ENDACK: %s\n", strerror(errno));
			exit(0);
		}
		return NULL;
	}

	char *chunkData = (char *) malloc(sizeof(char) * (chunkSize + 1));
	memset(chunkData, '\0', chunkSize + 1);
	strcpy(chunkData, chunk.msgText);

	return chunkData;
}

//return the next word as an output parameter
//return 1: indicates read a word
//retrun 0: indicates reach at the end of the stream
int getNextWord(int fd, char* buffer){
   char word[100];
   memset(word, '\0', 100);
   int i = 0;
   while(read(fd, &word[i], 1) == 1 ){
    if(word[i] == ' '|| word[i] == '\n' || word[i] == '\t'){
        strcpy(buffer, word);
        return 1;
    }
    if(word[i] == 0x0){
      break;
    }

    i++;
   }
   strcpy(buffer, word);
   return 0;
}

// sends chunks of size 1024 to the mappers in RR fashion
void sendChunkData(char *inputFile, int nMappers) {

	// open the message queue
	key_t key = ftok("test", 65);
	int msgqueue;
	if((msgqueue = msgget(key, 0666 | IPC_CREAT))<0){
		printf("error");
		exit(1);
	} else {
		msgctl(msgqueue, IPC_RMID, NULL);
		if((msgqueue = msgget(key, 0666 | IPC_CREAT))<0){
			printf("error");
			exit(1);
		}
	}

	// open the input file
	int fd = open(inputFile, O_RDONLY);
	if (fd < 0){
		printf("ERROR: %s do not exists\n", inputFile);
		exit(0);
	}

	int i = 0;
	char readArray[chunkSize + 1]; // +1 for '\0'
	memset(readArray, '\0', chunkSize + 1);

	
	//single word - end of word found using \n, space or ROF
	char buffer[100];
	memset(buffer, '\0', 100);
  while(getNextWord(fd, buffer) !=0 ){
    if(strlen(readArray) + strlen(buffer) > 1024){
      struct msgBuffer chunk;
      memset(chunk.msgText, '\0', MSGSIZE);
      strcpy(chunk.msgText, readArray);
      chunk.msgType = (i % nMappers) + 1;
      i++;
      // send the chunk to mapper i
      int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
      if(ret == -1){
            printf("Error occured for CHUNKSEND: %s\n", strerror(errno));
            exit(0);
      }
      memset(readArray, '\0', chunkSize + 1);
    }

    strcat(readArray, buffer);
    memset(buffer, '\0', 100);
  }

  //Check for final word in the buffer
  if(strlen(buffer) > 0)
    strcat(readArray, buffer);

  //Send final chunk of memory
  if(strlen(readArray) > 0){
    struct msgBuffer chunk;
    memset(chunk.msgText, '\0', MSGSIZE);
    strcpy(chunk.msgText, readArray);

    chunk.msgType = (i % nMappers) + 1;
    i++;
    int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
    if(ret == -1){
      printf("Error occured for CHUNKSEND: %s\n", strerror(errno));
      exit(0);
    }
  }

	close(fd);

	//inputFile read complete, send END message to mappers
	for(i = 0; i < nMappers; i++) {
		struct msgBuffer chunk;
		memset(chunk.msgText, '\0', MSGSIZE);
		strcpy(chunk.msgText, "END");
		chunk.msgType = (i % nMappers) + 1;

		int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
		if(ret == -1){
			printf("Error occured for CHUNKEND: %s\n", strerror(errno));
			exit(0);
		}
	}

	// wait to receive ACK from all mappers for END notification
	for(i = 0; i < nMappers; i++) {
		struct msgBuffer chunk;
		memset(chunk.msgText, '\0', MSGSIZE);

		if(msgrcv(msgqueue, (void*) &chunk, sizeof(chunk.msgText), ENDTYPE, 0) < 0) {
			printf("Error occured for ENDACKRECV: %s\n", strerror(errno));
			exit(0);
		}
	}

	//close msgqueue
	msgctl(msgqueue, IPC_RMID, NULL);
}

// hash function to divide the list of word.txt files across reducers
//http://www.cse.yorku.ca/~oz/hash.html
int hashFunction(char* key, int reducers){
	unsigned long hash = 0;
    int c;

    while ((c = *key++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

    return (hash % reducers);
}


int getInterData(char *key, int reducerID) {
	static int entryTime = 0;
	static int msgqueue;

	// one time opening of message queue using static variables
	if(entryTime == 0) {
		key_t mq_key = ftok("src", 66);
		if((msgqueue = msgget(mq_key, 0666 | IPC_CREAT))<0){
			printf("error");
			exit(1);
		}
	}
	entryTime = 1;

	struct msgBuffer chunk;
	memset(chunk.msgText, '\0', MSGSIZE);
	if (msgrcv(msgqueue, (void*) &chunk, sizeof(chunk.msgText), reducerID, 0) < 0) {
		printf("ERROR: Unable to KEYRECV: %s\n", strerror(errno));
		exit(0);
	}

	if(strcmp(chunk.msgText, "END") == 0){
		// send ack to master and then return NULL
		memset(chunk.msgText, '\0', MSGSIZE);
		strcpy(chunk.msgText, "ACK");
		chunk.msgType = ACKTYPE;

		int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
		if(ret == -1){
			printf("ERROR: Unable to KEYACK: %s\n", strerror(errno));
			exit(0);
		}
		return 0;
	}
	strcpy(key, chunk.msgText);
	return 1;
}

void shuffle(int nMappers, int nReducers) {
	// open the message queue
	key_t key = ftok("src", 66);
	int msgqueue;
	if((msgqueue = msgget(key, 0666 | IPC_CREAT))<0){
		printf("error");
		exit(1);
	} else {
		msgctl(msgqueue, IPC_RMID, NULL);
		if((msgqueue = msgget(key, 0666 | IPC_CREAT))<0){
			printf("error");
			exit(1);
		}
	}

	int i;
	char mapOutDir[50];
	// traverse the directory of each Mapper and send the word filepath to the reducers
	for(i = 1; i <= nMappers; i++){
		memset(mapOutDir, '\0', 50);
		sprintf(mapOutDir, "output/MapOut/Map_%d", i);

		//check if the directory exists
		DIR *dir = opendir(mapOutDir);
		if(dir == NULL){
			printf("ERROR: Unable to read directory %s\n", mapOutDir);
			exit(1);
		}

		struct dirent *dirContentPtr;

		//use https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
		while((dirContentPtr = readdir(dir)) != NULL){
			if (strcmp(dirContentPtr->d_name, ".") != 0 &&
				strcmp(dirContentPtr->d_name, "..") != 0 &&
	      strcmp(dirContentPtr->d_name, ".DS_Store") != 0 &&
	      (dirContentPtr->d_name[0] != '.')){
				if (dirContentPtr->d_type == DT_REG){
					//file
					char filePath[100];
					memset(filePath, '\0', 100);
					strcpy(filePath, mapOutDir);
					strcat(filePath, "/");
					strcat(filePath, dirContentPtr->d_name);

					struct msgBuffer chunk;
					memset(chunk.msgText, '\0', MSGSIZE);
					strcpy(chunk.msgText, filePath);

					// reducer selection
					chunk.msgType = hashFunction(dirContentPtr->d_name, nReducers) + 1 ;
					// send word filepath to reducer
					int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
					if(ret == -1){
						printf("Error occured for KEYSEND: %s\n", strerror(errno));
						exit(0);
					}
				}
			}
		}
		closedir(dir);
	}

	//inputFile read complete, send END message to reducers
	for(i = 0; i < nReducers; i++) {
		struct msgBuffer chunk;
		memset(chunk.msgText, '\0', MSGSIZE);
		strcpy(chunk.msgText, "END");
		chunk.msgType = i + 1;

		int ret = msgsnd(msgqueue, (void *) &chunk, strlen(chunk.msgText) + 1, 0);
		if(ret == -1){
			printf("Error occured for CHUNKEND: %s\n", strerror(errno));
			exit(0);
		}
	}

	// wait for ACK from the reducers for END notification
	for(i = 0; i < nReducers; i++) {
		struct msgBuffer chunk;
		memset(chunk.msgText, '\0', MSGSIZE);

		msgrcv(msgqueue, (void*) &chunk, sizeof(chunk.msgText), ACKTYPE, 0);
	}

	//close msgqueue
	msgctl(msgqueue, IPC_RMID, NULL);
}

// check if the character is valid for a word
int validChar(char c){
	return ((tolower(c) >= 'a') && tolower(c) <='z') ||
					(c >= '0' && c <= '9');
}

char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		// printf("%d\n", i);
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				// printf("%s\n", buffer);
				// fflush(stdout);
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	if(strlen(buffer) > 0){
		// printf("%s\n", buffer);
		// fflush(stdout);
		return buffer;

	}
	free(buffer);
	return NULL;
}

void createOutputDir(){
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}
