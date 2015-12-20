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

#define MCP4902_1_CS 			= 0x01
#define MCP4902_2_CS 			= 0x02
#define MCP4922_CS 				= 0x04
#define MCP23S08_LDAC			= 0x06
#define MCP23S08_SCK			= 0x08
#define MCP23S08_SDI			= 0x10
#define MCP23S08_SHUTTER		= 0x20
#define MCP23S08_SHUTTER_RETURN	= 0x40

int spiMCP23S08Example();
int spiADXL345Example();
int spiMCP4902Example();
int spiMCP4912Example();
int spiMCP4922Example();
int spiMC23S08_MCP4902Example();
