/*
 * gpio.h
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#ifndef GPIO_H_
#define GPIO_H_

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

typedef enum {
	INPUT_PIN=0,
	OUTPUT_PIN=1
} PIN_DIRECTION;

int export_gpio(unsigned int gpio_nr);
int unexport_gpio(unsigned int gpio_nr);
int gpio_set_dir(int gpio_nr, PIN_DIRECTION direction);
int gpio_set_value(unsigned int gpio, int value);
int gpio_get_value(unsigned int gpio);
int gpio_set_edge(unsigned int gpio, char *edge);
#endif /* GPIO_H_ */
