#ifndef SPI_H_
#define SPI_H_

#include "main.h"

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_MOSI	PORTB3
#define SPI_MISO	PORTB4
#define SPI_SCK		PORTB5
#define SPI_CS		PORTB2

void spi_init();
uint8_t spi_read_byte(void);


#endif /* SPI_H_ */