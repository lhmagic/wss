#include	"exti.h"

void setEXTI(uint8_t pin, uint8_t trigger) {
uint8_t port, offset, bit;
	
	port = pin>>4;
	bit = pin & 0x0F;
	offset = (pin%4)*4;
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[bit/4] &= ~(0x0F<<offset);
	SYSCFG->EXTICR[bit/4] |= port << offset;
	EXTI->IMR |= 1<<bit;
	
	if(trigger == RISING) {
		pinMode(pin, INPUT, PULLDOWN);
		EXTI->RTSR |= 1<<bit;
	}
	else {
		pinMode(pin, INPUT, PULLUP);
		EXTI->FTSR |= 1<<bit;
	}
	if(bit<=1)
		NVIC_EnableIRQ(EXTI0_1_IRQn);
	else if(bit<=3) 
		NVIC_EnableIRQ(EXTI2_3_IRQn);
	else
		NVIC_EnableIRQ(EXTI4_15_IRQn);
}

uint8_t getEXTILine(void) {
uint8_t line;
	
	for(line=0; line<16; line++) {
		if(EXTI->PR & (1<<line))
			break;
	}
	
	EXTI->PR |= (1<<line);
	
	return line;
}
