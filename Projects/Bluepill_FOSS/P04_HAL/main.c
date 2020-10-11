#include <stdio.h>
#include "main.h"



/** Updates adc temperature values on rotatory filter 
    Also triggers threshold LEDS
*/
void update_triggers_status(uint32_t temp_reading) {
    if (temp_reading < trigger_lowtemp) {
        write_dio(GPIO_LM35, CLEAR);
    }
    else { write_dio(GPIO_LM35, SET); }

    if (temp_reading > trigger_hightemp) {
        write_dio(GPIO_LM35, CLEAR);
    }
    else {write_dio(GPIO_LM35, SET); }
}

void en_UserCustomMode(char command) {
    uint16_t new_Threshvalue = 0;
    sprintf(bufferTX, "Enter the threshold in ºC and press l to load the value  \n ");
    if (command == 'x') {
        sprintf(bufferTX, "Set Max temperature threshold: ");
    }
    else if (command == 'n') {
        sprintf(bufferTX, "Set Min temperature threshold: ");
    }
    write_comm(0x1, bufferTX);
    int i = 1;
    char receive_new_command = 0;
    while (bufferRX[0] != 'l') {    //escape sequence to load value
        receive_new_command = receive_comm();
        while (!receive_new_command); //Wait for user response
        if (receive_new_command != 'l' && receive_new_command > 47 && receive_new_command < 58) {
            new_Threshvalue *= i;
            new_Threshvalue += (receive_new_command - 48);
            i = 10;
        }
    }
    
    
    if (command == 'x') {
        trigger_hightemp = (new_Threshvalue * 4094)/330.0;
    }
    else if (command == 'n') {
        trigger_lowtemp = (new_Threshvalue * 4094) / 330.0;
    }
    sprintf(bufferTX, "action finished: ");
    write_comm(0x0, bufferTX);
    BM_CLEAR(app_event_flags, USER_CUSTOMMODE);
}


void application(void) {
    open_tempsensor(100);
    open_comm();
    uint32_t current_temperature_read = 0;
    char receiver_comm = 0;
    for (;;) {
        if (get_timeup_status(0x03)) {       //UPDATE EVENT(overflow)
            current_temperature_read = read_tempsensor(CHANNEL_LM35);
            update_triggers_status(current_temperature_read);
        }
        receiver_comm = receive_comm();
        if (receiver_comm) {
            if (receiver_comm == 'x' || receiver_comm == 'n') {
                BM_SET(app_event_flags, USER_CUSTOMMODE);
                en_UserCustomMode(receiver_comm);
            }
            else {
                sprintf(bufferTX, "Please enter valid character \r\n");
                write_comm(0x1, bufferTX);
            }
        }


        //print status to console
        if (5 <= status_counter) {
            status_counter = 0;
            LM35_average_Tempvalue = (LM35_average_Tempvalue * 330) / 4094;
            sprintf(bufferTX, "Temp:%i C, MaxTempTh:%.1f C, MinTempTh:%.1f C\r\n", \
                LM35_average_Tempvalue, (trigger_hightemp * 330) / 4094.0, (trigger_lowtemp * 330) / 4094.0);
            write_comm(0x1, bufferTX);

            
        }

    }
}

int main(void) {
    application();
    return 0;
}
