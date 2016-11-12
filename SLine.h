#ifndef SLINE_H
#define SLINE_H
	/*	:DV  company (2016)
		DV Smart Line

		Протокол работает по одному проводу.
		Связываемые мк ОБЯЗАТЕЛЬНО должны иметь одинаковую частоту! В идеале, это должны быть два одинаковых мк.

		Время алгоритма на приём/отдачу (без учёты тактов мк): 42 * SL_WAIT_TIME
	*/

	#include "EDE.h"

	#define SL_VERSION					1.0

	#define SL_BYTE_COMMAND_AVAIBLE		(1 << 0)
	#define SL_BYTE_LINE_BUSY			(1 << 1)
	// #define SL_BYTE_LINE_ERROR			(1 << 2)

	#define SL_WAIT_TIME				20

	volatile uint8_t 	SL_sys_byte		= 0,
						SL_com			= 0,
						SL_id_device	= 0;

	uint16_t			SL_2_byte		= 0;

	ISR(INT0_vect) {
		if (bit_seted(SL_sys_byte, SL_BYTE_LINE_BUSY))	// Не пытаться ничего принять, если линия занята (да, это обязательно)
			return;

		cli();										// Остановка прерываний

		uint8_t d_id	= 0,
				b_count	= 0,
				count	= 0;

		set_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Занять линию
		unset_bit(EIMSK, INT0);						// Выключение INT0
		unset_bit(EICRA, ISC01);					// Отклю сение прерывания по повышению уровня на ножке
		unset_bit(EICRA, ISC00);

		while (PIN_Read(DPIN2));					// Ожидание понижения уровня на передающем мк

		delay_u(SL_WAIT_TIME);						// Для соблюдения баланса сил в природе (!!!НЕ ТРОГАТЬ!!!)

		for (count	= 0; count < 4; count++) {
			for (b_count	= 0; b_count < 8; b_count++) {

				switch (count) {
					case 0:	if (PIN_Read(DPIN2)) set_bit(d_id, b_count);		break;	// Отправка id
					case 1:	if ((PIN_Read(DPIN2)) && (d_id == SL_id_device)) set_bit(SL_com, b_count);		break;	// Отправка команды
					case 2:	if ((PIN_Read(DPIN2)) && (d_id == SL_id_device)) set_bit(SL_2_byte, b_count);	break;	// Отправка 2-х байт данных
					case 3:	if ((PIN_Read(DPIN2)) && (d_id == SL_id_device)) set_bit(SL_2_byte, b_count);	break;	// -//-
				}

				delay_u(SL_WAIT_TIME);
			}

			if ((count == 2) && (d_id == SL_id_device))
				SL_2_byte = SL_2_byte << 8;			// Сдвиг битов, чтобы получить второй байт

			if ((count == 0) && (d_id == SL_id_device))
				SL_com	= SL_2_byte	= 0;			// Очистка переменных, есле это данные для этого мк

			while (PIN_Read(DPIN2));				// Синхронизация
			delay_u(SL_WAIT_TIME);					// Для соблюдения баланса сил в природе (!!!НЕ ТРОГАТЬ!!!)
		}

		if (d_id == SL_id_device) {					// Если данные предназначались этому мк, то уведомить об этом
			set_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
		} else {
			unset_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
		}

		set_bit(EICRA, ISC01);						// Установка прерывания по повышению уровня на ноге
		set_bit(EICRA, ISC00);
		set_bit(EIMSK, INT0);						// Выключение INT0
		sei();										// Активация прерываний
		unset_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Освобождение линии
	}

	uint8_t SLsend(uint8_t id, uint8_t command, uint16_t value) {
		if (bit_seted(SL_sys_byte, SL_BYTE_LINE_BUSY))	// Если линия занята - отменить передачу
			return FALSE;

		set_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Занять линию
		cli();										// Остановка прерываний
		unset_bit(EIMSK, INT0);						// Выключение INT0
		unset_bit(EICRA, ISC01);					// Отклю сение прерывания по повышению уровня на ножке
		unset_bit(EICRA, ISC00);

		uint8_t value2	= value >> 8,				// Взятие вторых 8 бит из данных
				b_count	= 0,
				count	= 0;

		pinMode(INIT_DPIN2, OUTPUT);				// Вторая нога как выход
		digitalWrite(DPIN2, HIGH);					// Установка бита синхронизации
		delay_u(SL_WAIT_TIME * 2);					// Время на отзыв второго мк
		digitalWrite(DPIN2, LOW);					// Понижение уровня (Да! Я знаю, что тут, по идее, должно быть ещё задержка, но если она есть, то теряется первый бит!...
													// ... Просто помни - "Программирование - это магия")
		for (count	= 0; count < 4; count++) {
			for (b_count	= 0; b_count < 8; b_count++) {
				switch (count) {
					case 0:	digitalWrite(DPIN2, bit_seted(id, b_count));								break;	// Отправка id
					case 1: if (SL_id_device != id) digitalWrite(DPIN2, bit_seted(command, b_count));	break;	// Отправка команды
					case 2: if (SL_id_device != id) digitalWrite(DPIN2, bit_seted(value2, b_count));	break;	// Отправка 2-х байт данных
					case 3: if (SL_id_device != id) digitalWrite(DPIN2, bit_seted(value, b_count));		break;	// -//-
				}

				delay_u((SL_WAIT_TIME));
			}

			digitalWrite(DPIN2, HIGH);				// Установка бита синхронизации
			delay_u(SL_WAIT_TIME * 2);				// Время на отзыв второго мк
			digitalWrite(DPIN2, LOW);				// Понижение уровня
		}

		digitalWrite(DPIN2, LOW);					// Понижение уровня
		pinMode(INIT_DPIN2, INPUT);					// Нога как вход
		digitalWrite(DPIN2, LOW);					// На всякий случай понизить уровень ещё раз (Да, блин! Это обязательно!)
		unset_bit(PIND, PD2);						// Отключение высокого уровня на ноге, если это вход (Хватит задавать вопросы, это необходимо!!!)
		delay_u((SL_WAIT_TIME));					// Ожидание понижение всех сигналов
		set_bit(EICRA, ISC01);						// Установка прерывания по повышению уровня на ноге
		set_bit(EICRA, ISC00);
		set_bit(EIMSK, INT0);						// Выключение INT0
		sei();										// Активация прерываний
		unset_bit(SL_sys_byte, SL_BYTE_LINE_BUSY);	// Освобождение линии

		return TRUE;								// Передача успешно выполнена
	}

	void SLinit(uint8_t id) {
		pinMode(INIT_DPIN2, INPUT);

		SL_id_device	= id;
		EICRA			|= (1 << ISC01) | (1 << ISC00);	// Прерывание по повышению уровня на ножке
		EIMSK			|= (1 << INT0);					// Включение INT0

		sei();											// Активация прерываний
	}

	uint8_t SLavaible() {
		if (bit_seted(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE)) {
			unset_bit(SL_sys_byte, SL_BYTE_COMMAND_AVAIBLE);
			return TRUE;
		} else {
			return FALSE;
		}
	}

	uint8_t SL_command() {
		return SL_com;
	}

	uint8_t SL_data() {
		return SL_2_byte;
	}
#endif