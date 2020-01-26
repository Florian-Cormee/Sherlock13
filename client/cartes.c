#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cartes.h"
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
    memset(guiltGuess, 0, 13);
    int i = 0;
    for (i = 0; i < 4; i++) {
        memset(tableCartes[i],-1,8);
    }
    memset(hand, -1, 3);
}
