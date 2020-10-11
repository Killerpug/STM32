#ifndef registermap_H
#define registermap_H
/*RegisterMap.h
	Is a header that defines register mapping of stm32f103 into its memory locations.
	Permitting register access to microcontroller modules like peripherals, clocks, modules...
*/
// GPIO registers
struct gpio_reg {
	volatile unsigned long CRL;			//Configuration pins 0-7
	volatile unsigned long CHR;			//Configuration pins 8-15
	volatile unsigned long IDR;
	volatile unsigned long ODR;			//Output Data Register. Permits writing onto an (output) port.
	volatile unsigned long BSRR;
	volatile unsigned long BRR;
	volatile unsigned long LCKR;
};
#define GPIOA (*(struct gpio_reg *) 0x40010800)
#define GPIOB (*(struct gpio_reg *) 0x40010C00)
#define GPIOC (*(struct gpio_reg *) 0x40011000)
#define GPIOD (*(struct gpio_reg *) 0x40011400)
#define GPIOE (*(struct gpio_reg *) 0x40011800)
#define GPIOF (*(struct gpio_reg *) 0x40011C00)
#define GPIOG (*(struct gpio_reg *) 0x40012000)


 // Alternate function registers
struct afio_reg {
	volatile unsigned long EVCR;
	volatile unsigned long MAPR;
	volatile unsigned long EXTICR1;
	volatile unsigned long EXTICR2;
	volatile unsigned long EXTICR3;
	volatile unsigned long EXTICR4;
	volatile unsigned long MAPR2;
};
#define AFIO (*(struct gpio_reg *) 0x40010000)

// Reset an Clock Control registers
struct rcc_reg {
	volatile unsigned long CR;
	volatile unsigned long CFGR;
	volatile unsigned long CIR;
	volatile unsigned long APB2RSTR;
	volatile unsigned long APB1RSTR;
	volatile unsigned long AHBENR;
	volatile unsigned long APB2ENR;			//Enables clock on peripherals connected to APB2(e.g. -GPIOA-GPIOC    -AFIO   -USART1     -SPI1   -ADC1-3    -TIM1)
	volatile unsigned long APB1ENR;
	volatile unsigned long BDCR;
	volatile unsigned long CSR;
};
#define RCC (*(struct rcc_reg *) 0x40021000)

#endif