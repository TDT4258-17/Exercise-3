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
#define TILE_BYTE_SIZE 800

///////////////////////////////////////////////////////////////////
// MAPS //
///////////////////////////////////////////////////////////////////
extern const unsigned char map0[MAP_BYTE_SIZE];
extern const unsigned char map1[MAP_BYTE_SIZE];
extern const unsigned char map999[MAP_BYTE_SIZE];

///////////////////////////////////////////////////////////////////
// TILES //
///////////////////////////////////////////////////////////////////
extern const unsigned short tile0[TILE_PIXEL_COUNT];
extern const unsigned short tile1[TILE_PIXEL_COUNT];
extern const unsigned short tile2[TILE_PIXEL_COUNT];
extern const unsigned short tile3[TILE_PIXEL_COUNT];
extern const unsigned short tile4[TILE_PIXEL_COUNT];
extern const unsigned short tile5[TILE_PIXEL_COUNT];

extern const unsigned short tile8[TILE_PIXEL_COUNT];
extern const unsigned short tile9[TILE_PIXEL_COUNT];
extern const unsigned short tile10[TILE_PIXEL_COUNT];

extern const unsigned short tile14[TILE_PIXEL_COUNT];
extern const unsigned short tile15[TILE_PIXEL_COUNT];

///////////////////////////////////////////////////////////////////
// OTHER //
///////////////////////////////////////////////////////////////////
extern const unsigned short playerSprite[TILE_PIXEL_COUNT];

extern const unsigned char* maps[2];
extern const unsigned short* sprites[16];

#endif
