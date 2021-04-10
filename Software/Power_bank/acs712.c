#include "acs712.h"

static uint8_t meas_num;
static uint8_t adc_values[MAX_COUNTS];
float adc_average;

void adc_init(void)
{
	ADCSRA |= 1<<ADEN | 1<<ADIE
		   |  1<<ADPS0 | 1<<ADPS1 | 1<<ADPS2;
	ADMUX  |= 1<<ADLAR | 1<<REFS0 | 1<<MUX1; //ADC2						
}

void adc_start(void)
{
	if(oled_show_mode == MODEINPUT)
	{
		ADMUX  |= 1<<MUX0;
		ADMUX  &= ~(1<<MUX1);
	}
	if(oled_show_mode == MODEOUTPUT)
	{
		ADMUX  |= 1<<MUX1;
		ADMUX  &= ~(1<<MUX0);
	}
	ADCSRA |= (1<<ADSC);
}

float acs712_get_current(void)
{
	float voltage, current;
	
	voltage = (VREF * adc_average / 255) - OFFSET;
	current = (voltage - VREF / 2) / ACS712_SENSITIVITY5;
	if(current < 0)
		current *= -1;
	
	return current;
} 

ISR(ADC_vect)
{
	if(meas_num != MAX_COUNTS)
	{
		adc_values[meas_num] = ADCH;
		meas_num++;
	}
	else
	{
		uint16_t temp = 0;
		for(uint8_t i = 0; i < MAX_COUNTS; i++)
			temp += adc_values[i];
		adc_average = (float)temp / (float)MAX_COUNTS;
		meas_num = 0;
	}
	adc_start();
}
