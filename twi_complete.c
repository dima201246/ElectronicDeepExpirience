/*
 * twi_complete.c
 *
 *  Created on: 28.05.2017
 *      Author: viktor96
 */

#define DEBUG

#include "twi_complete.h"

#define TWI_TX_BUFFEND		(TWI_TX_BUFFSIZE-1)
#define TWI_RX_BUFFEND		(TWI_RX_BUFFSIZE-1)

/* Define ring buffers */
uint8_t twi_txrd, twi_txwr;
uint8_t twi_tx_buff[TWI_TX_BUFFSIZE];

uint8_t twi_rxrd, twi_rxwr;
uint8_t twi_rx_buff[TWI_RX_BUFFSIZE];

#define buff_empty(_a)\
		(twi_##_a##xrd == twi_##_a##xwr)

#define buff_free_len(_a, _buff)\
		((twi_##_a##xwr < twi_##_a##xrd) ? \
			(twi_##_a##xrd - twi_##_a##xwr + 1) : \
			(_buff##SIZE - (twi_##_a##xwr - twi_##_a##xrd)))\

#define GET_RXRD(handle)	((handle >> 8) & 0xFF)
#define GET_RXWR(handle)	((handle) & 0xFF)

#define SET_RXRDWR(handle)	(handle = (twihdl_t) ((twi_rxrd << 8) | (twi_rxwr)))

/* error handler */
void twi_error(uint8_t code);

void twi_load_tx_buff(uint8_t *data, uint8_t count)
{
	// if we have some place to write
	for(; count > 0; --count) {
		if (((twi_txwr+1) & TWI_TX_BUFFEND) != twi_txrd) {
			twi_tx_buff[twi_txwr++] = *data++;
			twi_txwr &= TWI_TX_BUFFEND;
		} else
			return;
	}
}

uint8_t twi_get_tx_byte()
{
	/* empty buffer */
	if(buff_empty(t)) {
		twi_error(TWI_BUFF_EMPTY);
		return TWI_BUFF_EMPTY;
	}

	uint8_t byte = twi_tx_buff[twi_txrd++];
	twi_txrd &= TWI_TX_BUFFEND;

	return byte;
}

uint8_t twi_get_rx_byte()
{
	/* empty buffer */
	if(buff_empty(r)) {
		twi_error(TWI_BUFF_EMPTY);
		return TWI_BUFF_EMPTY;
	}

	uint8_t byte = twi_rx_buff[twi_rxrd++];
	twi_rxrd &= TWI_RX_BUFFEND;

	return byte;
}

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

	/* clear pointers */
	twi_txrd = twi_txwr = twi_rxrd = twi_rxwr = 0;

	setbit(TWCR, TWIE);		// enable interrupts

	return 0;
}

void twi_send(uint8_t sla, uint8_t *data, uint8_t size)
{
	/* Full address with direction bit */
	uint8_t sla_w = (sla << 1) | TW_WRITE;

	/* load slave address */
	slave_address = sla_w;
	twi_load_tx_buff(data, size);

	twi_transmit_start();
}

twihdl_t twi_recv(uint8_t sla, uint8_t size)
{
	/* if we haven't any space in buffer -  error */
	if(buff_free_len(r, TWI_RX_BUFF) < size) {
		twi_error(TWI_BUFF_OVERFLOW);
		return (twihdl_t) TWI_BUFF_OVERFLOW;
	}

	/* Full address with direction bit */
	uint8_t sla_r = (sla << 1) | TW_READ;
	twihdl_t handle;

	/* load slave address */
	slave_address = sla_r;

	/* generate handle */
	SET_RXRDWR(handle);

	twi_rxwr = (twi_rxwr + size) & TWI_TX_BUFFEND;

	twi_transmit_start();

	return handle;
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

#define TASK_ERR 0xEE

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

void twi_startaction(enum action task[], uint8_t len)
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
		twi_error(TASK_ERR);
		curr_act_num = curr_task_len = 0;
		twi_transmit_stop();
	}

	switch(TW_STATUS)
	{
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
						++curr_act_num;
						twi_transmit_start();
					break;
				case DT_1:
						twi_transmit_data(tx_buff[tx_wr++]);
						++curr_act_num;
					break;
				case DT_N:
						twi_transmit_data(tx_buff[tx_wr++]);
						if (tx_wr == tx_buff_size)
							++curr_act_num;
					break;

				case DR_N:
					break;

				case ON_ACT:
						on_act_handler();
						++curr_act_num;
					break;
			}

			if (curr_act_num == curr_task_len)
				twi_transmit_stop();
		break;

	case TW_MT_DATA_NACK:
		twi_txrd = twi_txwr;
		twi_transmit_stop();
		break;

	/* Bus was broken? FUCK! */
	case TW_BUS_ERROR:
		break;
	}
}















