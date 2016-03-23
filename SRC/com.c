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
			default:
				rcv_fsm = RCV_IDLE;
		}
	}
}

void usart_packet_handle(void) {
	if(rcv_done == 1) {
		led1_on();
		si4432_tx(usart_rx_buf, rcv_cnt);
		rcv_done = 0;
		led1_off();
	}	
}

void tim16_out_isr(void) {
	TIM16->DIER &= ~TIM_DIER_UIE;
	rcv_fsm = RCV_IDLE;
	if(rcv_cnt > 0)
		rcv_done = 1;
}

uint8_t iic_buf[256];
static uint8_t iic_rcv_done=0;
static uint8_t iic_rcv_cnt=0;

void iic_isr(void) {
static uint8_t reg_addr, cur_byte_cnt;	
	
	if(I2C1->ISR & I2C_ISR_ADDR) {		
		I2C1->ICR |= I2C_ICR_ADDRCF;
		I2C1->CR1 |= (I2C_CR1_RXIE | I2C_CR1_TXIE | I2C_CR1_NACKIE | I2C_CR1_STOPIE);
		
		cur_byte_cnt=0;
	}
	
	if(is_iic_rxne()) {
		uint8_t data = I2C1->RXDR;
	
		if(cur_byte_cnt == 0) {
			reg_addr = data;
		} else {
			iic_buf[reg_addr++] = data;
		}
		cur_byte_cnt++;
	}
	
	if(is_iic_txis()) {		
		I2C1->TXDR = iic_buf[0x80|reg_addr++];
	}
	
	if(is_iic_nackf()) {
		I2C1->ICR |= I2C_ICR_NACKCF;
	}	
	
	if(is_iic_stopf()) {
	
		I2C1->ICR |= I2C_ICR_STOPCF;
		I2C1->CR1 &= ~(I2C_CR1_RXIE | I2C_CR1_TXIE | I2C_CR1_NACKIE | I2C_CR1_STOPIE);
		
		if(is_iic_read()) {
			iic_buf[0x80|127] = 0;
			I2C1->ISR |= I2C_ISR_TXE;
		}
		if(cur_byte_cnt>1){
			iic_buf[0x80|127] = 0x80;
			iic_rcv_cnt=cur_byte_cnt-1;
			iic_rcv_done = 1;
		}		
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
		
		iic_oa1_disable();
		
		set_header(iic_buf[0]);
		si4432_tx(iic_buf+1, iic_rcv_cnt-1);
		
		iic_rcv_done = 0;
		iic_rcv_cnt = 0;
		
		iic_oa1_enable();
		
		led1_off();
	}
}
