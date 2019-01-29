/*
 * examples.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include <string.h>
#include "./gpio/gpio_example.h"
#include "./i2c/i2c_example.h"
#include "./pwm/pwm_example.h"
#include "./spi/spi_example.h"
#include "./uart/uart_example.h"
#include "./leds/onboardleds.h"


int main(int argc, char **argv) {
	printf("Choose the example to run:\n");
	printf("1 - onboard leds\n");
	printf("2 - gpio led\n");
	printf("3 - gpio input\n");
	printf("4 - gpio input led\n");
	printf("5 - spi MCP23S08\n");
	printf("6 - spi ADXL345\n");
	printf("7 - spi MCP4902\n");
	printf("8 - i2c 8x8 Led Matrix\n");
	printf("9 - i2c ADXL345\n");

	char c[100];
	scanf("%s" , c) ;
	if (strcmp(c,"1") == 0) {
		onBoardLedsExample();
	} else if (strcmp(c,"2") == 0) {
		gpioLedExample();
	} else if (strcmp(c, "3") == 0) {
		gpioInputExample();
	}
//	if (c == 51 || c == 48)
//	if (c == 52 || c == 48)
//		gpioInputLedExample();
//	if (c == 53 || c == 48)
//		spiMCP23S08Example();
//	if (c == 54 || c == 48)
//		spiADXL345Example();
//	if (c == 55 || c == 48)
//		spiMCP4902Example();
//	if (c == 56 || c == 48)
//		spiMCP4912Example();
//	if (c == 57 || c == 48)
//		spiMCP4922Example();
//	if (c == 58 || c == 48)
//		i2c8x8LedMatrix();
//	if (c == 59 || c == 48)
//		i2cADXL345();
//	if (c == 60 || c == 48)
//		uartExample();
//	if (c == 61 || c == 48)
//		pwmExample();
//	if (c == 62 || c == 48)
//		spiMC23S08_MCP4902Example();

	return 0;
}
