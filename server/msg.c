#include "msg.h"

#include <stdio.h>
#include "com.h"

void broadcastCurrentPlayer(int aCurrentPlayer) {
    char data[DATA_SIZE];

    sprintf(data, M_PATTERN, aCurrentPlayer);
    broadcastMessage(data);
}

void broadcastCardValue(int aPlayerId, int aObjectId, int aValue) {
    char data[DATA_SIZE];

    sprintf(data, V_PATTERN, aPlayerId, aObjectId, aValue);
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

void broadcastWinner(int aPlayerId, int aCulpritId) {
    char data[DATA_SIZE];

    sprintf(data, W_PATTERN, aPlayerId, aCulpritId);

    broadcastMessage(data);
}

void broadcastTalk(int aPlayerId, char *aMsg) {
    char data[DATA_SIZE];

    sprintf(data, T_PATTERN, aPlayerId, aMsg);

    broadcastMessage(data);
}

void sendId(int aRecipientId) {
    char data[DATA_SIZE];

    sprintf(data, I_PATTERN, aRecipientId);
    sendMessageToClient(tcpClients[aRecipientId].ipAddress,
                        tcpClients[aRecipientId].port,
                        data);
}

void sendCards(int aPlayerId, int aCards[3]) {
    char data[DATA_SIZE];

    sprintf(data, D_PATTERN, aCards[0], aCards[1], aCards[2]);
    sendMessageToClient(
        tcpClients[aPlayerId].ipAddress, tcpClients[aPlayerId].port, data);
}

void sendCardValue(int aRecipientId,
                   int aPlayerId,
                   int aObjectId,
                   int aValue) {
    char data[DATA_SIZE];

    sprintf(data, V_PATTERN, aPlayerId, aObjectId, aValue);
    sendMessageToClient(tcpClients[aRecipientId].ipAddress,
                        tcpClients[aRecipientId].port,
                        data);
}

void sendExcluded(int aPlayerId) {
    char data[DATA_SIZE];

    sprintf(data, W_PATTERN, -1, -1);

    sendMessageToClient(
        tcpClients[aPlayerId].ipAddress, tcpClients[aPlayerId].port, data);
}
