#ifndef TEMPSENSOR_LM35_INT_H
#define TEMPSENSOR_LM35_INT_H
#include "global_system_int.h"
#include "../Modules/uc_adc.h"
#include "../Modules/uc_timer.h"
#include "digital_io_int.h"

uint32_t sample_counter = 0;
uint16_t LM35_raw_Tempvalue = 0;               /*!< Raw unfiltered adc DIRECTLY converted temperature*/
uint16_t LM35_filtered_Tempvalue[FILTER_ARRAY_SIZE] = { 0, 0, 0, 0 };      /*!< Applies rotatory filter of 4 samples in case of noisy adc readings */
uint16_t LM35_average_Tempvalue = 0;          /*!< Filtered value of temperature readings*/

//maps position 0, 1, 2, 3 ... to modules physical address location.
const Tim_FriendModule tim_friendmodule_sel[] = { TIM1_MODULE, TIM2_MODULE, TIM3_MODULE,  TIM4_MODULE,  TIM5_MODULE,  TIM6_MODULE, TIM7_MODULE};
typedef enum {
    MODULE0__I = 0,
    MODULE1__I = 1,
    MODULE2__I = 2,    
    MODULE3__I = 3,
    MODULE4__I = 4,
    MODULE5__I = 5,
    MODULE6__I = 6,
    MODULE7__I = 7
} Module_OfInteraction;

void open_tempsensor(uint32_t peridiocity_ms);
uint32_t read_tempsensor(uint32_t Channel);
void close_tempsensor(uint32_t hex_PortPin);
void periodic_read_request(uint32_t peridiocity_ms);
void rotatory_filter(uint32_t new_value);
int32_t get_timeup_status(Module_OfInteraction module_number);
#endif