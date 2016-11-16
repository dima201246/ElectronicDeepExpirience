#ifndef SLINE_H
#define SLINE_H
	/*	:DV  company (2016)
		DV Smart Line

		Протокол работает по одному проводу.
		Связываемые мк ОБЯЗАТЕЛЬНО должны иметь одинаковую частоту! В идеале, это должны быть два одинаковых мк.

		Время алгоритма на приём (без учёты тактов мк): 41 * SL_WAIT_TIME
		Время алгоритма на отдачу (без учёты тактов мк): 40 * SL_WAIT_TIME
	*/

	#include "EDE.h"

	#ifndef SL_PIN
	# warning Not setted SL_PIN, using PD2!
	#define SL_PIN	PD2
	#endif

	#if SL_PIN == PD2
		#define SL_INT	INT0
		#define SL_ISC0	ISC00
		#define SL_ISC1	ISC01
	#endif

	#if SL_PIN == PD3
		#define SL_INT	INT1
		#define SL_ISC0	ISC10
		#define SL_ISC1	ISC11
	#endif

	#define SL_VERSION					12

	#define SL_BYTE_COMMAND_AVAIBLE		0
	#define SL_BYTE_LINE_BUSY			1

	#define SL_WAIT_TIME				18

	volatile uint8_t 	SL_sys_byte		= 0,
						SL_com			= 0,
						SL_id_device	= 0;

	uint16_t			SL_data_byte	= 0;

	ISR(INT0_vect) {
		if (bit_seted(SL_sys_byte, SL_BYTE_LINE_BUSY))	// Не пытаться ничего принять, если линия занята (да, это обязательно)
			return;

		cli();										// Остановка прерываний

		uint8_t d_id	= 0,
				b_count	= 0,
				count	= 0;

		set_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Занять линию
		unset_bit(EIMSK, SL_INT);					// Выключение SL_INT
		unset_bit(EICRA, SL_ISC0);					// Отклю сение прерывания по повышению уровня на ножке
		unset_bit(EICRA, SL_ISC1);

		while (bit_seted(PIND, SL_PIN));			// Ожидание понижения уровня на передающем мк

		_delay_us(SL_WAIT_TIME);					// Сдвиговая задержка, чтобы чтение было на середине сигнала
 
		for (count	= 0; count < 4; count++) {
			for (b_count	= 0; b_count < 8; b_count++) {

				switch (count) {
					case 0:	if (bit_seted(PIND, SL_PIN)) set_bit(d_id, b_count);										break;	// Отправка id
					case 1:	if ((bit_seted(PIND, SL_PIN)) && (d_id == SL_id_device)) set_bit(SL_com, b_count);			break;	// Отправка команды
					case 2:	if ((bit_seted(PIND, SL_PIN)) && (d_id == SL_id_device)) set_bit(SL_data_byte, b_count);	break;	// Отправка 2-х байт данных
					case 3:	if ((bit_seted(PIND, SL_PIN)) && (d_id == SL_id_device)) set_bit(SL_data_byte, b_count);	break;	// -//-
				}

				_delay_us(SL_WAIT_TIME);
			}

			if ((count == 0) && (d_id == SL_id_device))
				SL_com	= SL_data_byte	= 0;		// Очистка переменных, есле это данные для этого мк

			if ((count == 2) && (d_id == SL_id_device))
				SL_data_byte = SL_data_byte << 8;	// Сдвиг битов, чтобы получить второй байт

			while (bit_seted(PIND, SL_PIN));		// Синхронизация
			_delay_us(SL_WAIT_TIME);				// Сдвиговая задержка, чтобы чтение было на середине сигнала
		}

		if (d_id == SL_id_device) {					// Если данные предназначались этому мк, то уведомить об этом
			set_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
		} else {
			unset_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
		}

		set_bit(EICRA, SL_ISC0);					// Установка прерывания по повышению уровня на ноге
		set_bit(EICRA, SL_ISC1);
		set_bit(EIMSK, SL_INT);						// Включение SL_INT
		sei();										// Активация прерываний
		unset_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Освобождение линии
	}

	uint8_t SLsend(uint8_t id, uint8_t command, uint16_t value) {
		if (bit_seted(SL_sys_byte, SL_BYTE_LINE_BUSY))	// Если линия занята - отменить передачу
			return FALSE;

		set_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Занять линию
		cli();										// Остановка прерываний
		unset_bit(EIMSK, SL_INT);					// Выключение SL_INT
		unset_bit(EICRA, SL_ISC0);					// Отклю сение прерывания по повышению уровня на ножке
		unset_bit(EICRA, SL_ISC1);

		uint8_t value2	= value >> 8,				// Взятие вторых 8 бит из данных
				b_count	= 0,
				count	= 0;

		set_bit(DDRD, SL_PIN);						// Вторая нога как выход
		set_bit(PORTD, SL_PIN);						// Установка бита синхронизации
		_delay_us(SL_WAIT_TIME * 2);				// Время на подготовку принимающего мк
		unset_bit(PORTD, SL_PIN);					// Понижение уровня

		for (count	= 0; count < 4; count++) {
			for (b_count	= 0; b_count < 8; b_count++) {
				switch (count) {
					case 0:	(bit_seted(id, b_count) ? set_bit(PORTD, SL_PIN) : unset_bit(PORTD, SL_PIN));								break;	// Отправка id
					case 1: if (SL_id_device != id) (bit_seted(command, b_count) ? set_bit(PORTD, SL_PIN) : unset_bit(PORTD, SL_PIN));	break;	// Отправка команды
					case 2: if (SL_id_device != id) (bit_seted(value2, b_count) ? set_bit(PORTD, SL_PIN) : unset_bit(PORTD, SL_PIN));	break;	// Отправка 2-х байт данных
					case 3: if (SL_id_device != id) (bit_seted(value, b_count) ? set_bit(PORTD, SL_PIN) : unset_bit(PORTD, SL_PIN));	break;	// -//-
				}

				_delay_us((SL_WAIT_TIME));
			}

			set_bit(PORTD, SL_PIN);					// Установка бита синхронизации
			_delay_us(SL_WAIT_TIME * 2);			// Время на подготовку принимающего мк
			unset_bit(PORTD, SL_PIN);				// Понижение уровня
		}

		unset_bit(PORTD, SL_PIN);					// Понижение уровня
		unset_bit(DDRD, SL_PIN);					// Нога как вход
		unset_bit(PORTD, SL_PIN);					// На всякий случай понизить уровень ещё раз (Да, блин! Это обязательно!)
		unset_bit(PIND, SL_PIN);					// Отключение высокого уровня на ноге, если это вход (Хватит задавать вопросы, это необходимо!!!)
		_delay_us((SL_WAIT_TIME));					// Ожидание понижение всех сигналов
		set_bit(EICRA, SL_ISC0);					// Установка прерывания по повышению уровня на ноге
		set_bit(EICRA, SL_ISC1);
		set_bit(EIMSK, SL_INT);						// Включение SL_INT
		sei();										// Активация прерываний
		unset_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Освобождение линии

		return TRUE;								// Передача успешно выполнена
	}

	void SLinit(uint8_t id) {
		unset_bit(DDRD, SL_PIN);

		SL_id_device	= id;
		set_bit(EICRA, SL_ISC0);					// Установка прерывания по повышению уровня на ноге
		set_bit(EICRA, SL_ISC1);
		set_bit(EIMSK, SL_INT);						// Включение SL_INT

		sei();										// Активация прерываний
	}

	uint8_t SLavaible() {
		if (bit_seted(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE)) {
			unset_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
			return TRUE;
		} else {
			return FALSE;
		}
	}

	uint8_t SLcommand() {
		return SL_com;
	}

	uint16_t SLdata() {
		return SL_data_byte;
	}
#endif