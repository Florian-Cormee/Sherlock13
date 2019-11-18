#ifndef MSG_H
#define MSG_H

#define DATA_SIZE 256

#define D_PATTERN "D %d %d %d"
#define L_PATTERN "L %s %s %s %s"
#define M_PATTERN "M %d"
#define V_PATTERN "V %d %d %d"
#define W_PATTERN "W %d %d"

void broadcastCurrentPlayer(int t_currentPlayer);
void broadcastCardValue(int t_playerId, int t_objectId, int value);
void broadcastPlayerList();
void broadcastWinner(int t_playerId, int t_guildId);

void sendId(int t_recipientId);
void sendCards(int t_playerId, int t_cards[3]);
void sendCardValue(int t_recipientId, int t_playerId,int t_objectId, int t_value);
#endif // !MSG_H
