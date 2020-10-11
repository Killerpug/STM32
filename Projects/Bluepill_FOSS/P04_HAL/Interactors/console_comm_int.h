#ifndef CONSOLE_COMM_INT_H
#define CONSOLE_COMM_INT_H

#include "global_system_int.h"
#include "../Modules/uc_usart.h"
#include "digital_io_int.h"


const Usart_FriendModule usart_friendmodule_sel[] = { USART1_MOD, USART2_MOD, USART3_MOD, UART4_MOD, UART5_MOD};
typedef enum {
    MODULE0 = 0,
    MODULE1 = 1,
    MODULE2 = 2,    
    MODULE3 = 3,
    MODULE4 = 4,
    MODULE5 = 5
} UsartModule_OfInteraction;





void open_comm(void);
void write_comm(UsartModule_OfInteraction comm, char* data);
char receive_comm(void);
uint32_t usart_mod(UsartModule_OfInteraction module_number);
uint32_t usart_rcc(uint32_t rcc);

#endif