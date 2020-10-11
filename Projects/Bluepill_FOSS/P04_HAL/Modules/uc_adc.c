#include "uc_adc.h"

void adc_setup(Adcx_Handler* adc_init_struct) {
    //Enable peripheral clock 
    rcc_periph_clock_enable(adc_init_struct->Adc_Rcc);

    adc_power_off(adc_init_struct->Module);            //for initialization
    // This are the system defaults
    adc_set_dual_mode(ADC_CR1_DUALMOD_IND);
    adc_set_right_aligned(adc_init_struct->Module);
    if(adc_init_struct->Mode1 == SINGLE_CONVERSION_REGULAR){
        adc_set_single_conversion_mode(adc_init_struct->Module);
    }
    adc_set_sample_time_on_all_channels(adc_init_struct->Module, adc_init_struct->SampleSpeed);
    //calibration
    adc_power_on(adc_init_struct->Module);
    adc_reset_calibration(adc_init_struct->Module);
    adc_calibrate_async(adc_init_struct->Module);
    while (adc_is_calibrating(adc_init_struct->Module));
    //interruptions
    if(adc_init_struct->Mode2 == ENDOFCONVERSION_IRQ){
        nvic_enable_irq(NVIC_ADC1_2_IRQ);   //enables interrupts on ADC modules
        adc_enable_eoc_interrupt(adc_init_struct -> Module);  //Enable interrupts when finishing conversions
    }

}

void adc_request_singread(uint8_t on_channel) {
    adc_set_regular_sequence(ADC1, 1, &on_channel);
    adc_start_conversion_direct(ADC1); //start conversion ADON
}

uint32_t adc_read_value(void){
    BM_CLEAR(sys_event_flags, ADC_CONVERSION);
    return adc_read;
}

/**
    Interrupt routine, declaration found on stm32/f1/nvic.h(cm3>Cortex-M NVIC Defines>user interrupts).
*/
void adc1_2_isr(void) {    
    BM_SET(sys_event_flags, ADC_CONVERSION);
    adc_read = adc_read_regular(ADC1);     /*!<Read operation clears interrupt flag */
}
