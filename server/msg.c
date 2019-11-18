#include "msg.h"

#include <stdio.h>
#include "com.h"

void broadcastCurrentPlayer(int t_currentPlayer) {
    char data[DATA_SIZE];

    sprintf(data, M_PATTERN, t_currentPlayer);
    broadcastMessage(data);
}

void broadcastCardValue(int t_playerId, int t_objectId, int t_value) {
    char data[DATA_SIZE];

    sprintf(data, V_PATTERN, t_playerId, t_objectId, t_value);
    broadcastMessage(data);
}

void broadcastPlayerList() {
    char data[DATA_SIZE];

    sprintf(data,
            L_PATTERN,
            tcpClients[0].name,
            tcpClients[1].name,
            tcpClients[2].name,
            tcpClients[3].name);
    broadcastMessage(data);
}

void broadcastWinner(int t_playerId, int t_guildId) {
    char data[DATA_SIZE];

    sprintf(data, W_PATTERN, t_playerId, t_guildId);

    broadcastMessage(data);
}

void sendId(int t_recipientId) {
    char data[DATA_SIZE];

    sprintf(data, "I %d", t_recipientId);
    sendMessageToClient(tcpClients[t_recipientId].ipAddress,
                        tcpClients[t_recipientId].port,
                        data);
}

void sendCards(int t_playerId, int t_cards[3]) {
    char data[DATA_SIZE];

    sprintf(data, D_PATTERN, t_cards[0], t_cards[1], t_cards[2]);
    sendMessageToClient(
        tcpClients[t_playerId].ipAddress, tcpClients[t_playerId].port, data);
}

void sendCardValue(int t_recipientId,
                   int t_playerId,
                   int t_objectId,
                   int t_value) {
    char data[DATA_SIZE];

    sprintf(data, V_PATTERN, t_playerId, t_objectId, t_value);
    sendMessageToClient(tcpClients[t_recipientId].ipAddress,
                        tcpClients[t_recipientId].port,
                        data);
}
