#include "i2c.h"


void i2c_init(enum i2c_speed speed)
{
	switch(speed)
	{
		case Standart:
			TWBR = (F_CPU / 100000 - 16) / 2;
			break;
			
		case Fast:
			TWBR = (F_CPU / 400000 - 16) / 2;
			break;
	}
}

void i2c_start(void)
{
	/*Set HIGH at SCL(TWINT), Generate START(TWSTA), Enable I2C(TWEN)*/
	TWCR = 1<<TWINT | 1<<TWSTA | 1<<TWEN;
	/*Wait till SCL LOW*/
	while(!(TWCR & 1<<TWINT));			 
}

void i2c_stop(void)
{
	/*Clear interrupt flag(TWINT), Generate STOP(TWSTO)*/
	TWCR = 1<<TWINT | 1<<TWSTO | 1<<TWEN;
}

void i2c_sendbyte(uint8_t byte)
{
	TWDR = byte;
	TWCR = 1<<TWINT | 1<<TWEN;
	/*Wait till SCL LOW*/
	while(!(TWCR & 1<<TWINT));
}

uint8_t i2c_readbyte(void)
{
	/*Set HIGH at SCL(TWINT), Enable I2C(TWEN), Set ACK bit(TWEA)*/
	TWCR = 1<<TWINT | 1<<TWEN | 1<<TWEA;
	/*Wait till SCL LOW*/
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}
