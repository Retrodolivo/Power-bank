#ifndef PWM_LED_H_
#define PWM_LED_H_

#include "main.h"

void pwm_led_init(void);

#define PWM_LED_DDR		DDRB
#define PWM_LED_PORT	PORTB
#define PWM_LED_PIN		PORTB1


#endif /* PWM_LED_H_ */