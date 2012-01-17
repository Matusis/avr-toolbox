#include "i2c.h"

void i2c_sta(void){
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)))  {; }
}

void i2c_sto(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
//	while ( chb(TWCR, TWSTO) );
}

void i2c_send(uint8_t byt){
	TWDR = byt;
	TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT))) {;}
}

uint8_t i2c_read(char last){
	if (last == 1){ TWCR = (1<<TWINT) | (1<<TWEN); }
	else { TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); }
    while (!(TWCR & (1<<TWINT))) ;

	return TWDR;
}
