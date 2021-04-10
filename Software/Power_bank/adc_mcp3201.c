#include "adc_mcp3201.h"

static uint16_t mcp3201_read(void)
{
	uint16_t data;
	uint16_t temp;
	
	PORT_SPI &= ~(1<<CS);
	data = spi_read_byte();
	data <<= 11;
	data >>= 3;
	temp = data;
	data = spi_read_byte();
	data |= temp;
	data >>= 1;
	
	PORT_SPI |= 1<<CS;
	
	return data;
}

float mcp3201_get_voltage(void)
{
	float voltage;
	float vref = 2.506;
	float real_voltage;					//taking into account resistor divider
	
	
	voltage = vref * (float)mcp3201_read() / 4096;
	real_voltage = voltage / RES_DIV;
	return real_voltage;	
} 