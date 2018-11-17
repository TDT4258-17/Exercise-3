#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 360;
	rect.height = 240;
	
	int fbfd = open("/dev/fb0", O_RDWR);
	lseek(fbfd, 3000, SEEK_SET);

	char* data = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";

	write(fbfd, data, 30);

//	ioctl(fbfd, 0x4680, &rect);


	close(fbfd);

	exit(EXIT_SUCCESS);
}
