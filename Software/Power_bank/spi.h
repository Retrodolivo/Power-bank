#ifndef SPI_H_
#define SPI_H_

#include "main.h"

#define DDR_SPI		DDRB
#define PORT_SPI	PORTB
#define MOSI		PORTB3
#define MISO		PORTB4
#define SCK			PORTB5
#define CS			PORTB2

void spi_init();
uint8_t spi_read_byte(void);


#endif /* SPI_H_ */