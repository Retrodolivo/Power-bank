#include "RTC_DS3231.h"

#define SEC_REG		0x00
#define MIN_REG		0x01
#define HOUR_REG	0x02
#define CNTRL_REG	0x0E
#define STATUS_REG	0x0F
#define A1M1_BIT	0x07

static uint8_t RTC_wr_addr = 0b11010000;
static uint8_t RTC_read_addr = 0b11010001;

uint8_t sec, min, hour;

static uint8_t dec_to_bin(uint8_t c)
{
	uint8_t ch = ((c/10)<<4)|(c%10);
	return ch;
}

static uint8_t bin_to_dec(uint8_t c)
{
	uint8_t ch = ((c>>4)*10+(0b00001111&c));
	return ch;
}

void RTC_init(void)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(CNTRL_REG);
	i2c_sendbyte(0b00000000);				//Enable 1Hz wave
	i2c_stop();
}

void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(SEC_REG);
	i2c_sendbyte(dec_to_bin(sec));				
	i2c_sendbyte(dec_to_bin(min));
	i2c_sendbyte(dec_to_bin(hour));
	i2c_stop();		
}

void RTC_read_time(void)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(SEC_REG);
	i2c_stop();
	i2c_start();
	i2c_sendbyte(RTC_read_addr);
	sec = i2c_readbyte();
	min = i2c_readbyte();
	hour = i2c_readbyte();
	sec = bin_to_dec(sec);
	min = bin_to_dec(min);
	hour = bin_to_dec(hour);

}

void RTC_SQW_set(void)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(CNTRL_REG);
	i2c_sendbyte(0x00);				//bits RS0 RS1 INTCN in 0
	i2c_stop();
}

void RTC_hour_increment(void)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(HOUR_REG);
	if(hour < 23)
		i2c_sendbyte(dec_to_bin(hour++));
	else
		i2c_sendbyte(0);
	i2c_stop();
}

void RTC_min_increment(void)
{
	i2c_start();
	i2c_sendbyte(RTC_wr_addr);
	i2c_sendbyte(MIN_REG);
	if(min < 59)
		i2c_sendbyte(dec_to_bin(min++));
	else
		i2c_sendbyte(0);
	i2c_stop();
}
