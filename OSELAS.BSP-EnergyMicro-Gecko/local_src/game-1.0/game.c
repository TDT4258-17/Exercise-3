#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	int fbfd = open("/dev/fb0", O_RDWR);

	volatile short* screen = mmap(0, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (screen == -1)
	{
		printf("ERROR creating memory map for the screen.\n");
	}

	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;


	screen[0] = 'Z' + ('Z' << 8);
	screen[1] = 'Z' + ('Z' << 8);
	screen[2] = 'Z' + ('Z' << 8);
	screen[3] = 'Z' + ('Z' << 8);

	screen[3000] = 'Z' + ('Z' << 8);
	screen[3001] = 'Z' + ('Z' << 8);
	screen[3002] = 'Z' + ('Z' << 8);
	screen[3003] = 'Z' + ('Z' << 8);

	screen[6000] = 3 << 12;
	screen[6001] = 15 << 12;
	screen[6002] = 'Z' + ('Z' << 8);
	screen[6003] = 'Z' + ('Z' << 8);


	char* data = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
	//lseek(fbfd, 0, SEEK_SET);
	int i;
	for (i = 0; i < 360*240; i++)
	{
		screen[i] = 'Z' + ('Z' << 8);
		//lseek(fbfd, 360*2*i, SEEK_SET);
		//write(fbfd, data, 360*2);
	}

	ioctl(fbfd, 0x4680, &rect);

	//char* data = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
	//lseek(fbfd, 3000, SEEK_SET);
	//write(fbfd, data, 30);


	close(fbfd);

	exit(EXIT_SUCCESS);
}
