#include "digital_io_int.h"

uint32_t dio_pin(Pin_OfInteraction pin) {
    Pinx_FriendPin pinselect;
    pinselect = pin_friendpin_sel[pin];
    return pinselect;
}
uint32_t dio_port(uint32_t port) {
    uint32_t Port = 0; 
    switch (port) {
        case 0xA:
            Port = PORTA;
            break;
        case 0xB:
            Port = PORTB;
            break;
        case 0xC:
            Port = PORTC;
            break;
        case 0xD:
            Port = PORTD;
            break;
        default:
            break;
    }
    return Port;
}

uint32_t dio_mode(Mode_OfInteraction direction) {
    PortMode_FriendMode mode_select;
    mode_select = mode_friendmode_sel[direction];
    return mode_select;
}

uint32_t dio_speed(Mode_OfInteraction direction) {
    uint32_t direction_mode = 0;
    if(direction >= MODE_OFFSETOUTPUT) {
        direction_mode = OUTPUT_50MHZ;
    } else
    {
        direction_mode = INPUT_MODE;
    }
    
    return direction_mode;
}

uint32_t dio_rcc(uint32_t rcc) {
    switch (rcc) {
    case 0xA:
        rcc = GPIOA_RCC;
        break;
    case 0xB:
        rcc = GPIOB_RCC;
        break;
    case 0xC:
        rcc = GPIOC_RCC;
        break;
    case 0xD:
        rcc = GPIOD_RCC;
        break;
    default:
        break;
    }
    return rcc;
}

void open_dio(uint32_t hex_PortPin, Mode_OfInteraction direction) {
    Gpiox_Handler io_handler;
    io_handler.Port = dio_port(BM_HPORT(hex_PortPin));
    io_handler.Pin = dio_pin(BM_HPIN(hex_PortPin));
    io_handler.Mode = dio_mode(direction);
    io_handler.Speed = dio_speed(direction);
    io_handler.Remap1_JTAG = 0;
    io_handler.Remap2_Funtion = 0;
    io_handler.Gpio_Rcc = dio_rcc(BM_HPORT(hex_PortPin));
    gpio_setup(&io_handler);
}

void write_dio(uint32_t hex_PortPin, uint32_t level) {
    uint32_t Port, Pin;
    Port = dio_port(BM_HPORT(hex_PortPin));
    Pin = dio_pin(BM_HPIN(hex_PortPin));
    gpio_write(Port, Pin, level);

}

uint32_t read_dio(uint32_t hex_PortPin ) {
    uint32_t Port, Pin;
    Port = dio_port(BM_HPORT(hex_PortPin) );
    Pin = dio_pin(BM_HPIN(hex_PortPin));
    return gpio_read(Port, Pin);
}


