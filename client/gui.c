#include "gui.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cartes.h"
#include "com.h"

int characterSel = -1;
int objectSel = -1;
int guiltSel = -1;
int goEnabled = 0;
int connectEnabled = 1;

void initGui() {}

void loadImg(SDL_Surface *deck[13],
             SDL_Surface *objet[8],
             SDL_Surface **pGobutton,
             SDL_Surface **pConnectbutton,
             SDL_Surface *winImg[2]) {
    deck[0] = IMG_Load("client/sprite/SH13_0.png");
    deck[1] = IMG_Load("client/sprite/SH13_1.png");
    deck[2] = IMG_Load("client/sprite/SH13_2.png");
    deck[3] = IMG_Load("client/sprite/SH13_3.png");
    deck[4] = IMG_Load("client/sprite/SH13_4.png");
    deck[5] = IMG_Load("client/sprite/SH13_5.png");
    deck[6] = IMG_Load("client/sprite/SH13_6.png");
    deck[7] = IMG_Load("client/sprite/SH13_7.png");
    deck[8] = IMG_Load("client/sprite/SH13_8.png");
    deck[9] = IMG_Load("client/sprite/SH13_9.png");
    deck[10] = IMG_Load("client/sprite/SH13_10.png");
    deck[11] = IMG_Load("client/sprite/SH13_11.png");
    deck[12] = IMG_Load("client/sprite/SH13_12.png");

    objet[0] = IMG_Load("client/sprite/SH13_pipe_120x120.png");
    objet[1] = IMG_Load("client/sprite/SH13_ampoule_120x120.png");
    objet[2] = IMG_Load("client/sprite/SH13_poing_120x120.png");
    objet[3] = IMG_Load("client/sprite/SH13_couronne_120x120.png");
    objet[4] = IMG_Load("client/sprite/SH13_carnet_120x120.png");
    objet[5] = IMG_Load("client/sprite/SH13_collier_120x120.png");
    objet[6] = IMG_Load("client/sprite/SH13_oeil_120x120.png");
    objet[7] = IMG_Load("client/sprite/SH13_crane_120x120.png");

    *pGobutton = IMG_Load("client/sprite/gobutton.png");
    *pConnectbutton = IMG_Load("client/sprite/connectbutton.png");

    winImg[0] = IMG_Load("client/sprite/SH13_LOSE.png");
    winImg[1] = IMG_Load("client/sprite/SH13_WIN.png");
}

void onMouseButtonDownEvent() {
    int mx, my;
    char sendBuffer[256];
    SDL_GetMouseState(&mx, &my);
    if ((mx < 200) && (my < 50) && (connectEnabled == 1)) {
        sprintf(sendBuffer, "C %s %d %s", gClientIpAddress, gClientPort, gName);
        // RAJOUTER DU CODE ICI
        sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
        connectEnabled = 0;
    } else if ((mx >= 0) && (mx < 200) && (my >= 90) && (my < 330)) {
        characterSel = (my - 90) / 60;
        guiltSel = -1;
    } else if ((mx >= 200) && (mx < 680) && (my >= 0) && (my < 90)) {
        objectSel = (mx - 200) / 60;
        guiltSel = -1;
    } else if ((mx >= 100) && (mx < 250) && (my >= 350) && (my < 740)) {
        characterSel = -1;
        objectSel = -1;
        guiltSel = (my - 350) / 30;
    } else if ((mx >= 250) && (mx < 300) && (my >= 350) && (my < 740)) {
        int ind = (my - 350) / 30;
        guiltGuess[ind] = 1 - guiltGuess[ind];
    } else if ((mx >= 500) && (mx < 700) && (my >= 350) && (my < 450) &&
               (goEnabled == 1)) {
        printf(
            "go! joueur=%d objet=%d guilt=%d\n", playerSel, objectSel, guiltSel);
        if (guiltSel != -1) {
            // Accusation
            sprintf(sendBuffer, "G %d %d", gId, guiltSel);

            // RAJOUTER DU CODE ICI
            sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
        } else if ((objectSel != -1) && (characterSel == -1)) {
            // Qui possède un Objet
            sprintf(sendBuffer, "O %d %d", gId, objectSel);

            // RAJOUTER DU CODE ICI
            sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
        } else if ((objectSel != -1) && (characterSel != -1)) {
            // Combien de fois le joueur possède l'Objet
            sprintf(sendBuffer, "S %d %d %d", gId, playerSel, objectSel);

            // RAJOUTER DU CODE ICI
            sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
        }
    } else {
        characterSel = -1;
        objectSel = -1;
        guiltSel = -1;
    }
}

void highlightSelections(SDL_Renderer *renderer) {
    // Player selection
    if (characterSel != -1) {
        SDL_SetRenderDrawColor(renderer, 255, 180, 180, 255);
        SDL_Rect rect1 = {0, 90 + characterSel * 60, 200, 60};
        SDL_RenderFillRect(renderer, &rect1);
    }

    // Object selection
    if (objectSel != -1) {
        SDL_SetRenderDrawColor(renderer, 180, 255, 180, 255);
        SDL_Rect rect1 = {200 + objectSel * 60, 0, 60, 90};
        SDL_RenderFillRect(renderer, &rect1);
    }

    // Character selection
    if (guiltSel != -1) {
        SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255);
        SDL_Rect rect1 = {100, 350 + guiltSel * 30, 150, 30};
        SDL_RenderFillRect(renderer, &rect1);
    }
}

void drawIcons(SDL_Renderer *renderer, SDL_Texture *texture_objet[8]) {
    SDL_Rect dstrect_pipe = {210, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    SDL_Rect dstrect_ampoule = {270, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    SDL_Rect dstrect_poing = {330, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    SDL_Rect dstrect_couronne = {390, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    SDL_Rect dstrect_carnet = {450, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[4], NULL, &dstrect_carnet);
    SDL_Rect dstrect_collier = {510, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[5], NULL, &dstrect_collier);
    SDL_Rect dstrect_oeil = {570, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[6], NULL, &dstrect_oeil);
    SDL_Rect dstrect_crane = {630, 10, 40, 40};
    SDL_RenderCopy(renderer, texture_objet[7], NULL, &dstrect_crane);
}

void drawTextIcons(SDL_Renderer *renderer, TTF_Font *Sans) {
    SDL_Color col1 = {0, 0, 0};
    int i;
    for (i = 0; i < 8; i++) {
        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(Sans, nbobjets[i], col1);
        SDL_Texture *Message =
            SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_Rect Message_rect; // create a rect
        Message_rect.x = 230 + i * 60; // controls the rect's x coordinate
        Message_rect.y = 50; // controls the rect's y coordinte
        Message_rect.w = surfaceMessage->w; // controls the width of the rect
        Message_rect.h = surfaceMessage->h; // controls the height of the rect

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_DestroyTexture(Message);
        SDL_FreeSurface(surfaceMessage);
    }
}

void drawTextCharacters(SDL_Renderer *renderer, TTF_Font *Sans) {
    SDL_Color col1 = {0, 0, 0};
    int i;
    for (i = 0; i < 13; i++) {
        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(Sans, nbnoms[i], col1);
        SDL_Texture *Message =
            SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_Rect Message_rect;
        Message_rect.x = 105;
        Message_rect.y = 350 + i * 30;
        Message_rect.w = surfaceMessage->w;
        Message_rect.h = surfaceMessage->h;

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_DestroyTexture(Message);
        SDL_FreeSurface(surfaceMessage);
    }
}

void drawBoard(SDL_Renderer *renderer, TTF_Font *Sans) {
    SDL_Color col1 = {0, 0, 0};
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 8; j++) {
            if (tableCartes[i][j] != -1) {
                char mess[10];
                if (tableCartes[i][j] == 100)
                    sprintf(mess, "*");
                else
                    sprintf(mess, "%d", tableCartes[i][j]);

                SDL_Surface *surfaceMessage =
                    TTF_RenderText_Solid(Sans, mess, col1);
                SDL_Texture *Message =
                    SDL_CreateTextureFromSurface(renderer, surfaceMessage);

                SDL_Rect Message_rect;
                Message_rect.x = 230 + j * 60;
                Message_rect.y = 110 + i * 60;
                Message_rect.w = surfaceMessage->w;
                Message_rect.h = surfaceMessage->h;

                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                SDL_DestroyTexture(Message);
                SDL_FreeSurface(surfaceMessage);
            }
        }
}

void drawCharactersIcons(SDL_Renderer *renderer,
                         SDL_Texture *texture_objet[8]) {
    // Sebastian Moran
    {
        SDL_Rect dstrect_crane = {0, 350, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[7], NULL, &dstrect_crane);
    }
    {
        SDL_Rect dstrect_poing = {30, 350, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    }
    // Irene Adler
    {
        SDL_Rect dstrect_crane = {0, 380, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[7], NULL, &dstrect_crane);
    }
    {
        SDL_Rect dstrect_ampoule = {30, 380, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    }
    {
        SDL_Rect dstrect_collier = {60, 380, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[5], NULL, &dstrect_collier);
    }
    // Inspector Lestrade
    {
        SDL_Rect dstrect_couronne = {0, 410, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    }
    {
        SDL_Rect dstrect_oeil = {30, 410, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[6], NULL, &dstrect_oeil);
    }
    {
        SDL_Rect dstrect_carnet = {60, 410, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[4], NULL, &dstrect_carnet);
    }
    // Inspector Gregson
    {
        SDL_Rect dstrect_couronne = {0, 440, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    }
    {
        SDL_Rect dstrect_poing = {30, 440, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    }
    {
        SDL_Rect dstrect_carnet = {60, 440, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[4], NULL, &dstrect_carnet);
    }
    // Inspector Baynes
    {
        SDL_Rect dstrect_couronne = {0, 470, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    }
    {
        SDL_Rect dstrect_ampoule = {30, 470, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    }
    // Inspector Bradstreet
    {
        SDL_Rect dstrect_couronne = {0, 500, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    }
    {
        SDL_Rect dstrect_poing = {30, 500, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    }
    // Inspector Hopkins
    {
        SDL_Rect dstrect_couronne = {0, 530, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[3], NULL, &dstrect_couronne);
    }
    {
        SDL_Rect dstrect_pipe = {30, 530, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    }
    {
        SDL_Rect dstrect_oeil = {60, 530, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[6], NULL, &dstrect_oeil);
    }
    // Sherlock Holmes
    {
        SDL_Rect dstrect_pipe = {0, 560, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    }
    {
        SDL_Rect dstrect_ampoule = {30, 560, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    }
    {
        SDL_Rect dstrect_poing = {60, 560, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    }
    // John Watson
    {
        SDL_Rect dstrect_pipe = {0, 590, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    }
    {
        SDL_Rect dstrect_oeil = {30, 590, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[6], NULL, &dstrect_oeil);
    }
    {
        SDL_Rect dstrect_poing = {60, 590, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[2], NULL, &dstrect_poing);
    }
    // Mycroft Holmes
    {
        SDL_Rect dstrect_pipe = {0, 620, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    }
    {
        SDL_Rect dstrect_ampoule = {30, 620, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    }
    {
        SDL_Rect dstrect_carnet = {60, 620, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[4], NULL, &dstrect_carnet);
    }
    // Mrs. Hudson
    {
        SDL_Rect dstrect_pipe = {0, 650, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[0], NULL, &dstrect_pipe);
    }
    {
        SDL_Rect dstrect_collier = {30, 650, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[5], NULL, &dstrect_collier);
    }
    // Mary Morstan
    {
        SDL_Rect dstrect_carnet = {0, 680, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[4], NULL, &dstrect_carnet);
    }
    {
        SDL_Rect dstrect_collier = {30, 680, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[5], NULL, &dstrect_collier);
    }
    // James Moriarty
    {
        SDL_Rect dstrect_crane = {0, 710, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[7], NULL, &dstrect_crane);
    }
    {
        SDL_Rect dstrect_ampoule = {30, 710, 30, 30};
        SDL_RenderCopy(renderer, texture_objet[1], NULL, &dstrect_ampoule);
    }
}

void drawX(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int i;
    for (i = 0; i < 13; i++) {
        if (guiltGuess[i]) {
            SDL_RenderDrawLine(renderer, 250, 350 + i * 30, 300, 380 + i * 30);
            SDL_RenderDrawLine(renderer, 250, 380 + i * 30, 300, 350 + i * 30);
        }
    }
}

void drawGuessBoard(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, 30 + 60, 680, 30 + 60);
    SDL_RenderDrawLine(renderer, 0, 30 + 120, 680, 30 + 120);
    SDL_RenderDrawLine(renderer, 0, 30 + 180, 680, 30 + 180);
    SDL_RenderDrawLine(renderer, 0, 30 + 240, 680, 30 + 240);
    SDL_RenderDrawLine(renderer, 0, 30 + 300, 680, 30 + 300);

    SDL_RenderDrawLine(renderer, 200, 0, 200, 330);
    SDL_RenderDrawLine(renderer, 260, 0, 260, 330);
    SDL_RenderDrawLine(renderer, 320, 0, 320, 330);
    SDL_RenderDrawLine(renderer, 380, 0, 380, 330);
    SDL_RenderDrawLine(renderer, 440, 0, 440, 330);
    SDL_RenderDrawLine(renderer, 500, 0, 500, 330);
    SDL_RenderDrawLine(renderer, 560, 0, 560, 330);
    SDL_RenderDrawLine(renderer, 620, 0, 620, 330);
    SDL_RenderDrawLine(renderer, 680, 0, 680, 330);
    int i;
    for (i = 0; i < 14; i++) {
        SDL_RenderDrawLine(renderer, 0, 350 + i * 30, 300, 350 + i * 30);
    }
    SDL_RenderDrawLine(renderer, 100, 350, 100, 740);
    SDL_RenderDrawLine(renderer, 250, 350, 250, 740);
    SDL_RenderDrawLine(renderer, 300, 350, 300, 740);
}

void drawHand(SDL_Renderer *renderer, SDL_Texture *texture_deck[13]) {
    if (hand[0] != -1) {
        SDL_Rect dstrect = {750, 0, 1000 / 4, 660 / 4};
        SDL_RenderCopy(renderer, texture_deck[hand[0]], NULL, &dstrect);
    }
    if (hand[1] != -1) {
        SDL_Rect dstrect = {750, 200, 1000 / 4, 660 / 4};
        SDL_RenderCopy(renderer, texture_deck[hand[1]], NULL, &dstrect);
    }
    if (hand[2] != -1) {
        SDL_Rect dstrect = {750, 400, 1000 / 4, 660 / 4};
        SDL_RenderCopy(renderer, texture_deck[hand[2]], NULL, &dstrect);
    }
}

void drawPlayersName(SDL_Renderer *renderer, TTF_Font *Sans) {
    SDL_Color col = {0, 0, 0};
    int i;
    for (i = 0; i < 4; i++)
        if (strlen(gNames[i]) > 0) {
            SDL_Surface *surfaceMessage =
                TTF_RenderText_Solid(Sans, gNames[i], col);
            SDL_Texture *Message =
                SDL_CreateTextureFromSurface(renderer, surfaceMessage);

            SDL_Rect Message_rect; // create a rect
            Message_rect.x = 10; // controls the rect's x coordinate
            Message_rect.y = 110 + i * 60; // controls the rect's y coordinte
            Message_rect.w =
                surfaceMessage->w; // controls the width of the rect
            Message_rect.h =
                surfaceMessage->h; // controls the height of the rect

            SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
            SDL_DestroyTexture(Message);
            SDL_FreeSurface(surfaceMessage);
        }
}
