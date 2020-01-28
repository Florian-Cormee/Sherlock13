#ifndef CARTES_H
#define CARTES_H

/** Name of each object. */
extern char *nbobjets[8];
/** Name of each character. */
extern char *nbnoms[13];
/** memo for accusation. */
extern int guiltGuess[13];
/** tab of player and symbole. */
extern int tableCartes[4][8];
/** indice of each character in the hand. */
extern int hand[3];

/** Initialize all the variables */
void initCartes();

#endif // !CARTES_H
