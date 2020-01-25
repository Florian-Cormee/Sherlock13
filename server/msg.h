/**
 * Handles message format and sending.
 */
#ifndef MSG_H
#define MSG_H

#define DATA_SIZE 256

#define D_PATTERN "D %d %d %d" // Deck message : <Card ID> <Card ID> <Card ID>
#define I_PATTERN "I %d" // Identifier message : <ID>
#define L_PATTERN "L %s %s %s %s" // List client message : <Name> <Name> <Name> <Name>
#define M_PATTERN "M %d" // Active player message : <Player ID>
#define T_PATTERN "T %d %s" // (Talk) Chat message : <Player ID> <Message>
#define V_PATTERN "V %d %d %d" // Value message : <Player ID> <Symbol ID> <Value>
#define W_PATTERN "W %d %d" // Win message : <Player ID> <Culprit ID>

void broadcastCurrentPlayer(int aCurrentPlayer);
/**
 * Broadcasts the exact amount a player has of a specific symbol.
 * @param aPlayerId the targeted player
 * @param aObjectId the identifier of the symbol
 * @param aValue the amount of that symbol
 */
void broadcastCardValue(int aPlayerId, int aObjectId, int aValue);
/**
 * Broadcasts the list of connected players.
 */
void broadcastPlayerList();
/**
 * Broadcasts the winner identifier.
 *
 * @param aPlayerId the player who won
 * @param aCulpritId the culprit card identifier
 */
void broadcastWinner(int aPlayerId, int aCulpritId);
/**
 * Broadcasts a chat message.
 *
 * @param aPlayerId the sender
 * @param aMsg the message
 */
void broadcastTalk(int aPlayerId, char *aMsg);

/**
 * Sends its identifier to the client.
 *
 * @param aRecipientId the recipient client
 */
void sendId(int aRecipientId);
/**
 * Sends its cards to a player.
 *
 * @param aPlayerId the client
 * @param aCards its cards
 */
void sendCards(int aPlayerId, int aCards[3]);
/**
 * Sends the specified amount a player has of a specific symbol.
 *
 * @param aRecipientId the recipient client
 * @param aPlayerId the targeted player
 * @param aObjectId the identifier of the symbol
 * @param aValue the amount of that symbol
 */
void sendCardValue(int aRecipientId, int aPlayerId, int aObjectId, int aValue);
/**
 * Notifies the client he is excluded.
 *
 * @param aPlayerId the client to notify
 */
void sendExcluded(int aPlayerId);
#endif // !MSG_H
