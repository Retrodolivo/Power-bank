#include "acs712.h"

/*
	ACS712 is analog hall-based current sensor IC.
	It output signal is linear changing according to current value
	Idle output signal is VCC/2. Sensitivity is 0.185V/A
*/

#define MAX_COUNTS		100

static const float Sensitivity = 0.185;
static const float Offset = 0;
static const float Vref = 5.093;

static uint8_t meas_num;
static uint8_t adc_values[MAX_COUNTS];
static float adc_average;

void adc_init(void)
{
	/*Enable ADC(ADEN) and Interrupts(ADIE)*/
	ADCSRA |= 1<<ADEN | 1<<ADIE
	/*Division factor is 128*/
		   |  1<<ADPS0 | 1<<ADPS1 | 1<<ADPS2;
	/*Left adjust result(ADLAR) AVCC is Vref(REFS0) adc idle channel is ADC0*/
	ADMUX  |= 1<<ADLAR | 1<<REFS0;
}

void adc_start(void)
{
	/*Switch between ACS712 at input and output*/
	
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
	/*Start next conversion(ADSC)*/
	ADCSRA |= 1<<ADSC;
}

/*
	Get average value of internal adc of MAX_COUNTS measurements	
*/
ISR(ADC_vect)
{
	if(meas_num != MAX_COUNTS)
	{
		adc_values[meas_num] = ADCH;
		meas_num++;
	}
	else
	{
		float sum = 0;
		for(uint8_t i = 0; i < MAX_COUNTS; i++)
			sum += adc_values[i];
		adc_average = sum / MAX_COUNTS;
		meas_num = 0;
	}
	adc_start();
}

/*
	Return averaged current value
*/
float get_current(void)
{
	float adc_voltage, current;
	
	adc_voltage = (Vref * adc_average / 255) - Offset;
	current = (adc_voltage - Vref / 2) / Sensitivity;
	/*get absolute value of 'current'*/
	if(current < 0)
		current *= -1;
	
	return current;
} 
