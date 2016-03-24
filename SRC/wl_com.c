#include	"wl_com.h"

static uint8_t rf_packet;
static uint8_t rf_cnt, rf_buf[256];
static uint8_t rf_rx_cnt, *rf_rx_pbuf = rf_buf;

void rf_isr(void) {
int status, mask;

  status = spi_rw_byte(0x03, 0x00) << 8;
  status += spi_rw_byte(0x04, 0x00);
  mask = spi_rw_byte(0x05, 0x00) << 8;
  mask += spi_rw_byte(0x06, 0x00);

  status &= mask;
	
	if(status & IRXFFAFULL) {
	uint8_t i;
		
		for(i=0; i<0x20; i++) {
			*rf_rx_pbuf++ = spi_rw_byte(0x7f,0x00);
			rf_rx_cnt++;
		}
		
		rxfaf_isr();
	}
	
	if(status & ITXFFAEM) {
		txfae_isr();
	}	
	
  if(status & IPKSENT) {
		si4432_rx();
  }

  if(status & IPKVALID) {
		uint8_t i;
		rf_cnt = spi_rw_byte(0x4b,0x00);
		
		if(rf_cnt > rf_rx_cnt) {
			for(i=0; i<rf_cnt-rf_rx_cnt; i++) {
				*rf_rx_pbuf++ = spi_rw_byte(0x7f,0x00);
			}
		}
		
		rf_rx_pbuf = rf_buf;
		rf_rx_cnt = 0;
		rf_packet=1;
		si4432_rx();
  }
}


void rf_packet_handle(void) {
	
	if(rf_packet == 1) {
		led2_on();
		rf_packet = 0;
		
		if(is_slave()) {
			usart_packet_handle();
		}
		usart_tx(rf_buf, rf_cnt);
		led2_off();
	}
}
