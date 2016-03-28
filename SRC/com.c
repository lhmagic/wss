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
		if(is_slave()) {
			setTIM3(3500);
		} else {
			setTIM3(350);
		}
		switch (rcv_fsm) {
			case RCV_IDLE:
				rcv_fsm = RCV_ING;
				rcv_cnt = 0;
				if(is_slave()) {
					usart_rx_buf[rcv_cnt++] = get_uid();	//节点返回的第一个字节用UID填充
				}
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
uint8_t offset = 0;
	
	if(rcv_done == 1) {
		led1_on();
	
		if(!is_slave()) {
			set_header(usart_rx_buf[0]&0x3F);
			offset = 1;
		}
		
		si4432_tx(usart_rx_buf+offset, rcv_cnt-offset);
		rcv_done = 0;
		usart_rx_enable();
		led1_off();
	} else if(is_slave()) {			//节点未收到串口数据返回UID
		uint8_t uid = get_uid();
		si4432_tx(&uid, 1);
	}
}

void tim3_out_isr(void) {
	TIM16->DIER &= ~TIM_DIER_UIE;
	rcv_fsm = RCV_IDLE;
	if(rcv_cnt > 0) {
		rcv_done = 1;
		usart_rx_disable();
	}
}
