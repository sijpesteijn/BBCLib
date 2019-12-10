/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "gpio_example.h"
#include <stdlib.h>
#include <unistd.h>

/*
 * This example will open the gpio 60 port and flash the led 5 times.
 */
int gpioLedExample() {
	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = LEDGPIO;
	gpio->direction = OUTPUT_PIN;

	int isOpen = gpio_open(gpio);

	if (isOpen == 0) {
		int i=0;
		for(i=0;i<5;i++) {
			gpio_set_value(gpio, 1);
			sleep(1);
			gpio_set_value(gpio, 0);
			sleep(1);
		}
        gpio_unexport(gpio->nr);
	}
	free(gpio);
#ifdef BBCLIB_DBG
    printf("Finished gpio led example.");
#endif
    return 0;
}

/*
 * This example will open the gpio 15 port and read the input value for 1 minute.
 */
int gpioInputExample() {
	int value, i;

	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = BUTTONGPIO;
	gpio->direction = INPUT_PIN;

	int isOpen = gpio_open(gpio);

	if (isOpen == 0) {
		for(i=0;i<60;i++) {
			value = gpio_get_value(gpio);
			printf("Value: %d\n", value);
			sleep(1);
		}
        gpio_unexport(gpio->nr);
	}
#ifdef BBCLIB_DBG
    printf("Finished gpio input example.");
#endif
	return 0;
}

/*
 * This example will open the gpio 15 for input and gpio 60 for output
 * A positive read will turn on the led, a negative will turn it off.
 * This example will run for 1 minute.
 */
int gpioInputLedExample() {
	int value, i;

	gpio_properties *led = malloc(sizeof(gpio_properties));
	led->nr = LEDGPIO;
	led->direction = OUTPUT_PIN;
	gpio_properties *button = malloc(sizeof(gpio_properties));
	button->nr = BUTTONGPIO;
	button->direction = INPUT_PIN;

	int isOpen = gpio_open(led);
	isOpen &= gpio_open(button);

	if (isOpen == 0) {
		for(i=0;i<2;i++) {
			value = gpio_get_value(button);
			gpio_set_value(led, value);
			sleep(1);
		}
        gpio_unexport(led->nr);
        gpio_unexport(button->nr);
	}
	free(led);
	free(button);
#ifdef BBCLIB_DBG
	printf("Finished gpio input led example.");
#endif
	return 0;
}
