/*
 * mcp23s08.h
 *
 *  Created on: Jul 10, 2015
 *      Author: gijs
 */

#ifndef EXAMPLES_SPI_MCP23S08_H_
#define EXAMPLES_SPI_MCP23S08_H_

#define MCP23S08_IODIR 0x0
#define MCP23S08_IPOL 0x1
#define MCP23S08_GPINTEN 0x2
#define MCP23S08_DEFVAL 0x3
#define MCP23S08_INTCON 0x4
#define MCP23S08_IOCON 0x5
#define MCP23S08_GPPU 0x6
#define MCP23S08_INTF 0x7
#define MCP23S08_INTCAP 0x8
#define MCP23S08_GPIO 0x9
#define MCP23S08_OLAT 0xA

typedef enum {
	SPI_SPEED_8M, SPI_SPEED_4M, SPI_SPEED_2M, SPI_SPEED_1M, SPI_SPEED_125K
} SPI_SPEED;
typedef struct {
	unsigned char chip_select;
	SPI_SPEED spi_speed;
	unsigned char port;
} mcp23s08;

#define SPI_MODE_MCP23S08 3                     ///< SPI mode of the MCP23S17 I/O expanders

#define MCP23S08_PIN_HIGH 1
#define MCP23S08_PIN_LOW 0

int_fast8_t mcp23s08_init(mcp23s08* device, unsigned char cs,
		SPI_SPEED spi_speed);
void mcp23s08_setPort(mcp23s08* device, unsigned char value);
unsigned char mcp23s08_readRegister(mcp23s08* device, unsigned char address);
void mcp23s08_writeRegister(mcp23s08* device, unsigned char address,
		unsigned char value);

#endif /* EXAMPLES_SPI_MCP23S08_H_ */
