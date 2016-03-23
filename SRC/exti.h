#ifndef		__EXTI_H__
#define		__EXTI_H__

#include	"stm32f0xx.h"
#include	"gpio.h"
#include	<stdint.h>


#define		RISING		0x00
#define		FALLING		0x01


void setEXTI(uint8_t pin, uint8_t trigger);
uint8_t getEXTILine(void) ;

#endif		//__EXTI_H__

