
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
double current_f= 100; // empezar en 1 s, 1 hz
uint8_t increment = 1; //flag to see if the frequency increases or dicreases 
unsigned int counter=0; // flag to transverse pwm_funct array

static int pwm_funct[100] = {   //sine function 0-167
        83,88,93,98,103,108,113,118,
    122,127,131,135,139,143,146,149,
    152,155,157,159,161,162,164,164,
    165,165,165,164,164,162,161,159,
    157,155,152,149,146,143,139,135,
    131,127,122,118,113,108,103,98,
    93,88,83,77,72,67,62,57,
    52,47,43,38,34,30,26,22,
    19,16,13,10,8,6,4,3,
    1,1,0,0,0,1,1,3,
    4,6,8,10,13,16,19,22,
    26,30,34,38,43,47,52,57,
    62,67,72,77
    };

#define HALF_SECOND 83 
static void gpio_setup(void) {
    //Enable peripheral clocks

    rcc_periph_clock_enable(RCC_AFIO);      
    rcc_periph_clock_enable(RCC_GPIOA);
    
    // Configure Output compare on TIM2.CH2 in PA1
    gpio_primary_remap(
        AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF,                   // Optional
        AFIO_MAPR_TIM2_REMAP_NO_REMAP);                     // default: TIM2.CH2=GPIOA1
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,           // High speed
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO1);                 // GPIOA1=TIM2.CH2
}


static void tim2_PWM_init(void) {
    // TIM2:
    rcc_periph_clock_enable(RCC_TIM2);
    timer_disable_counter(TIM2);
    //Timer counting mode
    timer_set_mode(TIM2,    
        TIM_CR1_CKD_CK_INT,          //clock divider = 1
        TIM_CR1_CMS_EDGE,            //Edge-aligned for upcounting
        TIM_CR1_DIR_UP);             //upcounting
    timer_set_prescaler(TIM2, 800);    //Sysclck = 8Mhz, por tanto TIMx clock = 10Khz(checar nota refman:clockTree)
    timer_enable_preload(TIM2);      //counter is reloaded from TIMx_ARR(auto reload register) 
    timer_continuous_mode(TIM2);     //Timer doesn't stop at update events
    timer_set_period(TIM2, 167);   //Initial period is  60hz, determines frequency or cycle 

    //Output control
    timer_disable_oc_output(TIM2, TIM_OC2);
    //to compare counter(TIMx_CNT) and comparison register(TIMx_CCRx)
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);  //Selects interrupt generations with PWM1 mode 
    timer_enable_oc_output(TIM2, TIM_OC2);          //enable Output compare with channel 2
    
    //To determine the duty cycle, take Period(in this case 10000) into account and set the value where high phase of duty cycle ends
    timer_set_oc_value(TIM2, TIM_OC2, pwm_funct[counter=0]);         //Determines duty cycle: after this value, PWM is off
    timer_enable_counter(TIM2);         //start counting

}

static void tim3_freqChanger_init(void) {
    rcc_periph_clock_enable(RCC_TIM3);
    timer_set_mode(TIM3,
        TIM_CR1_CKD_CK_INT,          //clock divider = 1
        TIM_CR1_CMS_EDGE,            //Edge-aligned for upcounting
        TIM_CR1_DIR_UP);             //upcounting
    timer_one_shot_mode(TIM3);           //timer stops at each update(overflow/underflow, CEN bit must be set by software to resume operation)
    timer_set_prescaler(TIM3, 80);    //Sysclck = 8Mhz, por tanto TIMx clock = 100Khz(checar nota refman:clockTree)
    timer_set_period(TIM3, 1000);   //Initial period is  100hz, determines frequency or cycle 
    timer_enable_preload(TIM3);      //counter is reloaded from TIMx_ARR(auto reload register) 
    timer_enable_counter(TIM3);     //Start counting
}

//This will have the updated period and duty cycle values
/*
static void update_period(void) {
    timer_set_period(TIM2, 33333);  //change period
    timer_set_oc_value(TIM2, TIM_OC2, second);
}
*/
int main(void) {

    gpio_setup();
    tim2_PWM_init();
    tim3_freqChanger_init();
    
    for (;;) {

        if (timer_get_flag(TIM3, TIM_SR_UIF) ) {       //UPDATE EVENT(overflow)
            timer_enable_counter(TIM3);     //restart counter 3
            timer_clear_flag(TIM3, TIM_SR_UIF);
            counter ++;
            if(counter == 100){ //1 cycle
                counter =0;
                if(increment){  //increasing freq
                    current_f += 10;
                    if(current_f > 1000){  //max freq
                        increment = 0;
                    }
                }
                else {
                    current_f -= 10;
                    if(current_f < 100){
                        increment = 1;
                    }

                }
                timer_set_period(TIM3,(100000/(float)current_f));   //update period

            }
            timer_set_oc_value(TIM2,TIM_OC2, pwm_funct[counter]);

        }
        
        
    }

    return 0;
}
