#ifndef I2C_H_
#define I2C_H_

#include "main.h"

enum i2c_speed {Standart, Fast};

void i2c_init(enum i2c_speed speed);
void i2c_start(void);
void i2c_stop(void);
void i2c_sendbyte(uint8_t byte);
uint8_t i2c_readbyte(void);

#endif /* I2C_H_ */
