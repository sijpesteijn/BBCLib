/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include <termios.h>
#include "../../src/core.h"
#include "../../src/i2c.h"

/*
 * UART.
 * This example will send the text 'send: n' (where is an index) a 100 times to the /dev/ttyO1 uart port.
 * The received text is printed on the screen.
 */
int uartExample() {
	init_bbc_lib();

	printf("Make sure you have enabled UART1 (/dev/ttyO1) see the README.md how to do this.");

	FILE *uart;
	char buf[30] = "/dev/ttyO1";
	struct termios uart1;
	int fd, count, i = 0;

	uart = fopen(SLOTS, "w");
	if(uart == NULL) printf("slots didn't open\n");
	fseek(uart,0,SEEK_SET);

	fprintf(uart, "BB-UART2");
	fflush(uart);
	fclose(uart);

	fd = open(buf, O_RDWR | O_NOCTTY);
	if(fd < 0) printf("port failed to open\n");

	bzero(&uart1,sizeof(uart1));

	uart1.c_cflag = B4800 | CS8 | CLOCAL | CREAD;
	uart1.c_iflag = IGNPAR | ICRNL;
	uart1.c_oflag = 0;
	uart1.c_lflag = 0;

	uart1.c_cc[VTIME] = 0;
	uart1.c_cc[VMIN]  = 1;

	//clean the line and set the attributes
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&uart1);
	unsigned char receive[100];
	while(i++ < 100)
	{
		char buf[30];
		sprintf(buf, "send: %d", i);
		write(fd, buf, strlen(buf) + 1);
		syslog(LOG_INFO,"Wrote %s to /dev/ttyO1", buf);
		usleep(5000);
		if( (count = read(fd,(void*)receive,100)) > 0)
			printf("%s\n",receive);
		usleep(50000);
	}

	close(fd);

	syslog(LOG_INFO, "%s", "Finished pwm example.");
	return 0;
}
