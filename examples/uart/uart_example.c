/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "uart_example.h"
#include "../../src/log.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

/*
 * UART.
 * This example will send the text 'foo n' (where is an index) a 10 times to the /dev/ttyO1 uart port.
 * The received text is printed on the screen.
 */
int uartExample() {

	printf("!!! Make sure you have enabled UART1 (/dev/ttyO1) see the README.md how to do this. !!!\n");

	uart_properties *uart = malloc(sizeof(uart_properties));
	uart->uart_id = uart1;
	uart->baudrate = B4800;

	uint8_t isOpen = uart_open(uart);
	int i = 0;

	if (isOpen == 0) {
		unsigned char receive[100];
		while(i++ < 10)
		{
			char buf[30];
			sprintf(buf, "foo %d", i);

			// Send data to uart1
			if (uart_send(uart, buf, strlen(buf) + 1) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}

			usleep(100000);

			// Read data from uart1
			if (uart_read(uart, receive, 100) < 0) {
				printf("Could not read data from uart port");
				return -1;
			}

			printf("Read: %s\n",receive);
			usleep(50000);
		}
		uart_close(uart);
	}

	debug("Finished pwm example.");
	return 0;
}
