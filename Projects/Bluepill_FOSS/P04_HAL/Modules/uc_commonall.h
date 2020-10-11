#ifndef UC_COMMONALL_H
#define UC_COMMONALL_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>

#define BM_SET(uWord, uMask)	(uWord |= (uMask))		//Sets the masked bits in an specified word
#define BM_CLEAR(uWord, uMask)	(uWord &= ~(uMask))  	//Disable the masked bits in a word
#define BM_CHECK(uWord, uMask)	(uWord & (uMask))	    //Verifies masked bits in a word

extern volatile uint32_t sys_event_flags;      /*!< Captures events to avoid long routines on isr */
    #define NO_EVENT        0x00
    #define ADC_CONVERSION  0x01    /*!< ADC has completed a conversion, reading is required */
    #define UART_RECEIVING  0x02    /*!< UART has new data, reading is required*/
   

#endif