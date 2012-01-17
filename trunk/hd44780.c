#include "hd44780.h"

void lcd_init(void){
	sbi(_lcd_cmd_ddr, _lcd_bit_rw);
	sbi(_lcd_cmd_ddr, _lcd_bit_rs);
	sbi(_lcd_cmd_ddr, _lcd_bit_e);
	cbi(_lcd_cmd_prt, _lcd_bit_rw);
	cbi(_lcd_cmd_prt, _lcd_bit_rs);
	cbi(_lcd_cmd_prt, _lcd_bit_e);

	// 8-pin LCD connect mode
	#ifdef __LCD8__
		lcd_wr_cmd(0b00111000);	// 8 bit / 2 lines
	#else
	// 4-pin LCD connect mode
	#ifdef __LCD4__
		lcd_wr_cmd(0b00101000);	// 4 bit / 2 lines
	#else
		// no connection mode selected, so die
		#error "Define __LCD8__ or __LCD4__ connection mode."
	#endif
	#endif
	lcd_wr_cmd(0b00000001);	// clear display
	lcd_wr_cmd(0b00000110);	// forward / no scrolling
	lcd_wr_cmd(0b00001100);	// display on
}

void lcd_wait(void){
	uint8_t pin = 0x00;

	while ( chb(pin, 7) ){
		_lcd_dat_ddr = 0x00;
		_lcd_dat_prt = 0xff;

		sbi(_lcd_cmd_prt, _lcd_bit_rw);
		cbi(_lcd_cmd_prt, _lcd_bit_rs);
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		_delay_ms(10);
		pin = _lcd_dat_pin;
		sbi(_lcd_cmd_prt, _lcd_bit_e);
	}
}

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
*	Proceed sending
*/
void lcd_wr(unsigned char dat, unsigned char cmd){

	// 8-pin LCD connect mode
	#ifdef __LCD8__

		// prepare port
		_lcd_dat_ddr = 0xff;

		// prepare command bits
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		cbi(_lcd_cmd_prt, _lcd_bit_rw);
		if (cmd == 1){ cbi(_lcd_cmd_prt, _lcd_bit_rs); } else {	sbi(_lcd_cmd_prt, _lcd_bit_rs); }

		// prepare data
		_lcd_dat_prt = dat;
		_delay_ms(10);
		// commit transmission
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		lcd_wait();

	#else
	// 4-pin LCD connect mode
	#ifdef __LCD4__

		// prepare port
		_lcd_dat_ddr = (_lcd_dat_ddr & 0b11100001) | 0b00011110;

		// prepare command bits
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		cbi(_lcd_cmd_prt, _lcd_bit_rw);
		if (cmd == 1){ cbi(_lcd_cmd_prt, _lcd_bit_rs); } else {	sbi(_lcd_cmd_prt, _lcd_bit_rs); }

		// prepare 1-st part of data byte
		_delay_ms(100);
		_lcd_dat_prt = (_lcd_dat_prt & 0b11100001) | ( (dat >> 3) & 0b00011110 );
		
		// commit transmission
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		//lcd_wait();
		_delay_ms(100);

		// prepare command bits
		sbi(_lcd_cmd_prt, _lcd_bit_e);
		_delay_ms(100);
		// prepare 2-nd part of data byte
		_lcd_dat_prt = (_lcd_dat_prt & 0b11100001) | ( (dat << 1) & 0b00011110);

		// commit transmission
		cbi(_lcd_cmd_prt, _lcd_bit_e);
		//lcd_wait();

	#else
		// no connection mode selected, so die
		#error "Define __LCD8__ or __LCD4__ connection mode."
	#endif
	#endif
}
