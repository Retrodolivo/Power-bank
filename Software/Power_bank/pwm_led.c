#include "pwm_led.h"

static uint8_t pwm_state;

void pwm_led_init(void)
{
	PWM_LED_DDR  |= 1<<PWM_LED_PIN;
	PWM_LED_PORT &= ~(1<<PWM_LED_PIN);
	
	/*---------Timer1--------------------*/
	/*OC1A toggle*/
	TCCR1A |= 1<<COM1A1 | 1<<WGM11;
	/*Fast PWM (mode #14) No prescaler*/
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	/*Fpwm = F_CPU / (ICR1 + 1) = 20kHz*/
	ICR1  = 799;					
	//----------Timer2-------------------
	TCCR2B |= 1<<CS22 | 1<<CS20;
	TIMSK2 = 1<<TOIE2;
}

/*
	duty cycle from 0% to 100% periodically 
*/
ISR(TIMER2_OVF_vect)
{
	if(pwm_state == 0)
		OCR1A++;
	if(OCR1A == 600)
		pwm_state = 1;
	if(pwm_state == 1)
		OCR1A--;
	if(OCR1A == 0)
		pwm_state = 0;
}