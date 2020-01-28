#include "cartes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *cardsName[] = {"Sebastian Moran",
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

int deck[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int cardsArray[4][8];

void shuffleDeck() {
    int i;
    int index1, index2, tmp;

    srand(time(NULL));
    for (i = 0; i < 1000; i++) {
        // Picks random indexes
        index1 = rand() % 13;
        index2 = rand() % 13;
        // Exchanges values from index1 and index2
        tmp = deck[index1];
        deck[index1] = deck[index2];
        deck[index2] = tmp;
    }
}

void createTable() {
    // Le joueur 0 possede les cartes d'indice 0,1,2
    // Le joueur 1 possede les cartes d'indice 3,4,5
    // Le joueur 2 possede les cartes d'indice 6,7,8
    // Le joueur 3 possede les cartes d'indice 9,10,11
    // Le coupable est la carte d'indice 12
    int i, j, c;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 8; j++)
            cardsArray[i][j] = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            c = deck[i * 3 + j];
            switch (c) {
            case 0: // Sebastian Moran
                cardsArray[i][7]++;
                cardsArray[i][2]++;
                break;
            case 1: // Irene Adler
                cardsArray[i][7]++;
                cardsArray[i][1]++;
                cardsArray[i][5]++;
                break;
            case 2: // Inspector Lestrade
                cardsArray[i][3]++;
                cardsArray[i][6]++;
                cardsArray[i][4]++;
                break;
            case 3: // Inspector Gregson
                cardsArray[i][3]++;
                cardsArray[i][2]++;
                cardsArray[i][4]++;
                break;
            case 4: // Inspector Baynes
                cardsArray[i][3]++;
                cardsArray[i][1]++;
                break;
            case 5: // Inspector Bradstreet
                cardsArray[i][3]++;
                cardsArray[i][2]++;
                break;
            case 6: // Inspector Hopkins
                cardsArray[i][3]++;
                cardsArray[i][0]++;
                cardsArray[i][6]++;
                break;
            case 7: // Sherlock Holmes
                cardsArray[i][0]++;
                cardsArray[i][1]++;
                cardsArray[i][2]++;
                break;
            case 8: // John Watson
                cardsArray[i][0]++;
                cardsArray[i][6]++;
                cardsArray[i][2]++;
                break;
            case 9: // Mycroft Holmes
                cardsArray[i][0]++;
                cardsArray[i][1]++;
                cardsArray[i][4]++;
                break;
            case 10: // Mrs. Hudson
                cardsArray[i][0]++;
                cardsArray[i][5]++;
                break;
            case 11: // Mary Morstan
                cardsArray[i][4]++;
                cardsArray[i][5]++;
                break;
            case 12: // James Moriarty
                cardsArray[i][7]++;
                cardsArray[i][1]++;
                break;
            }
        }
    }
}

void printDeck() {
    int i, j;

    for (i = 0; i < 13; i++) {
        printf("%d %s\n", deck[i], cardsName[deck[i]]);
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            printf("%2.2d ", cardsArray[i][j]);
        }
        puts("");
    }
}
