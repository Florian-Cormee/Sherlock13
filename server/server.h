#ifndef SERVER_H
#define SERVER_H

/**
 * The currently active player (aka. the only one allowed to perform action).
 */
extern int gCurrentPlayer;
/**
 * Array of booleans. If true, the player is excluded.
 */
extern int gExcludedPlayer[4];

/**
 * @brief Selects the next player.
 *
 * Round-robin selection among the eligable players. Broadcast the newly active
 * player to every clients.
 *
 */
void endRound();

/**
 * Prints the error and exit.
 */
void errorExit(const char *msg);

#endif // !SERVER_H
