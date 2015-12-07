/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "../../src/core.h"
#include "../../src/spi.h"
#include "../../src/gpio.h"
#include "../gpio/gpio_example.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int spiMCP23S08Example();
int spiADXL345Example();
int spiMCP4902Example();
int spiMCP4912Example();
int spiMCP4922Example();
