/**
 * A simple server in the internet domain using TCP
 * The port number is passed as an argument
 */

#include "server.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cartes.h"
#include "com.h"
#include "logger.h"
#include "msg.h"
#include "state_machine.h"

int gCurrentPlayer;
int gExcludedPlayer[4];

/**
 * Prints the error and exit.
 */
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd; // Socket's file descriptor.
    int newsockfd; // Socket's file descriptor on incoming message.
    int portno; // The server's port.
    char buffer[256]; // Message buffer.
    struct sockaddr_in serv_addr; // Server's address.
    struct sockaddr_in cli_addr; // Sender's address.
    socklen_t clilen; // Length of the sender's address.
    int n; // The amount of char read.
    int i; // Iteration variable.

    // Ensures there are enough arguments.
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // Creates the socket ; exists on failure.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    // Initializes the server's address.
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    // Binds the socket to the port & prepares it ; exists on failure.
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    // Shuffles the hands & prepare the game.
    shuffleDeck();
    createTable();
    puts("Deck:");
    printDeck();
    puts("");
    // Initializes active player selection variables.
    gCurrentPlayer = 0;
    memset(gExcludedPlayer, 0, 4);
    // Initializes clients address as dummy values.
    for (i = 0; i < 4; i++) {
        strcpy(tcpClients[i].ipAddress, "localhost");
        tcpClients[i].port = -1;
        strcpy(tcpClients[i].name, "-");
    }
    // Main loop
    while (1) {
        // Waits for an incoming message ; exists on failure.
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }
        // Reads the message's content ; exists on failure.
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        printf("Received packet from %s:%d\nData: [%s]\n\n",
               inet_ntoa(cli_addr.sin_addr),
               ntohs(cli_addr.sin_port),
               buffer);
        // Lets the state machine handle the message
        onMsg(buffer);

        close(newsockfd);
    } // End : Main loop
    close(sockfd);
    return 0;
}

void endRound() {
    int i = 0;
    do {
        gCurrentPlayer = (gCurrentPlayer + 1) % 4;
        i++;
    } while (gExcludedPlayer[gCurrentPlayer] && i < 4);
    // Sends the message on success
    if (i != 4) {
        broadcastCurrentPlayer(gCurrentPlayer);
    } else {
        warn("Tous les joueurs ont été éliminé!");
    }
}
