#include "uc_timer.h"


/**
    Enables interrupts(update events) on timer3 every 0.1 seconds.
    TIM_SR_UIF, update event flag must be cleared by software and timer must be re-enabled(TIMx_CR1->CEN).
*/
void setup_timer(Timx_Handler* tim_init_struct) {
    rcc_periph_clock_enable(tim_init_struct->Tim_Rcc);
    timer_set_mode(tim_init_struct->Module,
        TIM_CR1_CKD_CK_INT,          //clock divider = 1
        TIM_CR1_CMS_EDGE,            //Edge-aligned for upcounting
        tim_init_struct->Mode1);             //upcounting
    if(tim_init_struct->Mode2 == ONE_SHOT){
        timer_one_shot_mode(tim_init_struct->Module);       //timer stops at each update(overflow/underflow, CEN bit must be set by software to resume operation)
        timer_enable_preload(tim_init_struct->Module);      //counter is reloaded from TIMx_ARR(auto reload register) 
    }
    timer_set_prescaler(tim_init_struct->Module, 894);  //Sysclk = 8Mhz, por tanto TIMx clock = 10Khz(checar nota refman:clockTree)
    timer_set_period(tim_init_struct->Module, 894);    //Initial period is  10hz, determines frequency ((sysclk/prescaler) / period) 


    timer_enable_counter(tim_init_struct->Module);      //Start counting
}

void calculate_pomodoro_timer(uint32_t time_ms, Timx_Handler* timset_prescaler_period) {
    int count_divider = 1;
    uint32_t pomodoro_timer = ((SYSCLCK_DEFAULT * time_ms)/MILISECONDS_IN_SECOND);
    while (pomodoro_timer > MAX_2BYTES) {
        pomodoro_timer = pomodoro_timer>> 2;
        count_divider = count_divider << 1;
    }
    timset_prescaler_period->Period = pomodoro_timer;
    timset_prescaler_period->Prescaler = count_divider;
}

int32_t get_timer_status(uint32_t Module) {
    return timer_get_flag(Module, TIM_SR_UIF);
}

void clear_timer_status(uint32_t Module) {
    timer_clear_flag(Module, TIM_SR_UIF);
}

void enable_timer_status(uint32_t Module) {
    timer_enable_counter(Module);
}
