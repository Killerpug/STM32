
#include "registermap.h"	//Register mapping

/* In order to blink a led, there are few steps to complete:
    1) Map the registers. Register mapping memory is on registermap.h
    2) Enable System clock. By default is HSI 8Mhz
    3) Enable peripheral clock on PORT where pin is located. 
        On page 47 of manual reference, section 3.1 System architecture:
        APB1 peripheral clock is in charge of:
            -GPIOA-GPIOC    -AFIO   -USART1     -SPI1   -ADC1-3     -TIM1
    4) access and toggle pin.
*/
static void gpio_setup(void) {

	RCC.APB2ENR |= 1 << 4;		//Enable APB2 clock on PORTC.
	GPIOC.CHR &= ~(0xFF << 20);	//reset configuration(set registers to 0).
	GPIOC.CHR |= 0x3 << 20;		//PIN13 configured as: Output Mode 10Mhz max,general push-pull.
}

int
main(void) {
	int i;

	gpio_setup();

	for (;;) {
		GPIOC.ODR ^= 1 << 13;		// Toggle LED PC13
		for (i = 0; i < 1500000; i++)	// Wait
			__asm__("nop");
	}

	return 0;
}
