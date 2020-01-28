/**
 * Tiny state machine handling message parsing.
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
 * Enumerates the states of this SM
 */
typedef enum states
{
    CONNECTION,
    PLAYING
} eStates;

/**
 * The SM current state.
 */
extern eStates gState;

/**
 * Let's the SM handle incoming messages depending on its current state.
 *
 * @param aBuffer The received message as a string.
 */
void onMsg(char aBuffer[256]);

#endif // !STATE_MACHINE_H
