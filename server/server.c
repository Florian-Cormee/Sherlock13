/* A simple server in the internet domain using TCP
The port number is passed as an argument */
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
#include "msg.h"

int fsmServer;
int joueurCourant;

/*
 * Prints the error and exit.
 */
void error(const char *msg) {
    perror(msg);
    exit(1);
}

void on_msg_in_connection_state(char buffer[256]);

void on_msg_in_playing_state(char buffer[256]);

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int i;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // printDeck();
    melangerDeck();
    createTable();

    puts("Deck:");
    printDeck();
    puts("");

    joueurCourant = 0;

    for (i = 0; i < 4; i++) {
        strcpy(tcpClients[i].ipAddress, "localhost");
        tcpClients[i].port = -1;
        strcpy(tcpClients[i].name, "-");
    }

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");

        printf("Received packet from %s:%d\nData: [%s]\n\n",
               inet_ntoa(cli_addr.sin_addr),
               ntohs(cli_addr.sin_port),
               buffer);

        if (fsmServer == 0) {
            on_msg_in_connection_state(buffer);
        } else if (fsmServer == 1) {
            on_msg_in_playing_state(buffer);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}

void on_msg_in_connection_state(char buffer[256]) {
    char clientIpAddress[256];
    char clientName[256];
    int clientPort;
    char com;
    int id;

    switch (buffer[0]) {
    case 'C':
        sscanf(buffer,
               "%c %s %d %s",
               &com,
               clientIpAddress,
               &clientPort,
               clientName);
        printf("COM=%c ipAddress=%s port=%d name=%s\n",
               com,
               clientIpAddress,
               clientPort,
               clientName);

        // fsmServer==0 alors j'attends les connexions de tous les
        // joueurs
        strcpy(tcpClients[nbClients].ipAddress, clientIpAddress);
        tcpClients[nbClients].port = clientPort;
        strcpy(tcpClients[nbClients].name, clientName);
        nbClients++;

        printClients();

        // rechercher l'id du joueur qui vient de se connecter
        id = findClientByName(clientName);
        printf("id=%d\n", id);

        // lui envoyer un message personnel pour lui communiquer son id
        sendId(id);

        // Envoyer un message broadcast pour communiquer a tout le monde
        // la liste des joueurs actuellement connectes
        broadcastPlayerList();

        // Si le nombre de joueurs atteint 4, alors on peut lancer le
        // jeu
        if (nbClients == 4) {
            for (size_t id = 0; id < 4; id++) {
                // On envoie ses cartes au joueur 'id',
                sendCards(id, (int *)(deck + (id * 3)));
                // ainsi que la ligne qui lui correspond dans tableCartes
                for (size_t j = 0; j < 8; j++) {
                    sendCardValue(id, id, j, tableCartes[id][j]);
                }
            }

            // On envoie enfin un message a tout le monde pour definir
            // qui est le joueur courant=0
            broadcastCurrentPlayer(joueurCourant);
            fsmServer = 1;
        }
        break;
    }
}

void end_round() {
    // Selection du joeur suivant & broadcast
    joueurCourant = (joueurCourant + 1) % 4;
    broadcastCurrentPlayer(joueurCourant);
}

void on_msg_in_playing_state(char buffer[256]) {
    char com;

    switch (buffer[0]) {
    case 'G': {
        // Un joueur accuse
        int id = -1;
        int guiltSel = -1;
        sscanf(buffer, "%c %d %d", &com, &id, &guiltSel);
        printf("[G] com : %c id : %d guiltSel : %d\n", com, id, guiltSel);
        printf("Coupable : %d\n", deck[13]);
        if (id == -1) {
            puts("[G] Joueur non trouve");
        } else if (guiltSel == -1) {
            puts("[G] Selection invalide");
        } else if (joueurCourant != id) {
            printf("[%c] %s essaie de voler le tour de %s.",
                   com,
                   tcpClients[id].name,
                   tcpClients[joueurCourant].name);
        } else {
            if (deck[13] == guiltSel) {
                printf("%s(%d) a gagné!\n", tcpClients[id].name, id);
            } else {
                printf("%s(%d) a lance une fausse accusation.",
                       tcpClients[id].name,
                       id);
                end_round();
            }
        }
    } break;

    case 'O': {
        // Un joueur demande qui a un objet
        int id = -1;
        int objetSel = -1;
        sscanf(buffer, "%c %d %d", &com, &id, &objetSel);
        if (id == -1) {
            puts("[O] Identifiant invalide");
        } else if (objetSel == -1) {
            puts("[O] Selection invalide");
        } else if (joueurCourant != id) {
            printf("[%c] %s essaie de voler le tour de %s.",
                   com,
                   tcpClients[id].name,
                   tcpClients[joueurCourant].name);
        } else {
            for (size_t i = 0; i < 4; i++) {
                if (i == id) continue;

                broadcastCardValue(
                    i, objetSel, tableCartes[i][objetSel] > 0 ? 100 : 0);
                end_round();
            }
        }
    } break;

    case 'S': { // Un joueur demande à un autre la quantite d'un objet
        int id = -1;
        int joueurSel = -1;
        int objetSel = -1;

        sscanf(buffer, "%c %d %d %d", &com, &id, &joueurSel, &objetSel);

        if (id == -1) {
            puts("[S] Identifiant invalide");
        } else if (joueurSel == -1) {
            puts("[S] Selection du joueur invalide");
        } else if (joueurCourant != id) {
            printf("[%c] %s essaie de voler le tour de %s.",
                   com,
                   tcpClients[id].name,
                   tcpClients[joueurCourant].name);
        } else if (objetSel == -1) {
            puts("[S] Selection de l'objet invalide");
        } else {
            broadcastCardValue(
                joueurSel, objetSel, tableCartes[joueurSel][objetSel]);
            end_round();
        }
    } break;

    default:
        break;
    }
}
