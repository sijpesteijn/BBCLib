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

/*
 * This example will open the gpio 60 port and flash the led 20 times.
 */
int gpioExample() {
	init_bbc_lib();
	export_gpio(LEDGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);
	int i=0;
	for(i=0;i<20;i++) {
		gpio_set_value(LEDGPIO, HIGH);
		usleep(100000);
		gpio_set_value(LEDGPIO, LOW);
		usleep(100000);
	}
	unexport_gpio(LEDGPIO);
	syslog(LOG_INFO, "%s", "Finished gpio example.");
	return 0;
}
