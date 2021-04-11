#ifndef ACS712_H_
#define ACS712_H_

#include "main.h"

void adc_init(void);
void adc_start(void);
float get_current(void);

extern enum Oled_show_mode oled_show_mode;


#endif /* ACS712_H_ */