#include "uc_usart.h"

char usart_RX_buffer[RX_buffsize] = { '\0' };
void usart_setup(Usartx_Handler* usart_init_struct) {
    rcc_periph_clock_enable(usart_init_struct -> Usart_Rcc);
 
    usart_set_baudrate(usart_init_struct->Usart_Module, usart_init_struct->Baud_Rate);
    usart_set_databits(usart_init_struct->Usart_Module, usart_init_struct->Word_Lenght);
    usart_set_stopbits(usart_init_struct->Usart_Module, usart_init_struct->Stop_Bits);
    usart_set_mode(usart_init_struct->Usart_Module, usart_init_struct->Mode1);
    usart_set_parity(usart_init_struct->Usart_Module, usart_init_struct->Parity);
    usart_set_flow_control(usart_init_struct->Usart_Module, usart_init_struct->Hw_Flow_Control);

    //interrupts
    if(usart_init_struct->Mode2 == ISR_RX) {
        usart_enable_rx_interrupt(usart_init_struct->Usart_Module);  //Enable interrupts on reception
        enable_interrupts(usart_init_struct->Usart_Module);
        
    }
   
    usart_enable(usart_init_struct->Usart_Module);
}

void usartx_send(uint32_t module, char* pucBuffer) {
    while (*pucBuffer)
    {
        usart_send_blocking(module, *pucBuffer++);
        while (usart_get_flag(module, USART_SR_TC) == 0) {      //wait to complete transmission

        }
    }
}

void usart_receive(void) {
    usart_RX_buffer[0] = usart_recv_blocking(USART1);        //Clears reciever flags RXNE(receiver enable) and RXNEIE(Receiver Interrupt Enable)
    BM_SET(sys_event_flags, UART_RECEIVING);
}

/**
    Interrupt routine, declaration found on stm32/f1/nvic.h(cm3>Cortex-M NVIC Defines>user interrupts).
*/
void usart1_isr(void) {
    usart_receive();
}

void enable_interrupts(uint32_t module) {
 switch (module) {
    case USART1_MOD:
        nvic_enable_irq(NVIC_USART1_IRQ);   //Enable interrupt on USART1 module
        break;
    case USART2_MOD:
        nvic_enable_irq(NVIC_USART2_IRQ);
        break;
    case USART3_MOD:
        nvic_enable_irq(NVIC_USART3_IRQ);
        break;
    case UART4_MOD:
        nvic_enable_irq(NVIC_UART4_IRQ);
        break;
    case UART5_MOD:
        nvic_enable_irq(NVIC_UART5_IRQ);
        break;
    default:
        break;
    }

}
// Copyright 2020 Eduardo Tapia 