#ifndef __HD44780
#define	__HD44780

#include <util/delay.h>
#include <avr/io.h>
#include "base.h"

#define __LCD8__

#define _lcd_dat_prt	PORTD
#define _lcd_dat_ddr 	DDRD
#define	_lcd_dat_pin	PIND
#define _lcd_cmd_prt	PORTC
#define _lcd_cmd_ddr	DDRC

#define _lcd_bit_rs	1
#define _lcd_bit_rw	2
#define _lcd_bit_e	3

void lcd_wait(void);
void lcd_init();
void lcd_wr_dat(unsigned char dat);
void lcd_wr_cmd(unsigned char dat);
void lcd_wr(unsigned char dat, unsigned char cmd);

#endif
