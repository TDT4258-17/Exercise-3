#include <linux/fb.h>
#include <sys/ioctl.h>

//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/mman.h>
//#include <fcntl.h>
#include <unistd.h>

#include "gameLogic.h"
#include "gameData.h"

///////////////////////////////////////////////////////////////////
// GAME STATE & CONFIG VARIABLES //
///////////////////////////////////////////////////////////////////
static struct GameIO gameIO;
static struct Player plr;
static struct GameState gs;

static struct Buttons btns;
static struct Buttons btnsOld;

///////////////////////////////////////////////////////////////////
// GAME FUNCTIONS //
///////////////////////////////////////////////////////////////////

void setupGame(int screenfd, int buttonfd, volatile short* fbmmap_)
{
	gameIO.fbfd = screenfd;
	gameIO.fbmmap = fbmmap_;

	gameIO.gpfd = buttonfd;

	gs.currentMap = 0;

	plr.px = 20;
	plr.py = 20;
	plr.pxOld = 20;
	plr.pyOld = 20;
	plr.life = 100;

	drawMap();
}

void gameLoop()
{
	unsigned char quit = 0;
	unsigned char gpio = 0;

	while (quit == 0)
	{
		read(gameIO.gpfd, &gpio, 0);

		btnsOld = btns;
		readButtons(gpio);

		if (btns.left & !btnsOld.left)
		{
			//movePlayerDiscrete(gs.currentMap, gameIO.fbmmap, -20, 0, &plr);
			movePlayerDiscrete(-20, 0);
		}
		if (btns.up & !btnsOld.up)
		{
			movePlayerDiscrete(0, -20);
		}
		if (btns.right & !btnsOld.right)
		{
			movePlayerDiscrete(20, 0);
		}
		if (btns.down & !btnsOld.down)
		{
			movePlayerDiscrete(0, 20);
		}

		if (btns.x)
		{
			quit = 1;
		}

		usleep(33000);
	}
}

void readButtons(unsigned char gpio)
{
	btns.left = gpio & 0x01;
	btns.up = (gpio & 0x02) >> 1;
	btns.right = (gpio & 0x04) >> 2;
	btns.down = (gpio & 0x08) >> 3;

	btns.y = (gpio & 0x10) >> 4;
	btns.x = (gpio & 0x20) >> 5;
	btns.a = (gpio & 0x40) >> 6;
	btns.b = (gpio & 0x80) >> 7;
}

void drawMap()
{
	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	int k;
	int i;
	int j;

	const unsigned char* map = maps[gs.currentMap];

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
				gameIO.fbmmap[offset + 320*j + i] = sprite[i + 20*j];
			}
		}
	}
	// pushing buffer to screen
	ioctl(gameIO.fbfd, 0x4680, &rect);
}

void movePlayerDiscrete(signed char dx, signed char dy) 
{
	const unsigned char* map = maps[gs.currentMap];

	plr.pxOld = plr.px;
	plr.pyOld = plr.py;

	int pxNext = (plr.px + dx);
	int pyNext = (plr.py + dy);
	
	// checking borders
	if ((pxNext < 0 ) || (pxNext >= 320) || (pyNext < 0) || (pyNext >= 240))
	{
		return;
	}

	int mapTile = ((16*(pyNext/20))+(pxNext/20));
	
	if ((mapTile % 2 == 0) && ((map[mapTile/2] & (1 << 7)) == 0))	// mapTile is even AND move is valid
	{
		plr.px = pxNext;
		plr.py = pyNext;
	}
	else if ((mapTile % 2 == 1) && ((map[mapTile/2] & (1 << 3)) == 0))  	// mapTile is odd AND move is valid
	{
		plr.px = pxNext;
		plr.py = pyNext;
	}
	
	drawPlayerDiscrete();
}


void drawPlayerDiscrete()
{
	int i;
	int j;

	const unsigned char* map = maps[gs.currentMap];

	unsigned char tileIndex = plr.pxOld/20 + (plr.pyOld/20) * 16;
	unsigned char tileID = (map[tileIndex/2] >> 4*((tileIndex+1)%2)) & 0x0f;
	const unsigned short* tile = sprites[tileID];

	int offset = plr.pxOld + 320*plr.pyOld;
	// redrawing tile
	
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (tile[index] != 0)
			{
				gameIO.fbmmap[offset + 320*j + i] = tile[index];
			}
		}
	}


	// drawing new player pos
	offset = plr.px + 320*plr.py;
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (playerSprite[index] != 0)
			{
				gameIO.fbmmap[offset + 320*j + i] = playerSprite[index];
			}
		}
	}

	// easiest (also simplest?) way is to issue two ioctl calls
	// instead of one ioctl call where the rects are combined in some smart way
	struct fb_copyarea rect;
	rect.dx = plr.pxOld;
	rect.dy = plr.pyOld;
	rect.width = 20;
	rect.height = 20;

	ioctl(gameIO.fbfd, 0x4680, &rect);

	rect.dx = plr.px;
	rect.dy = plr.py;
	rect.width = 20;
	rect.height = 20;

	ioctl(gameIO.fbfd, 0x4680, &rect);
}

void drawPlayer()
{
	short tile_ul_x = plr.px/20;
	short tile_ul_y = plr.py/20;

	short tile_ur_x = plr.px/20 + 1;
	short tile_ur_y = plr.py/20;

	short tile_ll_x = plr.px/20;
	short tile_ll_y = plr.py/20 + 1;

	short tile_lr_x = plr.px/20 + 1;
	short tile_lr_y = plr.py/20 + 1;

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

	const unsigned char* map = maps[gs.currentMap];




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
				gameIO.fbmmap[offset + 320*j + i] = sprite[i + 20*j];
			}
		}
	}*/


	
	int offset = plr.px + 320*plr.py;
	for (j = 0; j < 20; j++)
	{
		for (i = 0; i < 20; i++)
		{
			int index = i + 20*j;
			if (playerSprite[index] != 0)
			{
				gameIO.fbmmap[offset + 320*j + i] = playerSprite[index];
			}
		}
	}


	// pushing buffer to screen
	ioctl(fd, 0x4680, &rect);
}
