#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>


int main () {

	fd_set rfds;
	struct timeval tv;
	int retval;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while(true) {
		retval = select(1, &rfds, NULL, NULL, &tv);

		if(retval == -1)
			perror("select()");
		else
			printf("This is a message!\n");
		
		tv.tv_sec = 1;
	}
	return 0;
}
