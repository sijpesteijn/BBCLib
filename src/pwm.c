/*
 * pwm.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "pwm.h"

int pwm_open(pwm_properties *pwm) {
	char filename[60] = PWM_PATH;
	strcat(filename, pwm->name);

	if (access(filename, F_OK) == -1) {
		perror("Error");
		return -1;
	}
	return 0;
}

int pwm_set_run(pwm_properties *pwm, int run) {
	char filename[60] = PWM_PATH;
	strcat(filename, pwm->name);
	strcat(filename, "/");

	strcat(filename, "run");

	int file = open(filename, O_RDWR);
	char buf[2];
	sprintf(buf, "%d", run);
	if (write(file, buf, 2) < 0) {
	    perror("Could not execute run.");
		return -1;
	}
	close(file);
	return 0;
}

int pwm_set_period(pwm_properties *pwm, int period) {
	char filename[60] = PWM_PATH;
	strcat(filename, pwm->name);
	strcat(filename, "/");

	strcat(filename, "period");

	int file = open(filename, O_RDWR);
	char buf[4];
	sprintf(buf, "%d", period);
	if (write(file, buf, 4) < 0) {
		perror("Could not set period.");
		return -1;
	}
	close(file);
	return 0;
}
int pwm_set_duty(pwm_properties *pwm, int duty) {
	char filename[60] = PWM_PATH;
	strcat(filename, pwm->name);
	strcat(filename, "/");

	strcat(filename, "duty");

	int file = open(filename, O_RDWR);
	char buf[4];
	sprintf(buf, "%d", duty);
	if (write(file, buf, 4) < 0) {
		perror("Could not set duty.");
		return -1;
	}
	close(file);
	return 0;
}
