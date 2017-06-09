/*
 * twi_complite.h
 *
 *  Created on: 28.05.2017
 *      Author: viktor96
 */

#ifndef TWI_COMPLETE_H_
#define TWI_COMPLETE_H_

#include <util/twi.h>
#include <string.h>
#include "myutils.h"

#ifdef DEBUG
	#include "usart.h"
#endif

#define TWI_PORT PORTC
#define TWI_DDR  DDRC
#define TWI_SCL  PINC5
#define TWI_SDA  PINC4

/*
 * When a general call is issued,
 * all slaves should respond by pulling
 * the SDA line low in the ACK cycle.
 */
#define TWI_GENERALCALL_ADDR 0b0000000

/*
 * Address of this MCU in slave mode
 */
#define TWI_SLAVE_ADDR	0b1010000

/*
 * Error codes
 */

#define TWI_BUSY 0xBA
#define TWI_BUFF_OVERFLOW 0xBF
#define TWI_BUFF_EMPTY 0xBE
#define TWI_TASK_ERR 0xEE

/* Frequency in kHz! */
int8_t twi_init(uint16_t freq);

/* Experimental section */

typedef void (*twi_onaction_t)();

enum twi_action { SR, SLA_R, SLA_W, DT_1, DR_1, DR_N, DT_N, ON_ACT };
enum twi_mode { MASTER, SLAVE };

uint8_t twi_startaction(enum twi_action task[], uint8_t len);
void twi_actionwait();
uint8_t twi_is_busy();
void twi_set_txbuff(uint8_t *buff, uint8_t len);
void twi_set_rxbuff(uint8_t *buff, uint8_t len);
void twi_set_on_action(twi_onaction_t handler);
void twi_setsla(uint8_t sla);
void twi_setown_addr(uint8_t own_address);
void twi_setmode(enum twi_mode);

#endif /* TWI_COMPLETE_H_ */
