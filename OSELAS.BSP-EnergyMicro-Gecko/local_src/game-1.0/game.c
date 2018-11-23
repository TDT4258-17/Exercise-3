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
#include "gameLogic.h"

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

void readButtons(struct Buttons* buttons, unsigned char gpio);

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	int fbfd = open("/dev/fb0", O_RDWR);
	int gpfd = open("/dev/gamepad", O_RDWR);
	if (gpfd < 0)
	{
		printf("ERROR opening gamepad device.\n");
	}

	volatile short* screen = mmap(0, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!screen)
	{
		printf("ERROR creating memory map for the screen.\n");
	}




	unsigned char currentMap = 0;

	unsigned short px = 140;
	unsigned short py = 120;


	drawMap(fbfd, currentMap, screen);

	int quit = 0;

	struct Buttons buttons;
	struct Buttons buttons_old;

	while (quit == 0)
	{
		unsigned char gpio;
		int test = read(gpfd, &gpio, 0);

		buttons_old = buttons;
		readButtons(&buttons, gpio);

		if (buttons.left & !buttons_old.left)
		{
			movePlayer(fbfd, currentMap, -20, 0, &px, &py, screen);
		}
		if (buttons.up & !buttons_old.up)
		{
			movePlayer(fbfd, currentMap, 0, -20, &px, &py, screen);
		}
		if (buttons.right & !buttons_old.right)
		{
			movePlayer(fbfd, currentMap, 20, 0, &px, &py, screen);
		}
		if (buttons.down & !buttons_old.down)
		{
			movePlayer(fbfd, currentMap, 0, 20, &px, &py, screen);
		}

		if (buttons.x)
		{
			quit = 1;
		}

		usleep(33000);
	}


	close(fbfd);
	close(gpfd);

	exit(EXIT_SUCCESS);
}


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