/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "spi_example.h"
#include "../../src/log.h"

int setup() {
	init_bbc_lib();
	overlay *ol = malloc(sizeof(overlay));
	ol->file_name = "BBCLIB-SPI0";
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
	spi->mode = 0;
	spi->speed = 10000000;
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
				usleep(100000);       //sleep for 100ms each loop
				mcp23s08_gpios[2] = mcp23s08_gpios[2] << 1;
			}
			mcp23s08_gpios[2] = 0x01;
		}
		mcp23s08_gpios[2] = 0x00;
		if (spi_send(spi, mcp23s08_gpios, sizeof(mcp23s08_gpios)) == -1) {
			perror("Failed to update the display");
			return -1;
		}
		spi_close(spi);
	}
	debug("Finished spi example.");
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

void write8Bits(spi_properties *spi, unsigned char reg, unsigned char value) {
	unsigned char data[2] = {};
	data[0] = reg | ((value & 0xf0) >> 4);
	data[1] = (value & 0x0f) << 4;
	if (spi_send(spi, data, sizeof(data)) == -1) {
		perror("Failed to update output.");
	}
}

/*
 * MCP4902 is a 8-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 * 255
 */
int spiMCP4902Example() {
	setup();

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2400000;
	spi->flags = O_RDWR;

	int delay = 1000;

	uint8_t isSpiOpen = spi_open(spi);
	if (isSpiOpen == 0) {

		int i = 0;
		unsigned char value = 0x00;
		while(i++ < 10) {
			while(value < 0xff) {
				write8Bits(spi, 0x70, value);
				write8Bits(spi, 0xf0, ~value);
				usleep(delay);
				value = value + 1;
			}
			while(value > 0x00) {
				value = value - 1;
				write8Bits(spi, 0x70, value);
				write8Bits(spi, 0xf0, ~value);
				usleep(delay);
			}
		}
		write8Bits(spi, 0x70, 0);
		write8Bits(spi, 0xf0, 0);
	}
	free(spi);
	debug("Finished spi example.");
	return 0;
}

void write10Bits(spi_properties *spi, unsigned char reg, unsigned short value) {
	unsigned char data[2] = {};
	data[0] = reg | ((value & 0xff00) >> 6);
	data[1] = (value & 0x00ff);
	if (spi_send(spi, data, sizeof(data)) == -1) {
		perror("Failed to update output.");
	}
}

/*
 * MCP4912 is a 10-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 * 1024
 */
int spiMCP4912Example() {
	setup();

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2400000;
	spi->flags = O_RDWR;

	int delay = 1000;

	uint8_t isSpiOpen = spi_open(spi);
	if (isSpiOpen == 0) {

		int i = 0;
		while(i++ < 10) {
			unsigned short value = 0x0000;
			while(value < 0x03ff) {
				write10Bits(spi, 0x70, value);
				write10Bits(spi, 0xf0, ~value);
				usleep(delay);
				value = value + 1;
			}
			while(value > 0x00) {
				value = value - 1;
				write10Bits(spi, 0x70, value);
				write10Bits(spi, 0xf0, ~value);
				usleep(delay);
			}
		}
		write10Bits(spi, 0x70, 0);
		write10Bits(spi, 0xf0, 0);
	}
	free(spi);
	debug("Finished spi example.");
	return 0;
}

void write12Bits(spi_properties *spi, unsigned char reg, unsigned short value) {
	unsigned char data[2] = {};
	data[0] = reg | ((value & 0xff00) >> 8);
	data[1] = (value & 0x00ff);
	if (spi_send(spi, data, sizeof(data)) == -1) {
		perror("Failed to update output.");
	}
}

/*
 * MCP4922 is a 12-Bit Dual Voltage Output Digital-to-Analog Converter with SPI Interface.
 * This example will increase the output for A untill the maximum and then decrease to the minimum 10 times.
 * Output B will reflect the inverse of output A.
 * 4096
 */
int spiMCP4922Example() {
	setup();

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 3;
	spi->speed = 2400000;
	spi->flags = O_RDWR;

	int delay = 1000;

	uint8_t isSpiOpen = spi_open(spi);
	if (isSpiOpen == 0) {

		int i = 0;
		unsigned short value = 0;
		while(i++ < 10) {
			while(value < 4096) {
				write12Bits(spi, 0x70, value);
				write12Bits(spi, 0xf0, ~value);
				usleep(delay);
				value = value + 1;
			}
			while(value > 0) {
				value = value - 1;
				write12Bits(spi, 0x70, value);
				write12Bits(spi, 0xf0, ~value);
				usleep(delay);
			}
		}
		write12Bits(spi, 0x70, 0);
		write12Bits(spi, 0xf0, 0);
	}
	free(spi);
	debug("Finished spi example.");
	return 0;
}

void dprint(unsigned char value) {
	debug("0x%02x", value);
}

int set_MCP23S08_GPIO_values(spi_properties *spi, unsigned char value) {
	unsigned char mcp23s08_gpios[] = { 0x40, 0x09, 0x00, };
	mcp23s08_gpios[2] = value;
//	dprint(value);
	spi_send(spi, mcp23s08_gpios, sizeof(mcp23s08_gpios));
	return 1;
}

int send_MCP49x2_values(spi_properties *spi, unsigned char value) {
	int bit_position = 8;
	while(bit_position > 0) {
		unsigned char bit_to_send = (value & 0x80) >> 2;
//		dprint(bit_to_send);
		unsigned char clkdata = bit_to_send;
		set_MCP23S08_GPIO_values(spi, clkdata);
		clkdata = (MCP23S08_SCK | bit_to_send);
		set_MCP23S08_GPIO_values(spi, clkdata);
		bit_position--;
		value = value << 1;
	}
	return 1;
}

void set_MCP49x2(unsigned char data[2], unsigned char chip, spi_properties* spi) {
	set_MCP23S08_GPIO_values(spi, chip | MCP23S08_SCK | MCP23S08_SDI);
	set_MCP23S08_GPIO_values(spi, MCP23S08_SCK | MCP23S08_SDI);
	set_MCP23S08_GPIO_values(spi, MCP23S08_SDI);
	set_MCP23S08_GPIO_values(spi, MCP23S08_ALL_DOWN);
	send_MCP49x2_values(spi, data[0]);
	send_MCP49x2_values(spi, data[1]);
	set_MCP23S08_GPIO_values(spi, MCP23S08_SCK | MCP23S08_SDI);
	set_MCP23S08_GPIO_values(spi, chip | MCP23S08_SCK | MCP23S08_SDI);
}

void setData(unsigned char *data, unsigned char reg, unsigned short value) {
	data[0] = reg | ((value & 0xf0) >> 4);
	data[1] = (value & 0x0f) << 4;
}

int spiMC23S08_MCP4902Example() {
	unsigned char mcp23s08_setup[] = { 0x40, 0x00, 0x00, };

	setup();

	spi_properties *spi = malloc(sizeof(spi_properties));
	spi->spi_id = spi0;
	spi->bits_per_word = 8;
	spi->mode = 0;
	spi->speed = 10000000;
	spi->flags = O_RDWR;

	uint8_t isOpen = spi_open(spi);

	if (isOpen == 0) {
		spi_send(spi, mcp23s08_setup, sizeof(mcp23s08_setup));

		unsigned char data[2] = {};
		int delay = 1000;
		int i = 0;
		while(i++ < 10) {
			unsigned short value = 0x0000;
			while(value < 0xff) {
				setData(data, 0x70, value);
				set_MCP49x2(data, MCP4902_1_CS, spi);
				setData(data, 0xf0, ~value);
				set_MCP49x2(data, MCP4902_1_CS, spi);
				usleep(delay);
				value = value + 1;
			}
			while(value > 0x00) {
				value = value - 1;
				setData(data, 0x70, value);
				set_MCP49x2(data, MCP4902_1_CS, spi);
				setData(data, 0xf0, ~value);
				set_MCP49x2(data, MCP4902_1_CS, spi);
				usleep(delay);
			}
		}
		data[0] = 0x70;
		data[1] = 0x00;
		set_MCP49x2(data, MCP4902_1_CS, spi);
		data[0] = 0xf0;
		data[1] = 0x00;
		set_MCP49x2(data, MCP4902_1_CS, spi);
	}

	free(spi);
	debug("Finished spi example.");
	return 0;
}
