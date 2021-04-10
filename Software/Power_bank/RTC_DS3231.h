#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#include "main.h"

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
} Time_t;

void rtc_init(void);
void rtc_get_time(Time_t *time);
void rtc_set_time(Time_t *time, uint8_t hour, uint8_t min);

#define DS3231_WRITE_ADDR           0x68
#define DS3231_READ_ADDR            0x69

/*REGISTERS*/
/*Time*/
#define DS3231_TIME_CAL_ADDR        0x00
#define DS3231_TIME_MIN_ADDR        0x01
#define DS3231_TIME_HOUR_ADDR       0x02
#define DS3231_ALARM1_ADDR          0x07
#define DS3231_ALARM2_ADDR          0x0B
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F
#define DS3231_AGING_OFFSET_ADDR    0x10
#define DS3231_TEMPERATURE_ADDR     0x11

/*Control*/
#define DS3231_CONTROL_A1IE			0x1		/* Alarm 2 Interrupt Enable */
#define DS3231_CONTROL_A2IE			0x2		/* Alarm 2 Interrupt Enable */
#define DS3231_CONTROL_INTCN		0x4		/* Interrupt Control */
#define DS3231_CONTROL_RS1			0x8		/* square-wave rate select 2 */
#define DS3231_CONTROL_RS2    		0x10	/* square-wave rate select 2 */
#define DS3231_CONTROL_CONV			0x20	/* Convert Temperature */
#define DS3231_CONTROL_BBSQW		0x40	/* Battery-Backed Square-Wave Enable */
#define DS3231_CONTROL_EOSC			0x80	/* not Enable Oscillator, 0 equal on */

/*Status*/
#define DS3231_STATUS_A1F			0x01		/* Alarm 1 Flag */
#define DS3231_STATUS_A2F			0x02		/* Alarm 2 Flag */
#define DS3231_STATUS_BUSY			0x04		/* device is busy executing TCXO */
#define DS3231_STATUS_EN32KHZ		0x08		/* Enable 32KHz Output  */
#define DS3231_STATUS_OSF			0x80		/* Oscillator Stop Flag */


#endif /* rtc_ds3231_H_ */