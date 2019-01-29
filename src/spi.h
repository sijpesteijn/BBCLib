/*
 * spi.h
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>
#include <stdint.h>
#include <linux/spi/spidev.h>

typedef enum {
	spi0 = 0, spi1 = 1
} spi;

typedef struct {
	int fd;
	spi spi_id;
	uint8_t bits_per_word; /*!< @brief is used to hold the bits per word size of SPI */
	uint8_t mode; /*!< @brief is used to hold the mode of SPI */
	uint32_t speed; /*!< @brief is used to hold the speed of SPI */
	uint8_t flags;
} spi_properties;

extern uint8_t spi_open(spi_properties* spi);
extern uint8_t spi_send(spi_properties *spi, unsigned char tx[], int length);
extern uint8_t spi_transfer(spi_properties *spi, unsigned char tx[], unsigned char rx[], int length);
extern uint8_t spi_close(spi_properties *spi);

#endif /* SPI_H_ */
