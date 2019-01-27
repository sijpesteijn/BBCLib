/*
 * gpio.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include <syslog.h>
#include "core.h"
#include "gpio.h"

int gpio_open(gpio_properties *gpio) {
	syslog (LOG_INFO, "gpio_open: export gpio: %d", gpio->nr);
	FILE *export;
	export = fopen(SYSFS_GPIO_DIR "/export", "w");
	if (export < 0) {
		perror("gpio/export");
		return 1;
	}
	char str[15];
	sprintf(str, "%d", gpio->nr);
	fputs(str, export);
	fclose(export);
	syslog (LOG_INFO, "gpio_open: set direction: %d, %d", gpio->nr, gpio->direction);
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio->nr);
	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/direction");
		return 1;
	}
	if (gpio->direction == OUTPUT_PIN)
		fputs("out", fd);
	else
		fputs("in", fd);
	fclose(fd);
	return 0;
}

int gpio_close(gpio_properties *gpio) {
	syslog (LOG_INFO, "gpio_close: unexport gpio: %d", gpio->nr);
	FILE *fd;
	fd = fopen(SYSFS_GPIO_DIR "/unexport", "w");
	if (fd < 0) {
		perror("gpio/unexport");
		return 1;
	}
	char str[15];
	sprintf(str, "%d", gpio->nr);
	fputs(str, fd);
	fclose(fd);

	return 0;
}

int gpio_set_value(gpio_properties *gpio, int value) {
    log::debug(string("gpio set value: %d, %d", gpio->nr, value));
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
	return 0;
}

int gpio_get_value(gpio_properties *gpio) {
    log::debug(string("gpio get value: %d", gpio->nr));
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
	return value;
}

int gpio_set_edge(gpio_properties *gpio, char *edge) {
    log::debug(string("gpio set edge: %d, %s", gpio->nr, edge));
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
	return 0;
}
