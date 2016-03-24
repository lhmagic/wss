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

void TIM3_IRQHandler(void) {
	if((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF) {
		TIM3->SR &= ~TIM_SR_UIF;
		TIM3->CR1 &= ~TIM_CR1_CEN;
		tim3_out_isr();
	}
}
