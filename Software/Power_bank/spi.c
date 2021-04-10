#include "spi.h"

void spi_init(void)
{
	SPI_DDR	  |=  1<<SPI_MOSI | 1<<SPI_SCK | 1<<SPI_CS;
	SPI_DDR   &= ~(1<<SPI_MISO);
	SPI_PORT  |= 1<<SPI_CS;
				
	SPCR = 1<<SPE | 1<<MSTR | 1<<SPR0;
}

uint8_t spi_read_byte(void)
{
	SPDR = 0x00;
	
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}


