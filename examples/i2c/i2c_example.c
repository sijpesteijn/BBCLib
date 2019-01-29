/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "i2c_example.h"
#include "../../src/log.h"
#include <unistd.h>
#include <stdlib.h>

const __u8 heart[4][16] = {
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x3c, 0x00, 0x24, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x24, 0x00, 0x5a, 0x00, 0x42, 0x00, 0x42, 0x00, 0x24, 0x00, 0x18, 0x00, 0x00 },
		{ 0x00, 0x42, 0x00, 0xa5, 0x00, 0x99, 0x00, 0x81, 0x00, 0x81, 0x00, 0x42, 0x00, 0x24, 0x00, 0x18 }
};

const __u8 smile[3][16] = {
		{ 0x00, 0x00, 0x66, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x42, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x66, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x3c, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x66, 0x00, 0x66, 0x00, 0x00, 0x00, 0x42, 0x00, 0x42, 0x00, 0x3c, 0x00, 0x00, 0x00 }
};

void clearDisplay(i2c_properties *i2c) {
	__u8 reg = 0x00;
	for (; reg < 0x10; reg++) {
		write_data_i2c(i2c, reg, 0x00);
	}
}

void play(i2c_properties *i2c, size_t nrows, const __u8 shape[nrows][16], int delay) {
	__u8 reg;
	int i, j, loop = 5;
	while (loop-- > 0) {
		for (j = 0; j < nrows; j++) {
			reg = 0x00;
			for (i = 0; i < 16; i++) {
				write_data_i2c(i2c, reg++, shape[j][i]);
			}
			usleep(delay * 10000);
		}
	}
	for (i = 0; i < 15; i++) {
		clearDisplay(i2c);
		usleep(delay * 800);
		int g;
		reg = 0x00;
		for (g = 0; g < 16; g++)
			write_data_i2c(i2c, reg++, shape[nrows - 1][g]);
	}

}

int i2c8x8LedMatrix() {
	i2c_properties *i2c = malloc(sizeof(i2c_properties));
	i2c->i2cnr = i2c1;
	i2c->deviceAddress = 0x70;
	i2c->openMode = O_RDWR;
	if (open_i2c(i2c) == -1) {
		debug("Could not open i2c bus.");
		return 0;
	}

	// Setup matrix
	write_byte_i2c(i2c,0x21); // Start oscillator
	write_byte_i2c(i2c,0x81); // Display on, blinking off
	write_byte_i2c(i2c,0xe7); // Full brightness
	clearDisplay(i2c);

	play(i2c, 4, heart, 10);
	play(i2c, 3, smile, 100);

	free(i2c);
	debug("Finished i2c example.");
	return 0;
}

// short is 16-bits in size on the BBB
short combineValues(unsigned char msb, unsigned char lsb) {
	//shift the msb right by 8 bits and OR with lsb
	return ((short) msb << 8) | (short) lsb;
}

/*
 * ADXL345 is a Digital Accelerometer. It can measure movement on 3 axis.
 * This example will probe the axis 20 times every second.
 */
int i2cADXL345() {
	i2c_properties *i2c = malloc(sizeof(i2c_properties));
	i2c->i2cnr = i2c1;
	i2c->deviceAddress = ADX_DEVID;
	i2c->openMode = O_RDWR;
	if (open_i2c(i2c) == -1) {
		debug("Could not open i2c bus.");
		return 0;
	}
	unsigned char *readBuffer = malloc(sizeof(unsigned char) * BUFFER_SIZE);

	// Setup adxl345
	write_data_i2c(i2c, POWER_CTL, 0x08);
	write_data_i2c(i2c, DATA_FORMAT, 0x00);
	write_data_i2c(i2c, 0x00, 0x00);

	read_i2c(i2c, readBuffer, BUFFER_SIZE);
	debug("The Device ID is: 0x%02x", readBuffer[0x00]);
	debug("The POWER_CTL mode is: 0x%02x", readBuffer[POWER_CTL]);
	debug("The DATA_FORMAT is: 0x%02x", readBuffer[DATA_FORMAT]);

	int i;
	for (i = 0; i < 20; i++) {
		write_data_i2c(i2c, 0x00, 0x00);
		read_i2c(i2c, readBuffer, BUFFER_SIZE);
		short x = combineValues(readBuffer[DATAX1], readBuffer[DATAX0]);
		short y = combineValues(readBuffer[DATAY1], readBuffer[DATAY0]);
		short z = combineValues(readBuffer[DATAZ1], readBuffer[DATAZ0]);

		printf("X = %i Y = %i Z = %i Sample: %i\n", x, y, z, i+1);
		usleep(1000000);
	}
	free(readBuffer);
	free(i2c);

	return 0;
}
