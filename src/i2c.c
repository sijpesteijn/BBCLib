/*
 * i2c.c
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#include "core.h"
#include "i2c.h"

int res, daddress, file;

//void  INThandler(int sig)
//{
//       // Closing file and turning off Matrix
//
////	unsigned short int clear[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//
////	displayImage(clear,res, daddress, file);
//
//        printf("Closing file and turning off the LED Matrix\n");
//        daddress = 0x20;
//        for(daddress = 0xef; daddress >= 0xe0; daddress--) {
//                res = i2c_smbus_write_byte(file, daddress);
//        }
//
//	signal(sig, SIG_IGN);
//
//        close(file);
//        exit(0);
//}

int open_i2c(i2c i2cnr, int deviceAddress, int openMode) {
	syslog(LOG_INFO, "i2c open - i2c:%d device address: 0x%x mode:%i", i2cnr,
			deviceAddress, openMode);
	char filename[20];

	sprintf(filename, "/dev/i2c-%d", i2cnr);
	file = open(filename, O_RDWR);
	if (file < 0) {
		if (errno == ENOENT) {
			syslog(LOG_ERR, "Error: Could not open file "
					"/dev/i2c-%d: %s\n", i2cnr, strerror(ENOENT));
		} else {
			syslog(LOG_ERR, "Error: Could not open file "

					"`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		return -1;
	}

//	signal(SIGINT, INThandler);
	if (ioctl(file, I2C_SLAVE, deviceAddress) < 0) {
		syslog(LOG_ERR, "Error: Could not set address to 0x%02x: %s\n",
				deviceAddress, strerror(errno));
		return -errno;
	}
	return 0;
}

int write_byte_i2c(unsigned char reg) {
	res = i2c_smbus_write_byte(file, reg);
	if (res < 0) {
		syslog(LOG_ERR, "Warning - write failed, filename=%i, register=%i\n",
				file, reg);
		return 1;
	}
	return 0;
}

int write_data_i2c(unsigned char reg, char value) {
	unsigned char buf[2];
	buf[0] = reg;
	buf[1] = value;
	if (write(file, buf, 2) != 2) {
		syslog(LOG_ERR,
				"Warning - write data failed, filename=%i, register=%i\n", file,
				reg);
		return 1;
	}
	return 0;
}

int read_i2c(unsigned char *readBuffer, int bufferSize) {
	if (read(file, readBuffer, bufferSize) != bufferSize) {
		perror("Failed to read in the buffer\n");
		return 1;
	}
	if(readBuffer[0x00]!=0xE5){
		syslog(LOG_INFO, "%s", "Problem detected! Device ID is wrong");
	}
	return 0;
}

