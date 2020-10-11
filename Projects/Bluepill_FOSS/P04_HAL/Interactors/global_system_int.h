#ifndef GLOBAL_SYSTEM_INT_H
#define GLOBAL_SYSTEM_INT_H

#include "../Modules/uc_commonall.h"


volatile char app_event_flags = 0;      /*!< Captures events to avoid long routines on isr */
    #define USER_CUSTOMMODE 0x04    /*!< User has requested enter to custom mode */
    #define MAX_TEMP_REACHED 0x08
    #define MIN_TEMP_REACHED 0x10

#define SET 1
#define CLEAR 0
#define BUF_SIZE 150

#define FILTER_ARRAY_SIZE 4      /*!< Elements on rotatory filter for ADC readings */
#define GPIO_LM35 0xA7             /*!< Temperature sensor LM35 is connected to GPIOA 7 */
#define CHANNEL_LM35 7

#define USART1_TX 0XA9
#define USART1_Module 0x01


/**Timer clock prescaler (TIM.PSC)
    TimX.clk = Sysclk/Tim.PSC
    Given a Sysclk, prescaler Gen_XHz_FREQ is applied to the Timer module clock.
    TIM.PSC has a value between 0 - 0xFFFF.
*/
#define GEN_10KHZ_FREQ 800      /*!< When Sysclk = 8Mhz(default), (Sysclk/GEN_10KHZ_FREQ) = TIMx clock = 10Khz */

/**Timer Period formula (TIMx_ARR)
   @note period(s) = 1/(((sysclk/Tim.PSC) / period_counter)), where:
        - Tim.PSC is the prescaler value of the Timer module, between 0 - 0xFFFF.
        - Period_counter is the value of the TIMx_ARR counter register in clock ticks, between 0 - 0xFFFF.
*/
#define GEN_10HZ_PERIOD 1000     //*!< Given Sysclk = 8Mhz and prescaler = GEN_10KHZ_FREQ, The period(s) = 0.1 s */

/** Temperature sensor readings and bounds
    Readings are performed via ADC with 12-bit resolution(0 - 4095), which means steps of 805uV(due to 3.3V range)
    LM35 is connected as Basic Centigrade Temperature Sensor, which has:
        - Range from 2-150�C. @note voltage supply must be 4-20V for proper function of LM35
        - increments of 10mv per 1�C
    @note TRIGGER_VALUE = (temperature(in 10mV increments))*(4095/3.3)
*/
double trigger_lowtemp = 298;     /*!< equivalent to 240mV which is 24.0�C */
double trigger_hightemp = 347;     /*!< Equivalent to 280mV which is 28.0�C */

char bufferTX[BUF_SIZE] = { '\0' };
volatile char bufferRX[1] = { '\0' };
uint32_t status_counter = 0;            /*!< Allows tracking of time, updates every 0.1s. Also, helps averaging temperature*/
uint8_t read_channel = GPIO_LM35;

#define timeruser 0x03





#endif