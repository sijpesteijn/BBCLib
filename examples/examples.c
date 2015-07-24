/*
 * examples.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "./gpio/gpio_example.h"
#include "./i2c/i2c_example.h"
#include "pwm_example.h"
#include "./spi/spi_example.h"
#include "uart_example.h"


int main(int argc, char **argv) {
	printf("Choose the example to run:\n");
	printf("1 - gpio\n");
	printf("2 - i2c\n");
	printf("3 - pwm\n");
	printf("4 - spi mcp4902\n");
	printf("5 - spi \n");
	printf("6 - uart\n");
	printf("0 - all\n");

	int c = '6'; // getchar( );
	if (c == '1' || c == '0')
		gpioExample();
	if (c == '2' || c == '0')
		i2c8x8LedMatrix();
	if (c == '3' || c == '0')
		i2cADXL345();
	if (c == '4' || c == '0')
		spiMCP23S08Example();
	if (c == '5' || c == '0')
		spiADXL345Example();
	if (c == '6' || c == '0')
		spiMCP4902Example();
//	if (c == '6' || c=='0')
//		spiTestExample(0,NULL);
	return 0;
}
