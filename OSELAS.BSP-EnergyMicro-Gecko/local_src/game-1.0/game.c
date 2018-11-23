#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
//#include <sys/read.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include "gamedata.h"

void drawMap(int fd, int mapIndex, volatile short* screen);
void drawPlayer(int fd, int mapIndex, unsigned short px, unsigned short py, volatile short* screen);

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	int fbfd = open("/dev/fb0", O_RDWR);
	int gpfd = open("/dev/gamepad", O_RDWR);
	if (gpfd < 0)
	{
		printf("ERROR opening gamepad device.\n");
	}

	char buttons;
	int test = read(gpfd, &buttons, 0);
	if (test < 0)
	{
		printf("ERROR reading buttons.\n");
	}
	else
	{
		printf("%i: what is this?\n", (int)buttons);
	}

	volatile short* screen = mmap(0, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!screen)
	{
		printf("ERROR creating memory map for the screen.\n");
	}




	unsigned char currentMap = 0;

	unsigned short px = 111;
	unsigned short py = 131;


	drawMap(fbfd, currentMap, screen);

	int quit = 0;

	while (quit == 0)
	{
		int test = read(gpfd, &buttons, 0);

		unsigned char btn_left = buttons & 0x01;
		unsigned char btn_up = (buttons & 0x02) >> 1;
		unsigned char btn_right = (buttons & 0x04) >> 2;
		unsigned char btn_down = (buttons & 0x08) >> 3;

		unsigned char btn_y = (buttons & 0x10) >> 4;
		unsigned char btn_x = (buttons & 0x20) >> 5;
		unsigned char btn_a = (buttons & 0x40) >> 6;
		unsigned char btn_b = (buttons & 0x80) >> 7;

		if (btn_left)
		{
			px = px - 3;
		}
		if (btn_up)
		{
			py = py - 3;
		}
		if (btn_right)
		{
			px = px + 3;
		}
		if (btn_down)
		{
			py = py + 3;
		}

		if (btn_x)
		{
			quit = 1;
		}
		drawPlayer(fbfd, currentMap, px, py, screen);
		usleep(100000);
	}


	close(fbfd);
	close(gpfd);

	exit(EXIT_SUCCESS);
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
	}
	// pushing buffer to screen
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