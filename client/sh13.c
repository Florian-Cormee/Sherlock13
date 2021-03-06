#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cartes.h"
#include "com.h"
#include "gui.h"

int main(int argc, char **argv) {
    /** Initialization of global variables. */
    initCartes();
    initCom();
    initGui();

    int i;
    int quit = 0;
    SDL_Event event;
    /** Check the number of arguments. */
    if (argc < 6) {
        printf("<app> <Main server ip address> <Main server port> <Client ip "
               "address> <Client port> <player name>\n");
        exit(1);
    }
    // Reading of the inputs
    strcpy(gServerIpAddress, argv[1]);
    gServerPort = atoi(argv[2]);
    strcpy(gClientIpAddress, argv[3]);
    gClientPort = atoi(argv[4]);
    strcpy(gName, argv[5]);

    // Initialization of the graphic window
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2 SH13",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1024,
                                          768,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *deck[13];
    SDL_Surface *objet[8];
    SDL_Surface *gobutton;
    SDL_Surface *connectbutton;
    SDL_Surface *winImg[2];
    loadImg(deck, objet, &gobutton, &connectbutton, winImg);

    SDL_Texture *texture_deck[13];
    SDL_Texture *texture_gobutton;
    SDL_Texture *texture_connectbutton;
    SDL_Texture *texture_objet[8];
    SDL_Texture *texture_winImg[2];

    for (i = 0; i < 13; i++) {
        texture_deck[i] = SDL_CreateTextureFromSurface(renderer, deck[i]);
    }
    for (i = 0; i < 8; i++) {
        texture_objet[i] = SDL_CreateTextureFromSurface(renderer, objet[i]);
    }
    for (i = 0; i < 2; i++) {
        texture_winImg[i] = SDL_CreateTextureFromSurface(renderer, winImg[i]);
    }

    texture_gobutton = SDL_CreateTextureFromSurface(renderer, gobutton);
    texture_connectbutton =
        SDL_CreateTextureFromSurface(renderer, connectbutton);

    TTF_Font *Sans = TTF_OpenFont("client/font/sans.ttf", 15);

    // Creation of the tcp server threads
    printf("Creation du thread serveur tcp !\n");
    pthread_create(&thread_serveur_tcp_id, NULL, fn_serveur_tcp, NULL);
    pthread_create(&thread_chat_id, NULL, &fn_chat, NULL);

    // Main loop
    while (!quit) {
        // Event handling
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                onMouseButtonDownEvent();
                break;
            }
        }
        // Communication handling
        if (has_message == 1) {
            pthread_mutex_lock(&mutex);
            printf("consomme |%s|\n", gbuffer);
            parseMsg(gbuffer);
            has_message = 0;
            pthread_mutex_unlock(&mutex);
        }

        // Design
        // Cleanning + Background
        SDL_SetRenderDrawColor(renderer, 255, 230, 230, 230);
        SDL_Rect rect = {0, 0, 1024, 768};
        SDL_RenderFillRect(renderer, &rect);
        // Draw of the graphic elements.
        highlightSelections(renderer);
        drawIcons(renderer, texture_objet);
        drawTextIcons(renderer, Sans);
        drawTextCharacters(renderer, Sans);
        drawBoard(renderer, Sans);
        drawCharactersIcons(renderer, texture_objet);
        // Draw the guesses
        drawX(renderer);
        drawGuessBoard(renderer);
        drawHand(renderer, texture_deck);

        // Go button
        if (goEnabled == 1) {
            SDL_Rect dstrect = {500, 350, 200, 150};
            SDL_RenderCopy(renderer, texture_gobutton, NULL, &dstrect);
        }
        // V/D image
        if (gWinnerId != -2) {
            SDL_Rect dstrect = {500, 350, 200, 150};
            int winId = gWinnerId == gId ? 1 : 0;
            SDL_RenderCopy(renderer, texture_winImg[winId], NULL, &dstrect);
        }
        // Connect button
        if (connectEnabled == 1) {
            SDL_Rect dstrect = {0, 0, 200, 50};
            SDL_RenderCopy(renderer, texture_connectbutton, NULL, &dstrect);
        }
        // Draw the name of each player
        drawPlayersName(renderer, Sans);

        SDL_RenderPresent(renderer);
    }
    // End of the program, free of the texture and surface.
    for (i = 0; i < 13; i++) {
        SDL_DestroyTexture(texture_deck[i]);
        SDL_FreeSurface(deck[i]);
    }

    for (i = 0; i < 8; i++) {
        SDL_DestroyTexture(texture_objet[i]);
        SDL_FreeSurface(objet[i]);
    }

    for (i = 0; i < 2; i++) {
        SDL_DestroyTexture(texture_winImg[i]);
        SDL_FreeSurface(winImg[i]);
    }

    SDL_DestroyTexture(texture_gobutton);
    SDL_FreeSurface(gobutton);
    SDL_DestroyTexture(texture_connectbutton);
    SDL_FreeSurface(connectbutton);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
