#include "spi.h"

void spi_init(void)
{
	DDR_SPI	  |=  1<<MOSI | 1<<SCK | 1<<CS;
	DDR_SPI   &= ~(1<<MISO);
	PORT_SPI  |= 1<<CS;
				
	SPCR = 1<<SPE | 1<<MSTR | 1<<SPR0;
}

uint8_t spi_read_byte(void)
{
	SPDR = 0x00;
	
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}


