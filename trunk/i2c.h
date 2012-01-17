#ifndef __I2C
#define	__I2C

#include <avr/io.h>
#include <util/delay.h>
#include "base.h"

void i2c_sta(void);
void i2c_sto(void);
void i2c_send(uint8_t byt);
uint8_t i2c_read(char last);

#endif
