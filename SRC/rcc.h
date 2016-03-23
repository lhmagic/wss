#ifndef		__RCC_H__
#define		__RCC_H__


#include	"bsp.h"


#endif		//__RCC_H__


#define		CLK_48MHZ		48000000


void setSystemClock(uint32_t clk);
uint32_t getSystemClock(void);
uint32_t getAHBClock(void);
uint32_t getAPBClock(void);
void setSysTick(uint32_t ticks);
