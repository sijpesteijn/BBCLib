/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "gpio_example.h"
#include "../../src/log.h"

/*
 * This example will open the gpio 60 port and flash the led 20 times.
 */
int gpioLedExample() {
	init_bbc_lib();
	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = LEDGPIO;
	gpio->direction = OUTPUT_PIN;

	int isOpen = gpio_open(gpio);

	if (isOpen == 0) {
		int i=0;
		for(i=0;i<20;i++) {
			gpio_set_value(gpio, 1);
			usleep(100000);
			gpio_set_value(gpio, 0);
			usleep(100000);
		}
		gpio_close(gpio);
	}
	free(gpio);
    debug("Finished gpio led example.");
	return 0;
}

/*
 * This example will open the gpio 15 port and read the input value for 1 minute.
 */
int gpioInputExample() {
	int value, i;
	init_bbc_lib();

	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = BUTTONGPIO;
	gpio->direction = INPUT_PIN;

	int isOpen = gpio_open(gpio);

	if (isOpen == 0) {
		for(i=0;i<600;i++) {
			value = gpio_get_value(gpio);
			printf("Value: %d\n", value);
			usleep(100000);
		}
		gpio_close(gpio);
	}
    debug("Finished gpio input example.");
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

	gpio_properties *led = malloc(sizeof(gpio_properties));
	led->nr = LEDGPIO;
	led->direction = OUTPUT_PIN;
	gpio_properties *button = malloc(sizeof(gpio_properties));
	button->nr = BUTTONGPIO;
	button->direction = INPUT_PIN;

	int isOpen = gpio_open(led) && gpio_open(button);

	if (isOpen == 0) {
		for(i=0;i<600;i++) {
			value = gpio_get_value(button);
			gpio_set_value(led, value);
			usleep(100000);
		}
		gpio_close(led);
		gpio_close(button);
	}
	free(led);
	free(button);
    debug("Finished gpio input led example.");
	return 0;
}
