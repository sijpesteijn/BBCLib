/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "../../src/core.h"
#include "../../src/gpio.h"

unsigned int LEDGPIO = 60;
unsigned int BUTTONGPIO = 15;

/*
 * This example will open the gpio 60 port and flash the led 20 times.
 */
int gpioLedExample() {
	init_bbc_lib();
	export_gpio(LEDGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);
	int i=0;
	for(i=0;i<20;i++) {
		gpio_set_value(LEDGPIO, 1);
		usleep(100000);
		gpio_set_value(LEDGPIO, 0);
		usleep(100000);
	}
	unexport_gpio(LEDGPIO);
	syslog(LOG_INFO, "%s", "Finished gpio led example.");
	return 0;
}

/*
 * This example will open the gpio 15 port and read the input value for 1 minute.
 */
int gpioInputExample() {
	int value, i;
	init_bbc_lib();
	export_gpio(BUTTONGPIO);
	gpio_set_dir(BUTTONGPIO, INPUT_PIN);
	for(i=0;i<60;i++) {
		value = gpio_get_value(BUTTONGPIO);
		printf("Value: %d\n", value);
		usleep(1000000);
	}
	unexport_gpio(BUTTONGPIO);
	syslog(LOG_INFO, "%s", "Finished gpio input example.");
	return 0;
}

/*
 * This example will open the gpio 15 for input and gpio 60 for output
 * A positive read will turn on the led, a negative will turn it off.
 * This example will run for 1 minute.
 */
int gpioInputLedExample() {
	int value, i;
	init_bbc_lib();
	export_gpio(LEDGPIO);
	export_gpio(BUTTONGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);
	gpio_set_dir(BUTTONGPIO, INPUT_PIN);
	for(i=0;i<60;i++) {
		value = gpio_get_value(LEDGPIO);
		gpio_set_value(LEDGPIO, value);
		usleep(1000000);
	}
	unexport_gpio(LEDGPIO);
	unexport_gpio(BUTTONGPIO);
	syslog(LOG_INFO, "%s", "Finished gpio input led example.");
	return 0;
}
