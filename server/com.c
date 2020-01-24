#include "com.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "logger.h"
#include "server.h"

client_t tcpClients[4];
int nbClients = 0;

void printClients() {
    int i;

    for (i = 0; i < nbClients; i++)
        printf("%d: %s %5.5d %s\n",
               i,
               tcpClients[i].ipAddress,
               tcpClients[i].port,
               tcpClients[i].name);
}

int findClientByName(char *aName) {
    int i;

    for (i = 0; i < nbClients; i++) {
        if (strcmp(tcpClients[i].name, aName) == 0) {
            return i;
        }
    }
    return -1;
}

void sendMessageToClient(char *aClientIp, int aClientPort, char *aMess) {
    int sockfd; // Socket file descriptor
    int n; // The amount of written chars
    struct sockaddr_in servAddr; // The address of the client's server
    struct hostent *server; // Server structure from the targeted client
    char buffer[256]; // the message content buffer

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Retrieves the server informations
    server = gethostbyname(aClientIp);
    if (server == NULL) {
        errorExit("ERROR, no such host\n");
        exit(0);
    }
    // Initializes the server address informations
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&servAddr.sin_addr.s_addr,
          server->h_length);
    servAddr.sin_port = htons(aClientPort);
    // Connects to the server
    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        errorExit("ERROR connecting");
        exit(1);
    }
    // Writes the message
    sprintf(buffer, "%s\n", aMess);
    n = write(sockfd, buffer, strlen(buffer));
    if (n == -1) {
        errorExit("Failed to write to the socket");
    }

    close(sockfd);
    log_f(INFO, "To %s:%d : %s\n", aClientIp, aClientPort, aMess);
}

void broadcastMessage(char *aMess) {
    int i;

    for (i = 0; i < nbClients; i++) {
        sendMessageToClient(tcpClients[i].ipAddress, tcpClients[i].port, aMess);
    }
}
