#ifndef GAMELOGIC_H
#define GAMELOGIC_H

///////////////////////////////////////////////////////////////////
// GAME FUNCTIONS //
///////////////////////////////////////////////////////////////////

void setupGame(int screenfd, int buttonfd, volatile short* fbmmap_);
void gameLoop();
void readButtons(unsigned char gpio);
void changeMap(unsigned char dir);


//void movePlayerDiscrete(signed char dx, signed char dy);

void acceleratePlayer(signed char ax, signed char ay);
void playerUpdate();


void drawMap();
void clearScreen();
//void drawPlayerDiscrete();
void drawPlayer();

#endif