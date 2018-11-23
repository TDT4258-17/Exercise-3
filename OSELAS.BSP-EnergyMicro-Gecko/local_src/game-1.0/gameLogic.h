#ifndef GAMELOGIC_H
#define GAMELOGIC_H

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


void readButtons(struct Buttons* buttons, unsigned char gpio);
void drawMap(int fd, int mapIndex, volatile short* screen);

//void movePlayer(int fd, int mapIndex, signed char dx, signed char dy, unsigned short* px, unsigned short* py, volatile short* screen);
void drawPlayerDiscrete(int fd, int mapIndex, unsigned short px, unsigned short py, unsigned short px_old, unsigned short py_old, volatile short* screen);
void drawPlayer(int fd, int mapIndex, unsigned short px, unsigned short py, volatile short* screen);


void movePlayer(int fd, int mapIndex, volatile short* screen, signed char dx, signed char dy, struct Player* player);/*
void drawPlayerDiscrete(int fd, int mapIndex, volatile short* screen, struct Player* player);
void drawPlayer(int fd, int mapIndex, volatile short* screen, struct Player* player);
*/
#endif