#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

void button_init(void);

#define BUTTON_DDR			DDRD
#define BUTTON_PORT			PORTD
#define BUTTON				PORTD2

#endif /* BUTTON_H_ */