#include "button.h"


/*
	Toggle indication
*/
void button_init(void)
{
	BUTTON_DDR  &= ~(1<<BUTTON);
	BUTTON_PORT |= 1<<BUTTON;
	/*Interrupt upon falling edge*/
	EICRA |= 1<<ISC01;
	/*Enable external interrupt INT0*/
	EIMSK |= 1<<INT0;	
}

