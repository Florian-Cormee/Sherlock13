/**
 * State machine used to handle incoming message.
 */

#include "state_machine.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "cartes.h"
#include "com.h"
#include "logger.h"
#include "msg.h"
#include "server.h"

eStates gState = CONNECTION;

void onMsgInConnectionState(char aBuffer[256]) {
    char clientIpAddress[256];
    char clientName[256];
    int clientPort;
    char com;
    int id;

    switch (aBuffer[0]) {
    case 'C':
        sscanf(aBuffer,
               "%c %s %d %s",
               &com,
               clientIpAddress,
               &clientPort,
               clientName);
        log_f(DEBUG,
              "Com : '%c', ipAddress : '%s', port : '%d', name : '%s'\n",
              com,
              clientIpAddress,
              clientPort,
              clientName);
        // Stores the address of the client's server
        strcpy(tcpClients[nbClients].ipAddress, clientIpAddress);
        tcpClients[nbClients].port = clientPort;
        strcpy(tcpClients[nbClients].name, clientName);
        nbClients++;

        printClients();

        // Looks for the id of the last connected player
        id = findClientByName(clientName);
        log_f(DEBUG, "ID : %d\n", id);
        // Sends its identifier
        sendId(id);
        // Broadcasts the updated list of connected players
        broadcastPlayerList();
        // When there are 4 player, the game can begin
        if (nbClients == 4) {
            for (size_t id = 0; id < 4; id++) {
                // Sends the player's cards
                sendCards(id, (int *)(deck + (id * 3)));
                // Sends the player's row from cardsArray
                for (size_t j = 0; j < 8; j++) {
                    sendCardValue(id, id, j, cardsArray[id][j]);
                }
            }
            // Broadcasts the current active player
            broadcastCurrentPlayer(gCurrentPlayer);
            // Switch to game mode
            gState = PLAYING;
        }
        break;
    }
}
void onMsgInPlayingState(char aBuffer[256]) {
    char com; // Command's symbol

    switch (aBuffer[0]) {
    case 'G': {
        // A player accuses
        int id = -1; // Id of the sender
        int culpritSel = -1; // Id of the target

        sscanf(aBuffer, "%c %d %d", &com, &id, &culpritSel);
        // Ensures the request is valid
        if (id == -1) {
            warn("[G] Joueur non trouve");
        } else if (culpritSel == -1) {
            warn("[G] Selection invalide");
        } else if (gCurrentPlayer != id) {
            log_f(WARN,
                  "[%c] %s essaie de voler le tour de %s.",
                  com,
                  tcpClients[id].name,
                  tcpClients[gCurrentPlayer].name);
        } else {
            // The request is valid
            if (deck[12] == culpritSel) {
                // Correct guess ; notify every clients
                log_f(INFO, "%s(%d) a gagné!\n", tcpClients[id].name, id);
                broadcastWinner(gCurrentPlayer, deck[12]);
            } else {
                // Wrong guess ; exclude the player and notify every clients
                log_f(INFO,
                      "%s(%d) a lance une fausse accusation.",
                      tcpClients[id].name,
                      id);
                sendExcluded(gCurrentPlayer);
                gExcludedPlayer[gCurrentPlayer] = 1;
                endRound();
            }
        }
    } break;

    case 'O': {
        // Asks who owns a specific object
        int id = -1; // Id of the sender
        int selectedObject = -1; // Id of the selected object
        sscanf(aBuffer, "%c %d %d", &com, &id, &selectedObject);
        if (id == -1) {
            warn("[O] Identifiant invalide");
        } else if (selectedObject == -1) {
            warn("[O] Selection invalide");
        } else if (gCurrentPlayer != id) {
            log_f(WARN,
                  "[%c] %s essaie de voler le tour de %s.",
                  com,
                  tcpClients[id].name,
                  tcpClients[gCurrentPlayer].name);
        } else {
            // Share the information
            for (size_t i = 0; i < 4; i++) {
                // Skips the interrogator
                if (i == id) {
                    continue;
                }

                broadcastCardValue(i,
                                   selectedObject,
                                   cardsArray[i][selectedObject] > 0 ? 100 : 0);
            }
            endRound();
        }
    } break;

    case 'S': {
        // Asks the exact amount of a specific symbol a player owns
        int id = -1;
        int playerSel = -1;
        int objectSel = -1;

        sscanf(aBuffer, "%c %d %d %d", &com, &id, &playerSel, &objectSel);

        if (id == -1) {
            warn("[S] Identifiant invalide");
        } else if (playerSel == -1) {
            warn("[S] Selection du joueur invalide");
        } else if (gCurrentPlayer != id) {
            log_f(WARN,
                  "[%c] %s essaie de voler le tour de %s.",
                  com,
                  tcpClients[id].name,
                  tcpClients[gCurrentPlayer].name);
        } else if (objectSel == -1) {
            warn("[S] Selection de l'objet invalide");
        } else {
            broadcastCardValue(
                playerSel, objectSel, cardsArray[playerSel][objectSel]);
            endRound();
        }
    } break;

    case 'T': {
        // Chats
        int playerId = -1;
        char msg[128];
        memset(msg, '\0', 128);

        sscanf(aBuffer, "%c %d %s", &com, &playerId, msg);

        if (playerId == -1) {
            warn("[T] Identifiant invalide");
        } else if (strlen(msg) == 0) {
            log_f(WARN,
                  "[%c] %s essaie d'envoyer un message vide.",
                  com,
                  tcpClients[playerId].name);
        } else {
            broadcastTalk(playerId, msg);
        }
    }

    default:
        break;
    }
}

void onMsg(char aBuffer[256]) {
    switch (gState) {
    case CONNECTION:
        onMsgInConnectionState(aBuffer);
        break;
    case PLAYING:
        onMsgInPlayingState(aBuffer);
        break;
    default:
        log_f(WARN, "État inconnu: %d. Aucune action n'a eu lieu.", gState);
        break;
    }
}
