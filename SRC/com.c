#include "com.h"

#define		RCV_IDLE		0
#define		RCV_ING 		1
#define		RCV_DONE		2

static uint8_t rcv_fsm;
static uint8_t usart_rx_buf[256];
static uint8_t rcv_done;
static uint8_t rcv_cnt;

void usart_isr(void) {

	if(is_usart_rxne()) {
		setTIM16(10);
		switch (rcv_fsm) {
			case RCV_IDLE:
				rcv_fsm = RCV_ING;
				rcv_cnt = 0;
				usart_rx_buf[rcv_cnt++] = usart_getc();
				break;
			case RCV_ING:
				usart_rx_buf[rcv_cnt++] = usart_getc();
				break;
		}
	}
}

void usart_packet_handle(void) {
uint8_t offset = 0;
	
	if(rcv_done == 1) {
		led1_on();
		if(!is_slave()) {
			set_header(usart_rx_buf[0]);
			offset = 1;
		}
		si4432_tx(usart_rx_buf+offset, rcv_cnt-offset);
		rcv_done = 0;
		led1_off();
	}	
}

void tim16_out_isr(void) {
	TIM16->DIER &= ~TIM_DIER_UIE;
	rcv_fsm = RCV_IDLE;
	rcv_done = 1;
}

uint8_t iic_buf[256];
static uint8_t iic_index=0;
static uint8_t iic_rcv_done=0;
static uint8_t iic_rcv_cnt=0;

void iic_isr(void) {
static uint8_t addr, i;	
	
	if(I2C1->ISR & I2C_ISR_ADDR) {
		I2C1->ICR |= I2C_ICR_ADDRCF;
		addr = 0;
		//£¡£¡£¡flush tx register, avoid to send unwanted data.£¡£¡£¡
		if(!is_iic_txe()) {
			I2C1->ISR |= I2C_ISR_TXE;
		}
	}
	
	if(is_iic_rxne()) {
		if((i==0) && (!is_iic_read())) {
			iic_index = I2C1->RXDR;
		} else {
			iic_buf[addr++] = I2C1->RXDR;
		}
		i++;
	}
	
	if(is_iic_txis()) {
		I2C1->TXDR = iic_buf[0x80+addr++];
	}
	
	if(is_iic_stopf()) {
		I2C1->ICR |= I2C_ICR_STOPCF;
		if((i>1) && (!is_iic_read())) {
			iic_rcv_cnt=i-1;
			iic_rcv_done = 1;
		}
		i=0;
	}	
}

void clear_iic_tx_buf(void) {
uint16_t i;
		for(i=80; i<0x100; i++) {
			iic_buf[i] = 0;
		}
}

void iic_packet_handle(void) {
	if(iic_rcv_done) {
		led1_on();
		
		clear_iic_tx_buf();
		iic_oa1_disable();
		
		set_header(iic_index);
		si4432_tx(iic_buf, iic_rcv_cnt);
		
		iic_rcv_done = 0;
		iic_rcv_cnt = 0;
		
		iic_oa1_enable();
		
		led1_off();
	}
}
