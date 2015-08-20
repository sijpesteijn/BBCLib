/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <stdio.h>
#include "../../src/core.h"
#include "../../src/i2c.h"
#include "matrix_shapes.h"

#define ADX_DEVID   0x53
#define POWER_CTL   0x2D
#define DATA_FORMAT 0x31
#define DATAX0      0x32
#define DATAX1      0x33
#define DATAY0      0x34
#define DATAY1      0x35
#define DATAZ0      0x36
#define DATAZ1      0x37
#define BUFFER_SIZE 0x40

int daddress;

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
	init_bbc_lib();
	if (open_i2c(i2c) == -1) {
		syslog(LOG_ERR, "%s", "Could not open i2c bus.");
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
	syslog(LOG_INFO, "%s", "Finished i2c example.");
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
	init_bbc_lib();
	i2c_properties *i2c = malloc(sizeof(i2c_properties));
	i2c->i2cnr = i2c1;
	i2c->deviceAddress = ADX_DEVID;
	i2c->openMode = O_RDWR;
	if (open_i2c(i2c) == -1) {
		syslog(LOG_ERR, "%s", "Could not open i2c bus.");
		return 0;
	}
	unsigned char *readBuffer = malloc(sizeof(unsigned char) * BUFFER_SIZE);

	// Setup adxl345
	write_data_i2c(i2c, POWER_CTL, 0x08);
	write_data_i2c(i2c, DATA_FORMAT, 0x00);
	write_data_i2c(i2c, 0x00, 0x00);

	read_i2c(i2c, readBuffer, BUFFER_SIZE);
	syslog(LOG_INFO, "The Device ID is: 0x%02x", readBuffer[0x00]);
	syslog(LOG_INFO, "The POWER_CTL mode is: 0x%02x", readBuffer[POWER_CTL]);
	syslog(LOG_INFO, "The DATA_FORMAT is: 0x%02x", readBuffer[DATA_FORMAT]);

	int i;
	for (i = 0; i < 20; i++) {
		write_data_i2c(i2c, 0x00, 0x00);
		read_i2c(i2c, readBuffer, BUFFER_SIZE);
		short x = combineValues(readBuffer[DATAX1], readBuffer[DATAX0]);
		short y = combineValues(readBuffer[DATAY1], readBuffer[DATAY0]);
		short z = combineValues(readBuffer[DATAZ1], readBuffer[DATAZ0]);

		syslog(LOG_INFO, "X = %i Y = %i Z = %i Sample: %i\n", x, y, z, i+1);
		usleep(1000000);
	}
	free(readBuffer);
	free(i2c);

	return 0;
}
