/*
 * spi.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include "spi.h"
#include "log.h"
#include <sys/ioctl.h>

uint8_t spi_open(spi_properties *spi) {
//	syslog (LOG_INFO, "spi open - spi:%d bits_per_word:%d speed:%d mode:%f", spi, bits_per_word, speed, mode);
    char filename[20];
    sprintf(filename, "/dev/spidev1.%d", spi->spi_id);
    spi->fd = open(filename, spi->flags);
    if (spi->fd < 0) {
		perror("could not open spi.");
		return -1;
    }
    info("FD: %i", spi->fd);
    if (ioctl(spi->fd, SPI_IOC_WR_MODE, &spi->mode)==-1){
       perror("SPI: Can't set SPI mode.");
       return -1;
    }
    if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &spi->bits_per_word)==-1){
       perror("SPI: Can't set bits per word.");
       return -1;
    }
    if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi->speed)==-1){
       perror("SPI: Can't set max speed HZ");
       return -1;
    }

    // Check that the properties have been set
    info("SPI fd is: %d\n", spi->fd);
    info("SPI Mode is: %d\n", spi->mode);
    info("SPI Bits is: %d\n", spi->bits_per_word);
    info("SPI Speed is: %d\n", spi->speed);
    return 0;
}

uint8_t spi_close(spi_properties *spi) {
    info("spi close - spi:%d", spi->fd);
    close(spi->fd);
    return 0;
}

uint8_t spi_send(spi_properties *spi, unsigned char tx[], int length) {
	unsigned char rx[length];
	return spi_transfer(spi, tx, rx, length);
}

uint8_t spi_transfer(spi_properties *spi, unsigned char tx[], unsigned char rx[], int length) {
	struct spi_ioc_transfer transfer = {
		   .tx_buf = (unsigned long)tx,
		   .rx_buf = (unsigned long)rx,
		   .len = length,
		   .delay_usecs = 0,
		   .speed_hz = spi->speed,
		   .bits_per_word = spi->bits_per_word,
   };
   // send the SPI message (all of the above fields, inc. buffers)
   int status = ioctl(spi->fd, SPI_IOC_MESSAGE(1), &transfer);
   if (status < 0) {
      perror("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return 0; //status;
}
