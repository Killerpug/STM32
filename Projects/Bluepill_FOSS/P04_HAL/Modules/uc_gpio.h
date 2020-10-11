#ifndef UC_GPIO_H
#define UC_GPIO_H

#include <libopencm3/stm32/gpio.h>
#include "uc_commonall.h"
//  SELECT INTERFACE TYPE
#define SELECTED_UCGPIO_INTERFACE FRIENDLY_GPIO_INTERFACE
#define FRIENDLY_GPIO_INTERFACE 1
#define REGISTERLEVEL_GPIO_INTERFACE 2
#if(SELECTED_UCGPIO_INTERFACE == FRIENDLY_GPIO_INTERFACE)

    typedef enum {
        PORTA = GPIOA,
        PORTB = GPIOB,
        PORTC = GPIOC,
        PORTD = GPIOD
    } Portx_FriendPort;

    typedef enum {
        PIN0 = GPIO0,
        PIN1 = GPIO1,
        PIN2 = GPIO2,
        PIN3 = GPIO3,
        PIN4 = GPIO4,
        PIN5 = GPIO5,
        PIN6 = GPIO6,
        PIN7 = GPIO7,
        PIN8 = GPIO8,
        PIN9 = GPIO9,
        PIN10 = GPIO10,
        PIN11 = GPIO11,
        PIN12 = GPIO12,
        PIN13 = GPIO13,
        PIN14 = GPIO14,
        PIN15 = GPIO15,
        ALL = GPIO_ALL  
    } Pinx_FriendPin;

    typedef enum {
        ANALOG_IN = GPIO_CNF_INPUT_ANALOG,
        DIGFLOATING_IN = GPIO_CNF_INPUT_FLOAT,
        DIGPULLUPDOWN_IN = GPIO_CNF_INPUT_PULL_UPDOWN,  
        DIGPUSHPULL_OUT = GPIO_CNF_OUTPUT_PUSHPULL,
        DIGOPENDRAIN_OUT = GPIO_CNF_OUTPUT_OPENDRAIN,
        DIGALTFN_PUSHPULL_OUT = GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
        DIGALTFN_OPENDRAIN_OUT = GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN 
    } PortMode_FriendMode;

    typedef enum {
        INPUT_MODE = GPIO_MODE_INPUT,
        OUTPUT_2MHZ = GPIO_MODE_OUTPUT_2_MHZ,
        OUTPUT_10MHZ = GPIO_MODE_OUTPUT_10_MHZ,
        OUTPUT_50MHZ = GPIO_MODE_OUTPUT_50_MHZ
    } PortSpeed_FriendSpeed;

    typedef enum {
        FULL = AFIO_MAPR_SWJ_MASK,
        FULL_NOJNTRST = AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST,
        SW_NOJTAG = AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
        NOSW_NOJTAG = AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF
    } JTAG_FriendOptions;
    typedef enum {
        SPI1 = AFIO_MAPR_SPI1_REMAP,
        I2C1 = AFIO_MAPR_I2C1_REMAP,
        PORTD0_D1 = AFIO_MAPR_PD01_REMAP,   
        TIM5_CH4 = AFIO_MAPR_TIM5CH4_IREMAP,
        ADC1_EXT_INJ = AFIO_MAPR_ADC1_ETRGINJ_REMAP,
        ADC1_EXT_TRGR = AFIO_MAPR_ADC1_ETRGREG_REMAP,
        ADC2_EXT_INJ = AFIO_MAPR_ADC2_ETRGINJ_REMAP,
        ADC2_EXT_TRGR = AFIO_MAPR_ADC2_ETRGREG_REMAP,
        TIM2_NOREMAP = AFIO_MAPR_TIM2_REMAP_NO_REMAP,
        TIM2_fULL_REMAP = AFIO_MAPR_TIM2_REMAP_FULL_REMAP
    } Function_FriendRemap;
    typedef struct {
        Function_FriendRemap Function;
        JTAG_FriendOptions JTAG;
    } Remap_FriendOptions;
     
    typedef enum {
        GPIOA_RCC = RCC_GPIOA,
        GPIOB_RCC = RCC_GPIOB,
        GPIOC_RCC = RCC_GPIOC,
        GPIOD_RCC = RCC_GPIOD
    } Gpio_FriendRcc;

    typedef struct {
        Portx_FriendPort Port;
        Pinx_FriendPin Pin;
        PortMode_FriendMode Mode;
        PortSpeed_FriendSpeed Speed;
        Remap_FriendOptions SelectRemap;
        Gpio_FriendRcc Gpio_Rcc;
    } Gpiox_FriendlyHandler;

#elif (SELECTED_UCGPIO_INTERFACE == REGISTERLEVEL_GPIO_INTERFACE)

#else 
    #error "USART interface type not defined. Please select an interfacing connection type"

#endif // FRIENDLY_INTERFACE

typedef struct {
    uint32_t Port;
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Speed;
    uint32_t Remap1_JTAG;
    uint32_t Remap2_Funtion;
    uint32_t Gpio_Rcc;
} Gpiox_Handler;

void gpio_write(uint32_t  port, uint16_t  pin, uint32_t data);
uint32_t gpio_read(uint32_t  port, uint16_t  pin);
void gpio_setup(Gpiox_Handler* gpio_init_struct);
#endif