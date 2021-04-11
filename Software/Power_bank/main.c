#include "main.h"

static void print_time(uint8_t x, uint8_t y);
static void print_Wh(uint8_t x, uint8_t y);
static void print_current(uint8_t x, uint8_t y);
static void print_voltage(uint8_t x, uint8_t y);
static void ports_init(void);
static void enable_wh_time_calc(void);
static void welcome_message(void);

enum Oled_show_mode oled_show_mode = MODEIDLE;

enum i2c_speed speed = Fast;
Time_t t = {0};
	
static char buff[100];			//display buffer
static float total_Wh = 0;
static uint8_t cnt, temp;

static void init_routine(void)
{
	button_init();
	pwm_led_init();
	ports_init();
	spi_init();
	i2c_init(speed);
	rtc_init();
	oled_init();
	adc_init();
	adc_start();
	sei();
}
static void ports_init(void)
{
	/*K176KT1 is analog switch IC. Is used for switching adc mcp3201 input.*/	
	ANLG_SWITCH_DDR  |= 1<<SWITCH_INPUT_PIN | 1<<SWITCH_OUTPUT_PIN;
	ANLG_SWITCH_PORT &= ~(1<<SWITCH_INPUT_PIN | 1<<SWITCH_OUTPUT_PIN);
	/*Is used to know if power bank is charging*/
	INPUT_SENS_DDR  &= ~(1<<INPUT_SENS_PIN);
	INPUT_SENS_PORT &= ~(1<<INPUT_SENS_PIN);							
}

static void welcome_message(void)
{
	sprintf(buff, "Power bank");
	oled_print(31, 3, buff);
	sprintf(buff, "10000mAh");
	oled_print(38, 4, buff);
}

int main(void)
{
	init_routine();
	
	while(1)
	{
		_delay_ms(300);
		if(oled_show_mode == MODEIDLE)
		{
			welcome_message();
			if(CHARGING)
			{
				sprintf(buff,"IN");
				oled_print(10, 7, buff);
				if(temp == 0)
				{
					rtc_set_time(&t, 0, 0);
					total_Wh = 0;
					
					SWITCH_INPUT_PIN_HIGH;
					SWITCH_OUTPUT_PIN_LOW;
					enable_wh_time_calc();
					
					temp = 1;
				}
			}
			if(NOT_CHARGING && (get_current() > 0.15))
			{
				sprintf(buff,"OUT");
				oled_print(90, 7, buff);
				if(temp == 0)
				{
					rtc_set_time(&t, 0, 0);
					total_Wh = 0;
					
					SWITCH_OUTPUT_PIN_LOW;
					SWITCH_INPUT_PIN_HIGH;
					enable_wh_time_calc();
					
					temp = 1;
				}
			}
		}
	}
}

/*
	Interrupt upon button press
*/
ISR(INT0_vect)
{
	if(oled_show_mode == MODEIDLE)
	{
		if(CHARGING)
		{
			oled_clear();
			sprintf(buff, "IN");
			oled_print(95, 0, buff);
			oled_show_mode = MODEINPUT;
			SWITCH_INPUT_PIN_HIGH;
			SWITCH_OUTPUT_PIN_LOW;
			enable_wh_time_calc();			
		}
		
		if(NOT_CHARGING && (get_current() > 0.15))
		{
			oled_clear();
			sprintf(buff, "OUT");
			oled_print(95, 0, buff);
			oled_show_mode = MODEOUTPUT;
			SWITCH_INPUT_PIN_LOW;
			SWITCH_OUTPUT_PIN_HIGH;
			enable_wh_time_calc();			
		}			
	} 		
}

static void print_time(uint8_t x, uint8_t y)
{
	oled_goto(x, y);
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{	
		rtc_set_time(&t, 0, 0);
		sprintf(buff,"t = %02d:%02d:%02d", t.hour, t.min, t.sec);
		oled_print6x8(buff);
	}
}

static void print_Wh(uint8_t x, uint8_t y)
{
	oled_goto(x, y);
	
	float power;
	
	power = get_voltage() * get_current();
	/*get absolute value of 'power'*/
	if(power < 0)
		power *= -1;
	total_Wh += power / 3600;
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"E = %.3fWh", total_Wh);
		oled_print6x8(buff);
	}
}

static void print_voltage(uint8_t x, uint8_t y)
{
	oled_goto(x, y);
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"U = %.2fV", get_voltage());
		oled_print6x8(buff);
	}
}

static void print_current(uint8_t x, uint8_t y)
{
	oled_goto(x, y);
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"I = %.2fA", get_current());
		oled_print6x8(buff);
	}
}

static void enable_wh_time_calc(void)
{
	/*------------set Wh and time fps--------------*/
	/*Prescaler - 1024*/
	TCCR0B = 1<<CS00 | 1<<CS02;	
	/*Enable interrupt upon overflow*/
	TIMSK0 |= 1<<TOIE0;							
	/*Interrupt upon falling edge*/
	EICRA |= 1<<ISC11;
	/*Enable external interrupt INT1*/
	EIMSK |= 1<<INT1;	
}

/*------------Set fps of I and V indication-----------*/
ISR(TIMER0_OVF_vect)
{
	if(cnt == 5)
	{
		print_voltage(0, 0);
		print_current(0, 2);
		cnt = 0;
	}
	cnt++;
}

/*
	Interrupt at 1Hz wave of RTC
	print Wh and time every sec	
*/
ISR(INT1_vect)
{
	print_Wh(0, 4);
	print_time(0, 6);			
}
