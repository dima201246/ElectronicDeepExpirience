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
	//uint8_t sec, min, hour;
	//sec = (clock_reg[2] & 0x0F) + ((clock_reg[2] & 0x70)*10);
	//min = (clock_reg[1] & 0x0F) + ((clock_reg[1] & 0x70)*10);
	//hour = ((clock_reg[0] & 0x10)*10) + ((clock_reg[0] & 0x0F));

	uint8_t i = 0;
	for(; i < sizeof(clock_reg); ++i) {
		USART_TransmitHexNum(clock_reg[i]);
		USART_Transmit('\n');
	}
}

void clock_recv(uint8_t *data, uint8_t size, twi_onaction_t hdl)
{
	/* load slave address */
	twi_setsla(CLOCK_SLA);

	twi_set_txbuff(&reg_addr, 1);
	twi_set_rxbuff(data, size);
	twi_set_on_action(hdl);

	enum twi_action task[] = { SLA_W, DT_1, SR, SLA_R, DR_N, ON_ACT };

	twi_startaction(task, sizeof(task));
}

void clock_send(uint8_t *data, uint8_t size, twi_onaction_t hdl)
{
	twi_setsla(CLOCK_SLA);
	twi_set_txbuff(data, size);
	twi_set_rxbuff(NULL, 0);
	if (hdl)
		twi_set_on_action(hdl);

	enum twi_action task[] = { SLA_W, DT_N };

	twi_startaction(task, sizeof(task));
}

uint8_t goforward = true;
void cont()
{
	goforward = true;
}

int main()
{

    USART_Setup();
    twi_init();

    sei();

    uint8_t settings_on[2] = {0x00, 0x80};
	clock_send(settings_on, sizeof(settings_on), NULL); _delay_ms(100);
	uint8_t settings[4] = {0x00, 0x80, 0x53, 0x21};
	clock_send(settings, sizeof(settings), NULL); _delay_ms(100);
	uint8_t settings_off[2] = {0x00, 0x00};
	clock_send(settings_off, sizeof(settings_off), NULL); _delay_ms(100);

	while(1)
	{
		//twi_send(CLOCK_SLA, data, 4);
		clock_recv(clock_reg, sizeof(clock_reg), read_buff);
		_delay_ms(2000);
	}
}
