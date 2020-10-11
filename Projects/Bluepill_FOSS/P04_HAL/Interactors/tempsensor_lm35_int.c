#include "tempsensor_lm35_int.h"



void open_tempsensor(uint32_t peridiocity_ms) {
     //TODO: receive PORTPIN to increase modularity
    open_dio(GPIO_LM35, 0x0);  //open gpio
    Adcx_Handler adc_handler;// = (Adcx_Handler*) malloc(sizeof(Adcx_Handler));
    adc_handler.Module = ADC1_MODULE;
    adc_handler.Channel = 0;
    adc_handler.Mode1 = SINGLE_CONVERSION_REGULAR;
    adc_handler.Mode2 = ENDOFCONVERSION_IRQ;
    adc_handler.SampleSpeed = SAMPLETIME8;
    adc_handler.Adc_Rcc = ADC1_RCC;
    adc_setup(&adc_handler);
    if(peridiocity_ms > 0){
        periodic_read_request(peridiocity_ms);
    }
}


uint32_t read_tempsensor(uint32_t Channel) {
    adc_request_singread(Channel);
    while (!BM_CHECK(sys_event_flags, ADC_CONVERSION));
    return adc_read_value();
}



void periodic_read_request(uint32_t peridiocity_ms) {
    Timx_Handler tim_init_struct;
    tim_init_struct.Module = TIM3_MODULE;
    tim_init_struct.Prescaler = 0;
    tim_init_struct.Period = 0;
    tim_init_struct.Mode1 = UPCOUNTING;
    tim_init_struct.Mode2 = ONE_SHOT;
    tim_init_struct.Tim_Rcc = TIM3_RCC;
    calculate_pomodoro_timer(peridiocity_ms, &tim_init_struct);
    setup_timer(&tim_init_struct);
}

void rotatory_filter(uint32_t new_value) {
    LM35_filtered_Tempvalue[sample_counter % 4] = new_value;   
    uint16_t temporal_temp = 0;
    for (int i = 0; i < FILTER_ARRAY_SIZE; i++) {
        temporal_temp += LM35_filtered_Tempvalue[i];
    }
    LM35_average_Tempvalue = temporal_temp/4;
}

int32_t get_timeup_status(Module_OfInteraction module_number) {
    Tim_FriendModule Module;
    Module = tim_friendmodule_sel[module_number];
    uint32_t tempstatus = get_timer_status(Module);
    if (tempstatus) {   //if module is stopped, reset it
        clear_timer_status(Module);
        enable_timer_status(Module);
    }
    return tempstatus;
}
