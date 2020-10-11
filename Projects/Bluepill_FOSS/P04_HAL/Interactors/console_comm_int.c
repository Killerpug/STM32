#include "console_comm_int.h"

uint32_t usart_rcc(uint32_t rcc) {
    switch (rcc) {
    case 0x1:
        rcc = USART1_RCC;
        break;
    case 0x2:
        rcc = USART2_RCC;
        break;
    case 0x3:
        rcc = USART3_RCC;
        break;
    case 0x4:
        rcc = UART4_RCC;
        break;
    case 0x5:
        rcc = UART5_RCC;
        break;
    default:
        break;
    }
    return rcc;
}

uint32_t usart_mod(UsartModule_OfInteraction module_number) {
    Usart_FriendModule Module;
    Module = usart_friendmodule_sel[module_number];
    return Module;
}

void open_comm(void) {

    // GPIO_USART1_TX/GPIO13 on GPIO port A for tx
    //TODO: receive baudrate and communication module from1-5 then fill the structure with appropiated 
    //mapping of module.
    open_dio(USART1_TX, DIGALTFN_PUSHPULL_OUT); 
    Usartx_Handler hUSART = {
        BRSPEED_7,
        WORD_9BITS,
        usart_mod(MODULE0),
        STOPBITS_1,
        NONE,
        RXTX,
        ISR_RX,
        NONE,
        usart_rcc(USART1_RCC)
    };
    usart_setup(&hUSART);
}


void write_comm(UsartModule_OfInteraction comm, char* data) {
    uint32_t module = usart_mod(comm);
    usartx_send(module, data);
}
char receive_comm(void) {
    if(sys_event_flags, UART_RECEIVING) {
        BM_CLEAR(sys_event_flags, UART_RECEIVING);
        return usart_RX_buffer[0];
    }
    else return 0;
}