/*
*	1-wire.h
*
*	Последнее изменение:	31.12.11 г.
*
*	Библиотека для работы с устройствами по протоколу 1-wire.
*
*	Serge Ivanov <asmi1470@gmail.com>
*/

#ifndef __1wire
#define __1wire

#include <util/delay.h>
#include <avr/io.h>
#include "base.h"

// назначаем порт, к которому подключена шина
#define _1w_ddr	DDRD
#define _1w_pin	PIND
#define	_1w_prt	PORTD

// назначаем ножку, к которой подключена шина
#define _1w_dq_byt		3

// максимальное количество устройств на шине
#define _1w_max_dev		2

// массив для хранения индивидуальных номеров устройств
volatile static uint8_t _1w_dev[_1w_max_dev][8];

/*
*	Заголовки функций
*/
unsigned char ow_rst(void);
void ow_wrt_l(void);
void ow_wrt_h(void);
unsigned char ow_read(void);
void ow_snd_dat(uint8_t dat);
uint8_t ow_get_byt(void);
void ow_scan_bus(void);
void ow_match_rom(unsigned char num);

#endif
