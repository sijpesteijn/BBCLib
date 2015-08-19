/*
 * pwm.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <unistd.h>
#include "./pwm.h"

int pwm_open(pwm_properties *pwm) {
	char filename[60] = PWM_PATH;
	strcat(filename, pwm->name);

	if (access(filename, F_OK) == -1) {
		syslog(LOG_ERR, "Could not open file: %s", filename);
		return 0;
	}
	return 1;
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
		syslog(LOG_ERR, "Could not execute run. %d", run);
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
		syslog(LOG_ERR, "Could not set period. %d", period);
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
		syslog(LOG_ERR, "Could not set duty. %d", duty);
		return -1;
	}
	close(file);
	return 0;
}

//FILE *duty,*period,*run;
//
//period = fopen("/sys/devices/ocp.3/pwm_test_P9_22.12/period", "w");
//if (period == -1) {
//	perror("Failed to open pwm period");
//	return -1;
//}
//fseek(period,0,SEEK_SET);
//fprintf(period,"%d",10000);
//fflush(period);
//
//duty = fopen("/sys/devices/ocp.3/pwm_test_P9_22.12/duty", "w");
//if (duty == -1) {
//	perror("Failed to open pwm duty");
//	return -1;
//}
//fseek(duty,0,SEEK_SET);
//fprintf(duty,"%d",0);
//fflush(duty);
//
//run = fopen("/sys/devices/ocp.3/pwm_test_P9_22.12/run", "w");
//if (run == -1) {
//	perror("Failed to open pwm run");
//	return -1;
//}
//fseek(run,0,SEEK_SET);
//fprintf(run,"%d",0);
//fflush(run);
//
//fseek(run,0,SEEK_SET);
//fprintf(run,"%d",1);
//fflush(run);
//int i = 0, j = 0;
//while(j++ < 10) {
//	while(i++ < 100) {
//		duty = fopen("/sys/devices/ocp.3/pwm_test_P9_22.12/duty", "w");
//		fseek(duty,0,SEEK_SET);
//		fprintf(duty,"%d",i*100);
//		fflush(duty);
//		syslog(LOG_INFO,"Wrote %d to pwm (P9_22)", i);
//		usleep(10000);
//	}
//	i = 0;
//}
//
//fclose(duty);
//fclose(period);
//fclose(run);

