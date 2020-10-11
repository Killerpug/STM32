#ifndef UC_ADC_H
#define UC_ADC_H

#include <libopencm3/stm32/adc.h>
#include "uc_commonall.h"


//  SELECT INTERFACE TYPE
#define SELECTED_UCADC_INTERFACE FRIENDLY_ADC_INTERFACE
#define FRIENDLY_ADC_INTERFACE 1
#define REGISTERLEVEL_ADC_INTERFACE 2
#if(SELECTED_UCADC_INTERFACE == FRIENDLY_ADC_INTERFACE)

    typedef enum {
        ADC1_MODULE = ADC1
    } Adc_FriendModule;

    typedef enum {
        SINGLE_CONVERSION_REGULAR = 1,
        SINGLE_CONVERSION_INJECTED,
        CONTINUOUS_CONVERSION_REGULAR,
        CONTINUOUS_CONVERSION_INJECTED
    } AdcMode_FriendMode1;

    typedef enum {
        ENDOFCONVERSION_IRQ = 1
    } AdcMode_FriendMode2;

    typedef enum {
        SAMPLETIME1 = ADC_SMPR_SMP_1DOT5CYC,
        SAMPLETIME2 = ADC_SMPR_SMP_7DOT5CYC,
        SAMPLETIME3 = ADC_SMPR_SMP_13DOT5CYC,
        SAMPLETIME4 = ADC_SMPR_SMP_28DOT5CYC,
        SAMPLETIME5 = ADC_SMPR_SMP_41DOT5CYC,
        SAMPLETIME6 = ADC_SMPR_SMP_55DOT5CYC,
        SAMPLETIME7 = ADC_SMPR_SMP_71DOT5CYC,
        SAMPLETIME8 = ADC_SMPR_SMP_239DOT5CYC
    } AdcSpeed_FriendSampleTime;
    
    typedef enum {
         ADC1_RCC = RCC_ADC1 
    } Adc_FriendRcc;

    typedef struct {
        Adc_FriendModule Module;
        AdcMode_FriendMode1 Mode1;
        AdcMode_FriendMode2 Mode2;
        AdcSpeed_FriendSampleTime Speed;
        Adc_FriendRcc Adc_Rcc;
    } Adcx_FriendlyHandler;

#elif (SELECTED_UCADC_INTERFACE == REGISTERLEVEL_ADC_INTERFACE)

#else 
    #error "ADC interface type not defined. Please select an interfacing connection type"

#endif // FRIENDLY_INTERFACE
volatile uint32_t adc_read = 0;
typedef struct {
    uint32_t Module;
    uint32_t Channel;
    uint32_t Mode1;           //conversion
    uint32_t Mode2;           //interrupts
    uint32_t SampleSpeed;     //cycles
    uint32_t Adc_Rcc;
} Adcx_Handler;


void adc_setup(Adcx_Handler* adc_init_struct);
void adc_request_singread(uint8_t on_channel);
uint32_t adc_read_value(void);
#endif