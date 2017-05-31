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

uint8_t buff[4];

void read_buff()
{
	int i = 0;
	for (; i < sizeof(buff); ++i)
	{
		USART_TransmitHexNum(buff[i]);
		USART_Transmit('\n');
	}
}

void clock_recv(uint8_t addr, uint8_t *data, uint8_t size, twi_onaction_t hdl)
{
	/* load slave address */
	twi_setsla(CLOCK_SLA);

	twi_set_txbuff(&addr, 1);
	twi_set_rxbuff(data, size);
	twi_set_on_action(read_buff);

	enum twi_action task[] = { SLA_W, DT_1, SR, SLA_R, DR_N, ON_ACT };

	twi_startaction(task, sizeof(task));
}

int main()
{

    USART_Setup();
    twi_init();

    sei();

    uint8_t data[4] = {15, 16, 17, 18};

	while(1)
	{
		//twi_send(CLOCK_SLA, data, 4);
		clock_recv(0x00, buff, sizeof(buff), read_buff);
		_delay_ms(3000);
	}
}
