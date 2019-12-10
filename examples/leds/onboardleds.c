/*
 * onboardleds.h
 *
 *  Created on: Jul 28, 2015
 *      Author: gijs
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "onboardleds.h"


void getLedTrigger(led_info *led) {
    int i, j = 0, start = -1, end = -1;
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

    char *buffer = NULL;
    size_t size = 1024;

    FILE *fp = fopen(buf, "r");
    if (fp < 0) {
        perror("On board leds: could not get led trigger");
        return;
    }

    buffer = malloc((size + 1) * sizeof(*buffer));
    fread(buffer, size, 1, fp);
    fclose(fp);
    for (i = 0; i < strlen(buffer); i++) {
        switch (buffer[i]) {
            case '[':
                start = i;
                break;
            case ']':
                end = i;
                break;
        }
    }
    if (start == -1 || end == -1) {
        perror("Could not find trigger in values.\n");
    }

    led->trigger = malloc(sizeof(char) * ((end - start) + 1));

    for (i = start + 1; i < end; i++) {
        led->trigger[j++] = buffer[i];
    }
    free(buffer);
    led->trigger[end - 1] = '\0';
#ifdef BBCLIB_DBG
    printf("Get Led %d trigger: %s.\n", led->lednr, led->trigger);
#endif
}

void getLedBrightness(led_info *led) {
    FILE *fd;
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

    fd = fopen(buf, "r");
    char str[2];
    fgets(str, 2, fd);
    led->brightness = atoi(str);
    fclose(fd);
#ifdef BBCLIB_DBG
    printf("Get led %d brightness: %d.\n", led->lednr, led->brightness);
#endif
}

void setLedTrigger(led_info *led) {
    FILE *fd;
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

    fd = fopen(buf, "w");
    fputs(led->trigger, fd);
    fclose(fd);
#ifdef BBCLIB_DBG
    printf("Set Led %d trigger to %s.\n", led->lednr, led->trigger);
#endif
}

void setLedBrightness(led_info *led) {
    FILE *fd;
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

    fd = fopen(buf, "w");
    char str[2];
    sprintf(str, "%d", led->brightness);
    fputs(str, fd);
    fclose(fd);
#ifdef BBCLIB_DBG
    printf("Set led %d brightness %d.\n", led->lednr, led->brightness);
#endif
}

void saveState() {
    org_led0 = malloc(sizeof(led_info));
    org_led1 = malloc(sizeof(led_info));
    org_led2 = malloc(sizeof(led_info));
    org_led3 = malloc(sizeof(led_info));
    org_led0->lednr = 0;
    org_led1->lednr = 1;
    org_led2->lednr = 2;
    org_led3->lednr = 3;

    getLedTrigger(org_led0);
    getLedBrightness(org_led0);
    getLedTrigger(org_led1);
    getLedBrightness(org_led1);
    getLedTrigger(org_led2);
    getLedBrightness(org_led2);
    getLedTrigger(org_led3);
    getLedBrightness(org_led3);
}

void resetState() {
    char *none = "none";
    setLedTrigger(org_led0);
    if (strcmp(org_led0->trigger, none) == 0) {
        setLedBrightness(org_led0);
    }
    setLedTrigger(org_led1);
    if (strcmp(org_led1->trigger, none) == 0) {
        setLedBrightness(org_led1);
    }
    setLedTrigger(org_led2);
    if (strcmp(org_led2->trigger, none) == 0) {
        setLedBrightness(org_led2);
    }
    setLedTrigger(org_led3);
    if (strcmp(org_led3->trigger, none) == 0) {
        setLedBrightness(org_led3);
    }
}

void clearLeds() {
    led0->trigger = "none";
    led0->brightness = 0;
    led1->trigger = "none";
    led1->brightness = 0;
    led2->trigger = "none";
    led2->brightness = 0;
    led3->trigger = "none";
    led3->brightness = 0;
    setLedTrigger(led0);
    setLedBrightness(led0);
    setLedTrigger(led1);
    setLedBrightness(led1);
    setLedTrigger(led2);
    setLedBrightness(led2);
    setLedTrigger(led3);
    setLedBrightness(led3);
}

void walker() {
    int i;
    for (i = 0; i < 5; i++) {
        led0->brightness = 1;
        setLedBrightness(led0);
        usleep(DELAY / 3);
        led0->brightness = 0;
        setLedBrightness(led0);
        usleep(DELAY / 3);
        led1->brightness = 1;
        setLedBrightness(led1);
        usleep(DELAY / 3);
        led1->brightness = 0;
        setLedBrightness(led1);
        usleep(DELAY / 3);
        led2->brightness = 1;
        setLedBrightness(led2);
        usleep(DELAY / 3);
        led2->brightness = 0;
        setLedBrightness(led2);
        usleep(DELAY / 3);
        led3->brightness = 1;
        setLedBrightness(led3);
        usleep(DELAY / 3);
        led3->brightness = 0;
        setLedBrightness(led3);
        usleep(DELAY / 3);
    }
}

void filler() {
    int i;
    for (i = 0; i < 5; i++) {
        led0->brightness = 1;
        setLedBrightness(led0);
        usleep(DELAY);
        led1->brightness = 1;
        setLedBrightness(led1);
        usleep(DELAY);
        led2->brightness = 1;
        setLedBrightness(led2);
        usleep(DELAY);
        led3->brightness = 1;
        setLedBrightness(led3);
        usleep(DELAY);
        clearLeds();
        usleep(DELAY);
    }
}

void shader() {
    int i;
    for (i = 0; i < 5; i++) {
        led0->brightness = 1;
        setLedBrightness(led0);
        led1->brightness = 1;
        setLedBrightness(led1);
        led2->brightness = 1;
        setLedBrightness(led2);
        led3->brightness = 1;
        setLedBrightness(led3);
        usleep(DELAY);

        led0->brightness = 0;
        setLedBrightness(led0);
        usleep(DELAY);
        led0->brightness = 1;
        led1->brightness = 0;
        setLedBrightness(led0);
        setLedBrightness(led1);
        usleep(DELAY);
        led1->brightness = 1;
        led2->brightness = 0;
        setLedBrightness(led1);
        setLedBrightness(led2);
        usleep(DELAY);
        led2->brightness = 1;
        led3->brightness = 0;
        setLedBrightness(led2);
        setLedBrightness(led3);
        usleep(DELAY);
    }
}

void flipflop() {
    int i;
    for (i = 0; i < 5; i++) {
        led0->brightness = 1;
        led1->brightness = 0;
        led2->brightness = 1;
        led3->brightness = 0;
        setLedBrightness(led0);
        setLedBrightness(led1);
        setLedBrightness(led2);
        setLedBrightness(led3);
        usleep(DELAY);
        led0->brightness = 0;
        led1->brightness = 1;
        led2->brightness = 0;
        led3->brightness = 1;
        setLedBrightness(led0);
        setLedBrightness(led1);
        setLedBrightness(led2);
        setLedBrightness(led3);
        usleep(DELAY);
    }
}

int onBoardLedsExample() {
    printf("Running onboard leds example.\n");
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
    free(org_led0);
    free(org_led1);
    free(org_led2);
    free(org_led3);
    printf("Finished on board leds example");
    return 0;
}
