#include	"time.h"

static volatile uint32_t private_timer_t;

void time_tick_int(void) {
	private_timer_t++;
}
	
uint32_t millis(void) {
	return private_timer_t*10;
}

void delay_ms(uint16_t ms) {
	if(ms <= 0)
		return;
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	TIM14->PSC = getAPBClock()/1000-1;
	TIM14->ARR = ms;
	
	TIM14->CNT = 1;
	TIM14->CR1 |= TIM_CR1_CEN;
	TIM14->SR &= ~TIM_SR_UIF;
	while((TIM14->SR & TIM_SR_UIF) != TIM_SR_UIF);
	TIM14->CR1 &= ~TIM_CR1_CEN;
	
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;
}

void TIM16_init(void) {
	NVIC_SetPriority(TIM16_IRQn, 0);	
	NVIC_EnableIRQ(TIM16_IRQn);	
	NVIC_SetPriority(TIM16_IRQn, 0);
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16->PSC = getAPBClock()/1000-1;
	TIM16->ARR = 1;
	TIM16->CNT = 1;
	TIM16->CR1 = TIM_CR1_URS | TIM_CR1_CEN;
	TIM16->SR &= ~TIM_SR_UIF;
	TIM16->DIER |= TIM_DIER_UIE;
	delay_ms(1);								//解决第一次开启定时器就进入中断的问题
}

void setTIM16(uint16_t ms) {
	TIM16->PSC = getAPBClock()/1000-1;
	TIM16->ARR = ms;
	TIM16->CNT = 1;
	TIM16->CR1 = TIM_CR1_URS | TIM_CR1_CEN;
	TIM16->SR &= ~TIM_SR_UIF;
	TIM16->DIER |= TIM_DIER_UIE;
}
