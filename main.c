/*
 * main.c
 *
 *  Created on: 27.05.2017
 *      Author: viktor96
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <ctype.h>

#define DEBUG

#include "myutils.h"
#include "twi_complete.h"

#define false 0
#define true 1

#define CLOCK_SLA 0b1101000

int main()
{

    USART_Setup();
    twi_init();

    sei();

    uint8_t data[1] = {15};

	while(1)
	{
		twi_send(CLOCK_SLA, data, 1);
		_delay_ms(3000);
	}
}
