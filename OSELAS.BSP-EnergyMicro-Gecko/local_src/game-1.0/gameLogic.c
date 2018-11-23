#include <linux/fb.h>

#include "gameLogic.h"
#include "gamedata.h"


void readButtons(struct Buttons* buttons, unsigned char gpio)
{
	buttons->left = gpio & 0x01;
	buttons->up = (gpio & 0x02) >> 1;
	buttons->right = (gpio & 0x04) >> 2;
	buttons->down = (gpio & 0x08) >> 3;

	buttons->y = (gpio & 0x10) >> 4;
	buttons->x = (gpio & 0x20) >> 5;
	buttons->a = (gpio & 0x40) >> 6;
	buttons->b = (gpio & 0x80) >> 7;
}

void drawMap(int fd, int mapIndex, volatile short* screen)
{

	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;

	int k;
	int i;
	int j;

	const unsigned char* map = maps[mapIndex];

	// in the moment it seemed to me to be easier to iterate 
	// through tiles/sprites and not through the bytes in the map ...
	for (k = 0; k < MAP_SPRITE_COUNT; k++)
	{
		// .. which is why k is divided by two here while
		// decoding the tile/sprite
		unsigned char spriteID = (map[k/2] >> 4*((k+1)%2)) & 0x0f;
		const unsigned short* sprite = sprites[spriteID];

		// decoding tile position
		int x = (20*k)%320;
		int y = 20*((20*k)/320) - 1;
		int offset = x + 320*y;

		// drawing tile to the screen buffer
		for (j = 0; j < 20; j++)
		{
			for (i = 0; i < 20; i++)
			{
				screen[offset + 320*j + i] = sprite[i + 20*j];
			}
		}
	}
	// pushing buffer to screen
	ioctl(fd, 0x4680, &rect);
}

void movePlayer(int fd, int mapIndex, volatile short* screen, signed char dx, signed char dy, struct Player* player) 
{
	const unsigned char* map = maps[mapIndex];

	int pxOld = player->px;
	int pyOld = player->py;

	int pxNext = (player->px + dx);
	int pyNext = (player->py + dy);
	
	// checking borders
	if ((pxNext < 0 ) || (pxNext >= 320) || (pyNext < 0) || (pyNext >= 240))
	{
		return;
	}

	int mapTile = ((16*(pyNext/20))+(pxNext/20));
	
	if ((mapTile % 2 == 0) && ((map[mapTile/2] & (1 << 7)) == 0))	// mapTile is even AND move is valid
	{
		player->px = pxNext;
		player->py = pyNext;
	}
	else if ((mapTile % 2 == 1) && ((map[mapTile/2] & (1 << 3)) == 0))  	// mapTile is odd AND move is valid
	{
		player->px = pxNext;
		player->py = pyNext;
	}
	
	drawPlayerDiscrete(fd, mapIndex, player->px, player->py, pxOld, pyOld, screen);	
}

/*
void movePlayer(int fd, int mapIndex, signed char dx, signed char dy, unsigned short* px, unsigned short* py, volatile short* screen) 
{
	const unsigned char* map = maps[mapIndex];

	int pxOld = *px;
	int pyOld = *py;

	int pxNext = (*px + dx);
	int pyNext = (*py + dy);
	
	// checking borders
	if ((pxNext < 0 ) || (pxNext >= 320) || (pyNext < 0) || (pyNext >= 240))
	{
		return;
	}

	int mapTile = ((16*(pyNext/20))+(pxNext/20));
	
	if ((mapTile % 2 == 0) && ((map[mapTile/2] & (1 << 7)) == 0))	// mapTile is even AND move is valid
	{
		*px = pxNext;
		*py = pyNext;
	}
	else if ((mapTile % 2 == 1) && ((map[mapTile/2] & (1 << 3)) == 0))  	// mapTile is odd AND move is valid
	{
		*px = pxNext;
		*py = pyNext;
	}
	
	drawPlayerDiscrete(fd, mapIndex, *px, *py, pxOld, pyOld, screen);	
}*/

void drawPlayerDiscrete(int fd, int mapIndex, unsigned short px, unsigned short py, unsigned short px_old, unsigned short py_old, volatile short* screen)
{
	int i;
	int j;

	const unsigned char* map = maps[mapIndex];
	//const unsigned short* player = sprites[2];

	unsigned char tileIndex = px_old/20 + (py_old/20) * 16;
	unsigned char tileID = (map[tileIndex/2] >> 4*((tileIndex+1)%2)) & 0x0f;
	const unsigned short* tile = sprites[tileID];

	int offset = px_old + 320*py_old;
	// redrawing tile
	
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (tile[index] != 0)
			{
				screen[offset + 320*j + i] = tile[index];
			}
		}
	}

	// drawing new player pos
	offset = px + 320*py;
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (player[index] != 0)
			{
				screen[offset + 320*j + i] = player[index];
			}
		}
	}

	// easiest (also simplest?) way is to issue two ioctl calls
	// instead of one ioctl call where the rects are combined in some smart way
	struct fb_copyarea rect;
	rect.dx = px_old;
	rect.dy = py_old;
	rect.width = 20;
	rect.height = 20;

	ioctl(fd, 0x4680, &rect);

	rect.dx = px;
	rect.dy = py;
	rect.width = 20;
	rect.height = 20;

	ioctl(fd, 0x4680, &rect);
}


void drawPlayer(int fd, int mapIndex, unsigned short px, unsigned short py, volatile short* screen)
{
	short tile_ul_x = px/20;
	short tile_ul_y = py/20;

	short tile_ur_x = px/20 + 1;
	short tile_ur_y = py/20;

	short tile_ll_x = px/20;
	short tile_ll_y = py/20 + 1;

	short tile_lr_x = px/20 + 1;
	short tile_lr_y = py/20 + 1;

	int tile_ul_ID = 16*tile_ul_y + tile_ul_x;
	int tile_ur_ID = 16*tile_ur_y + tile_ur_x;
	int tile_ll_ID = 16*tile_ll_y + tile_ll_x;
	int tile_lr_ID = 16*tile_lr_y + tile_lr_x;

	struct fb_copyarea rect;
	rect.dx = tile_ul_x*20;
	rect.dy = tile_ul_y*20;
	rect.width = 40;
	rect.height = 40;

	int k;
	int i;
	int j;

	const unsigned char* map = maps[mapIndex];

	const unsigned short* sprite = sprites[2];




/*

	for (k = 0; k < SCREEN_SPRITE_COUNT; k++)
	{
		// .. which is why k is divided by two here while
		// decoding the tile/sprite
		unsigned char spriteID = (map[k/2] >> 4*(k%2)) & 0x0f;
		const unsigned short* sprite = sprites[spriteID];

		// decoding tile position
		int x = (20*k)%320;
		int y = 20*((20*k)/320);
		int offset = x + 320*y;

		// drawing tile to the screen buffer
		for (j = 0; j < 20; j++)
		{
			for (i = 0; i < 20; i++)
			{
				screen[offset + 320*j + i] = sprite[i + 20*j];
			}
		}
	}*/


	
	int offset = px + 320*py;
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (sprite[index] != 0)
			{
				screen[offset + 320*j + i] = sprite[index];
			}
		}
	}


	// pushing buffer to screen
	ioctl(fd, 0x4680, &rect);
}
