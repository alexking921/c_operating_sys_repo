#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int numOfEntries(char* path) {
    /*
	TODO: 
        Count the number of entries in path
    */
}

int main(int argc, char** argv){
    if (argc < 2) {
        printf("Pass the path as an argument to the program");
        exit(1);
    }

    char* path = argv[1];

    DIR* dir = opendir(path);
    if(dir==NULL){
        printf("The path passed is invalid");
        return -1;
    }
    struct dirent* entry;
    
    /*
	TODO: *DONE  
	Iterate through the elements in directory(aka argv[1])
	1.Check DT_DIR and DT_REG
	2.What if the entry is "." and ".." 
    */
    
    closedir(dir);
    return 0;
}



// void Extract (char *infile, char *outfile, int from, int to){
    
//     int numBytes = (from - to);
//     char buff[numBytes]; // make room for tot bytes needing to read
//     int fd1 = open(infile, O_RDONLY); // open file to read

//     fseek(fd1, from, SEEK_SET); // moves the reading pointer to spot thats exactly 'from' num bytes from the start instead of the default position @ start of file
//     ssize_t r = read(fd1, buff, numBytes); //

//     // now we have the data we need from infile so can close
//     int close(fd1);

//     int fd2 = open(outfile, O_WRONLY);
//     ssize_t w1 = write(fd2, buff, numBytes);

//     int close(fd2);








//     //int close(fd);  // 
// }



// int main(int argc, char** argv){






//     return 0;
// }
