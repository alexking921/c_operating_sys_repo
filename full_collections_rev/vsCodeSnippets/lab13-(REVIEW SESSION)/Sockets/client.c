#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>

#define LOCALHOST "127.0.0.1"
#define MAX 516
#define PORT 44061
#define NCLIENTS 5
#define SA struct sockaddr

void func(int sockfd, int cid) {
    char clientID[MAX];
    memset(clientID, 0, MAX);
    sprintf(clientID, "%d", cid);

    if (write(sockfd, clientID, strlen(clientID)) < 0) {
        perror("Cannot write");
        exit(1);
    }
    printf("\tLogged in (client ID: %s) to server.\n", clientID);

    char recv[MAX];
    memset(recv, 0, MAX);
    if (read(sockfd, recv, MAX) < 0) {
        perror("cannot read");
        exit(1);
    }
    printf("\tMessage received from server: %s\n", recv);

    // close the socket
    close(sockfd);
}

int main(int argc, char **argv) {
    int cid;
    for (cid = 0; cid < NCLIENTS; cid++) {
        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;

        // socket create and verification
        // TODO: complete next line (socket)
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            printf("Socket creation failed...\n");
            exit(0);
        } else
            printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(LOCALHOST);
        servaddr.sin_port = htons(PORT);

        // connect the client socket to server socket
        // TODO: fill out if condition (connect)
        if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
            printf("Connection with the server failed...\n");
            exit(0);
        } else
            printf("Connected to the server..\n");

        pid_t pid = fork();
        if (pid == 0) {
            func(sockfd, cid);
            exit(0);
        } else if (pid < 0) {
            printf("Failure creating child process (cid: %d)\n", cid);
        }
    }

    while(wait(NULL) > 0);
    return 0;
}