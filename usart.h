/*
 * usart.h
 *
 *  Created on: 08.05.2017
 *      Author: viktor96
 */

#ifndef USART_H_
#define USART_H_

#define UART_RATE		9600
#define UART_BUFSIZE	256
#define UART_BUFEND		(UART_BUFSIZE-1)

#include "myutils.h"

int USART_Setup();
void USART_Init( uint16_t bauds );
void USART_Transmit( uint8_t data );
uint8_t USART_Recive();
uint8_t USART_RX_Count();
void USART_TransmitMsg( uint8_t *msg );
void USART_TransmitNum(uint16_t num);
void USART_TransmitHexNum(uint16_t num);

#endif /* USART_H_ */
