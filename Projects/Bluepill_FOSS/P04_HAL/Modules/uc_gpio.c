#include "uc_gpio.h"



void gpio_setup(Gpiox_Handler* gpio_init_struct) {
    //Enable peripheral clock 
    rcc_periph_clock_enable(gpio_init_struct->Gpio_Rcc);

     // Configure threshold GPIO LEDs
    gpio_set_mode(
        gpio_init_struct->Port, 
        gpio_init_struct->Speed,           // High speed
        gpio_init_struct->Mode,
        gpio_init_struct->Pin);             // output LEDs
}

void gpio_write(uint32_t  port, uint16_t  pin, uint32_t data) {
    if(data) gpio_set(port, pin);
    else gpio_clear(port, pin);
}

uint32_t gpio_read(uint32_t  port, uint16_t  pin){
    return gpio_get(port, pin);
}