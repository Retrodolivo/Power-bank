#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#include "main.h"


void RTC_init(void);
void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec);
void RTC_read_time(void);
void RTC_SQW_set(void);
void RTC_hour_increment(void);
void RTC_min_increment(void);



#endif /* RTC_DS3231_H_ */