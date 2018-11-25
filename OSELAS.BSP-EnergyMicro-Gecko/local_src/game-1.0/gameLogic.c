#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdio.h>

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
static struct GameState gs;

static struct Buttons btns;
static struct Buttons btnsOld;

static struct Player plr;

///////////////////////////////////////////////////////////////////
// GAME FUNCTIONS //
///////////////////////////////////////////////////////////////////

void setupGame(int screenfd, int buttonfd, volatile short* fbmmap_)
{
	gameIO.fbfd = screenfd;
	gameIO.fbmmap = fbmmap_;

	gameIO.gpfd = buttonfd;

	gs.currentMap = 0;

	plr.px = 40;
	plr.py = 120;
	plr.pxOld = plr.px;
	plr.pyOld = plr.py;

	plr.vx = 0;
	plr.vy = 0;

	plr.posUpdate = 1;
	plr.life = 100;

	drawMap();
	drawPlayer();
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

		if (btns.x)
		{
			quit = 1;
		}

		signed char ax = 0;
		signed char ay = 0;

		if (btns.left)
		{
			ax = -1;
		}
		if (btns.up)
		{
			ay = -1;
		}
		if (btns.right)
		{
			ax += 1;
		}
		if (btns.down)
		{
			ay += 1;
		}

		acceleratePlayer(ax, ay);
		if (plr.vx/10 > 0 || plr.vy/10 > 0 || plr.vx/10 < 0 || plr.vy/10 < 0)
		{
			playerUpdate();
			drawPlayer();
			plr.posUpdate = 0;
		}
		else
		{
			plr.vx = 0;
			plr.vy = 0;
		}

		usleep(20000);
	}

	clearScreen();
}

void readButtons(unsigned char gpio)
{
	btns.left  =  gpio & 0x01;
	btns.up    = (gpio & 0x02) >> 1;
	btns.right = (gpio & 0x04) >> 2;
	btns.down  = (gpio & 0x08) >> 3;

	btns.y = (gpio & 0x10) >> 4;
	btns.x = (gpio & 0x20) >> 5;
	btns.a = (gpio & 0x40) >> 6;
	btns.b = (gpio & 0x80) >> 7;
}

void acceleratePlayer(signed char ax, signed char ay)
{
	plr.vx += 18*ax - plr.vx/2;
	plr.vy += 18*ay - plr.vy/2;
}

unsigned char checkOutOfBounds(short pxn, short pyn)
{

	unsigned char outOfBounds = 0;

	if ((pxn < 0 ))
	{
		changeMap(3);
		outOfBounds = 1;
	}
	if ((pxn + SPRITE_SIZE-1) >= 320)
	{
		changeMap(4);
		outOfBounds = 1;
	}
	if (pyn < 0)
	{
		changeMap(1);
		outOfBounds = 1;
	}
	if ((pyn + SPRITE_SIZE-1) >= 240)
	{
		changeMap(2);
		outOfBounds = 1;
	}
	return outOfBounds;
}

void playerUpdate()
{

	plr.pxOld = plr.px;
	plr.pyOld = plr.py;

	short dx = plr.vx/10;
	short dy = plr.vy/10;

	short pxNext = plr.px + dx;
	short pyNext = plr.py + dy;

	const unsigned char* map = maps[gs.currentMap]->mapTiles;

	// checking borders
	unsigned char outOfBounds = 0;
	outOfBounds = checkOutOfBounds(pxNext, pyNext);

	unsigned char goodMoveX = 0;
	unsigned char goodMoveY = 0;
	unsigned char collision = 0;
	if (outOfBounds)
	{
		// do something appropriate (maybe do not need to)
		printf("Out of bounds. player pos: %i, %i; player old pos: %i, %i\n", plr.px, plr.py, plr.pxOld, plr.pyOld);
	}
	else
	{
	// check for collisions
		// check for collisions along X axis

		if (dx > 0)
		{ // need to check upper and lower right tile

			short tile_ur_x = 20*((pxNext + SPRITE_SIZE-1)/20);
			short tile_ur_y = 20*( plr.py/20);

			short tile_lr_x = 20*((pxNext + SPRITE_SIZE-1)/20);
			short tile_lr_y = 20*((plr.py + SPRITE_SIZE-1)/20);

			int tileIndex_ur = ((16*(tile_ur_y/20))+(tile_ur_x/20));
			int tileIndex_lr = ((16*(tile_lr_y/20))+(tile_lr_x/20));

			unsigned char tileID_ur = (map[tileIndex_ur/2] >> 4*((tileIndex_ur+1)%2)) & 0x0f;
			unsigned char tileID_lr = (map[tileIndex_lr/2] >> 4*((tileIndex_lr+1)%2)) & 0x0f;

			if (((tileID_ur & (1 << 3)) == 0) && ((tileID_lr & (1 << 3)) == 0)) // no wall in the way
			{
				goodMoveX = 1;
			}
			else
			{
				collision = 1;
			}
		}
		else
		{ // need to check upper and lower left tile

			short tile_ul_x = 20*(pxNext/20);
			short tile_ul_y = 20*(plr.py/20);

			short tile_ll_x = 20*( pxNext/20);
			short tile_ll_y = 20*((plr.py + SPRITE_SIZE-1)/20);

			
			int tileIndex_ul = ((16*(tile_ul_y/20))+(tile_ul_x/20));
			int tileIndex_ll = ((16*(tile_ll_y/20))+(tile_ll_x/20));

			unsigned char tileID_ul = (map[tileIndex_ul/2] >> 4*((tileIndex_ul+1)%2)) & 0x0f;
			unsigned char tileID_ll = (map[tileIndex_ll/2] >> 4*((tileIndex_ll+1)%2)) & 0x0f;

			if (((tileID_ul & (1 << 3)) == 0) && ((tileID_ll & (1 << 3)) == 0)) // no wall in the way
			{
				goodMoveX = 1;
			}
			else
			{
				collision = 1;
			}
		}

		if (dy > 0)
		{ // need to check lower left and right tile

			short tile_ll_x = 20*( plr.px/20);
			short tile_ll_y = 20*((pyNext + SPRITE_SIZE-1)/20);

			short tile_lr_x = 20*((plr.px + SPRITE_SIZE-1)/20);
			short tile_lr_y = 20*((pyNext + SPRITE_SIZE-1)/20);

			int tileIndex_ll = ((16*(tile_ll_y/20))+(tile_ll_x/20));
			int tileIndex_lr = ((16*(tile_lr_y/20))+(tile_lr_x/20));

			unsigned char tileID_ll = (map[tileIndex_ll/2] >> 4*((tileIndex_ll+1)%2)) & 0x0f;
			unsigned char tileID_lr = (map[tileIndex_lr/2] >> 4*((tileIndex_lr+1)%2)) & 0x0f;

			if (((tileID_ll & (1 << 3)) == 0) && ((tileID_lr & (1 << 3)) == 0)) // no wall in the way
			{
				goodMoveY = 1;
			}
			else
			{
				collision = 1;
			}
		}
		else
		{ // need to check upper left and right tile

			short tile_ul_x = 20*(plr.px/20);
			short tile_ul_y = 20*(pyNext/20);

			short tile_ur_x = 20*((plr.px + SPRITE_SIZE-1)/20);
			short tile_ur_y = 20*( pyNext/20);

			int tileIndex_ul = ((16*(tile_ul_y/20))+(tile_ul_x/20));
			int tileIndex_ur = ((16*(tile_ur_y/20))+(tile_ur_x/20));

			unsigned char tileID_ul = (map[tileIndex_ul/2] >> 4*((tileIndex_ul+1)%2)) & 0x0f;
			unsigned char tileID_ur = (map[tileIndex_ur/2] >> 4*((tileIndex_ur+1)%2)) & 0x0f;

			if (((tileID_ul & (1 << 3)) == 0) && ((tileID_ur & (1 << 3)) == 0)) // no wall in the way
			{
				goodMoveY = 1;
			}
			else
			{
				collision = 1;
			}
		}

		if (goodMoveX)
			plr.px = pxNext;
		if (goodMoveY)
			plr.py = pyNext;

		if (collision)
		{
			plr.vx = 0;
			plr.vy = 0;
		}
	}
}	

///////////////////////////////////////////////////////////////////
// GAME GRAPHICS //
///////////////////////////////////////////////////////////////////

void drawMap()
{
	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	unsigned char tileIndex;
	unsigned int i;
	unsigned int j;

	const unsigned char* map = maps[gs.currentMap]->mapTiles;


	// in the moment it seemed to me to be easier to iterate 
	// through tiles and not through the bytes in the map ...
	for (tileIndex = 0; tileIndex < MAP_SPRITE_COUNT; tileIndex++)
	{
		// .. which is why tileIndex is divided by two here while
		// decoding the tile, since two tiles are stored in one byte
		unsigned char tileID = (map[ tileIndex/2 ] >> 4*((tileIndex+1)%2)) & 0x0f;

		const unsigned short* tile = tiles[tileID];

		// decoding tile pixel position
		int x = (20*tileIndex)%320;
		int y = 20*((20*tileIndex)/320);

		int offset = x + 320*y;

		// drawing tile to the screen buffer
		for (j = 0; j < TILE_SIZE; j++)
		{
			for (i = 0; i < TILE_SIZE; i++)
			{
				gameIO.fbmmap[offset + 320*j + i] = tile[i + 20*j];
			}
		}
	}
	// pushing buffer to screen
	ioctl(gameIO.fbfd, 0x4680, &rect);
}

void changeMap(unsigned char dir)
{
	char nextMapID;
	switch (dir)
	{
	case 0: //block
		nextMapID = maps[gs.currentMap]->nextMapIDb;
		break;
	case 1: //up
		nextMapID = maps[gs.currentMap]->nextMapIDu;
		plr.py = SCREEN_HEIGHT - SPRITE_SIZE;
		break;
	case 2: //down
		nextMapID = maps[gs.currentMap]->nextMapIDd;
		plr.py = 0;
		break;
	case 3: //left
		nextMapID = maps[gs.currentMap]->nextMapIDl;
		plr.px = SCREEN_WIDTH - SPRITE_SIZE;
		break;
	case 4: //right
		nextMapID = maps[gs.currentMap]->nextMapIDr;
		plr.px = 0;
		break;
	}
	if (nextMapID != 255)
	{
		gs.currentMap = nextMapID;
		printf("New MAP: %i!\n", gs.currentMap);

		drawMap();
		drawPlayer();
	}
	else
	{
		plr.px = SCREEN_WIDTH/2;
		plr.py = SCREEN_HEIGHT/2;
		printf("Not actually new map: %i!\n", gs.currentMap);
	}
}

void clearScreen()
{
	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	int i;
	int j;

	// drawing tile to the screen buffer
	for (j = 0; j < SCREEN_HEIGHT; j++)
	{
		for (i = 0; i < SCREEN_WIDTH; i++)
		{
			gameIO.fbmmap[320*j + i] = 0;
		}
	}

	// pushing buffer to screen
	ioctl(gameIO.fbfd, 0x4680, &rect);
}

void drawPlayer()
{

	short tile_ul_x = (plr.pxOld/20);
	short tile_ul_y = (plr.pyOld/20);

	short tile_ur_x = ((plr.pxOld + SPRITE_SIZE-1)/20);
	short tile_ur_y = ( plr.pyOld/20);

	short tile_ll_x = ( plr.pxOld/20);
	short tile_ll_y = ((plr.pyOld + SPRITE_SIZE-1)/20);

	short tile_lr_x = ((plr.pxOld + SPRITE_SIZE-1)/20);
	short tile_lr_y = ((plr.pyOld + SPRITE_SIZE-1)/20);

	unsigned char tile_ul_Index = 16*tile_ul_y + tile_ul_x;
	unsigned char tile_ur_Index = 16*tile_ur_y + tile_ur_x;
	unsigned char tile_ll_Index = 16*tile_ll_y + tile_ll_x;
	unsigned char tile_lr_Index = 16*tile_lr_y + tile_lr_x;


	unsigned char tileRedrawCase = 0;

	struct fb_copyarea rect;
	rect.dx = tile_ul_x*20;
	rect.dy = tile_ul_y*20;
	rect.width = 40;
	rect.height = 40;

	if (tile_ul_Index == tile_ur_Index)
	{
		rect.width = 20;
		tileRedrawCase |= 1;
	}
	if (tile_ul_Index == tile_ll_Index)
	{
		rect.height = 20;
		tileRedrawCase |= 2;
	}

	unsigned char tileRedrawCount;
	unsigned char tilesToRedraw[4];

	switch (tileRedrawCase)
	{
	case 0:	// redraw all four tiles
		tileRedrawCount = 4;
		tilesToRedraw[0] = tile_ul_Index;
		tilesToRedraw[1] = tile_ur_Index;
		tilesToRedraw[2] = tile_ll_Index;
		tilesToRedraw[3] = tile_lr_Index;
		break;
	case 1: // redraw upper and lower tiles
		tileRedrawCount = 2;
		tilesToRedraw[0] = tile_ul_Index;
		tilesToRedraw[1] = tile_ll_Index;
		break;
	case 2: // redraw left and right tiles
		tileRedrawCount = 2;
		tilesToRedraw[0] = tile_ul_Index;
		tilesToRedraw[1] = tile_ur_Index;
		break;
	case 3: // redraw oe tile
		tileRedrawCount = 1;
		tilesToRedraw[0] = tile_ul_Index;
		break;
	}


	unsigned char tileIndex;
	int k;
	int i;
	int j;

	const unsigned char* map = maps[gs.currentMap]->mapTiles;

	for (k = 0; k < tileRedrawCount; k++)
	{
		tileIndex = tilesToRedraw[k];

		unsigned char tileID = (map[ tileIndex/2 ] >> 4*((tileIndex+1)%2)) & 0x0f;

		const unsigned short* tile = tiles[tileID];

		// decoding tile pixel position
		int x = (20*tileIndex)%320;
		int y = 20*((20*tileIndex)/320);

		int offset = x + 320*y;

		// drawing tile to the screen buffer
		for (j = 0; j < TILE_SIZE; j++)
		{
			for (i = 0; i < TILE_SIZE; i++)
			{
				gameIO.fbmmap[offset + 320*j + i] = tile[i + 20*j];
			}
		}
	}


	int offset = plr.px + 320*plr.py;
	int index;
	for (j = 1; j < SPRITE_SIZE; j++)
	{
		for (i = 0; i < SPRITE_SIZE; i++)
		{

			index = i + SPRITE_SIZE*j;
			if (playerSprite2[index] != 0)
			{
				gameIO.fbmmap[offset + 320*j + i] = playerSprite2[index];
			}
		}
	}


	ioctl(gameIO.fbfd, 0x4680, &rect);

	rect.dx = plr.px;
	rect.dy = plr.py;
	rect.width = SPRITE_SIZE;
	rect.height = SPRITE_SIZE;

	// pushing buffer to screen
	ioctl(gameIO.fbfd, 0x4680, &rect);
}