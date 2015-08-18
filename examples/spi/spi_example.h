/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "../../src/core.h"
#include "../../src/spi.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int setup() {
	init_bbc_lib();
	overlay *ol = malloc(sizeof(overlay));
	ol->file_name = "/lib/firmware/BBCLIB-SPI0-00A0.dtbo";
	ol->board_name = "BBCLib";
	ol->manufacturer = "BBCLIB";
	ol->part_number = "BBCLIB-SPI0";
	ol->version = "00A0";
	load_device_tree_overlay(ol);
	return 1;
}

/*
 * MCP23S08 is a 8-Bit I/O Expander with Serial Interface.
 * This example lights the 8 leds in sequence for 10 times.
 */
int spiMCP23S08Example() {
	unsigned char mcp23s08_setup[] = { 0x40, 0x00, 0x00, };
	unsigned char mcp23s08_gpios[] = { 0x40, 0x09, 0x01, };

	setup();

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2400000;
	spi->flags = O_RDWR;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		spi_send(spi, mcp23s08_setup, sizeof(mcp23s08_setup));
		// Loop from 0 to 255 and light the LED bar as a binary counter
		int i = 0, j = 0;
		while(j++ < 10) {
			for (i = 0; i < 8; i++) {
				// This function can send and receive data, just sending here
				if (spi_send(spi, mcp23s08_gpios, sizeof(mcp23s08_gpios)) == -1) {
					perror("Failed to update the display");
					return -1;
				}
				mcp23s08_gpios[2] = mcp23s08_gpios[2] << 1;
				printf("%4d\r", i);   //print the number on the same line
				fflush(stdout);       //need to flush the output, no \n
				usleep(100000);       //sleep for 100ms each loop
			}
			mcp23s08_gpios[2] = 0x01;
		}
		spi_close(spi);
	}
	syslog(LOG_INFO, "%s", "Finished spi example.");
	return 0;
}


/*
 * ADXL345 is a Digital Accelerometer. It can measure movement on 3 axes.
 * This example will probe the axis 20 times every second.
 */
int spiADXL345Example() {
	setup();

	printf("WORK IN PROGRESS\n");
	unsigned char adxl345_setup[] = {};
	unsigned char receive[6];
	signed short data[3];
	float x, y, z;

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2000000;
	spi->flags = O_RDWR;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		adxl345_setup[0] = 0x31;
		adxl345_setup[1] = 0x08;
		spi_send(spi, adxl345_setup, 2);
		adxl345_setup[0] = 0x2D;
		adxl345_setup[1] = 0x08;
		spi_send(spi, adxl345_setup, 2);
		while(1) {
			adxl345_setup[0] = 0x80|0x40|0x32;
			adxl345_setup[1] = 0x00;
			spi_transfer(spi, adxl345_setup, receive, 6);
			data[0] = receive[1]<<8 | receive[0]; // combine MSB and LSB
			data[1] = receive[3]<<8 | receive[2];
			data[2] = receive[5]<<8 | receive[4];
			printf("x = %i \ty = %i\t z = %i\n", data[0], data[1], data[2]);
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
			usleep(1000000);
		}
	}
	return 0;
}

void setOutput(spi_properties *spi, unsigned char reg, unsigned char value) {
	unsigned char data[2] = {};
	data[0] = reg | ((value & 0xf0) >> 4);
	data[1] = (value & 0x0f) << 4;
//	syslog(LOG_INFO, "Output: 0x%02x 0x%02x\n", data[0], data[1]);
	if (spi_send(spi, data, sizeof(data)) == -1) {
		perror("Failed to update output.");
	}
}

/*
 * MCP4902 is a 8-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 */
int spiMCP4902Example() {
	setup();

	export_gpio(LEDGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2000000;
	spi->flags = O_RDWR;

	int delay = 1;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		gpio_set_value(LEDGPIO, 0);

		int i = 0;
		while(i++ < 1000) {
			unsigned char value = 0x00;
			while(value < 0xff) {
//				gpio_set_value(LEDGPIO, 1);
				setOutput(spi, 0x70, value);
				setOutput(spi, 0xf0, ~value);
//				usleep(delay);
//				gpio_set_value(LEDGPIO, 0);
				value = value + 1;
			}
			while(value > 0x00) {
				value = value - 1;
//				gpio_set_value(LEDGPIO, 1);
				setOutput(spi, 0x70, value);
				setOutput(spi, 0xf0, ~value);
//				usleep(delay);
//				gpio_set_value(LEDGPIO, 0);
			}
		}
	}
	return 1;
}

/*
 * MCP4912 is a 10-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 */
int spiMCP4912Example() {
	setup();
	export_gpio(LEDGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);

	unsigned char regA = 0x70;
	unsigned char regB = 0xB0;
	unsigned char dataA[2] = {};
	unsigned char dataB[2] = {};
	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 500000;
	spi->flags = O_RDWR;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		int i = 0;
		while(i++ < 10) {
			unsigned char lvalue = 0x00;
			unsigned char hvalue = 0x00;
			int j = 0;
			while(j++ < 1024) {
				unsigned char LSB = (((lvalue & 0xf0) >> 2) & 0xf0) >> 4;
				dataA[0] = LSB | (hvalue << 2) | regA;
				dataA[1] = (lvalue << 2) & 0xff;
				dataB[0] = LSB | (~hvalue << 2) | regB;
				dataB[1] = (~lvalue << 2) & 0xff;
				if (lvalue == 0xff) {
					hvalue = hvalue + 1;
				}
				gpio_set_value(LEDGPIO, 1);
				if (spi_send(spi, dataA, sizeof(dataA)) == -1) {
					perror("Failed to update output A");
					return -1;
				}
				if (spi_send(spi, dataB, sizeof(dataB)) == -1) {
					perror("Failed to update output B");
					return -1;
				}
				gpio_set_value(LEDGPIO, 0);
				usleep(100000);
				lvalue = lvalue + 1;
			}
		}
	}
	return 1;
}

/*
 * MCP4912 is a 12-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 */
int spiMCP4922Example() {
	setup();
	export_gpio(LEDGPIO);
	gpio_set_dir(LEDGPIO, OUTPUT_PIN);

	unsigned char regA = 0x70;
	unsigned char regB = 0xB0;
	unsigned char dataA[2] = {};
	unsigned char dataB[2] = {};
	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 500000;
	spi->flags = O_RDWR;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		int i = 0;
		while(i++ < 10) {
			unsigned char value = 0x00;
			int j = 0;
			while(j++ < 4096) {
				dataA[0] = regA | ((value & 0xf0) >> 4);
				dataA[1] = value;
				dataB[0] = regB | ((~value & 0xf0) >> 4);
				dataB[1] = ~value;
				gpio_set_value(LEDGPIO, 1);
				if (spi_send(spi, dataA, sizeof(dataA)) == -1) {
					perror("Failed to update output A");
					return -1;
				}
				if (spi_send(spi, dataB, sizeof(dataB)) == -1) {
					perror("Failed to update output B");
					return -1;
				}
				gpio_set_value(LEDGPIO, 0);
				usleep(10000);
				value = value + 1;
			}
		}
	}
	return 1;
}
