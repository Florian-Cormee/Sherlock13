#ifndef CARTES_H
#define CARTES_H

extern char *nomcartes[];
extern int deck[13];
extern int tableCartes[4][8];

void melangerDeck();
void createTable();
void printDeck();
#endif // !CARTES_H
