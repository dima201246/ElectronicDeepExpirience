#ifndef EDE_H
#define EDE_H

	#include <avr/io.h>

	#define OUTPUT	1
	#define INPUT	0
	#define HIGH	255
	#define LOW		0
	#define TRUE	1
	#define FALSE	0

	#define set_bit(m,b)	((m) |= (1 << b))
	#define unset_bit(m,b)	((m) &= ~(1 << b))
	#define bit_seted(m,b)	((1 << b) == ((m) & (1 << b)))

	#define digitalWrite(m,b)	pinMode(m, b)

	// #if defined(__AVR_ATmega328__)
		#define F_CPU 16000000UL

		/*DIGITAL PINS*/
		#define INIT_DPIN0	&DDRD, 0
		#define INIT_DPIN1	&DDRD, 1
		#define INIT_DPIN2	&DDRD, 2
		#define INIT_DPIN3	&DDRD, 3
		#define INIT_DPIN4	&DDRD, 4
		#define INIT_DPIN5	&DDRD, 5
		#define INIT_DPIN6	&DDRD, 6
		#define INIT_DPIN7	&DDRD, 7
		#define INIT_DPIN8	&DDRB, 0
		#define INIT_DPIN9	&DDRB, 1
		#define INIT_DPIN10	&DDRB, 2
		#define INIT_DPIN11	&DDRB, 3
		#define INIT_DPIN12	&DDRB, 4
		#define INIT_DPIN13	&DDRB, 5

		#define DPIN0		&PORTD, 0
		#define DPIN1		&PORTD, 1
		#define DPIN2		&PORTD, 2
		#define DPIN3		&PORTD, 3
		#define DPIN4		&PORTD, 4
		#define DPIN5		&PORTD, 5
		#define DPIN6		&PORTD, 6
		#define DPIN7		&PORTD, 7
		#define DPIN8		&PORTB, 0
		#define DPIN9		&PORTB, 1
		#define DPIN10		&PORTB, 2
		#define DPIN11		&PORTB, 3
		#define DPIN12		&PORTB, 4
		#define DPIN13		&PORTB, 5

		/*ANALOG PINS*/
		#define INIT_APIN0	&DDRC, 0
		#define INIT_APIN1	&DDRC, 1
		#define INIT_APIN2	&DDRC, 2
		#define INIT_APIN3	&DDRC, 3
		#define INIT_APIN4	&DDRC, 4
		#define INIT_APIN5	&DDRC, 5
		#define INIT_APIN6	&DDRC, 6

		#define APIN0		&PORTC, 0
		#define APIN1		&PORTC, 1
		#define APIN2		&PORTC, 2
		#define APIN3		&PORTC, 3
		#define APIN4		&PORTC, 4
		#define APIN5		&PORTC, 5
		#define APIN6		&PORTC, 6
	// #endif

	void pinMode(volatile uint8_t *ioAddr, uint8_t pinNum, uint8_t pinMode) {
		if (pinMode) {
			*ioAddr |= (1 << pinNum);
		} else {
			*ioAddr &= ~(1 << pinNum);
		}
	}

	uint8_t digitalRead (volatile uint8_t *ioAddr, uint8_t pinNum) {
		return ((1 << pinNum) == ((*ioAddr) & (1 << pinNum)));
	}

#endif