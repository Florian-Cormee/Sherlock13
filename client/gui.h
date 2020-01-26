#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern int joueurSel;
extern int objetSel;
extern int guiltSel;
extern int goEnabled;
extern int connectEnabled;

void initGui();
void loadImg(SDL_Surface * deck[13], SDL_Surface *objet[8], SDL_Surface **gobutton, SDL_Surface **connectbutton, SDL_Surface *winImg[2]);
void onMouseButtonDownEvent();

void highlightSelections(SDL_Renderer *renderer);
void drawIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);
void drawTextIcons(SDL_Renderer *renderer, TTF_Font *Sans);
void drawTextCharacters(SDL_Renderer *renderer, TTF_Font *Sans);
void drawTab(SDL_Renderer *renderer, TTF_Font *Sans);
void drawCharactersIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);
void drawX(SDL_Renderer *renderer);
void drawGuessChart(SDL_Renderer *renderer);
void drawHand(SDL_Renderer *renderer, SDL_Texture *texture_deck[13]);
void drawPlayersName(SDL_Renderer *renderer, TTF_Font *Sans);
#endif // !GUI_H
