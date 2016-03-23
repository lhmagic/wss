#ifndef		__COM_H__
#define		__COM_H__

#include 	"bsp.h"

#define		SYNC			0xFA


void usart_isr(void);
void tim16_out_isr(void);
void usart_packet_handle(void);
void iic_isr(void);
void iic_packet_handle(void);
void clear_iic_tx_buf(void);

#endif		//¡_COM_H__
