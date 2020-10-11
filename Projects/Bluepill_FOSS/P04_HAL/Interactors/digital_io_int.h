#ifndef DIGITAL_IO_INT_H
#define DIGITAL_IO_INT_H
#include "global_system_int.h"
#include "../Modules/uc_gpio.h"

#define MASK_PIN 0x0F
#define POSITION_PORT 4
#define DIOMODE_ALTFNCTOUTPP 4

#define BM_HPORT(port_pin) (port_pin >> POSITION_PORT)
#define BM_HPIN(port_pin) (port_pin & MASK_PIN)
#define MODE_OFFSETOUTPUT 3

const Pinx_FriendPin pin_friendpin_sel[] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15};

typedef enum {
    PIN_0 = 0,
    PIN_1 = 1,
    PIN_2 = 2,    
    PIN_3 = 3,
    PIN_4 = 4,
    PIN_5 = 5,
    PIN_6 = 6,
    PIN_7 = 7,
    PIN_8 = 8,
    PIN_9 = 9,
    PIN_10 = 10,
    PIN_11 = 11,
    PIN_12 = 12,
    PIN_13 = 13,
    PIN_14 = 14,
    PIN_15 = 15
} Pin_OfInteraction;

const Portx_FriendPort port_friendport_sel[] = {PORTA, PORTB, PORTC, PORTD};
typedef enum {
    PORT_A = 0,
    PORT_B = 1,
    PORT_C = 2,
    PORT_D = 3
} Port_OfInteraction;

const PortMode_FriendMode mode_friendmode_sel[] = { ANALOG_IN, DIGFLOATING_IN, DIGPULLUPDOWN_IN, DIGPUSHPULL_OUT, DIGOPENDRAIN_OUT, DIGALTFN_PUSHPULL_OUT, DIGALTFN_OPENDRAIN_OUT };
typedef enum {
    ANALOG_IN__I = 0,
    DIGFLOATING_IN__I = 1,
    DIGPULLUPDOWN_IN__I = 2,
    DIGPUSHPULL_OUT__I = 3,
    DIGOPENDRAIN_OUT__I = 4,
    DIGALTFN_PUSHPULL_OUT__I = 5,
    DIGALTFN_OPENDRAIN_OUT__I = 6
} Mode_OfInteraction;

uint32_t dio_pin(Pin_OfInteraction pin);
uint32_t dio_port(uint32_t port);
uint32_t dio_mode(Mode_OfInteraction direction);
uint32_t dio_speed(Mode_OfInteraction direction);
uint32_t dio_rcc(uint32_t rcc);

void open_dio(uint32_t hex_PortPin, Mode_OfInteraction direction);
void write_dio(uint32_t hex_PortPin, uint32_t level);
uint32_t read_dio(uint32_t hex_PortPin);
void close_dio(uint32_t hex_PortPin);

#endif