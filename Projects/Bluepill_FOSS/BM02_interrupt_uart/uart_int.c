/* This UART works at 9600 bps, transmission causes interrupt and receptions are also via interrupts.
This example receives characters from user until "Enter" key is pressed. Capitalization of letters is inverted.
"end" word terminates the program.
*/

#include "registermap.h"	//Register mapping for stm32f1

#define CARRIER_RETURN 13		// Ascii for Enter/Carrier Return
#define BUF_SIZE 150
char bufferTX[BUF_SIZE] = { '\0' };
char bufferRX[BUF_SIZE] = { '\0' };
// Data counter
static uint8_t usart_rec_data = 0;				// Counts the number of characters received, cleared by "enter"/CR
int PROGRAM_ACTIVE = 1;							// Allows/terminates program execution
int main(void);
void usart1_handler(void);
void enable_interrupt(IRQn_type IRQn);

/** Letter case inversion
 * Invert lowercase <-> uppercase
 */ 
static void inverse_caps() {
	for( int i = 0; i < usart_rec_data ; i++) {// Iterate for every received data 
		char letter = bufferRX[i];
		if (letter >= 65 && letter <= 90) {		// Capital letter
			bufferRX[i] = letter + 32;
		} else if (letter >= 96 && letter <= 122){		// Non-Capital letter
			bufferRX[i] = letter - 32;
		}
	}
}

/** Program terminator checker
 * Checks if the command "end" has been received and in that case sends the signal to end the program
 */ 
static void check_end(){
	if (usart_rec_data >= 2){							// end command has at least 3 letters
		for( int i = 0; i < usart_rec_data-2 ; i++) {	// Iterate for every received data 
			if (bufferRX[i] == 'e') {					// Capital letter
				if (bufferRX[i+1] == 'n') {
					if (bufferRX[i+2] == 'd') {
						PROGRAM_ACTIVE = 0;
					}
				}
			} 
		}
	}

}


/** Vector Table
 * Attribute puts table in beginning of .vector section which is the beginning of .text section in the linker script.
 * This vector table calls the corresponding handler, add the function call to appropiate IRQ_Handler number.
 * Vector table can be found on page 197 in RM0008 in combination with page 33 of PM0056
*/
uint32_t (* const vector_table[]) __attribute__ ((section(".vectors"))) = {
	(uint32_t *) STACKINIT,         /* 0x000 Stack Pointer                   */
	(uint32_t *) main,                   /* 0x004 Reset                           */
	0,                              /* 0x008 Non maskable interrupt          */
	0,                              /* 0x00C HardFault                       */
	0,                              /* 0x010 Memory Management               */
	0,                              /* 0x014 BusFault                        */
	0,                              /* 0x018 UsageFault                      */
	0,                              /* 0x01C Reserved                        */
	0,                              /* 0x020 Reserved                        */
	0,                              /* 0x024 Reserved                        */
	0,                              /* 0x028 Reserved                        */
	0,                              /* 0x02C System service call             */
	0,                              /* 0x030 Debug Monitor                   */
	0,                              /* 0x034 Reserved                        */
	0,                              /* 0x038 PendSV                          */
	0,                              /* 0x03C System tick timer               */
	0,                              /* 0x040 Window watchdog                 */
	0,                              /* 0x044 PVD through EXTI Line detection */
	0,                              /* 0x048 Tamper                          */
	0,                              /* 0x04C RTC global                      */
	0,                              /* 0x050 FLASH global                    */
	0,                              /* 0x054 RCC global                      */
	0,                              /* 0x058 EXTI Line0                      */
	0,                              /* 0x05C EXTI Line1                      */
	0,                              /* 0x060 EXTI Line2                      */
	0,                              /* 0x064 EXTI Line3                      */
	0,                              /* 0x068 EXTI Line4                      */
	0,                              /* 0x06C DMA1_Ch1                        */
	0,                              /* 0x070 DMA1_Ch2                        */
	0,                              /* 0x074 DMA1_Ch3                        */
	0,                              /* 0x078 DMA1_Ch4                        */
	0,                              /* 0x07C DMA1_Ch5                        */
	0,                              /* 0x080 DMA1_Ch6                        */
	0,                              /* 0x084 DMA1_Ch7                        */
	0,                              /* 0x088 ADC1 and ADC2 global            */
	0,                              /* 0x08C CAN1_TX                         */
	0,                              /* 0x090 CAN1_RX0                        */
	0,                              /* 0x094 CAN1_RX1                        */
	0,                              /* 0x098 CAN1_SCE                        */
	0,                              /* 0x09C EXTI Lines 9:5                  */
	0,                              /* 0x0A0 TIM1 Break                      */
	0,                              /* 0x0A4 TIM1 Update                     */
	0,                              /* 0x0A8 TIM1 Trigger and Communication  */
	0,                              /* 0x0AC TIM1 Capture Compare            */
	0,                              /* 0x0B0 TIM2                            */
	0,     							/* 0x0B4 TIM3                            */
	0,                              /* 0x0B8 TIM4                            */
	0,                              /* 0x0BC I2C1 event                      */
	0,                              /* 0x0C0 I2C1 error                      */
	0,                              /* 0x0C4 I2C2 event                      */
	0,                              /* 0x0C8 I2C2 error                      */
	0,                              /* 0x0CC SPI1                            */
	0,                              /* 0x0D0 SPI2                            */
	(uint32_t *) usart1_handler,    /* 0x0D4 USART1                          */
	0,                              /* 0x0D8 USART2                          */
	0,                              /* 0x0DC USART3                          */
	0,                              /* 0x0E0 EXTI Lines 15:10                */
	0,                              /* 0x0E4 RTC alarm through EXTI line     */
	0,                              /* 49  USB OTG FS Wakeup through EXTI  */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* -   Reserved                        */
	0,                              /* 57  TIM5                            */
	0,                              /* 58  SPI3                            */
	0,                              /* 59  USART4                          */
	0,                              /* 60  USART5                          */
	0,                              /* 61  TIM6                            */
	0,                              /* 62  TIM7                            */
	0,                              /* 63  DMA2_Ch1                        */
	0,                              /* 64  DMA2_Ch2                        */
	0,                              /* 65  DMA2_Ch3                        */
	0,                              /* 66  DMA2_Ch4                        */
	0,                              /* 67  DMA2_Ch5                        */
	0,                              /* 68  Ethernet                        */
	0,                              /* 69  Ethernet wakeup                 */
	0,                              /* 70  CAN2_TX                         */
	0,                              /* 71  CAN2_RX0                        */
	0,                              /* 72  CAN2_RX1                        */
	0,                              /* 73  CAN2_SCE                        */
	0,                              /* 74  USB OTG FS                      */
};

/** Enables clocks on used modules
*/ 
static void gpio_setup(void) {
	RCC->APB2ENR |= (1<<2 | 1 << 4);		// Enable APB2 clock on PORTA and PORTC.
	// Debug LED
	GPIOC->CRH &= ~(0xFF << 20);			// Reset configuration(set registers to 0).
	GPIOC->CRH |= 0x3 << 20;				// PIN13 configured as: Output Mode 10Mhz max,general push-pull.
}

/** Single byte communication
 * Sends 1 byte at a time.
*/
static void USARTSend( char* pucBuffer) {
	while (BM_CHECK(USART1->SR,1<<7) == 0) {      // TXE: wait to complete(Empty for) transmission
    }
    USART1->DR = *pucBuffer++;
}

/** Receives a byte and stores it on bufferRx. Whenever enter key is pressed prints all data.
 * Also calls function inverse_caps() to invert caps and check_end() to terminate program in case of "end" string.
 */ 
static void USARTReceive(void) {
	// Receive new character
	while (BM_CHECK(USART1->SR,1<<5) == 0) {}       // RXNE: wait for valid content on Receive Data Register(Not Empty)
	bufferRX[usart_rec_data] = USART1->DR;       	// Reading DR Clears reciever flags RXNE(receiver enable) and RXNEIE(Receiver Interrupt Enable)
	__asm__("nop");

	// Counts data until user presses enter and at that moment replicates all data(with inverted caps) and clear counter.
	if(bufferRX[usart_rec_data] != CARRIER_RETURN){ // any character but "enter" has been received
		usart_rec_data++;							// Prepares bufferRx for next data. For performance, this goes first.				
	} else {										// User has pressed "enter"/CR key
		check_end();								// Check if end command was entered
		inverse_caps();								// Invert caps
		for( int i = 0; i < usart_rec_data ; i++) { // Iterate for every received data 
			USARTSend(&bufferRX[i]);				// Send each byte via UART
			bufferRX[i] = '\0';						// Clear buffer
		}	
	}						// Otherwise prepare for next data reception
}

/*
 * Enable given interrupt
 *
 * Each ISER {0-7} holds 32 interrupts. Thus take mod32 of the given interrupt
 *   to choose the ISER number (ISER[0] for IRQn 0-31, and ISER[1] for IRQn 32-63 ..)
 *   Then, enable the given bit on that register based on the remainder of the mod.
 */
void enable_interrupt(IRQn_type IRQn)
{
	NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/** Configures Usart as
 * TX:	A9
 * RX:	A10
 * Baud:	9600
 * Databits: 8 data bits(default)
 * Stopbits: 1 stop bit(default)
 * Parity: no parity(default)
 * Flow-control: No flow control
 * Other: Interruptions
 */
static void open_usart(void) {
	RCC->APB2ENR |= 1<<14; 						  // Enable USART clock, APB2 == HSI == sysclock == 8 MHZ by default.
	GPIOA->CRH &= 0xFFFFF00F;				      // Clear GPIO register
	GPIOA->CRH |= 0x8B<<4;	                      // Rx(A9) as input and Tx(A9) configured as: Output Mode 50Mhz, output push-pull. 
    USART1->BRR = 0x34<<4 | 0x1;                  // Baudrate = apb2clk/(16*BRR). BRR = 52.0625(given by: 0x34 + 1/16). baud = 9603.84.
    // Interrupts
    USART1->CR1 |= 1<<5 ;                 	 	  // Enable RX interrupts on reception
    enable_interrupt(USART1_IRQn);				  // Globar interrupt enabler on USART1
	// Enable module
    USART1->CR1 |= (1<<13 | 1<<2 | 1<<3);         // Enable USART module, enable Tx and Rx, 8 data bits(default),.
}

int main(void) {
	PROGRAM_ACTIVE = 1;
    gpio_setup();
	open_usart();
	GPIOC->ODR ^= 1 << 13;				// Toggle LED PC13
	for (int i = 0; i < 1500000; i++){	// Wait
		__asm__("nop");
	}
    
	while ( PROGRAM_ACTIVE != 0) {			// Runs as long as program is not shutdown via "end" command
		GPIOC->ODR ^= 1 << 13;				// Toggle LED PC13
		for (int i = 0; i < 1500000; i++)	// Wait
			__asm__("nop");
	}

	return -1;
}

/**
    Interrupt routine, declaration found on stm32/f1/nvic.h(cm3>Cortex-M NVIC Defines>user interrupts).
*/
void usart1_handler(void) {
	if(BM_CHECK(USART1->SR,1<<5)){					// Interrupt caused on reception
		USARTReceive();
	} 
}
