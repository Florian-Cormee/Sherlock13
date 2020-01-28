/**
 * Gui api. Hides away the draw operations.
 */
#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/** Index of the player selected. */
extern int joueurSel;
/** Index of the object selected. */
extern int objetSel;
/** Index of the character selected. */
extern int guiltSel;
/** state of th button go. */
extern int goEnabled;
/** state of th button connect. */
extern int connectEnabled;

/** Initialize all the variables */
void initGui();
/**
 * load of the images of the game
 *
 * @param deck[13] image deck
 * @param objet[8] image objet
 * @param gobutton image go button
 * @param connectbutton image connect button
 * @param winImg image victory/failure
 */
void loadImg(SDL_Surface * deck[13], SDL_Surface *objet[8], SDL_Surface **gobutton, SDL_Surface **connectbutton, SDL_Surface *winImg[2]);
/** Event from mouse button down action. */
void onMouseButtonDownEvent();
/**
 * highlight the selections
 *
 * @param renderer the graphic window
 */
void highlightSelections(SDL_Renderer *renderer);
/**
 * draw the icons
 *
 * @param renderer the graphic window
 * @param texture_objet[8] picture of object
 */
void drawIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);
/**
 * highlight the selections
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawTextIcons(SDL_Renderer *renderer, TTF_Font *Sans);
/**
 * write name of the characters
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawTextCharacters(SDL_Renderer *renderer, TTF_Font *Sans);
/**
 * draw board
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawTab(SDL_Renderer *renderer, TTF_Font *Sans);
/**
 * draw the number of each object
 *
 * @param renderer the graphic window
 * @param texture_objet[8] picture of object
 */
void drawCharactersIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);
/**
 * draw the suggestions
 *
 * @param renderer the graphic window
 */
void drawX(SDL_Renderer *renderer);
/**
 * draw the guess chart
 *
 * @param renderer the graphic window
 */
void drawGuessChart(SDL_Renderer *renderer);
/**
 * draw the hand of the player
 *
 * @param renderer the graphic window
 * @param texture_deck[13] image of each card
 */
void drawHand(SDL_Renderer *renderer, SDL_Texture *texture_deck[13]);
/**
 * write the player name
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawPlayersName(SDL_Renderer *renderer, TTF_Font *Sans);
#endif // !GUI_H
