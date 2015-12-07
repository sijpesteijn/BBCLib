/*
 * i2c.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include "core.h"
#include "i2c.h"

int res, daddress;

int open_i2c(i2c_properties *i2c) {
	syslog(LOG_INFO, "i2c open - i2c:%d device address: 0x%x mode:%i", i2c->i2cnr,
			i2c->deviceAddress, i2c->openMode);
	char filename[20];

	sprintf(filename, "/dev/i2c-%d", i2c->i2cnr);
	i2c->fd = open(filename, i2c->openMode);
	if (i2c->fd < 0) {
		if (errno == ENOENT) {
			syslog(LOG_ERR, "Error: Could not open file "
					"/dev/i2c-%d: %s\n", i2c->i2cnr, strerror(ENOENT));
		} else {
			syslog(LOG_ERR, "Error: Could not open file "

					"`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		return -1;
	}

	if (ioctl(i2c->fd, I2C_SLAVE, i2c->deviceAddress) < 0) {
		syslog(LOG_ERR, "Error: Could not set address to 0x%02x: %s\n",
				i2c->deviceAddress, strerror(errno));
		return -errno;
	}
	return 0;
}

int write_byte_i2c(i2c_properties *i2c, unsigned char reg) {
	res = i2c_smbus_write_byte(i2c->fd, reg);
	if (res < 0) {
		syslog(LOG_ERR, "Warning - write failed, filename=%i, register=%i\n",
				i2c->fd, reg);
		return 1;
	}
	return 0;
}

int write_data_i2c(i2c_properties *i2c, unsigned char reg, char value) {
	unsigned char buf[2];
	buf[0] = reg;
	buf[1] = value;
	if (write(i2c->fd, buf, 2) != 2) {
		syslog(LOG_ERR,
				"Warning - write data failed, filename=%i, register=%i\n", i2c->fd,
				reg);
		return 1;
	}
	return 0;
}

int read_i2c(i2c_properties *i2c, unsigned char *readBuffer, int bufferSize) {
	if (read(i2c->fd, readBuffer, bufferSize) != bufferSize) {
		perror("Failed to read in the buffer\n");
		return 1;
	}
	if(readBuffer[0x00]!=0xE5){
		syslog(LOG_INFO, "%s", "Problem detected! Device ID is wrong");
	}
	return 0;
}

