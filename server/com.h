/**
 * Communication api. Hides away all the low level socket operations.
 */
#ifndef COM_H
#define COM_H

typedef struct _client {
    char ipAddress[40];
    int port;
    char name[40];
} client_t;

/** Array of clients represented by there address, port number and name. */
extern client_t tcpClients[4];
/** The amount of connected clients. */
extern int nbClients;

/** Prints all client to the console. */
void printClients();
/**
 * Gets the index of the client in tcpClients from its name.
 *
 * @param aName the name to seek for
 * @return the id of the client with the given name ; otherwise -1
 */
int findClientByName(char *aName);
/** Sends the char sequence to a client. */
void sendMessageToClient(char *clientip, int clientport, char *mess);
/** Sends a message to every connected clients. */
void broadcastMessage(char *mess);

#endif
