/* FPGA DotMatirx Test Application
File : fpga_test_dot.c
Auth : largest@huins.com */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FPGA_DEVICE "/dev/fpga_control"

int main(int argc, char **argv)
{
	int dev,i,j;
	unsigned char buffer;
	unsigned char score[4] = {0,};
	dev = open(FPGA_DEVICE, O_RDWR);
	if (dev<0) {
		printf("Device open error : %s\n",FPGA_DEVICE);
		exit(1);
	}
	
	for (i=0;i< 20;i++)
	{
		read(dev,&buffer,1);
		score[2] = i/10;
		score[3] = i%10;
		write(dev, &score, 4);
		printf("%d", buffer);
		
		printf("\n");
		sleep(1);
	}

	close(dev);
	
	return 0;
}
