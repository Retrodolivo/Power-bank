#include "main.h"

static void show_time(void);
static void show_voltage(void);
static void show_current(void);
static void show_Wh(void);
static void ports_init(void);
static void wh_time_calc(void);
static void init_pwm_led(void);

static char buff[100];  //буфер дисплея
extern uint8_t sec, min, hour;

float total_Wh = 0;

uint8_t oled_show_mode = MODEIDLE;

//-------------temp variables------------------
static uint8_t cnt,
			   temp,
			   pwm_state;
//---------------------------------------------

static void ports_init(void)
{
	DDRD  |= 1<<SW_IN_PIN | 1<<SW_OUT_PIN;
	PORTD &= ~(1<<SW_IN_PIN | 1<<SW_OUT_PIN);
	
	DDRD  &= ~(1<<SW_BTN_PIN);							//External ADC switch button
	PORTD |= 1<<SW_BTN_PIN;

	EICRA |= 1<<ISC01;
	EIMSK |= 1<<INT0;
	
	DDRD  &= ~(1<<INPUT_SENS_PIN);
	PORTD &= ~(1<<INPUT_SENS_PIN);	
//	EICRA |= 1<<ISC11;							//Interrupt upon falling edge
//	EIMSK |= 1<<INT1;							//Enable external interrupt

	DDRB  |= 1<<PWM_LED_PIN;
	PORTB &= ~(1<<PWM_LED_PIN); 
}

//----------OUT/IN puts-----------------------------------
ISR(INT0_vect)
{
	if(oled_show_mode == MODEIDLE)
	{
		if((PIND&(1<<INPUT_SENS_PIN)))
		{
			oled_clear();
			oled_goto(95, 0);
			sprintf(buff,"IN");
			oled_print6x8(buff);
			oled_show_mode = MODEINPUT;
			SW_IN_PIN_HIGH;
			SW_OUT_PIN_LOW;
			wh_time_calc();			
		}
		
		if((!(PIND&(1<<INPUT_SENS_PIN))) && (acs712_get_current() > 0.15))
		{
			oled_clear();
			oled_goto(95, 0);
			sprintf(buff,"OUT");
			oled_print6x8(buff);
			oled_show_mode = MODEOUTPUT;
			SW_IN_PIN_LOW;
			SW_OUT_PIN_HIGH;
			wh_time_calc();			
		}			
	} 		
}
//-------------------------------------------------------- 

static void init_pwm_led(void)
{
	//---------Timer1--------------------
	TCCR1A |= 1<<COM1A1				// OC1A toggle
		   |  1<<WGM11;
		   
	TCCR1B |= 1<<WGM12 | 1<<WGM13	//Fast PWM (mode #14)
		   |  1<<CS10;				//No prescaler
	TCNT1 = 0;
	ICR1  = 799;					//Fpwm = Fcpu / (ICR1 + 1) = 10kHz					
	OCR1A = 0;
	//----------Timer2-------------------
	TCCR2B |= 1<<CS22 | 1<<CS20;
	TIMSK2 = 1<<TOIE2;
	
}
//--------------LED PWM---------------------------------------
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

//--------------INTERRUPT AT 1HZ WAVE of RTC------------------
ISR(INT1_vect)
{
	oled_goto(0, 4);
	show_Wh();
	oled_goto(0, 6);
	show_time();			
}
//------------set fps of I and V indication-----------
ISR(TIMER0_OVF_vect)
{
	if(cnt == 5)
	{
		oled_goto(0, 0);
		show_voltage();
		oled_goto(0, 2);
		show_current();
		cnt = 0;
	}
	cnt++;
}
//---------------------------------------------------
int main(void)
{
	ports_init();
	init_pwm_led();	
	i2c_init();
	spi_init();
	oled_init();
	adc_init();
	adc_start();
	RTC_init();

	sei();
	
	while(1) 
    {
		_delay_ms(300);
		if(oled_show_mode == MODEIDLE)
		{
			oled_goto(31, 3);
			sprintf(buff,"Power bank");
			oled_print6x8(buff);
			oled_goto(38, 4);
			sprintf(buff,"10000mAh");
			oled_print6x8(buff);
			
			if((PIND&(1<<INPUT_SENS_PIN)))
			{
				oled_goto(10, 7);
				sprintf(buff,"IN");
				oled_print6x8(buff);
				if(temp == 0)
				{
					RTC_set_time(0, 0, 0);
					total_Wh = 0;
					
					SW_IN_PIN_HIGH;
					SW_OUT_PIN_LOW;
					wh_time_calc();
					
					temp = 1;
				}
			}
			if((!(PIND&(1<<INPUT_SENS_PIN))) && (acs712_get_current() > 0.15))
			{
				oled_goto(90, 7);
				sprintf(buff,"OUT");
				oled_print6x8(buff);
				if(temp == 0)
				{
					RTC_set_time(0, 0, 0);
					total_Wh = 0;
					 
					SW_IN_PIN_LOW;
					SW_OUT_PIN_HIGH;
					wh_time_calc();
					
					temp = 1;
				}								
			}
		}		
    }
}
//---------------------PEREDELAT' ETOT POZOR!!!!(hotia eto rabotaet)----------------
static void show_time(void)
{
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{	
		RTC_read_time();
		if(hour < 10 && min < 10 && sec < 10)
			sprintf(buff,"t = 0%d:0%d:0%d", hour, min, sec);
		else if(hour >= 10 && min > 10 && sec < 10)
			sprintf(buff,"t = %d:%d:0%d", hour, min, sec);
		else if(hour >= 10 && min < 10 && sec >= 10)
			sprintf(buff,"t = %d:0%d:%d", hour, min, sec);
		else if(hour >= 10 && min < 10 && sec < 10)
			sprintf(buff,"t = %d:0%d:0%d", hour, min, sec);
		else if(hour < 10 && min >= 10 && sec >= 10)
			sprintf(buff,"t = 0%d:%d:%d", hour, min, sec);
		else if(hour < 10 && min >= 10 && sec < 10)
			sprintf(buff,"t = 0%d:%d:0%d", hour, min, sec);
		else if(hour < 10 && min < 10 && sec >= 10)
			sprintf(buff,"t = 0%d:0%d:%d", hour, min, sec);
		else
			sprintf(buff,"t = %d:%d:%d", hour, min, sec);
		oled_print6x8(buff);
	}
}
//-----------------------------------------------------------------
static void show_voltage(void)
{
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"U = %.2fV", mcp3201_get_voltage());
		oled_print6x8(buff);
	}
}

static void show_current(void)
{
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"I = %.2fA", acs712_get_current());
		oled_print6x8(buff);
	}
}

static void show_Wh(void)
{
	float power;
	
	power = mcp3201_get_voltage() * acs712_get_current();
	if(power < 0)
		power *= -1;
	total_Wh += power / 3600;
	if(oled_show_mode == MODEINPUT || oled_show_mode == MODEOUTPUT)
	{
		sprintf(buff,"E = %.3fWh", total_Wh);
		oled_print6x8(buff);		
	}	
}

static void wh_time_calc(void)
{
	//------------set Wh and time fps--------------
	TCCR0B = 1<<CS00 | 1<<CS02;					//Prescaler - 1024
	TIMSK0 |= 1<<TOIE0;							//Allow interrupt
	//---------------------------------------------
	EICRA |= 1<<ISC11;
	EIMSK |= 1<<INT1;	
}
