#ifndef GAMEDATA_H
#define GAMEDATA_H


///////////////////////////////////////////////////////////////////
// MACROS //
///////////////////////////////////////////////////////////////////
#define RGB_TO_16BIT(r, g, b) (((r & 0x1f) << 11) + ((g & 0x3f) << 5) + (b & 0x1f))

///////////////////////////////////////////////////////////////////
// CONSTANTS //
///////////////////////////////////////////////////////////////////
#define MAP_SPRITE_COUNT 16*12 // 192
#define MAP_BYTE_SIZE 16*12/2 // 96

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TILE_SIZE 20
#define TILE_PIXEL_COUNT TILE_SIZE*TILE_SIZE // 400
#define TILE_BYTE_SIZE TILE_PIXEL_COUNT*2 // 800

#define SPRITE_SIZE 16
#define SPRITE_PIXEL_COUNT SPRITE_SIZE*SPRITE_SIZE // 256
#define SPRITE_BYTE_SIZE SPRITE_PIXEL_COUNT*2 // 512

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
	unsigned short entryX;
	unsigned short entryY;

	char speed;

	signed short vx;
	signed short vy;

	//const unsigned short* currentSprite;

	unsigned char facing; // acts as index for which sprite is used to draw the player
	unsigned char onMapChangeTile;
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

struct Map{
	const unsigned char* mapTiles;
	const unsigned char nextMapIDb;
	const unsigned char nextMapIDu;
	const unsigned char nextMapIDd;
	const unsigned char nextMapIDl;
	const unsigned char nextMapIDr;
	const unsigned short startX;
	const unsigned short startY;
};

///////////////////////////////////////////////////////////////////
// GAME DATA //
///////////////////////////////////////////////////////////////////
extern const struct Map* maps[7];

/*
extern const unsigned short playerSprite[TILE_PIXEL_COUNT];
extern const unsigned short playerSprite2[SPRITE_PIXEL_COUNT];*/

extern const unsigned short* playerSprites[4];

extern const unsigned short* tiles[16];

#endif
