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

void TIM3_init(void) {
	NVIC_SetPriority(TIM3_IRQn, 0);	
	NVIC_EnableIRQ(TIM3_IRQn);	
	NVIC_SetPriority(TIM3_IRQn, 0);
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = getAPBClock()/1000000-1;
	TIM3->CNT = 1;
	TIM3->CR1 = TIM_CR1_URS | TIM_CR1_UDIS;
	TIM3->SR &= ~TIM_SR_UIF;
	TIM3->DIER |= TIM_DIER_UIE;
}

void setTIM3(uint16_t ns) {
	TIM3->ARR = ns;
	TIM3->CNT = 1;
	TIM3->CR1 = TIM_CR1_CEN;
}
