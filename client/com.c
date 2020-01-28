#include "com.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cartes.h"
#include "gui.h"

pthread_t thread_serveur_tcp_id;
pthread_t thread_chat_id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char gbuffer[256];
char gServerIpAddress[256];
int gServerPort;
char gClientIpAddress[256];
int gClientPort;
char gName[256];
char gNames[4][256];
int gId = -1;
int gWinnerId = -2;

volatile int has_message = 0;

void *fn_serveur_tcp(void *arg) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("sockfd error\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = gClientPort;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind error\n");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            printf("accept error\n");
            exit(1);
        }

        bzero(gbuffer, 256);
        n = read(newsockfd, gbuffer, 255);
        if (n < 0) {
            printf("read error\n");
            exit(1);
        }

        pthread_mutex_lock(&mutex);
        has_message = 1;
        pthread_mutex_unlock(&mutex);

        while (has_message)
            ;
    }
}

void sendMessageToServer(char *ipAddress, int portno, char *mess) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char sendbuffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(ipAddress);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting\n");
        exit(1);
    }

    sprintf(sendbuffer, "%s\n", mess);
    n = write(sockfd, sendbuffer, strlen(sendbuffer));
    if (n == -1) {
        puts("ERROR writing");
    }
    close(sockfd);
}

void *fn_chat(void *arg) {
    char msg[128];
    char data[256];

    while (1) {
        fgets(msg, 128, stdin);
        if (gId >= 0) {
            sprintf(data, "T %d %s", gId, msg);
            sendMessageToServer(gServerIpAddress, gServerPort, data);
        }
    }
}

void initCom() {
    int i;
    for (i = 0; i < 4; i++) {
        strcpy(gNames[i], "-");
    }
}

void parseMsg(char inputs[256]) {
    switch (inputs[0]) {
    // Message 'I' : le joueur recoit son Id
    case 'I':
        // RAJOUTER DU CODE ICI
        // "I %d"
        sscanf(inputs + 2, "%d", &gId);
        printf("Mon Id c'est %d\n", gId);
        break;
    // Message 'L' : le joueur recoit la liste des joueurs
    case 'L':
        // RAJOUTER DU CODE ICI
        // "L %s %s %s %s"
        sscanf(inputs + 2,
               "%s %s %s %s",
               *gNames,
               *(gNames + 1),
               *(gNames + 2),
               *(gNames + 3));
        break;
    // Message 'D' : le joueur recoit ses trois cartes
    case 'D':
        // RAJOUTER DU CODE ICI
        // "D %d %d %d"
        sscanf(inputs + 2, "%d %d %d", hand, hand + 1, hand + 2);
        break;
    // Message 'M' : le joueur recoit le n° du joueur courant
    // Cela permet d'affecter goEnabled pour autoriser l'affichage du
    // bouton go
    case 'M': {
        // RAJOUTER DU CODE ICI
        // "M %d"
        size_t num;
        sscanf(inputs + 2, "%ld", &num);
        printf("Num c'est %ld\n", num);
        goEnabled = (num == gId);
        printf("GoEnabled : %d\n", goEnabled);
    } break;
    // Message 'V' : le joueur recoit une valeur de tableCartes
    case 'V': {
        // RAJOUTER DU CODE ICI
        // "V %d %d %d"
        // Joueur Objet "Valeur"
        int joueur, objet, valeur;
        sscanf(inputs + 2, "%d %d %d", &joueur, &objet, &valeur);
        if (tableCartes[joueur][objet] == -1 ||
            tableCartes[joueur][objet] == 100) {
            tableCartes[joueur][objet] = valeur;
        }
    } break;
    case 'W': {
        // RAJOUTER DU CODE ICI
        // "W %d %d"
        int idcc;
        sscanf(inputs + 2, "%d %d", &gWinnerId, &idcc);
        goEnabled = 0;
        if (gWinnerId == -1) {
            printf("Vous pourriez savourer l'air de surprise sur mon visage "
                   "parce que je serais surpris %s, très surpris.\n",
                   gNames[gId]);
        } else {
            printf("Elementaire my dear %s, c'etait bien %s !\n",
                   gNames[gWinnerId],
                   nbnoms[idcc]);
        }
    } break;

    case 'T': {
        int playerId = -1;
        char msg[128];
        char com;
        memset(msg, '\0', 128);

        sscanf(inputs, "%c %d %s", &com, &playerId, msg);

        if (playerId == -1) {
            puts("[T] Identifiant invalide");
        } else if (strlen(msg) == 0) {
            printf("[%c] %s essaie d'envoyer un message vide.",
                   com,
                   gNames[playerId]);
        } else {
            printf("%s > %s\n", gNames[playerId], msg);
        }
    } break;
    }
}
