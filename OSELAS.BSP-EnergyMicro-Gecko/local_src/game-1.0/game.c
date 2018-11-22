#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#define RGB_TO_16BIT(r, g, b) (((r & 0x1f) << 11) + ((g & 0x3f) << 5) + (b & 0x1f))

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

	int i;
	for (i = 0; i < 360*240; i++)
	{
		//screen[i] = 'Z' + ('Z' << 8);
		screen[i] = RGB_TO_16BIT(i%32,2*(i%32),i%32);
	}

	ioctl(fbfd, 0x4680, &rect);

	close(fbfd);

	exit(EXIT_SUCCESS);
}
