/*
 * examples.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "./gpio/gpio_example.h"
#include "./i2c/i2c_example.h"
#include "./pwm/pwm_example.h"
#include "./spi/spi_example.h"
#include "./uart/uart_example.h"
#include "./leds/onboardleds.h"


int main(int argc, char **argv) {
	printf("Choose the example to run:\n");
	printf("1 - user leds\n");
	printf("2 - gpio led\n");
	printf("3 - gpio input\n");
	printf("4 - gpio input led\n");
	printf("5 - spi MCP23S08\n");
	printf("6 - spi ADXL345\n");
	printf("7 - spi MCP4902\n");
	printf("8 - i2c 8x8 Led Matrix\n");
	printf("9 - i2c ADXL345\n");
	printf("0 - all\n");

//	unsigned char k = getchar();
	int c = 11; // atoi(k);
	if (c == 1 || c == 0)
		onBoardLedsExample();
	if (c == 2 || c == 0)
		gpioLedExample();
	if (c == 3 || c == 0)
		gpioInputExample();
	if (c == 4 || c == 0)
		gpioInputLedExample();
	if (c == 5 || c == 0)
		spiMCP23S08Example();
	if (c == 6 || c == 0)
		spiADXL345Example();
	if (c == 7 || c == 0)
		spiMCP4902Example();
	if (c == 8 || c == 0)
		i2c8x8LedMatrix();
	if (c == 9 || c == 0)
		i2cADXL345();
	if (c == 10 || c == 0)
		uartExample();
	if (c == 11 || c == 0)
		pwmExample();
	return 0;
}
