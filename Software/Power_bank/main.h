#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "i2c.h"
#include "spi.h"
#include "oled_ssd1306.h"
#include "oled_font.h"
#include "adc3201.h"
#include "acs712.h"
#include "rtc_ds3231.h"
#include "pwm_led.h"
#include "button.h"


#define ANLG_SWITCH_DDR				DDRD
#define ANLG_SWITCH_PORT			PORTD
 
#define SWITCH_INPUT_PIN			PORTD6
#define SWITCH_OUTPUT_PIN			PORTD7

#define SWITCH_INPUT_PIN_LOW		ANLG_SWITCH_PORT &= ~(1<<SWITCH_INPUT_PIN);
#define SWITCH_OUTPUT_PIN_LOW		ANLG_SWITCH_PORT &= ~(1<<SWITCH_OUTPUT_PIN);
#define SWITCH_INPUT_PIN_HIGH		ANLG_SWITCH_PORT |= 1<<SWITCH_INPUT_PIN;
#define SWITCH_OUTPUT_PIN_HIGH		ANLG_SWITCH_PORT |= 1<<SWITCH_OUTPUT_PIN;

#define INPUT_SENS_DDR				DDRD
#define INPUT_SENS_PORT				PORTD
#define INPUT_SENS_PIN				PORTD5

#define	CHARGING					(PIND&(1<<INPUT_SENS_PIN))
#define	NOT_CHARGING				(!(PIND&(1<<INPUT_SENS_PIN)))

enum Oled_show_mode
{
	MODEIDLE,
	MODEINPUT,
	MODEOUTPUT
};


#endif /* MAIN_H_ */