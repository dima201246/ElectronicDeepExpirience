/*
 * twi_complite.h
 *
 *  Created on: 28.05.2017
 *      Author: viktor96
 */

#ifndef TWI_COMPLETE_H_
#define TWI_COMPLETE_H_

#include <util/twi.h>
#include "myutils.h"

#ifdef DEBUG
	#include "usart.h"
#endif

#define TWI_PORT PORTC
#define TWI_DDR  DDRC
#define TWI_SCL  PINC5
#define TWI_SDA  PINC4


#define TWI_MASTER_ADDR	0b1010000

#define TWI_TX_BUFFSIZE	32		// Queue to send
#define TWI_RX_BUFFSIZE	32		// size shall be a power of two (2, 4, 8, 16, etc)

/*
 * Error codes
 */

#define TWI_BUFF_OVERFLOW 0xBF
#define TWI_BUFF_EMPTY 0xBE


typedef	uint16_t twihdl_t;
int8_t twi_init();
void twi_send(uint8_t sla, uint8_t *data, uint8_t size);

/* Experimental section */

#include <string.h>

typedef void twi_onaction_t();
enum action { SR, SLA_R, SLA_W, DT_1, DR_N, DT_N, ON_ACT };
enum mode { MASTER, SLAVE };

void twi_startaction(enum action task[], uint8_t len);
void twi_set_txbuff(uint8_t *buff, uint8_t len);
void twi_set_rxbuff(uint8_t *buff, uint8_t len);
void twi_set_on_action(twi_onaction_t handler);
void twi_setsla(uint8_t sla);
void twi_setmode(enum mode);

#endif /* TWI_COMPLETE_H_ */
