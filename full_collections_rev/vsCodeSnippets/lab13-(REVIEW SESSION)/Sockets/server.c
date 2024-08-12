#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 516
#define PORT 44061
#define NCLIENTS 5
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd) {
    char clientid[MAX];
    memset(clientid, 0, MAX);
    int size = read(sockfd, clientid, sizeof(clientid));
    if (size < 0) {
        perror("read error");
        exit(1);
    }
    printf("\tClient (client ID: %s) has logged in.\n", clientid);

    char buffer[MAX];
    memset(buffer, 0, MAX);
    sprintf(buffer, "Hi! Client %s", clientid);
    if (write(sockfd, buffer, strlen(buffer)) < 0) {
        perror("write error");
        exit(1);
    }
    printf("\tSent \"%s\" to client.\n", buffer);
}




















int main(int argc, char **argv) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *) &servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, NCLIENTS)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");
    len = sizeof(cli);

    while(1) {
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA *) &cli, &len);
        if (connfd < 0) {
            printf("Server accept failed...\n");
            exit(0);
        } else
            printf("Server accept the client...\n");

        // Function for chatting between client and server
        func(connfd);
    }

    // Server never shut down
    close(sockfd);
}