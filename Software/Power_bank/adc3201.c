#include "adc3201.h"

/*Voltage divider coeff at IN and OUT*/
static const float Res_div = 5.6 / (13.0 + 5.6);
/*REF192 output voltage*/
static const float Vref = 2.506;
static const uint16_t Max_code = 4096;


static uint16_t adc3201_raw(void)
{
	uint16_t data;
	uint16_t temp;
	
	SPI_PORT &= ~(1<<SPI_CS);
	data = spi_read_byte();
	data <<= 11;
	data >>= 3;
	temp = data;
	data = spi_read_byte();
	data |= temp;
	data >>= 1;
	
	SPI_PORT |= 1<<SPI_CS;
	
	return data;
}

float get_voltage(void)
{
	float voltage;

	voltage = Vref * (float)adc3201_raw() / Max_code;
	/*taking into account resistor voltage divider*/	
	voltage /= Res_div;
	return voltage;	
} 