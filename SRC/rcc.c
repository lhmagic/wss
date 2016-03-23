#include		"rcc.h"

void setSystemClock(uint32_t clk) {
uint32_t wait;
uint8_t pre;
	
	if(clk == CLK_48MHZ) {
		pre = clk / HSE_VALUE;
		RCC->CFGR &= ~RCC_CFGR_PLLMUL;
		RCC->CFGR |= (pre-2)<<18;
	}	
	
	RCC->CR |= RCC_CR_HSEON;
	wait = 0x100000;
	while(RCC->CR & RCC_CR_HSERDY) {
		if(--wait == 0)
			break;
	}
	
	if(wait != 0) {
		RCC->CFGR &= ~RCC_CFGR_PLLSRC;
		RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;
	}
	
	wait = 0x100000;
	RCC->CR |= RCC_CR_PLLON;
	while(RCC->CR & RCC_CR_PLLRDY) {
		if(--wait == 0)
			return;
	}	
	
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	wait = 0x100000;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {
		if(--wait == 0)
			break;
	}	
}

uint32_t getSystemClock(void) {
uint32_t clk=0;	
		
	switch(RCC->CFGR & RCC_CFGR_SWS) {
		case RCC_CFGR_SWS_HSI:
			clk = HSI_VALUE;
			break;
		case RCC_CFGR_SWS_HSE:
			clk = HSE_VALUE;
			break;
		case RCC_CFGR_SWS_PLL:
			switch (RCC->CFGR & RCC_CFGR_PLLSRC) {
				case RCC_CFGR_PLLSRC_HSI_DIV2:
					clk = HSI_VALUE / 2;
					break;
				case RCC_CFGR_PLLSRC_HSI_PREDIV:
					clk = HSI_VALUE / (RCC->CFGR2+1);
					break;
				case RCC_CFGR_PLLSRC_HSE_PREDIV:
					clk = HSE_VALUE / (RCC->CFGR2+1);
					break;
				case RCC_CFGR_PLLSRC_HSI48_PREDIV:
					clk = HSI48_VALUE / (RCC->CFGR2+1);
					break;
			}
			clk *= (((RCC->CFGR & RCC_CFGR_PLLMUL) >> 18)+2);
			break;
		case RCC_CFGR_SWS_HSI48:
			clk = HSI48_VALUE;
			break;
	}
	return clk;
}

uint32_t getAHBClock(void) {
uint8_t pre;
	
	if(RCC->CFGR & RCC_CFGR_HPRE_3) {
		pre = (RCC->CFGR & RCC_CFGR_HPRE)>>4;
		pre = (pre&0x07)+1;
		return getSystemClock() >> pre;
	} else {
		return getSystemClock();
	}
}

uint32_t getAPBClock(void) {
uint8_t pre;
	
	if(RCC->CFGR & RCC_CFGR_PPRE_2) {
		pre = (RCC->CFGR & RCC_CFGR_PPRE)>>4;
		pre = (pre&0x03)+1;
		return getSystemClock() >> pre;
	} else {
		return getSystemClock();
	}	
}

void setSysTick(uint32_t ticks) {

	ticks = getSystemClock()/ticks;
	
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) return;

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */		
}
