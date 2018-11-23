#ifndef GAMELOGIC_H
#define GAMELOGIC_H

void drawMap(int fd, int mapIndex, volatile short* screen);
void drawPlayerDiscrete(int fd, int mapIndex, unsigned short px, unsigned short py, unsigned short px_old, unsigned short py_old, volatile short* screen);
void drawPlayer(int fd, int mapIndex, unsigned short px, unsigned short py, volatile short* screen);
void movePlayer(int fd, int mapIndex, signed char dx, signed char dy, unsigned short* px, unsigned short* py, volatile short* screen);

#endif