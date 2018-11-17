#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	int fbfd = open("/dev/fb0", O_RDWR);

	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 360;
	rect.height = 240;

	char* screen = mmap(NULL, 153592, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if (screen == -1)
	{
		printf("ERROR");
	}

	screen[0] = 'Z';
	screen[1] = 'Z';
	screen[2] = 'Z';


	ioctl(fbfd, 0x4680, &rect);
	
	

//	char* data = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
//	lseek(fbfd, 3000, SEEK_SET);
//	write(fbfd, data, 30);


	close(fbfd);

	exit(EXIT_SUCCESS);
}
