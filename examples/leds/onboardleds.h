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

typedef struct {
	char* org_trigger;
	int org_brightness;
	int lednr;
} led_info;

led_info *led0, *led1, *led2, *led3;
int DELAY = 300000;

void getLedTrigger(led_info *led) {
	FILE *fd;
	int i, j = 0, start, end;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

	fd = fopen(buf, "r");
	if (fd < 0) {
		perror("led/trigger");
	}

	char str[92];
	fgets(str, 92, fd);
	for(i = 0; i < 92; i++) {
		if (str[i] == '[')
			start = i + 1;
		if (str[i] == ']')
			end = i;
	}
	led->org_trigger = malloc(sizeof(char)*((end - start) + 1));

	for( i = start; i < end; i++) {
		led->org_trigger[j++] = str[i];
	}
	led->org_trigger[end] = '\0';
	fclose(fd);
}

void setLedTrigger(led_info *led, char *value) {
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

	fd = fopen(buf, "w");
	fputs(value, fd);
	fclose(fd);
}

void getLedBrightness(led_info *led) {
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

	fd = fopen(buf, "r");
	char str[2];
	fgets(str, 2, fd);
	led->org_brightness = atoi(str);
	fclose(fd);
}

void setLedBrightness(led_info *led, int value) {
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

	fd = fopen(buf, "w");
	char str[2];
	sprintf(str, "%d", value);
	fputs(str, fd);
	fclose(fd);
}

void saveState() {
	getLedTrigger(led0);
	getLedBrightness(led0);
	getLedTrigger(led1);
	getLedBrightness(led1);
	getLedTrigger(led2);
	getLedBrightness(led2);
	getLedTrigger(led3);
	getLedBrightness(led3);
}

void resetState() {
	setLedTrigger(led0, led0->org_trigger);
	setLedBrightness(led0, led0->org_brightness);
	setLedTrigger(led1, led1->org_trigger);
	setLedBrightness(led1, led1->org_brightness);
	setLedTrigger(led2, led2->org_trigger);
	setLedBrightness(led2, led2->org_brightness);
	setLedTrigger(led3, led3->org_trigger);
	setLedBrightness(led3, led3->org_brightness);
}

void clearLeds() {
	setLedTrigger(led0, "none");
	setLedBrightness(led0, 0);
	setLedTrigger(led1, "none");
	setLedBrightness(led1, 0);
	setLedTrigger(led2, "none");
	setLedBrightness(led2, 0);
	setLedTrigger(led3, "none");
	setLedBrightness(led3, 0);
}

void walker() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		usleep(DELAY/3);
		setLedBrightness(led0, 0);
		usleep(DELAY/3);
		setLedBrightness(led1, 1);
		usleep(DELAY/3);
		setLedBrightness(led1, 0);
		usleep(DELAY/3);
		setLedBrightness(led2, 1);
		usleep(DELAY/3);
		setLedBrightness(led2, 0);
		usleep(DELAY/3);
		setLedBrightness(led3, 1);
		usleep(DELAY/3);
		setLedBrightness(led3, 0);
		usleep(DELAY/3);
	}
}

void filler() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		usleep(DELAY);
		setLedBrightness(led1, 1);
		usleep(DELAY);
		setLedBrightness(led2, 1);
		usleep(DELAY);
		setLedBrightness(led3, 1);
		usleep(DELAY);
		clearLeds();
		usleep(DELAY);
	}
}

void shader() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 1);
		usleep(DELAY);

		setLedBrightness(led0, 0);
		usleep(DELAY);
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 0);
		usleep(DELAY);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 0);
		usleep(DELAY);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 0);
		usleep(DELAY);
	}
}

void flipflop() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 0);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 0);
		usleep(DELAY);
		setLedBrightness(led0, 0);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 0);
		setLedBrightness(led3, 1);
		usleep(DELAY);


	}
}

int onBoardLedsExample() {
	led0 = malloc(sizeof(led_info));
	led1 = malloc(sizeof(led_info));
	led2 = malloc(sizeof(led_info));
	led3 = malloc(sizeof(led_info));
	led0->lednr = 0;
	led1->lednr = 1;
	led2->lednr = 2;
	led3->lednr = 3;

	saveState();
	clearLeds();
	walker();
	filler();
	shader();
	flipflop();
	resetState();

	free(led0);
	free(led1);
	free(led2);
	free(led3);
	return 0;
}

#endif /* EXAMPLES_LEDS_ONBOARDLEDS_H_ */
