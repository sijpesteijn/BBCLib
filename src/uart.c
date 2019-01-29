/*
 * uart.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include "uart.h"
#include <string.h>
#include <fcntl.h>

int uart_open(uart_properties *uart) {
	FILE *slots;
	char buf[30] = "/dev/ttyO";
	char port_nr[2];
	sprintf(port_nr, "%d", uart->uart_id);
	strcat(buf,port_nr);
	struct termios uart_port;

//	slots = fopen(SLOTS, "w");
//	if(slots == NULL) printf("slots didn't open\n");
//	fseek(slots,0,SEEK_SET);
//
//	fprintf(slots, "BB-UART%i", uart->uart_id+1);
//	fflush(slots);
//	fclose(slots);

	uart->fd = open(buf, O_RDWR | O_NOCTTY);
	if(uart->fd < 0) printf("port failed to open\n");

	bzero(&uart_port,sizeof(uart_port));

	uart_port.c_cflag = uart->baudrate | CS8 | CLOCAL | CREAD;
	uart_port.c_iflag = IGNPAR | ICRNL;
	uart_port.c_oflag = 0;
	uart_port.c_lflag = 0;

	uart_port.c_cc[VTIME] = 0;
	uart_port.c_cc[VMIN]  = 1;

	//clean the line and set the attributes
	tcflush(uart->fd,TCIFLUSH);
	tcsetattr(uart->fd,TCSANOW,&uart_port);
	return 0;
}

int uart_send(uart_properties *uart, char *tx, int length) {
	if (write(uart->fd, tx, length) == -1) {
		error("Could not write %s to uart %i", tx, uart->uart_id);
		return -1;
	}
	info("Wrote %s to uart %i", tx, uart->uart_id);
	return 0;
}

int uart_read(uart_properties *uart,unsigned char *rx, int length) {
	int count;
	if( (count = read(uart->fd,(void*)rx,length)) > 0) {
		error("Could not read from uart %i", uart->uart_id);
		return count;
	}
	info("Read %s from uart %i", rx, uart->uart_id);
	return 0;
}

int uart_close(uart_properties *uart) {
	close(uart->fd);
	return 0;
}
