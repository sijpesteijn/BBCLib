/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "pwm_example.h"
#include "../../src/log.h"

int pwmExample() {

	pwm_properties *pwm = malloc(sizeof(pwm_properties));
	pwm->name = "pwm_test_P9_22.12";

	int isOpen = pwm_open(pwm);

	if (isOpen == 1) {
		if (pwm_set_run(pwm, 0) < 0) {
			printf("Could not stop pwm port.\n");
			return -1;
		}
		if (pwm_set_period(pwm, 10000) < 0) {
			printf("Could not set period.\n");
			return -1;
		}
		if (pwm_set_duty(pwm, 1000) < 0) {
			printf("Could not set duty.\n");
			return -1;
		}
		if (pwm_set_run(pwm, 1) < 0) {
			printf("Could not start pwm port.\n");
			return -1;
		}

		int i=0,j=0;
		while(j++ < 10) {
			while(i-- > 0) {
				if (pwm_set_duty(pwm, i*100) < 0) {
					printf("Could not set duty.\n");
					return -1;
				}
				debug("Wrote %d to pwm (%s)", i, pwm->name);
				usleep(100000);
			}
			i = 10;
		}

		pwm_set_run(pwm, 0);
	}
	debug("Finished pwm example.");
	return 0;
}
