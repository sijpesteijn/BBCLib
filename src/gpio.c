/*
 * gpio.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "gpio.h"

void gpio_export(int nr) {
    FILE *fd1 = fopen(SYSFS_GPIO_DIR "/export", "w");
    if (fd1 < 0) {
        perror("Error - Could not export gpio");
    }
    char str[15];
    sprintf(str, "%d", nr);
    fputs(str, fd1);
    fflush(fd1);
    fclose(fd1);
    sleep(1);
}

int gpio_open(gpio_properties *gpio) {
    char gpio_dir[MAX_BUF];
    snprintf(gpio_dir, sizeof(gpio_dir), SYSFS_GPIO_DIR "/gpio%d", gpio->nr);

    struct stat buffer;
    int exist = stat(gpio_dir, &buffer);
    if(exist != 0) {
        gpio_export(gpio->nr);
    }

    char gpio_direction[MAX_BUF];
    strcpy(gpio_direction, gpio_dir);
    strcat(gpio_direction, "/direction");
	FILE *fd2 = fopen(gpio_direction, "w");
	if (fd2 == NULL) {
		perror("Error - Could not set gpio direction");
		return -1;
	}
	if (gpio->direction == OUTPUT_PIN)
		fputs("out", fd2);
	else
		fputs("in", fd2);
	fclose(fd2);
#ifdef BBCLIB_DBG
    printf("Exporting gpio %d and direction %d", gpio->nr, gpio->direction);
#endif
	return 0;
}

int gpio_unexport(int nr) {
	FILE *fd;
	fd = fopen(SYSFS_GPIO_DIR "/unexport", "w");
	if (fd < 0) {
		perror("gpio/unexport");
		return 1;
	}
	char str[15];
	sprintf(str, "%d", nr);
	fputs(str, fd);
	fclose(fd);

#ifdef BBCLIB_DBG
    printf("Closing gpio - unexport gpio: %d", gpio->nr);
#endif
    return 0;
}

int gpio_set_value(gpio_properties *gpio, int value) {
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio->nr);

	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/set-value");
		return 1;
	}

	char str[2];
	sprintf(str, "%d", value);
	fputs(str, fd);

	fclose(fd);
#ifdef BBCLIB_DBG
    printf("Set gpio %d value: %d", gpio->nr, value);
#endif
	return 0;
}

int gpio_get_value(gpio_properties *gpio) {
 	int value;
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio->nr);
	fd = fopen(buf, "r");
	if (fd < 0) {
		perror("gpio/get-value");
		return 1;
	}

	char str[2];
	fgets(str, 2, fd);

	if (strcmp(str, "1") == 0) {
		value = 1;
	} else {
		value = 0;
	}

	fclose(fd);
#ifdef BBCLIB_DBG
    printf("Get gpio %d value: %d", gpio->nr, value);
#endif
	return value;
}

int gpio_set_edge(gpio_properties *gpio, char *edge) {
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio->nr);

	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/set-edge");
		return 1;
	}

	fputs(edge, fd);
	fclose(fd);
#ifdef BBCLIB_DBG
    printf("Set gpio %d edge: %s", gpio->nr, edge);
#endif
	return 0;
}
