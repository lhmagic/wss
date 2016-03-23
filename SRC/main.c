#include "bsp.h"

int main(void) {
uint8_t uid;
uint8_t grp;
uint8_t sid;
	
	RCC->APB2ENR |= RCC_APB2ENR_DBGMCUEN;
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	iwdg_cfg();
	systick_cfg();
	gpio_cfg();
	TIM16_init();
	usart_cfg(9600);
	spi_cfg();
	
	si4432_init();
	
	uid = get_uid();
	sid = get_sid();
	grp = get_gid();
		
	if(!is_slave()) {
		iic_cfg();
		broadcast_enable();
	}else {
		set_header(sid);
	}	
	set_channel(grp);
	
	led1_off();
	led2_off();
	si4432_rx(); 
	
	while(1) {
		
		iwdg_refresh();
		
		if(get_uid() != uid) {
			NVIC_SystemReset();			
		}
		
		if(!is_slave()) {
			iic_packet_handle();
		} else {
//			usart_packet_handle();
		}

		rf_packet_handle();
	}
}
