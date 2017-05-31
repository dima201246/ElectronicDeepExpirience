/*
 * myutils.c
 *
 *  Created on: 08.05.2017
 *      Author: viktor96
 */
#include "myutils.h"

void waitForPress(volatile uint8_t *port, uint8_t pin, uint8_t front)
{
	while( !( (bitseted(*port, pin)) ^ front ) );	  	// ждём 0 (f = 0) или 1 (f = 1)
	while( ( (bitseted(*port, pin)) ^ front ) );		// ждём 1 (f = 0) или 0 (f = 1)
	_delay_ms(30);
}

/* Проверка нажатия кнопки без ожидания
 * port - адрес порта
 * pin - номер пина
 * front - по какому фронту идёт ожидание: HIGH (1) - по высокому, LOW (0) - по низкому
 */
uint8_t buttonPressed(volatile uint8_t *port, uint8_t pin, uint8_t front)
{
	if( !( (bitseted(*port, pin)) ^ front ) )  {// ждём 0 (f = 0) или 1 (f = 1)
		_delay_ms(30);
		if( !( (bitseted(*port, pin)) ^ front ) )  return 1;
	}

	return 0;
}

/*
 * Циклический сдвиг влево на sh бит
 */
void lShCycl(uint8_t *num, uint8_t sh)
{
	sh %= 8;
	uint8_t topmask = ((*num & 0xFF) >> (8 - sh));
	*num <<= sh;
	setmask(*num, topmask);
}

/*
 * Циклический сдвиг вправо на sh бит
 */
void rShCycl(uint8_t *num, uint8_t sh)
{
	sh %= 8;
	uint8_t topmask = ((*num & 0xFF) << (8 - sh));
	*num >>= sh;
	setmask(*num, topmask);
}
