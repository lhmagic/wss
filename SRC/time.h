#ifndef		__TIME_H__
#define		__TIME_H__

#include	"bsp.h"


void delay_ms(uint16_t ms);
void TIM16_init(void);
void setTIM16(uint16_t ms);
void time_tick_int(void);
uint32_t millis(void) ;

#endif		//__TIME_H__
