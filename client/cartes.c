#include "cartes.h"
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
#include "com.h"
#include "gui.h"

char *nbobjets[8] = {"5", "5", "5", "5", "4", "3", "3", "3"};
char *nbnoms[13] = {"Sebastian Moran",
                    "irene Adler",
                    "inspector Lestrade",
                    "inspector Gregson",
                    "inspector Baynes",
                    "inspector Bradstreet",
                    "inspector Hopkins",
                    "Sherlock Holmes",
                    "John Watson",
                    "Mycroft Holmes",
                    "Mrs. Hudson",
                    "Mary Morstan",
                    "James Moriarty"};
int guiltGuess[13];
int tableCartes[4][8];
int hand[3];

void initCartes() {
    int i, j;

    for(i = 0; i < 13;i++){
        guiltGuess[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8;j++){
            tableCartes[i][j] = -1;
        }
    }

    for(i = 0;i<3;i++){
        hand[i] = -1;
    }
}
