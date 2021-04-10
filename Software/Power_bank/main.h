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
#include "ssd1306_font.h"
#include "adc_mcp3201.h"
#include "cur_sens_acs712.h"
#include "RTC_DS3231.h"

#define SW_IN_PIN			PORTD6
#define SW_OUT_PIN			PORTD7

#define SW_BTN_PIN			PORTD2

#define SW_IN_PIN_LOW		PORTD &= ~(1<<SW_IN_PIN);
#define SW_OUT_PIN_LOW		PORTD &= ~(1<<SW_OUT_PIN);
#define SW_IN_PIN_HIGH		PORTD |= 1<<SW_IN_PIN;
#define SW_OUT_PIN_HIGH		PORTD |= 1<<SW_OUT_PIN;

#define INPUT_SENS_PIN		PORTD5
#define PWM_LED_PIN			PORTB1

#define MODEIDLE			0		
#define MODEINPUT			1
#define MODEOUTPUT			2


#endif /* MAIN_H_ */