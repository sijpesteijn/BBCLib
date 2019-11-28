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
	printf("10 - i2c ADXL345\n");
	printf("11 - i2c ADXL345\n");
	printf("12 - i2c ADXL345\n");
	printf("13 - i2c ADXL345\n");
	printf("14 - i2c ADXL345\n");

    char c;

    printf("Enter character: ");
    c = getchar();

    printf("Character entered: ");
    putchar(c);

//	scanf("%s" , c);
    printf("aaa%s\n", c);
	if (strcmp(c,"1") == 0) {
		onBoardLedsExample();
	} else if (strcmp(c,"2") == 0) {
		gpioLedExample();
	} else if (strcmp(c, "3") == 0) {
		gpioInputExample();
	} else if (strcmp(c, "4") == 0) {
		gpioInputLedExample();
	} else if (strcmp(c, "5") == 0) {
		spiMCP23S08Example();
	} else if (strcmp(c, "6") == 0) {
		spiADXL345Example();
	} else if (strcmp(c, "7") == 0) {
		spiMCP4902Example();
	} else if (strcmp(c, "8") == 0) {
		spiMCP4912Example();
	} else if (strcmp(c, "9") == 0) {
		spiMCP4922Example();
	} else if (strcmp(c, "10") == 0) {
		i2c8x8LedMatrix();
	} else if (strcmp(c, "11") == 0) {
		i2cADXL345();
	} else if (strcmp(c, "12") == 0) {
		uartExample();
	} else if (strcmp(c, "13") == 0) {
		pwmExample();
	} else if (strcmp(c, "14") == 0) {
		spiMC23S08_MCP4902Example();
	}

	return 0;
}
