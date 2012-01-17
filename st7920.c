#include "st7920.h"
#include "base.h"

/*
*	Send a command
*/
void lcd_wr_cmd(unsigned char dat){
	lcd_wr(dat, 1);
}

/*
*	Send data
*/
void lcd_wr_dat(unsigned char dat){
	lcd_wr(dat, 0);
}

/*
*	Read data
*
*	Do not forget about dummy read!
*/
uint8_t lcd_rd_dat(void){
	uint8_t ret;

	// prepare port
	_lcd_dat_ddr = 0x00;
	_lcd_dat_prt = 0x00; // Hi-Z mode set

	// prepare command bits
//	cbi(_lcd_cmd_prt, _lcd_bit_e);
	sbi(_lcd_cmd_prt, _lcd_bit_rw);
	sbi(_lcd_cmd_prt, _lcd_bit_rs);
	
	// commit transmission
	sbi(_lcd_cmd_prt, _lcd_bit_e);
	_delay_us(100);
	ret = _lcd_dat_pin;
	cbi(_lcd_cmd_prt, _lcd_bit_e);
	_delay_us(10);

	return ret;
}

/*
*	Proceed sending
*/
void lcd_wr(unsigned char dat, unsigned char cmd){

	// 8-pin LCD connect mode
	#ifdef __LCD8__

		// prepare port
		_lcd_dat_ddr = 0xff;

		// prepare command bits
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		cbi(_lcd_cmd_prt, _lcd_bit_rw);
		if (cmd == 1){ cbi(_lcd_cmd_prt, _lcd_bit_rs); } else {	sbi(_lcd_cmd_prt, _lcd_bit_rs); }

		// prepare data
		_lcd_dat_prt = dat;

		// commit transmission
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		_delay_us(100);

	#else
	// 4-pin LCD connect mode
	#ifdef __LCD4__

		// prepare port
		_lcd_dat_ddr = (_lcd_dat_ddr & 0b11100001) | 0b00011110;

		// prepare command bits
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		cbi(_lcd_cmd_prt, _lcd_bit_rw);
		if (cmd == 1){ cbi(_lcd_cmd_prt, _lcd_bit_rs); } else {	sbi(_lcd_cmd_prt, _lcd_bit_rs); }

		// prepare 1-st part of data byte
		_lcd_dat_prt = (_lcd_dat_prt & 0b11100001) | ( (dat >> 3) & 0b00011110 );

		// commit transmission
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		_delay_us(100);

		// prepare command bits
		cbi(_lcd_cmd_prt, _lcd_bit_e);

		// prepare 2-nd part of data byte
		_lcd_dat_prt = (_lcd_dat_prt & 0b11100001) | ( (dat << 1) & 0b00011110);

		// commit transmission
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		_delay_us(100);

	#else
		// no connection mode selected, so die
		#error "Define __LCD8__ or __LCD4__ connection mode."
	#endif
	#endif
}

void lcd_init(void){

	sbi(_lcd_cmd_ddr, _lcd_bit_rw);
	sbi(_lcd_cmd_ddr, _lcd_bit_rs);
	sbi(_lcd_cmd_ddr, _lcd_bit_e);
	sbi(_lcd_cmd_ddr, _lcd_bit_rst);
	sbi(_lcd_cmd_ddr, _lcd_bit_psb);
	cbi(_lcd_cmd_prt, _lcd_bit_rw);
	cbi(_lcd_cmd_prt, _lcd_bit_rs);
	cbi(_lcd_cmd_prt, _lcd_bit_e);
	cbi(_lcd_cmd_prt, _lcd_bit_rst);
	cbi(_lcd_cmd_prt, _lcd_bit_psb);

	_delay_ms(50);
	sbi(_lcd_cmd_prt, _lcd_bit_rst);
	sbi(_lcd_cmd_prt, _lcd_bit_psb);

	// 8-pin LCD connect mode
	#ifdef __LCD8__
		lcd_wr_cmd(0b00110000);
		_delay_us(800);
		lcd_wr_cmd(0b00110110);
	#else
	// 4-pin LCD connect mode
	#ifdef __LCD4__
		lcd_wr_cmd(0b00100000);
		_delay_us(800);
		lcd_wr_cmd(0b00100110);
	#else
		// no connection mode selected, so die
		#error "Define __LCD8__ or __LCD4__ connection mode."
	#endif
	#endif

	_delay_us(800);
	lcd_wr_cmd(0b00000001);
	_delay_us(800);
	lcd_wr_cmd(0b00001100);
	_delay_us(800);
	lcd_wr_cmd(0b00000010);
	_delay_us(800);

}

void lcd_16x32_char(unsigned char x, unsigned char y, unsigned char n){
	for (unsigned char i = 0; i < 32; i++){
		lcd_wr_cmd(0b10000000 + y + i);
		lcd_wr_cmd(0b10000000 + x);
		
		if ( (i == 0) || (i == 31) ){
			lcd_wr_dat(0x00);
			lcd_wr_dat(0x00);
		} else {
			lcd_wr_dat(font_16x30[n][i-1][0]);
			lcd_wr_dat(font_16x30[n][i-1][1]);
		}
	}

}

void lcd_clear_gram(void){
	for (unsigned char i = 0; i < 32; i++){
		for (unsigned char j = 0; j < 16; j++){
			lcd_wr_cmd(0b10000000 + i);
			lcd_wr_cmd(0b10000000 + j);

			lcd_wr_dat(0x00);
			lcd_wr_dat(0x00);
		}
	}
}

void lcd_16x32_clear(unsigned char x, unsigned char y){
	for (unsigned char i = 0; i < 32; i++){
		lcd_wr_cmd(0b10000000 + y + i);
		lcd_wr_cmd(0b10000000 + x);
		
		lcd_wr_dat(0x00);
		lcd_wr_dat(0x00);
	}
}

void lcd_gram_pix(unsigned char x, unsigned char y, unsigned char p){
	unsigned char xx, yy, wx;
	uint8_t b0, b1;

	wx = x / 16;
	xx = x - (wx*16);

	if (y >= 32){
		yy = y - 32;
		wx = wx + 8;
	} else {
		yy = y;
	}

	lcd_wr_cmd(0b10000000 + yy);
	lcd_wr_cmd(0b10000000 + wx);

	lcd_rd_dat();
	b0 = lcd_rd_dat();
	b1 = lcd_rd_dat();

	lcd_wr_cmd(0b10000000 + yy);
	lcd_wr_cmd(0b10000000 + wx);

	if (xx < 8){
		sbi(b0, 7-xx);
	} else {
		sbi(b1, 7 - (xx-8) );
	}

	lcd_wr_dat(b0);
	lcd_wr_dat(b1);
}
