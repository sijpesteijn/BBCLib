/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "../../src/core.h"
#include "../../src/i2c.h"


int pwmExample() {
	init_bbc_lib();
	syslog(LOG_INFO, "%s", "Finished pwm example.");
	return 0;
}
