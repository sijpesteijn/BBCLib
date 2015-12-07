/*
 * i2c.h
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#ifndef I2C_H_
#define I2C_H_

#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
//#include <linux/i2c.h>
//#include <linux/i2c-dev.h>

typedef enum {
	i2c0 = 0, i2c1 = 1
} i2c;

typedef struct {
	int fd;
	i2c i2cnr;
	int deviceAddress;
	int openMode;
} i2c_properties;

extern int open_i2c(i2c_properties *i2c);
extern int write_byte_i2c(i2c_properties *i2c, unsigned char reg);
extern int write_data_i2c(i2c_properties *i2c, unsigned char reg, char value);
extern int read_i2c(i2c_properties *i2c, unsigned char *readBuffer, int bufferSize);

#endif /* I2C_H_ */
