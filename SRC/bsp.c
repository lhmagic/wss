#include "bsp.h"

//systick 中断中用于1ms计数
static uint32_t ticks;

void gpio_cfg(void) {
	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	//配置拨码开关 上拉输入
	GPIOA->MODER &= ~0xFFFF;
	GPIOA->PUPDR &= ~0xFFFF;
	GPIOA->PUPDR |= 0x5555;
	
	//配置LED 推挽输出
	GPIOA->MODER &= ~0x03C00000;
	GPIOA->MODER |= 0x01400000;
	GPIOA->OTYPER &= ~0x1800;
	GPIOA->OSPEEDR |= 0x03C00000;
	
	//配置RF_SDN 推挽输出
	GPIOB->MODER &= ~0x03;
	GPIOB->MODER |= 0x01;
	GPIOB->OTYPER &= ~0x01;
	GPIOB->OSPEEDR |= 0x03;
	
	//配置RF_NIRQ	上拉输入
	setEXTI(PB1, FALLING);
	
	//配置串口
	pinMode(PA9, ALTER, PULLUP);
	pinMode(PA10, ALTER, PULLUP);
	GPIOA->AFR[1] &= ~0xFF0;
	GPIOA->AFR[1] |= 0x110;
	
	//配置SPI
	pinMode(PA15, ALTER, PULLUP);
	pinMode(PB3, ALTER, PULLDOWN);
	pinMode(PB4, ALTER, PULLDOWN);
	pinMode(PB5, ALTER, PULLDOWN);
	
	//配置IIC
	pinMode(PB6, ALTER, PULLUP);
	pinMode(PB7, ALTER, PULLUP);
	GPIOB->AFR[0] &= ~0xFF000000;
	GPIOB->AFR[0] |= 0x11000000;	
}

void iic_cfg(void) {
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW;
	
//	I2C1->CR2 |= I2C_CR2_AUTOEND;
//	I2C1->CR2 &= ~0x3FF;
//	I2C1->CR2 |= 0xA0;
	
	I2C1->OAR1 = 0x8000 | get_iic_gid();
	I2C1->CR2 = 0;
	I2C1->CR2 = I2C_CR2_RELOAD;	
	
	I2C1->TIMINGR = 0x0090194B;
	
	I2C1->CR1 |= (I2C_CR1_GCEN | I2C_CR1_RXIE | I2C_CR1_TXIE);
	I2C1->CR1 |= (I2C_CR1_ADDRIE | I2C_CR1_STOPIE);
	I2C1->CR1 |= I2C_CR1_PE;

	NVIC_SetPriority(I2C1_IRQn, 1<<__NVIC_PRIO_BITS);
	NVIC_EnableIRQ(I2C1_IRQn);	
}

static uint8_t *iic_buf, iic_cnt;
void iic_write(uint8_t addr, uint8_t *buf, uint8_t cnt) {

	iic_buf = buf;
	iic_cnt = cnt;
	
	I2C1->CR2 &= ~0xFF0000;
	I2C1->CR2 |= ((cnt+1)<<16);
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	
	I2C1->TXDR = addr;
//	I2C1->CR1 |= I2C_CR1_TXIE;
}

uint8_t is_iic_txis(void) {
	return (I2C1->ISR & I2C_ISR_TXIS);
}
	
uint8_t is_iic_txe(void) {
	return (I2C1->ISR & I2C_ISR_TXE);
}

uint8_t is_dummy_write(void) {
	if(((I2C1->CR2 >> 16) & 0xFF) == 1) {
		return 1;
	}
	return 0;
}

void iic_tx_isr(void) {

	if(iic_cnt-- > 0)
		I2C1->TXDR = *iic_buf++;
	else {
		I2C1->CR1 &= ~I2C_CR1_TXIE;
	}
}

static void iic_dummy_write(uint8_t addr) {
	iic_write(addr, NULL, 0);
}

void iic_read(uint8_t addr, uint8_t *buf, uint8_t cnt) {
uint32_t i;	
	
	iic_dummy_write(addr);
	
	while(!(I2C1->ISR & I2C_ISR_STOPF));
	i=0x200;
	while(i--);	//加延时才能正常读
	
	iic_buf = buf;
	iic_cnt = 0;	
	
	I2C1->CR2 &= ~0xFF0000;
	I2C1->CR2 |= (cnt<<16);
	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
}

uint8_t is_iic_rxne(void) {
	return (I2C1->ISR & I2C_ISR_RXNE);
}

uint8_t is_rx_done(void) {
	if(iic_cnt >= ((I2C1->CR2 >> 16)&0xFF))
		return 1;
	return 0;
}

void iic_rx_isr(void) {
	if(is_iic_rxne()) {
		*iic_buf++ = I2C1->RXDR;
		iic_cnt++;
	}
	if(is_rx_done()) {
		
	}
}

uint8_t is_iic_read(void) {
	return (I2C1->ISR & I2C_ISR_DIR);
}

uint8_t is_iic_stopf(void) {
	return (I2C1->ISR & I2C_ISR_STOPF);
}

void iic_oa1_disable(void) {
	I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
}

void iic_oa1_enable(void) {
	I2C1->OAR1 |= I2C_OAR1_OA1EN;
}

void spi_cfg(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	SPI1->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_BR_1);
	SPI1->CR2 |= (SPI_CR2_DS | SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_RXNEIE);
	
	SPI1->CR1 |= SPI_CR1_SPE;
}

void usart_cfg(uint32_t bandrate) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	USART1->BRR = (uint16_t)(SYS_CLK*1.0/bandrate+0.5);
	USART1->CR1 = 0x162C;
	USART1->CR1 |= USART_CR1_UE;
		
	NVIC_SetPriority(USART1_IRQn, 1<<__NVIC_PRIO_BITS);
	NVIC_EnableIRQ(USART1_IRQn);	
}

void usart_putc(uint8_t ch) {
	while(!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = ch;	
}

int fputc(int ch, FILE *f) {
  usart_putc(ch);
	return 0;
}

void usart_tx(uint8_t *buf, uint8_t len) {
	while(len--) {
		usart_putc(*buf++);
	}	
}

uint8_t is_usart_rxne(void) {
	if(USART1->ISR & USART_ISR_RXNE)
		return 1;
	else 
		return 0;
}

uint8_t usart_getc(void) {
	return USART1->RDR;
}

uint8_t spi_rw_byte(uint8_t addr, uint8_t val) {
	rf_nss_low();
	SPI1->DR = (addr<<8) | val;
	SPI1->CR1 |= SPI_CR1_SPE;
	while(!(SPI1->SR & SPI_SR_RXNE));
	val = SPI1->DR;
	rf_nss_high();		
	
	return val;
}

void systick_cfg(void) {
	SysTick_Config(SYS_CLK/1000);
}

uint32_t get_ticks(void) {
	return ticks;
}

void ticks_isr(void) {
	ticks++;
}

void delay(uint16_t n_ms) {
uint32_t start;
	start = get_ticks();
	while((get_ticks() - start) < n_ms);
}

void iwdg_cfg(void) {
	IWDG->KR = 0xCCCC;
	IWDG->KR = 0x5555;
	IWDG->PR = 0x01;
	IWDG->RLR = 0xFFF;
	while(IWDG->SR);
	iwdg_refresh();
}

void iwdg_refresh(void) {
	IWDG->KR = 0xAAAA;
}
