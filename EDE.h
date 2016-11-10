#ifndef EDE_H
#define EDE_H

	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>
	#include <avr/eeprom.h>

	#define OUTPUT	1
	#define INPUT	0
	#define HIGH	255
	#define LOW		0
	#define TRUE	1
	#define FALSE	0

	#define set_bit(m,b)	((m) |= (1 << b))
	#define unset_bit(m,b)	((m) &= ~(1 << b))
	#define bit_seted(m,b)	((m) & (1 << b))

	#define digitalWrite(m,b)	pinMode(m, b)
	#define delay_u(d)			_delay_us(d)
	#define delay(d)			_delay_ms(d)
	#define delay_s(d)			_delay_ms(d * 1000)

	// #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
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
		#define INIT_APIN6	&DDRC, 7

		#define APIN0		&PORTC, 0
		#define APIN1		&PORTC, 1
		#define APIN2		&PORTC, 2
		#define APIN3		&PORTC, 3
		#define APIN4		&PORTC, 4
		#define APIN5		&PORTC, 5
		#define APIN6		&PORTC, 6
		#define APIN6		&PORTC, 7
	// #endif

	void pinMode(volatile uint8_t *ioAddr, uint8_t pinNum, uint8_t pinMode) {
		if (pinMode) {
			*ioAddr	|= (1 << pinNum);
		} else {
			*ioAddr	&= ~(1 << pinNum);
		}
	}

	uint8_t digitalRead (volatile uint8_t *ioAddr, uint8_t pinNum) {
		if (ioAddr != &PORTC)
			return ((*ioAddr) & (1 << pinNum));
		else
			return ((PINC) & (1 << pinNum));
	}

	/*ШИМ Начало*/
	void analogWrite(volatile uint8_t *ioAddr, uint8_t pinNum, uint8_t value) {
		// unset_bit(*ioAddr, pinNum);

		if (((ioAddr == &PORTD) && (pinNum == 6)) || ((ioAddr == &PORTD) && (pinNum == 5))) {	// 5 и 6 цифровые пины
			if (pinNum == 6) {
				OCR0A	= value;								// Установка ШИМ
				TCCR0A	|= ((value == 0 ? 0 : 1) << COM0A1);	// Неинвертирующий режим
			}

			if (pinNum == 5) {
				OCR0B	= value;								// Установка ШИМ
				TCCR0A	|= ((value == 0 ? 0 : 1) << COM0B1);	// Неинвертирующий режим
			}

			if ((!bit_seted(TCCR0A, COM0A1)) && (!bit_seted(TCCR0A, COM0B1))) {	// Отключение таймера, если два пина в нуле
				TCCR0A	|= (0 << WGM00) | (0 << WGM01) ;
				TCCR0B	|= (0 << CS00) | (0 << CS01) | (0 << CS02);				
			} else {
				TCCR0A	|= (1 << WGM01) | (1 << WGM00);			// Режим fast PWM
				TCCR0B	|= (1 << CS01);							// Пределитель на 8
			}
		} else if (((ioAddr == &PORTB) && (pinNum == 1)) || ((ioAddr == &PORTB) && (pinNum == 2))) {	// 9 и 10 цифровые пины
			if (pinNum == 1) {
				OCR1A	= value;								// Установка ШИМ
				TCCR1A	|= ((value == 0 ? 0 : 1) << COM1A1);	// Неинвертирующий режим
			}

			if (pinNum == 2) {
				OCR1B	= value;								// Установка ШИМ
				TCCR1A	|= ((value == 0 ? 0 : 1) << COM1B1);	// Неинвертирующий режим
			}

			if ((!bit_seted(TCCR1A, COM1A1)) && (!bit_seted(TCCR1A, COM1B1))) {	// Отключение таймера, если два пина в нуле
				TCCR1A	|= (0 << WGM10) | (0 << WGM11) ;
				TCCR1B	|= (0 << CS10) | (0 << CS11) | (0 << CS12);				
			} else {
				TCCR1A	|= (1 << WGM11) | (1 << WGM10);			// Режим fast PWM
				TCCR1B	|= (1 << CS11);							// Пределитель на 8
			}
		} else if (((ioAddr == &PORTB) && (pinNum == 3)) || ((ioAddr == &PORTD) && (pinNum == 3))) {	// 3 и 12 цифровые пины
			if (ioAddr == &PORTB) {
				OCR2A	= value;								// Установка ШИМ
				TCCR2A	|= ((value == 0 ? 0 : 1) << COM2A1);	// Неинвертирующий режим
			}

			if (ioAddr == &PORTD) {
				OCR2B	= value;								// Установка ШИМ
				TCCR2A	|= ((value == 0 ? 0 : 1) << COM2B1);	// Неинвертирующий режим
			}

			if ((!bit_seted(TCCR2A, COM2A1)) && (!bit_seted(TCCR2A, COM2B1))) {	// Отключение таймера, если два пина в нуле
				TCCR2A	|= (0 << WGM20) | (0 << WGM21) ;
				TCCR2B	|= (0 << CS20) | (0 << CS21) | (0 << CS22);				
			} else {
				TCCR2A	|= (1 << WGM21) | (1 << WGM20);			// Режим fast PWM
				TCCR2B	|= (1 << CS21);							// Пределитель на 8
			}
		}
	}
	/*ШИМ Конец*/

	/*АЦП Начало*/
	void ADC_Init(void) {
		ADCSRA	|= (1 << ADEN)					// Включаем АЦП
				| (1 << ADPS1) | (1 << ADPS0);	// Пределитель на 8
		ADMUX	|= (1 << REFS0) | (0 << REFS1);	// Опорное напряжение VCC
	}

	uint16_t analogRead(volatile uint8_t *ioAddr, uint8_t pinNum) {
		ADMUX	|= pinNum << 4;					// С какого порта снимать значения
		ADCSRA	|= (1 << ADSC);					// Начинаем преобразование 

		while (!bit_seted(ADCSRA, ADIF));		// Пока не будет окончано преобразоване
 
		return (ADCL | ADCH << 8);
	}
	/*АЦП Конец*/

	/*EEPROM Начало*/
	#define WriteDataEEPROM(iD, ba)	WriteDataEEPROM_SizedT(iD, ba, sizeof(*iD));
	#define ReadDataEEPROM(iD, ba)	ReadDataEEPROM_Sized(iD, ba, sizeof(*iD));

	void WriteDataEEPROM_Sized(void *iData, uint8_t byte_adress, uint8_t DataSize) {
		uint8_t		*byte_Data	= (uint8_t *) iData,
					block		= 0;

		for(block	= 0; block < DataSize; block++) {
			eeprom_write_byte(block + byte_adress, *byte_Data);
			byte_Data++;
		}
	}

	void ReadDataEEPROM_Sized(void *iData, uint8_t byte_adress, uint8_t DataSize) {
		uint8_t		*byte_Data	= (uint8_t *) iData,
					block		= 0;

		for(block	= 0; block < DataSize; block++) {
			*byte_Data = eeprom_read_byte(block + byte_adress);
			byte_Data++;
		}
	}
	/*EEPROM Конец*/
#endif