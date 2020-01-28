/**
 * Communication api. Hides away all the server communication.
 */
#ifndef COM_H
#define COM_H

#include <pthread.h>

extern pthread_t thread_serveur_tcp_id;
extern pthread_t thread_chat_id;
extern pthread_mutex_t mutex;
/** message content. */
extern char gbuffer[256];
/** Main server Ip adress. */
extern char gServerIpAddress[256];
/** Main server port number. */
extern int gServerPort;
/** Main client Ip adress. */
extern char gClientIpAddress[256];
/** Main client port number. */
extern int gClientPort;
/** Player's Name. */
extern char gName[256];
/** Array of all the player's name. */
extern char gNames[4][256];
/** Index of the player. */
extern int gId;
/** Index of the winner. */
extern int gWinnerId;

/** Flag for message from server. */
extern volatile int has_message;

/**
 * Connect the client to the server
 *
 * @param arg list of the argument put at the beginning
 */
void *fn_serveur_tcp(void *arg);

/**
 * Send a message to the server
 *
 * @param ipAddress main server ip address
 * @param portno main server port
 * @param mess message contents
 */
void sendMessageToServer(char *ipAddress, int portno, char *mess);

/**
 * Send a message to the server for the chat
 *
 * @param arg list of the arguments
 */
void *fn_chat(void *arg);

/** Initialize all the variables */
void initCom();

/**
 * Read a message from the server
 *
 * @param ipAddress main server ip adress
 * @param portno main server port
 * @param mess message contents
 */
void parseMsg(char inputs[256]);

#endif // !COM_H
