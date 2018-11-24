#ifndef GAMELOGIC_H
#define GAMELOGIC_H


///////////////////////////////////////////////////////////////////
// GAME STATE STRUCTS //
///////////////////////////////////////////////////////////////////

struct Buttons{
	unsigned char left;
	unsigned char up;
	unsigned char right;
	unsigned char down;

	unsigned char y;
	unsigned char x;
	unsigned char a;
	unsigned char b;
};

struct Player{
	unsigned short px;
	unsigned short py;
	unsigned short pxOld;
	unsigned short pyOld;

	unsigned char life;
};

struct GameState{
	unsigned char currentMap;
};

struct GameIO{
	int fbfd;
	volatile short* fbmmap;
	int gpfd;
};

///////////////////////////////////////////////////////////////////
// GAME FUNCTIONS //
///////////////////////////////////////////////////////////////////

void setupGame(int screenfd, int buttonfd, volatile short* fbmmap_);
void gameLoop();
void readButtons(unsigned char gpio);

void drawMap();

void movePlayerDiscrete(signed char dx, signed char dy);
void drawPlayerDiscrete();

void drawPlayer();

#endif