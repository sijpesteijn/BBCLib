/*
 * onboardleds.h
 *
 *  Created on: Jul 28, 2015
 *      Author: gijs
 */

#ifndef EXAMPLES_LEDS_ONBOARDLEDS_H_
#define EXAMPLES_LEDS_ONBOARDLEDS_H_

#include <fcntl.h>
#define BUF_SIZE 64
#define leddir "/sys/class/leds/beaglebone:green:usr%d/"
#define DELAY 300000
#define BBCLIB_DBG

typedef struct {
	char* trigger;
	int brightness;
	int lednr;
} led_info;

led_info *org_led0, *org_led1, *org_led2, *org_led3;
led_info *led0, *led1, *led2, *led3;
int onBoardLedsExample();
#endif /* EXAMPLES_LEDS_ONBOARDLEDS_H_ */
