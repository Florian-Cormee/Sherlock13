#ifndef COM_H
#define COM_H

#include <pthread.h>

extern pthread_t thread_serveur_tcp_id;
extern pthread_t thread_chat_id;
extern pthread_mutex_t mutex;
extern char gbuffer[256];
extern char gServerIpAddress[256];
extern int gServerPort;
extern char gClientIpAddress[256];
extern int gClientPort;
extern char gName[256];
extern char gNames[4][256];
extern int gId;
extern int gWinnerId;

extern volatile int has_message;

void *fn_serveur_tcp(void *arg);

void sendMessageToServer(char *ipAddress, int portno, char *mess);

void *fn_chat(void *arg);

void initCom();

void parseMsg(char inputs[256]);

#endif // !COM_H
