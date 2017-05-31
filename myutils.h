/*
 * myutils.h

 *
 *  Created on: 25.10.2016
 *      Author: viktor96
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef MYUTILS_H
#define MYUTILS_H 1

#define HIGH	1
#define LOW		0

#define setbit(byte, bit)		(byte) |= _BV(bit)
#define clearbit(byte, bit)		(byte) &= ~_BV(bit)
#define bitseted(byte, bit)		(byte) & (_BV(bit))

/*
 * Макросы для работы с масками битов.
 * setmask   - 	установка маски битов в значение 1
 * unsetmask - 	установка маски битов в значение 0
 * maskseted - 	вернёт истинное логическое значение если
 * 				маска выставлена в точности значению mask
 */
#define setmask(byte, mask)		(byte) |= (mask)
#define clearmask(byte, mask)	(byte) &= ~(mask)
#define maskseted(byte, mask)	(mask) == ((byte) & (mask))

/* Ожидание нажатия кнопки
 * port - адрес порта
 * pin - номер пина
 * front - по какому фронту идёт ожидание: HIGH (1) - по высокому, LOW (0) - по низкому
 */
void waitForPress(volatile uint8_t *port, uint8_t pin, uint8_t front);

/* Проверка нажатия кнопки без ожидания
 * port - адрес порта
 * pin - номер пина
 * front - по какому фронту идёт ожидание: HIGH (1) - по высокому, LOW (0) - по низкому
 */
uint8_t buttonPressed(volatile uint8_t *port, uint8_t pin, uint8_t front);

/*
 * Циклический сдвиг влево на sh бит
 */
void lShCycl(uint8_t *num, uint8_t sh);
/*
 * Циклический сдвиг вправо на sh бит
 */
void rShCycl(uint8_t *num, uint8_t sh);

#endif /* MYUTILS_H */
