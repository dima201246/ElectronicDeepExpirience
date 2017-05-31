/*
 * twi_complete.c
 *
 *  Created on: 28.05.2017
 *      Author: viktor96
 */

#define DEBUG

#include "twi_complete.h"

/* error handler */
void twi_error(uint8_t code);


// transmit start condition
#define twi_transmit_start() \
	do {setmask(TWCR, _BV(TWINT) | _BV(TWSTA) | _BV(TWEN));}while(0)\

// transmit stop condition
#define twi_transmit_stop() \
	do {setmask(TWCR, _BV(TWINT) | _BV(TWSTO) | _BV(TWEN));}while(0)\

#define twi_transmit_data(_data) \
	do {\
		TWDR = (_data);\
		clearmask(TWCR, _BV(TWSTA) | _BV(TWSTO));\
		setmask(TWCR, _BV(TWEN) | _BV(TWINT));\
	} while(0)\

#define twi_recv_ack(_to)\
	do{\
		(_to) = TWDR;\
		clearmask(TWCR, _BV(TWSTA) | _BV(TWSTO));\
		setmask(TWCR, _BV(TWEN) | _BV(TWINT) | _BV(TWEA));\
	} while(0)\

#define twi_recv_nack(_to)\
	do{\
		(_to) = TWDR;\
		clearmask(TWCR, _BV(TWSTA) | _BV(TWSTO)| _BV(TWEA));\
		setmask(TWCR, _BV(TWEN) | _BV(TWINT));\
	} while(0)\

#ifdef DEBUG
void twi_status_print()
{
	USART_TransmitMsg("TWI:STATUS: ");
	USART_TransmitHexNum(TW_STATUS);
	USART_Transmit('\n');
}
#endif

int8_t twi_init()
{
#ifdef DEBUG
	USART_TransmitMsg("TWI Init\n");
#endif

	clearmask(TWI_DDR, _BV(TWI_SCL) | _BV(TWI_SDA));
	setmask(TWI_PORT, _BV(TWI_SCL) | _BV(TWI_SDA));  // enable pull-up resistors

	//TWBR = 72; 	// 100 KHz
	TWBR = 250;	// DEBUG
	TWSR = 0;

	setbit(TWCR, TWIE);		// enable interrupts

	return 0;
}

void twi_error(uint8_t code)
{
#ifdef DEBUG
	USART_TransmitMsg("TWI:ERROR! CODE:");
	USART_TransmitHexNum(code);
	USART_Transmit('\n');
#endif
}

/* ----------------- Experimental section ------------------ */

uint8_t *tx_buff, tx_buff_size;
uint8_t *rx_buff, rx_buff_size;

uint8_t tx_wr, rx_wr;

uint8_t curr_task[16], curr_act_num, curr_task_len;

/* Current slave address */
uint8_t slave_address;

void do_nothing()
{
}

twi_onaction_t on_act_handler = do_nothing;

void twi_startaction(enum twi_action task[], uint8_t len)
{
	curr_task_len = len;
	memcpy(curr_task, task, len);

	curr_act_num = 0;
	tx_wr = rx_wr = 0;

	twi_transmit_start();
}

void twi_set_txbuff(uint8_t *buff, uint8_t len)
{
	tx_buff = buff;
	tx_buff_size = len;
}

void twi_set_rxbuff(uint8_t *buff, uint8_t len)
{
	rx_buff = buff;
	rx_buff_size = len;
}

void twi_set_on_action(twi_onaction_t handler)
{
	on_act_handler = handler;
}

void twi_setsla(uint8_t sla)
{
	slave_address = sla << 1;
}

// Main TWI interrupt handler
ISR(TWI_vect)
{
#ifdef DEBUG
	twi_status_print();
#endif

	if (curr_act_num >= curr_task_len)
	{
		curr_act_num = curr_task_len = 0;
		twi_transmit_stop();
		return;
	}

	if (curr_task[curr_act_num] == ON_ACT)
	{
		curr_act_num++;
		on_act_handler();
	}

	switch(TW_STATUS)
	{
		/*--------------- MASTER TRANSIVER MODE ---------------*/

		/* START condition has begin transmitted
		 * We should load sla_w or sla_r
		 */
		case TW_START:
		case TW_REP_START:
				TWDR = (curr_task[curr_act_num++] == SLA_R) ?
							slave_address | TW_READ :
							slave_address | TW_WRITE;

				setmask(TWCR, _BV(TWINT) | _BV(TWEN));
			break;

		case TW_MT_SLA_ACK:			/*  SLA+W transmitted, ACK received */
		case TW_MT_DATA_ACK:		/* data transmitted, ACK received */
				switch(curr_task[curr_act_num])
				{
					case SR:
							curr_act_num++;
							twi_transmit_start();
						break;
					case DT_1:
							twi_transmit_data(tx_buff[tx_wr++]);
							curr_act_num++;
						break;
					case DT_N:
							twi_transmit_data(tx_buff[tx_wr++]);
							if (tx_wr == tx_buff_size)
								curr_act_num++;
						break;
				}
			break;

		/* Something is wrong */
		case TW_MT_DATA_NACK:
			twi_error(TW_MT_DATA_NACK);
			curr_act_num = curr_task_len = 0;
			twi_transmit_stop();
			break;

		case TW_MR_ARB_LOST:	/* Someone becomes to MASTER */
			curr_act_num = 0;	/* repeat all */
			twi_transmit_start();
			break;

		/*--------------- MASTER RECIVE MODE ---------------*/

		case TW_MR_SLA_ACK:
		case TW_MR_DATA_ACK:
			switch(curr_task[curr_act_num])
			{
				case DR_1:
					if (rx_wr < rx_buff_size-1)
					{
						twi_recv_ack(rx_buff[rx_wr++]);
					}
					else
					{
						twi_recv_nack(rx_buff[rx_wr++]);
					}

					curr_act_num++;
					break;

				case DR_N:
					if (rx_wr >= rx_buff_size-1)
					{
						twi_recv_nack(rx_buff[rx_wr++]);
						curr_act_num++;
					}
					else
					{
						twi_recv_ack(rx_buff[rx_wr++]);
					}
					break;
			}
			break;

		case TW_MR_DATA_NACK:
			switch(curr_task[curr_act_num])
			{
				default:
					twi_transmit_stop();
					break;
			}
			break;

		/* Bus was broken? FUCK! */
		case TW_BUS_ERROR:
			break;
	}
}















