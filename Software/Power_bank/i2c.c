#include "i2c.h"

#define F_SCL 350000 

void i2c_init(void)
{
	TWBR = ((F_CPU/F_SCL) - 16)/2;			//fscl
}

void i2c_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//set HIGH at SCL(TWINT), set state START(TWSTA), enable interface(TWEN)
	while(!(TWCR&(1<<TWINT)));				//wait until SCL goes LOW
}

void i2c_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2c_sendbyte(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t i2c_readbyte(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}