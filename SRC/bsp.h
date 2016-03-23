#ifndef		__BSP_H__
#define		__BSP_H__

#include <stdio.h>
#include "stm32f0xx.h"
#include "gpio.h"
#include "exti.h"
#include "si4432.h"
#include "wl_com.h"
#include "com.h"
#include "rcc.h"
#include "time.h"


#define		SYS_CLK				48000000

#define		led1_on()			GPIOA->BSRR = (1<<27)
#define		led1_off()		GPIOA->BSRR = (1<<11)

#define		led2_on()			GPIOA->BSRR = (1<<28)
#define		led2_off()		GPIOA->BSRR = (1<<12)

#define		rf_sdn_high()	GPIOB->BSRR = 1
#define		rf_sdn_low()	GPIOB->BSRR = (1<<16)

#define		rf_nss_high()	GPIOA->BSRR = 0x8000
#define		rf_nss_low()	GPIOA->BSRR = 0x80000000

#define		get_uid()			(GPIOA->IDR & 0xFF)

#define		get_sid()			(get_uid() & 0x3F)

#define		get_gid()			(get_uid() >> 6)

#define		get_iic_gid()	(get_uid() & 0xC0)

#define 	is_slave()		(get_sid())

//外部函数声明

void iic_cfg(void);
void iic_write(uint8_t addr, uint8_t *buf, uint8_t cnt);
uint8_t is_iic_txis(void);
uint8_t is_iic_txe(void);
void iic_tx_isr(void);
uint8_t is_iic_rxne(void);
void iic_rx_isr(void);
void iic_read(uint8_t addr, uint8_t *buf, uint8_t cnt);
uint8_t is_iic_read(void) ;
uint8_t is_iic_stopf(void);
void gpio_cfg(void);
void usart_cfg(uint32_t bandrate);
void usart_putc(uint8_t ch);
void usart_tx(uint8_t *buf, uint8_t len);
uint8_t is_usart_rxne(void);
uint8_t usart_getc(void);
void iic_oa1_enable(void);
void iic_oa1_disable(void);
void spi_cfg(void);
uint8_t spi_rw_byte(uint8_t addr, uint8_t val);
void systick_cfg(void);
uint32_t get_ticks(void);
void ticks_isr(void);
void delay(uint16_t n_ms);
void iwdg_cfg(void);
void iwdg_refresh(void);

#endif		//__BSP_H__
