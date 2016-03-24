#include "bsp.h"                  // Device header


void SysTick_Handler(void) {
	ticks_isr();
}

void USART1_IRQHandler(void) {
	usart_isr();
}

void EXTI0_1_IRQHandler(void) {
	getEXTILine();
	if(digitalRead(PB1) == LOW) {
		rf_isr();		
	}
}

void TIM16_IRQHandler(void) {
	if((TIM16->SR & TIM_SR_UIF) == TIM_SR_UIF) {
		TIM16->SR &= ~TIM_SR_UIF;
		TIM16->CR1 &= ~TIM_CR1_CEN;
		tim16_out_isr();
	}
}
