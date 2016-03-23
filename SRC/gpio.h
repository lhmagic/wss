#ifndef		__GPIO_H__
#define		__GPIO_H__

#include	"stm32f0xx.h"
#include	<stdarg.h>


#define		INPUT			0x00
#define		OUTPUT		0x01
#define		ALTER			0x02
#define		AIN				0X03

#define		LOW				0x00
#define		HIGH			0x01

#define		PULLOFF		0x00
#define		PULLUP		0x01
#define		PULLDOWN	0x02


//GPIOA pins definition
#define		PA0				0x00
#define		PA1				0x01
#define		PA2				0x02
#define		PA3				0x03
#define		PA4				0x04
#define		PA5				0x05
#define		PA6				0x06
#define		PA7				0x07
#define		PA8				0x08
#define		PA9				0x09
#define		PA10			0x0A
#define		PA11			0x0B
#define		PA12			0x0C
#define		PA13			0x0D
#define		PA14			0x0E
#define		PA15			0x0F

//GPIOB pins definition
#define		PB0				0x10
#define		PB1				0x11
#define		PB2				0x12
#define		PB3				0x13
#define		PB4				0x14
#define		PB5				0x15
#define		PB6				0x16
#define		PB7				0x17
#define		PB8				0x18
#define		PB9				0x19
#define		PB10			0x1A
#define		PB11			0x1B
#define		PB12			0x1C
#define		PB13			0x1D
#define		PB14			0x1E
#define		PB15			0x1F

//GPIOC pins definition
#define		PC0				0x20
#define		PC1				0x21
#define		PC2				0x22
#define		PC3				0x23
#define		PC4				0x24
#define		PC5				0x25
#define		PC6				0x26
#define		PC7				0x27
#define		PC8				0x28
#define		PC9				0x29
#define		PC10			0x2A
#define		PC11			0x2B
#define		PC12			0x2C
#define		PC13			0x2D
#define		PC14			0x2E
#define		PC15			0x2F

//GPIOD pins definition
#define		PD0				0x30
#define		PD1				0x31
#define		PD2				0x32
#define		PD3				0x33
#define		PD4				0x34
#define		PD5				0x35
#define		PD6				0x36
#define		PD7				0x37
#define		PD8				0x38
#define		PD9				0x39
#define		PD10			0x3A
#define		PD11			0x3B
#define		PD12			0x3C
#define		PD13			0x3D
#define		PD14			0x3E
#define		PD15			0x3F

//GPIOE pins definition
#define		PE0				0x40
#define		PE1				0x41
#define		PE2				0x42
#define		PE3				0x43
#define		PE4				0x44
#define		PE5				0x45
#define		PE6				0x46
#define		PE7				0x47
#define		PE8				0x48
#define		PE9				0x49
#define		PE10			0x4A
#define		PE11			0x4B
#define		PE12			0x4C
#define		PE13			0x4D
#define		PE14			0x4E
#define		PE15			0x4F

//GPIOF pins definition
#define		PF0				0x50
#define		PF1				0x51
#define		PF2				0x52
#define		PF3				0x53
#define		PF4				0x54
#define		PF5				0x55
#define		PF6				0x56
#define		PF7				0x57
#define		PF8				0x58
#define		PF9				0x59
#define		PF10			0x5A
#define		PF11			0x5B
#define		PF12			0x5C
#define		PF13			0x5D
#define		PF14			0x5E
#define		PF15			0x5F


void pinMode(uint8_t pin, uint8_t mode, uint8_t pull);
void digitalWrite(uint8_t pin, uint8_t val);
uint8_t	digitalRead(uint8_t pin);
	

#endif		//__GPIO_H__
