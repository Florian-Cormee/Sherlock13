#ifndef COM_H
#define COM_H

struct _client {
    char ipAddress[40];
    int port;
    char name[40];
};

typedef struct _client client_t;

extern client_t tcpClients[4];
extern int nbClients;

void printClients();
int findClientByName(char *name);
void sendMessageToClient(char *clientip, int clientport, char *mess);
void broadcastMessage(char *mess);

#endif
