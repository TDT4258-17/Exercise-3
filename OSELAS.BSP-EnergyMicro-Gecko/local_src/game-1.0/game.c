#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/read.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "gameData.h"
#include "gameLogic.h"


int main(int argc, char *argv[])
{
	// Opening IO devices

	int error = 0;

	int fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0)
	{
		printf("ERROR opening screen file\n");
		error = 1;
	}

	volatile short* screen = mmap(0, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!screen)
	{
		printf("ERROR creating memory map for the screen.\n");
		error = 1;
	}

	int gpfd = open("/dev/gamepad", O_RDWR);
	if (gpfd < 0)
	{
		printf("ERROR opening gamepad device\n");
		error = 1;
	}

	int sdfd = open("/dev/sound", O_RDWR);
	if (sdfd < 0)
	{
		printf("ERROR opening sound device\n");
		error = 1;
	}

	// starting game if everything was fine
	
	if (error == 0)
	{
		setupGame(fbfd, gpfd, sdfd, screen);
		gameLoop();
	}

	// closing IO devices

	close(fbfd);
	close(gpfd);
	close(sdfd);

	exit(EXIT_SUCCESS);
}

