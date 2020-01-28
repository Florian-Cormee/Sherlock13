/**
 * Gui api. Hides away the draw operations.
 */
#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/** Index of the selected character. */
extern int playerSel;
/** Index of the selected object. */
extern int objectSel;
/** Index of the selected character. */
extern int guiltSel;
/** state of th button go. */
extern int goEnabled;
/** state of th button connect. */
extern int connectEnabled;

/** Initialize all the variables */
void initGui();

/**
 * Load the images of the game
 *
 * @param deck image deck
 * @param objet image objet
 * @param gobutton image go button
 * @param connectbutton image connect button
 * @param winImg image victory/failure
 */
void loadImg(SDL_Surface * deck[13], SDL_Surface *objet[8], SDL_Surface **gobutton, SDL_Surface **connectbutton, SDL_Surface *winImg[2]);

/** Handle the event from mouse button down. */
void onMouseButtonDownEvent();

/**
 * Highlight the selections
 *
 * @param renderer the graphic window
 */
void highlightSelections(SDL_Renderer *renderer);

/**
 * Draw the icons
 *
 * @param renderer the graphic window
 * @param texture_objet picture of object
 */
void drawIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);

/**
 * Highlight the selections
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawTextIcons(SDL_Renderer *renderer, TTF_Font *Sans);

/**
 * Write the name of the characters
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawTextCharacters(SDL_Renderer *renderer, TTF_Font *Sans);

/**
 * Draw the board
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawBoard(SDL_Renderer *renderer, TTF_Font *Sans);

/**
 * Draw the amount of each object
 *
 * @param renderer the graphic window
 * @param texture_objet[8] picture of object
 */
void drawCharactersIcons(SDL_Renderer *renderer,SDL_Texture *texture_objet[8]);

/**
 * Draw the suggestions
 *
 * @param renderer the graphic window
 */
void drawX(SDL_Renderer *renderer);

/**
 * Draw the guess board
 *
 * @param renderer the graphic window
 */
void drawGuessBoard(SDL_Renderer *renderer);

/**
 * Draw the hand of the player
 *
 * @param renderer the graphic window
 * @param texture_deck[13] image of each card
 */
void drawHand(SDL_Renderer *renderer, SDL_Texture *texture_deck[13]);

/**
 * Write the players' name
 *
 * @param renderer the graphic window
 * @param Sans style font
 */
void drawPlayersName(SDL_Renderer *renderer, TTF_Font *Sans);

#endif // !GUI_H
