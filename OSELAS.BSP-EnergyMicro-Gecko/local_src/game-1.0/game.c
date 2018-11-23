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

	unsigned short px = 20;
	unsigned short py = 20;

	struct Player player;
	player.px = px;
	player.py = py;
	player.pxOld = px;
	player.pyOld = py;
	player.life = 100;


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

		if (gpio)
		{
			printf("Px: %i; Py %i\n", px, py);
		}

		if (buttons.left & !buttons_old.left)
		{
			//movePlayer(fbfd, currentMap, -20, 0, &px, &py, screen);
			movePlayer(fbfd, currentMap, screen, -20, 0, &player);
		}
		if (buttons.up & !buttons_old.up)
		{
			//movePlayer(fbfd, currentMap, 0, -20, &px, &py, screen);
			movePlayer(fbfd, currentMap, screen, 0, -20, &player);
		}
		if (buttons.right & !buttons_old.right)
		{
			//movePlayer(fbfd, currentMap, 20, 0, &px, &py, screen);
			movePlayer(fbfd, currentMap, screen, 20, 0, &player);
		}
		if (buttons.down & !buttons_old.down)
		{
			//movePlayer(fbfd, currentMap, 0, 20, &px, &py, screen);
			movePlayer(fbfd, currentMap, screen, 0, 20, &player);
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

