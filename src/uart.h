/*
 * uart.h
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <termios.h>
#include "core.h"

typedef enum {
	uart0 = 0, uart1 = 1, uart2 = 2, uart3 = 3, uart4 = 4, uart5 = 5
} uart;


typedef struct {
	int fd;
	uart uart_id;
	int baudrate;
} uart_properties;

int uart_open(uart_properties *uart);
int uart_send(uart_properties *uart, char *tx, int length);
int uart_read(uart_properties *uart,unsigned char *rx, int length);
int uart_close(uart_properties *uart);
#endif /* UART_H_ */
