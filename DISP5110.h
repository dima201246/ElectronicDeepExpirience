#ifndef DISP5110_H
#define DISP5110_H

	volatile uint8_t fontEng[][6] =
	{
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },	// sp
		{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },	// !
		{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },	// "
		{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },	// #
		{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },	// $
		{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },	// %
		{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },	// &
		{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },	// '
		{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },	// (
		{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },	// )
		{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },	// *
		{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },	// +
		{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },	// ,
		{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },	// -
		{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },	// .
		{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },	// /
		{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },	// 0
		{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },	// 1
		{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },	// 2
		{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },	// 3
		{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },	// 4
		{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },	// 5
		{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },	// 6
		{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },	// 7
		{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },	// 8
		{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },	// 9
		{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },	// :
		{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },	// ;
		{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },	// <
		{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },	// =
		{ 0x00, 0x00, 0xFE, 0x7C, 0x38, 0x08 },	// >
		{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },	// ?
		{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },	// @
		{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },	// A
		{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },	// B
		{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },	// C
		{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },	// D
		{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },	// E
		{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },	// F
		{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },	// G
		{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },	// H
		{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },	// I
		{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },	// J
		{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },	// K
		{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },	// L
		{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },	// M
		{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },	// N
		{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },	// O
		{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },	// P
		{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },	// Q
		{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },	// R
		{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },	// S
		{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },	// T
		{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },	// U
		{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },	// V
		{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },	// W
		{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },	// X
		{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },	// Y
		{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },	// Z
		{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },	// [
		{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },	// 55
		{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },	// ]
		{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },	// ^
		{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },	// _
		{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },	// '
		{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },	// a
		{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },	// b
		{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },	// c
		{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },	// d
		{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },	// e
		{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },	// f
		{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },	// g
		{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },	// h
		{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },	// i
		{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },	// j
		{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },	// k
		{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },	// l
		{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },	// m
		{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },	// n
		{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },	// o
		{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },	// p
		{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },	// q
		{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },	// r
		{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },	// s
		{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },	// t
		{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },	// u
		{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },	// v
		{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },	// w
		{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },	// x
		{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },	// y
		{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },	// z
		{ 0x3C, 0x42, 0x42, 0x4A, 0x1C, 0x08 },	// Turnovers icon
		{ 0x70, 0x8E, 0x81, 0x81, 0x8E, 0x70 }, // Termometr icon
		{ 0xFF, 0xF9, 0xF9, 0xC9, 0xFF, 0x1C }	// Fuel icon
	};

	// #ifdef LANG_RUSSIA
/*	volatile uint8_t fontRus[][6] =
	{
		{ 0x7F, 0x01, 0x01, 0x01, 0x7F, 0x00 },	// П
		{ 0x60, 0x3E, 0x21, 0x21, 0x3F, 0x60 },	// Д
		{ 0x7F, 0x01, 0x01, 0x01, 0x01, 0x00 },	// Г
		{ 0x27, 0x48, 0x48, 0x48, 0x3F, 0x00 },	// У
		{ 0x40, 0x3E, 0x01, 0x01, 0x7F, 0x00 },	// Л
		{ 0x7F, 0x49, 0x49, 0x49, 0x31, 0x00 }	// Б
	};*/
	// #endif

	#include "EDE.h"

	void LCD_write_byte(uint8_t dat, uint8_t command)
	{
		if (command == 1)
			digitalWrite(PIN_LCD_DC, LOW);
		else
			digitalWrite(PIN_LCD_DC, HIGH);

		for(uint8_t i = 0; i < 8; i++)
		{
			if (dat & 0x80)
				digitalWrite(PIN_LCD_DIN, HIGH);
			else
				digitalWrite(PIN_LCD_DIN, LOW);

			digitalWrite(PIN_LCD_SCLK, LOW);
			dat = dat << 1;
			digitalWrite(PIN_LCD_SCLK, HIGH);
		}
	}

	void LCD_clear() 
	{
		LCD_write_byte(0x0c, 1);
		LCD_write_byte(0x80, 1);

		for (uint16_t i = 0; i < 504; i++)
			LCD_write_byte(0, 0);
	}

	void LCD_init() 
	{
		pinMode(INIT_PIN_LCD_RST, OUTPUT);
		pinMode(INIT_PIN_LCD_DC, OUTPUT);
		pinMode(INIT_PIN_LCD_DIN, OUTPUT);
		pinMode(INIT_PIN_LCD_SCLK, OUTPUT);

		digitalWrite(PIN_LCD_RST, LOW);
		_delay_us(1);
		digitalWrite(PIN_LCD_RST, HIGH);
		_delay_us(1);
	 
		LCD_write_byte(0x21, 1);	// Установка LCD режима
		LCD_write_byte(0xc8, 1);	// set bias voltage
		LCD_write_byte(0x60, 1);	// Установка контрастности
		LCD_write_byte(0x13, 1);	// 1:48
		LCD_write_byte(0x20, 1);	// use bias command, vertical
		LCD_write_byte(0x0c, 1);	// set LCD mode,display normally
		LCD_clear();
	}

	void LCD_write_char(uint8_t c)
	{
		c -= 32;

		for (uint8_t line = 0; line <= 6; line++)
			LCD_write_byte(fontEng[c][line], 0);
	}

	void LCD_set_XY(uint8_t X, uint8_t Y)
	{
		LCD_write_byte(0x40 | Y, 1);	// column
		LCD_write_byte(0x80 | X, 1);	// row
	}

	void LCD_print(uint8_t X, uint8_t Y, uint8_t *s)
	{
		LCD_set_XY(X,Y);

		while (*s)
		{
			LCD_write_char(*s);

			s++;
		}
	}

	void LCD_print_int(uint8_t X, uint8_t Y, int16_t s)
	{
		LCD_set_XY(X,Y);

		while (s != 0)
		{

			for (uint8_t line = 0; line <= 6; line++)
				LCD_write_byte(fontEng[((s % 10) + 16)][line], 0);
			s /= 10;
		}
	}
#endif