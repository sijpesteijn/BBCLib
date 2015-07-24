/*
 * gpio.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include "core.h"
#include "gpio.h"

int export_gpio(unsigned int gpio) {
	syslog (LOG_INFO, "export gpio: %d", gpio);
	FILE *fd;
	fd = fopen(SYSFS_GPIO_DIR "/export", "w");
	if (fd < 0) {
		perror("gpio/export");
		return 1;
	}
	char str[15];
	sprintf(str, "%d", gpio);
	fputs(str, fd);
	fclose(fd);

	return 0;
}

int unexport_gpio(unsigned int gpio) {
	syslog (LOG_INFO, "unexport gpio: %d", gpio);
	FILE *fd;
	fd = fopen(SYSFS_GPIO_DIR "/unexport", "w");
	if (fd < 0) {
		perror("gpio/unexport");
		return 1;
	}
	char str[15];
	sprintf(str, "%d", gpio);
	fputs(str, fd);
	fclose(fd);

	return 0;
}

int gpio_set_dir(int gpio, PIN_DIRECTION out_flag) {
	syslog (LOG_INFO, "gpio set direction: %d, %d", gpio, out_flag);
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/direction");
		return 1;
	}
	if (out_flag == OUTPUT_PIN)
		fputs("out", fd);
	else
		fputs("in", fd);
	fclose(fd);
	return 0;
}

int gpio_set_value(unsigned int gpio, PIN_VALUE value) {
	syslog (LOG_INFO, "gpio set value: %d, %d", gpio, value);
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/set-value");
		return 1;
	}

	if (value==LOW)
		fputs("0", fd);
	else
		fputs("1", fd);

	fclose(fd);
	return 0;
}

int gpio_get_value(unsigned int gpio, unsigned int *value) {
	syslog (LOG_INFO, "gpio get value: %d", gpio);
	FILE *fd;
	char buf[MAX_BUF];
	char ch;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = fopen(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return 1;
	}

	ch = fgetc(fd);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	fclose(fd);
	return 0;
}

int gpio_set_edge(unsigned int gpio, char *edge) {
	syslog (LOG_INFO, "gpio set edge: %d, %s", gpio, edge);
	FILE *fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = fopen(buf, "w");
	if (fd < 0) {
		perror("gpio/set-edge");
		return 1;
	}

	fputs(edge, fd);
	fclose(fd);
	return 0;
}
