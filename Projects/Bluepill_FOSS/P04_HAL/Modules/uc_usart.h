
#ifndef HAL_UC_USART_H
#define HAL_UC_USART_H


#include <libopencm3/stm32/usart.h>
#include "uc_commonall.h"
/////////////SELECT INTERFACE TYPE///////////////////////////
/////////////////////////////////////////////////////////////
#define SELECTED_UCUSART_INTERFACE FRIENDLY_INTERFACE
#define RX_buffsize 100
#define TX_buffsize 100
////////////////////////////////////////////////////////////
extern char usart_RX_buffer[RX_buffsize];
extern char usart_TX_buffer[TX_buffsize];
#define FRIENDLY_INTERFACE 1
#define REGISTERLEVEL_INTERFACE 2

#if (SELECTED_UCUSART_INTERFACE == FRIENDLY_INTERFACE)

    typedef enum {
        BRSPEED_1 = 110,
        BRSPEED_2 = 300,
        BRSPEED_3 = 600,
        BRSPEED_4 = 1200,
        BRSPEED_5 = 2400,
        BRSPEED_6 = 4800,
        BRSPEED_7 = 9600,
        BRSPEED_8 = 14400,
        BRSPEED_9 = 19200,
        BRSPEED_10 = 38400,
        BRSPEED_11 = 57600,
        BRSPEED_12 = 115200,
        BRSPEED_13 = 128000,
        BRSPEED_14 = 256000, 
        BRSPEED_CUSTOM = 1

    } Usartx_FriendBaudRate;
    
    typedef enum {
        WORD_8BITS = 8,
        WORD_9BITS = 9
    } Usart_FriendWordLength;

    typedef enum {
        USART1_MOD = USART1,
        USART2_MOD = USART2,
        USART3_MOD = USART3,
        UART4_MOD = UART4,
        UART5_MOD = UART5
    } Usart_FriendModule;

    typedef enum {
        STOPBITS_0_5 = USART_CR2_STOPBITS_0_5, 
        STOPBITS_1 =  USART_CR2_STOPBITS_1,
        STOPBITS_1_5 = USART_CR2_STOPBITS_1_5,
        STOPBITS_2  =  USART_CR2_STOPBITS_2 
    } Usart_FriendStopBits;

    typedef enum {
        NONE = USART_PARITY_NONE,
        EVEN = USART_CR1_PCE,
        ODD = (USART_CR1_PS | USART_CR1_PCE)
    } Usart_FriendParity;

    typedef enum {
        RX = USART_MODE_RX,
        TX = USART_MODE_TX,
        RXTX = USART_MODE_TX_RX  
    } Usartx_FriendMode1;

     typedef enum {
        ISR_RX = 1,
        ISR_TX,
        ISR_RXTX,
        DMA_RX,
        DMA_TX,
        DMA_RXTX   
    } Usartx_FriendMode2;

    typedef enum {
        RTS_RECIEVE =  USART_CR3_RTSE,
        CTS_TRANSMIT = USART_CR3_CTSE,
        RTS_CTS = (USART_CR3_RTSE | USART_CR3_CTSE)
    } Usart_FriendHWControl;

    typedef enum {
        USART1_RCC = RCC_USART1,
        USART2_RCC = RCC_USART2,
        USART3_RCC = RCC_USART3,
        UART4_RCC = RCC_UART4,
        UART5_RCC = RCC_UART5 
    } Usart_FriendRcc;

    typedef struct {
        Usartx_FriendBaudRate Baud_Rate;
        Usart_FriendWordLength Word_Lenght;
        Usart_FriendModule Usart_Module;
        Usart_FriendStopBits Stop_Bits;       
        Usart_FriendParity Parity;
        Usartx_FriendMode1 Mode1;
        Usartx_FriendMode2 Mode2;
        Usart_FriendHWControl Hw_Flow_Control;
        Usart_FriendRcc Usart_Rcc;
    } Usartx_FriendlyHandler;
#elif (SELECTED_UCUSART_INTERFACE == REGISTERLEVEL_INTERFACE)
#else 
    #error "USART interface type not defined. Please select an interfacing connection type"

#endif // FRIENDLY_INTERFACE


typedef struct {
    uint32_t Baud_Rate;
    uint32_t Word_Lenght;
    uint32_t Usart_Module;
    uint32_t Stop_Bits;       
    uint32_t Parity;
    uint32_t Mode1;
    uint32_t Mode2;
    uint32_t Hw_Flow_Control;
    uint32_t Usart_Rcc;
} Usartx_Handler;


void usart_setup(Usartx_Handler* usart_init_struct);
void usartx_send(uint32_t module, char* pucBuffer);
void usart_receive(void);
void enable_interrupts(uint32_t module);

#endif