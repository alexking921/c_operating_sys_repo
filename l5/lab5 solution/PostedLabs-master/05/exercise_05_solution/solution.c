#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

// count the number of entries in path
int numOfEntry(char* path) {
    int count=0;
    DIR* dir = opendir(path);
    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) 
            continue;
        
        count++;
    }
    
    closedir(dir);

    return count;
}








int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Pass the path as an argument to the program");
        exit(1);
    }














    












    //char* path = argv[1];

    char* path = "/home/etc/blah/blah";
    DIR* dir = opendir(path);
    if(dir==NULL){
        printf("The path passed is invalid");
        return -1;
    }
    struct dirent* entry;




     /*
	TODO: *DONE  
	Iterate through the elements in argv[1]
	1.Check DT_DIR and DT_REG
	2.What if the entry is "." and ".." 
    */
    // while ((entry = readdir(dir)) != NULL) { // while the dir is not empty...
        
        // if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) 
        //     continue;

        
    //     if (entry->d_type == DT_DIR) {
    //         char next[strlen(path) + strlen(entry->d_name) + 2];
    //         next[0] = '\0';
    //         strcat(next, path);
    //         strcat(next, "/");
    //         strcat(next, entry->d_name);
    //         printf("Directory: %s\n\tEntries: %d\n", entry->d_name, numOfEntry(next));
    //     } 
    //     else if (entry->d_type == DT_REG) {
            // struct stat* buf = (struct stat*)malloc(sizeof(struct stat));
            // stat(entry->d_name, buf); // FREE buf LATER!! like: free(buf);

    //         printf("Regular File: %s\n\tOwner: %d\n\tSize: %f\n\tInode: %llu\n",
    //             entry->d_name, buf->st_uid, (double) buf->st_size, buf->st_ino);
    //         free(buf);
    //     }
    //     else {
    //         printf("File: %s\n\tType:%hhu\n", entry->d_name, entry->d_type);
    //     }
    // }
    
    // closedir(dir);




    // MY ATTEMPT: 

    int searchThisDir(char *path){
        // open dir
        char* path = "/home/etc/blah/blah";
        DIR* dir = opendir(path);
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
       
            // check if entry is file or dir
            if(entry->d_type == DT_DIR){
                // its a dir
                // NEXT 5 lines just building string to define the path to 
                char next[strlen(path) + strlen(entry->d_name) + 2];
                next[0] = '\0';
                strcat(next, path);
                strcat(next, "/");
                strcat(next, entry->d_name);
                searchThisDir(next);
            }
            else{ 
                // its a file...
                // getting meta data of this file
                struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
                stat(entry->d_name, buff); // FREE buf LATER!! like: free(buf);

                }
        } // end of while loop
        
        close(dir);    
        return 0;
    }









    while ((entry = readdir(dir)) != NULL) { // while the dir is not empty...
        
        // skip these entries
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
        continue;
        }

        // strcmp return 0 if both strings are same
        int nameComparison = strcmp(entry->d_name, "search_name");
        if(nameComparison == 0){
            // found match -- so copy this files FULL PATH to save

        }


        // if the current thing is another directory...then call the recursion method on it
        if(entry->d_type == DT_DIR){
            // call recursion on this dir
        }



        
        if (entry->d_type == DT_DIR) {
            char next[strlen(path) + strlen(entry->d_name) + 2];
            next[0] = '\0';
            strcat(next, path);
            strcat(next, "/");
            strcat(next, entry->d_name);
            printf("Directory: %s\n\tEntries: %d\n", entry->d_name, numOfEntry(next));
        } 
        else if (entry->d_type == DT_REG) {
            struct stat* buf = (struct stat*)malloc(sizeof(struct stat));
            stat(entry->d_name, buf);
            printf("Regular File: %s\n\tOwner: %d\n\tSize: %f\n\tInode: %llu\n",
                entry->d_name, buf->st_uid, (double) buf->st_size, buf->st_ino);
            free(buf);
        }
        else {
            printf("File: %s\n\tType:%hhu\n", entry->d_name, entry->d_type);
        }
    }
    
    closedir(dir);    
    return 0;
}