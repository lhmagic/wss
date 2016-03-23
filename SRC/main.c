#include "bsp.h"

int main(void) {
uint8_t uid;
uint8_t grp;
uint8_t sid;
	
	RCC->APB2ENR |= RCC_APB2ENR_DBGMCUEN;
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM16_STOP;
	
	iwdg_cfg();
	systick_cfg();
	gpio_cfg();
	TIM16_init();
	usart_cfg(115200);
	spi_cfg();
	
	si4432_init();
	
	uid = get_uid();
	sid = get_sid();
	grp = get_gid();
		
	if(!is_slave()) {
		broadcast_enable();
//		printf("\tRun as master.\r\n");
	}else {
		set_header(sid);
//		printf("\tRun as slave.\r\n");
	}	
	set_channel(grp);
	
//	printf("Build date:%s %s\r\n", __DATE__, __TIME__);
//	printf("GROUP: %#2x\r\nSID: %#2x\r\n", grp, sid);
//	printf("Freq is:%f MHz\r\n", 433.5+spi_rw_byte(0x79,0)*spi_rw_byte(0x7a,0)/50);
	
	led1_off();
	led2_off();
	si4432_rx(); 
	
	while(1) {
		
		iwdg_refresh();
		
		if(get_uid() != uid) {
			NVIC_SystemReset();			
		}
		
		usart_packet_handle();
		rf_packet_handle();
	}
}
