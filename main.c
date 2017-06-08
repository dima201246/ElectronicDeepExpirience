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

uint8_t clock_reg[4];
uint8_t reg_addr;

void read_buff()
{
	uint8_t i = 3;
	for (; i > 0; --i)
	{
		USART_Transmit(((clock_reg[i] & 0x70) >> 4)+'0');
		USART_Transmit((clock_reg[i] & 0x0F)+'0');
		USART_Transmit(' ');
	}

	USART_Transmit('\n');
}

uint8_t clock_recv(uint8_t *data, uint8_t size, twi_onaction_t hdl)
{
	/* load slave address */
	twi_setsla(CLOCK_SLA);

	twi_set_txbuff(&reg_addr, 1);
	twi_set_rxbuff(data, size);
	twi_set_on_action(hdl);

	enum twi_action task[] = { SLA_W, DT_1, SR, SLA_R, DR_N, ON_ACT };

	return twi_startaction(task, sizeof(task));
}

uint8_t clock_send(uint8_t *data, uint8_t size)
{
	twi_setsla(CLOCK_SLA);
	twi_set_txbuff(data, size);
	twi_set_rxbuff(NULL, 0);

	enum twi_action task[] = { SLA_W, DT_N };

	return twi_startaction(task, sizeof(task));
}

uint8_t clock_send_wait(uint8_t *data, uint8_t size)
{
	uint8_t status = clock_send(data, size);
	if (!status)
		twi_actionwait();

	return status;
}

int main()
{

    USART_Setup();
    twi_init();

    sei();

    uint8_t settings_on[2] = {0x00, 0x80};
	clock_send_wait(settings_on, sizeof(settings_on));
	uint8_t settings[4] = {0x00, 0x80, 0x50, 0x19};
	clock_send_wait(settings, sizeof(settings));
	uint8_t settings_off[2] = {0x00, 0x00};
	clock_send_wait(settings_off, sizeof(settings_off));

	while(1)
	{
		//twi_send(CLOCK_SLA, data, 4);
		clock_recv(clock_reg, sizeof(clock_reg), read_buff);
		_delay_ms(100);
	}
}
