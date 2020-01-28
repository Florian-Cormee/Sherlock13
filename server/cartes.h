#ifndef CARTES_H
#define CARTES_H

/** Name of each card. */
extern char *cardsName[];
extern int deck[13];
/** Array containing amount of symbols held by each player */
extern int cardsArray[4][8];

/**
 * Shuffles the deck.
 */
void shuffleDeck();
/**
 * Fills cardsArray from deck.
 */
void createTable();
/**
 * Prints the deck in the console.
 */
void printDeck();
#endif // !CARTES_H
