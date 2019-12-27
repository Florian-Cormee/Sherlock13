#include "com.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

int findClientByName(char *name) {
    int i;

    for (i = 0; i < nbClients; i++)
        if (strcmp(tcpClients[i].name, name) == 0) return i;
    return -1;
}

void sendMessageToClient(char *clientip, int clientport, char *mess) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(clientip);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(clientport);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting\n");
        exit(1);
    }

    sprintf(buffer, "%s\n", mess);
    n = write(sockfd, buffer, strlen(buffer));
    if(n == -1) {
        puts("[ERROR] Failed to write to the socket");
    }

    close(sockfd);
    printf("To %s:%d : %s\n", clientip, clientport, mess);
}

void broadcastMessage(char *mess) {
    int i;

    for (i = 0; i < nbClients; i++)
        sendMessageToClient(tcpClients[i].ipAddress, tcpClients[i].port, mess);
}
