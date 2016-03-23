#include		"gpio.h"

GPIO_TypeDef * const GPIOs[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};

const uint16_t pinBits[] = {0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};

void pinMode(uint8_t pin, uint8_t mode, uint8_t pull) {
GPIO_TypeDef * GPIO;
//uint16_t	bit;
		
	GPIO = GPIOs[pin>>4];
	RCC->AHBENR |= pinBits[pin>>4]<<17;
	
	pin  = pin & 0x0F;
//	bit	 = pinBits[pin];
	
	GPIO->MODER &= ~(0x03 << pin*2);
	GPIO->MODER |= (mode << pin*2);
	
	if(mode != AIN) {
		GPIO->PUPDR &= ~(0x03 << pin*2);
		GPIO->PUPDR |= (pull << pin*2);
	}
}

void digitalWrite(uint8_t pin, uint8_t val) {
GPIO_TypeDef * GPIO;
uint16_t	bit;
		
	GPIO = GPIOs[pin>>4];
	pin  = pin & 0x0F;
	bit	 = pinBits[pin];
	
	GPIO->BSRR = val? bit : (bit<<16);
}

uint8_t	digitalRead(uint8_t pin) {
GPIO_TypeDef * GPIO;
uint16_t	bit;
		
	GPIO = GPIOs[pin>>4];
	pin  = pin & 0x0F;
	bit	 = pinBits[pin];
	
	return ((GPIO->IDR & bit) ? HIGH : LOW);
}
