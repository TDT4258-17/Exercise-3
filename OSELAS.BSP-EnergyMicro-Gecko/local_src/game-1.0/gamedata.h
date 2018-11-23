#ifndef GAMEDATA_H
#define GAMEDATA_H

#define RGB_TO_16BIT(r, g, b) (((r & 0x1f) << 11) + ((g & 0x3f) << 5) + (b & 0x1f))
#define MAP_SPRITE_COUNT 16*12
#define MAP_BYTE_SIZE 16*12/2

#define SCREEN_WIDTH 320
#define SCREEN_HEIGTH 240
#define TILE_SIZE 20

extern const unsigned char map0[96];
extern const unsigned char map999[96];

extern const unsigned short tile0[400];
extern const unsigned short tile1[400];
extern const unsigned short tile2[400];
extern const unsigned short tile3[400];
extern const unsigned short tile4[400];

extern const unsigned short tile8[400];
extern const unsigned short tile9[400];
extern const unsigned short tile10[400];

extern const unsigned short tile14[400];
extern const unsigned short tile15[400];

extern const unsigned short player[400];

extern const unsigned char* maps[1];
extern const unsigned short* sprites[16];

#endif
