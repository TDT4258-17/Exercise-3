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
	printf("New Game Started!!\n");
	
	int fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0)
		printf("ERROR opening screen file\n");
	int gpfd = open("/dev/gamepad", O_RDWR);
	if (gpfd < 0)
		printf("ERROR opening gamepad device\n");

	volatile short* screen = mmap(0, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!screen)
		printf("ERROR creating memory map for the screen.\n");

	setupGame(fbfd, gpfd, screen);	
	gameLoop();

	close(fbfd);
	close(gpfd);

	printf("Game Exit\n");
	exit(EXIT_SUCCESS);
}

