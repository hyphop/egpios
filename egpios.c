
// ## hyphop ##

/* 

   about ./egpios

   watch gpios value changes via epool

   example:

   ./egpios 14 16 3 7

   to compile:

   arm-openwrt-linux-gcc -o egpios egpios.c

   notice:

   !!! quickly writed just for test and full of bugs )))) !!!

   based on examples from:

   https://github.com/contactless/wiegand-linux-sysfs/blob/master/wiegand_epoll.c
   https://gist.github.com/jadonk/2587524

*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>

#define MAX_BUF 64

int init_gpio(int gpio) {
	// export gpio to userspace
	FILE * tmpf = fopen("/sys/class/gpio/export", "w");
	char path[42];
	fprintf(tmpf, "%d\n", gpio);
	fclose(tmpf);

	// set output direction
	sprintf(path, "/sys/class/gpio/gpio%d/direction", gpio);
	tmpf = fopen(path, "w");
	fprintf(tmpf, "%s\n", "in");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/active_low", gpio);
	tmpf = fopen(path, "w");
	fprintf(tmpf, "%s\n", "1");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/edge", gpio);
	tmpf = fopen(path, "w");
	fprintf(tmpf, "%s\n", "both");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/value", gpio);
//	int fd = open(path, O_RDWR);// | O_NONBLOCK);
	int fd = open(path, O_RDWR | O_NONBLOCK );
//	int fd = open(path, 0);// | O_NONBLOCK);
	if (fd <= 0) {
		fprintf(stderr, "open of gpio %d returned %d: %s\n", gpio, fd, strerror(errno));
	}
	return fd;

}

int main(int argc, char** argv) {
	int n;
	int epfd;
	int fd_d0, fd_d1;
	int fd_d2, fd_d3;

	if (argc < 4) {
		fprintf(stderr, "USAGE: %s n0 n1 n2 n3 \n", argv[0]);
		return 2;
	}

	int gpio_d0 = atoi(argv[1]);
	int gpio_d1 = atoi(argv[2]);
	int gpio_d2 = atoi(argv[3]);
	int gpio_d3 = atoi(argv[4]);

	fprintf(stderr, "Using GPIO %d %d %d %d\n", gpio_d0, gpio_d1, gpio_d2, gpio_d3 );

	epfd = epoll_create(1);

	fd_d0 = init_gpio(gpio_d0);
	fd_d1 = init_gpio(gpio_d1);
	fd_d2 = init_gpio(gpio_d2);

	if ( gpio_d3 ) fd_d3 = init_gpio(gpio_d3);


	if( !(fd_d0 > 0) || !(fd_d1 > 0)) {
		fprintf(stderr, "error opening gpio sysfs entries\n");
		return 1;
	}



    struct epoll_event ev_d0, ev_d1, ev_d2, ev_d3;
    struct epoll_event events[10];
    ev_d0.events = EPOLLET;
    ev_d1.events = EPOLLET;
    ev_d2.events = EPOLLET;
    ev_d3.events = EPOLLET;

    ev_d0.data.fd = fd_d0;
    ev_d1.data.fd = fd_d1;
    ev_d2.data.fd = fd_d2;
    ev_d3.data.fd = fd_d3;

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d0, &ev_d0);

    if (n != 0) {
	fprintf(stderr, "epoll_ctl returned %d %d: %s\n", 0, n, strerror(errno));
	return 1;
    }


    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d1, &ev_d1);

    if (n != 0) {
	fprintf(stderr, "epoll_ctl returned %d %d: %s\n", 1, n, strerror(errno));
	return 1;
    }

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d2, &ev_d2);

    if (n != 0) {
	fprintf(stderr, "epoll_ctl returned %d %d: %s\n", 2, n, strerror(errno));
	return 1;
    }

    if ( gpio_d3 ) {
    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d3, &ev_d3);

    if (n != 0) {
	fprintf(stderr, "epoll_ctl returned %d %d: %s\n", 3, n, strerror(errno));
	return 1;
    }

    }


	size_t i	;

//	unsigned int value = 0;
//	unsigned bit_counter = 0;

    char buf[16];

    while(1) {

		printf("wait\n");
		n = epoll_wait(epfd, events, 6, -1);

		printf("have %d events\n", n);

//		for ( i = 0;  i < n; ++i) {
//			value <<= 1;
//			if (events[i].data.fd == ev_d1.data.fd) {
//				value |= 0x01;
//			}
//
//			bit_counter += 1;
//		}

//		if (bit_counter && (n == 0)) {
//			printf("got value %d = 0x%x (%d bits)\n", value, value, bit_counter);
			i=read(fd_d0, buf, 1);
			lseek( fd_d0, 0,0);

			i=read(fd_d1, buf+1, 1);
			lseek( fd_d1, 0,0);

			i=read(fd_d2, buf+2, 1);
			lseek( fd_d2, 0,0);

			if ( gpio_d3 ) {
    			i=read(fd_d3, buf+3, 1);
			lseek( fd_d3, 0,0);
			}

			printf("%s\n", buf );

//			value = 0;
//			bit_counter = 0;
//		}
    }


  return(0);
}
