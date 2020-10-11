#ifndef UC_TIMER_H
#define UC_TIMER_H

#include <libopencm3/stm32/timer.h>
#include "uc_commonall.h"

//  SELECT INTERFACE TYPE
#define SELECTED_UCTIM_INTERFACE FRIENDLY_TIM_INTERFACE
#define FRIENDLY_TIM_INTERFACE 1
#define REGISTERLEVEL_TIM_INTERFACE 2
#if(SELECTED_UCTIM_INTERFACE == FRIENDLY_TIM_INTERFACE)

typedef enum {
    TIM1_MODULE = TIM1,
    TIM2_MODULE = TIM2,
    TIM3_MODULE = TIM3,
    TIM4_MODULE = TIM4,
    TIM5_MODULE = TIM5,
    TIM6_MODULE = TIM6,
    TIM7_MODULE = TIM7
} Tim_FriendModule;

typedef enum {
    UPCOUNTING = TIM_CR1_DIR_UP,
    DOWNCOUNTING = TIM_CR1_DIR_DOWN
} TimMode_FriendMode1;

typedef enum {
    ONE_SHOT,
    INTERRUPT
} TimMode_FriendMode2;

typedef enum {
    TIM1_RCC = RCC_TIM1,
    TIM2_RCC = RCC_TIM2,
    TIM3_RCC = RCC_TIM3,
    TIM4_RCC = RCC_TIM4,
    TIM5_RCC = RCC_TIM5,
    TIM6_RCC = RCC_TIM6,
    TIM7_RCC = RCC_TIM7
} Tim_FriendRcc;

typedef struct {
    uint32_t Module;
    uint32_t Prescaler;
    uint32_t Period;
    uint32_t Mode1;     //counting type
    uint32_t Mode2;     //interrupts
    uint32_t Tim_Rcc;
} Timx_FriendlyHandler; 
#elif (SELECTED_UCTIM_INTERFACE == REGISTERLEVEL_TIM_INTERFACE)

#else 
    #error "Timer interface type not defined. Please select an interfacing connection type"

#endif // FRIENDLY_INTERFACE

typedef struct {
    uint32_t Module;
    uint32_t Prescaler;
    uint32_t Period;
    uint32_t Mode1;     //counting type
    uint32_t Mode2;     //interrupts
    uint32_t Tim_Rcc;
} Timx_Handler;
#define MAX_2BYTES 0xFFFF
#define SYSCLCK_DEFAULT 8000000
#define MILISECONDS_IN_SECOND 1000

void setup_timer(Timx_Handler* tim_init_struct);
void setup_pwm(void);
void otf_setpwm(void);
void calculate_pomodoro_timer(uint32_t time_ms, Timx_Handler* timset_prescaler_period);   //used to calculate prescaler and period for a timer
int32_t get_timer_status(uint32_t Module);
void clear_timer_status(uint32_t Module);
void enable_timer_status(uint32_t Module);
#endif