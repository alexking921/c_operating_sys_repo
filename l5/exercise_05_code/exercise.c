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
   DIR* dir = opendir(path);
   struct dirent* entry;
   int count = 0;
   while((entry = readdir(dir)) != NULL){
       count = count + 1; 
   }
    int closedir(dir);
    return count; 
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
	TODO:   
	Iterate through the elements in argv[1]
	1.Check DT_DIR and DT_REG
	2.What if the entry is "." and ".."
    */

    int i;
    int len = numOfEntries(path);
    for(i = 0; i < len; i++){ // loop iterations 
        
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            printf("Met .. or . \n");
        }
        else if(entry->d_type == DT_DIR){ // if its a file...
            // print name of dir 
            // print type of dir
            // print number of files in it
            printf("Directory %S\n", entry->d_name);
            char* subpath = path;
            strcat(subpath, "/");
            strcat(subpath, entry->d_name);
            int entries = numOfEntries(subpath);
            printf("Entries: %d\n", entries);

        }



        else if(entry->d_type == DT_REG){ // if it's a directory...
            // print name
            // print type
            // print owner
            // print size
            // print inode #
            printf("Regular File: (non-dir) %S\n", entry->d_name);
            char* filePath = path;
            // strcat(filePath, "/");
            // strcat(filePath, entry->d_name);
            struct stat* buff = (struct stat *)malloc((sizeof(struct stat))); // alloc mem
            
            int s = stat(filePath, buff);
            if(s < 0){
                perror("Failed to get stats");
                exit(1);
            }

            printf("owner: %S\n", buff->st_uid);
            printf("size: %S\n", buff->st_size);
            printf("inode: %S\n", buff->st_ino);
            
            // DEALLOCATE MEMORY !!! 
            free(buff);

        }



        else{ // if anything other than a file or dir...
            printf("Not a dir or file: %S\n", entry->d_name);
        }



    }
    
    
    closedir(dir);
    return 0;
}
