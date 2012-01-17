#ifndef __ST2970
#define	__ST2970

#include <util/delay.h>
#include <avr/io.h>
#include "font.h"

#define __LCD8__

//#define _lcd_max_x		128
//#define _lcd_max_y		64

#define _lcd_cmd_ddr	DDRC
#define _lcd_cmd_pin	PINC
#define	_lcd_cmd_prt	PORTC

#define _lcd_dat_ddr	DDRD
#define _lcd_dat_pin	PIND
#define	_lcd_dat_prt	PORTD

#define _lcd_bit_rw		2
#define	_lcd_bit_rs		1
#define	_lcd_bit_e		3
#define	_lcd_bit_rst	5
#define _lcd_bit_psb	4

void lcd_wr(unsigned char dat, unsigned char cmd);
void lcd_wr_cmd(unsigned char dat);
void lcd_wr_dat(unsigned char dat);
uint8_t lcd_rd_dat(void);
void lcd_init(void);
void lcd_clear_gram(void);
void lcd_16x32_char(unsigned char x, unsigned char y, unsigned char n);
void lcd_16x32_clear(unsigned char x, unsigned char y);
void lcd_gram_pix(unsigned char x, unsigned char y, unsigned char p);

#endif
