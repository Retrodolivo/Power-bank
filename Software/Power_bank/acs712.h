#ifndef ACS712_H_
#define ACS712_H_

#include "main.h"

void adc_init(void);
void adc_start(void);
float acs712_get_current(void);

extern uint8_t oled_show_mode;

#define ACS712_SENSITIVITY5 0.185
#define OFFSET				0
#define VREF				5.093
#define MAX_COUNTS			100

#endif /* ACS712_H_ */