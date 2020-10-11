#ifndef MAIN_H
#define MAIN_H

#include "Interactors/console_comm_int.h"
#include "Interactors/global_system_int.h"
#include "Interactors/tempsensor_lm35_int.h"
#include "Interactors/digital_io_int.h"
#include <stdio.h>

void update_triggers_status(uint32_t temp_reading);
void en_UserCustomMode(char command);
void application(void);
#endif