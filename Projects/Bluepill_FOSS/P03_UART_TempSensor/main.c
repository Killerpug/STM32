

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>

volatile char uevent_flags = 0;      /*!< Captures events to avoid long routines on isr */
    #define no_event        0x00
    #define adc_conversion  0x01    /*!< ADC has completed a conversion, reading is required */
    #define uart_receiving  0x02    /*!< UART has new data, reading is required*/
    #define user_customMode 0x04    /*!< User has requested enter to custom mode */
    #define max_tempReached 0x08
    #define min_tempReached 0x10
#define BM_SET(uWord, uMask)	(uWord |= (uMask))		//Sets the masked bits in an specified word
#define BM_CLEAR(uWord, uMask)	(uWord &= ~(uMask))		//Disable the masked bits in a word
#define BM_CHECK(uWord, uMask)	(uWord & (uMask))	    //Verifies masked bits in a word

#define BUF_SIZE 150
#define FILTER_ARRAY_SIZE 4      /*!< Elements on rotatory filter for ADC readings */
#define GPIO_LM35 7             /*!< Temperature sensor LM35 is connected to GPIOA 7 */

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

uint16_t LM35_raw_Tempvalue = 0;               /*!< Raw unfiltered adc DIRECTLY converted temperature*/
uint16_t LM35_filtered_Tempvalue[FILTER_ARRAY_SIZE] = { 0, 0, 0, 0 };      /*!< Applies rotatory filter of 4 samples in case of noisy adc readings */
uint16_t LM35_average_Tempvalue = 0;          /*!< Filtered value of temperature readings*/
char bufferTX[BUF_SIZE] = { '\0' };
volatile char bufferRX[1] = { '\0' };
uint32_t status_counter = 0;            /*!< Allows tracking of time, updates every 0.1s. Also, helps averaging temperature*/
uint8_t read_channel = GPIO_LM35;

static void USARTSend(char* pucBuffer) {
    while (*pucBuffer)
    {
        usart_send_blocking(USART1, *pucBuffer++);
        while (usart_get_flag(USART1, USART_SR_TC) == 0) {      //wait to complete transmission

        }
    }
}

static void USARTReceive(void) {
    bufferRX[0] = usart_recv_blocking(USART1);        //Clears reciever flags RXNE(receiver enable) and RXNEIE(Receiver Interrupt Enable)
    BM_SET(uevent_flags, uart_receiving);
}

static void gpio_setup(void) {
    //Enable peripheral clock 
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);
     // Configure threshold GPIO LEDs
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,           // High speed
        GPIO_CNF_OUTPUT_PUSHPULL, GPIO5|GPIO6);             // output LEDs

    //Configure temperature input sensor to ADC
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,           // High speed
        GPIO_CNF_INPUT_ANALOG, GPIO7);

    // Configure Output compare on TIM2.CH2 in PA1
    gpio_primary_remap(
        AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF,                   // Optional
        AFIO_MAPR_TIM2_REMAP_NO_REMAP);                     // default: TIM2.CH2=GPIOA1
}

/**
    Enables interrupts(update events) on timer3 every 0.1 seconds.
    TIM_SR_UIF, update event flag must be cleared by software and timer must be re-enabled(TIMx_CR1->CEN).
*/
static void triggerconversion_timer_setup(void) {
    rcc_periph_clock_enable(RCC_TIM3);
    timer_set_mode(TIM3,
        TIM_CR1_CKD_CK_INT,          //clock divider = 1
        TIM_CR1_CMS_EDGE,            //Edge-aligned for upcounting
        TIM_CR1_DIR_UP);             //upcounting
    timer_one_shot_mode(TIM3);       //timer stops at each update(overflow/underflow, CEN bit must be set by software to resume operation)
    timer_set_prescaler(TIM3, GEN_10KHZ_FREQ);  //Sysclk = 8Mhz, por tanto TIMx clock = 10Khz(checar nota refman:clockTree)
    timer_set_period(TIM3, GEN_10HZ_PERIOD);    //Initial period is  10hz, determines frequency ((sysclk/prescaler) / period) 
    timer_enable_preload(TIM3);      //counter is reloaded from TIMx_ARR(auto reload register) 
    timer_enable_counter(TIM3);      //Start counting
}

static void adc_setup(void){
    rcc_periph_clock_enable(RCC_ADC1);  //enable ADC peripheral
    adc_power_off(ADC1);            //for initialization
    // This are the system defaults
         adc_set_dual_mode(ADC_CR1_DUALMOD_IND);
         adc_set_single_conversion_mode(ADC1);
         adc_set_right_aligned(ADC1);
         adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_239DOT5CYC);
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while (adc_is_calibrating(ADC1));
    nvic_enable_irq(NVIC_ADC1_2_IRQ);   //enables interrupts on ADC modules
    adc_enable_eoc_interrupt(ADC1);  //Enable interrupts when finishing conversions
}


static void usart_setup(void) {
    rcc_periph_clock_enable(RCC_USART1);
    //////////////////////////////////////////////////////////////
    // STM32F103C8T6:
    //	RX:	A9
    //	TX:	A10
    //	Baud:	9600
    //////////////////////////////////////////////////////////////

    // GPIO_USART1_TX/GPIO13 on GPIO port A for tx
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

    usart_set_baudrate(USART1, 9600);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    //interrupts
    usart_enable_rx_interrupt(USART1);  //Enable interrupts on reception
    nvic_enable_irq(NVIC_USART1_IRQ);   //Enable interrupt on USART1 module
    usart_enable(USART1);
}

/** Updates adc temperature values on rotatory filter 
    Also triggers threshold LEDS
*/
static void update_tempon_adc(void) {
    LM35_filtered_Tempvalue[status_counter % 4] = LM35_raw_Tempvalue;   
    uint16_t temporal_temp = 0;
    for (int i = 0; i < FILTER_ARRAY_SIZE; i++) {
        temporal_temp += LM35_filtered_Tempvalue[i];
    }
    LM35_average_Tempvalue = temporal_temp/4;
    if (LM35_average_Tempvalue < trigger_lowtemp) {
        gpio_clear(GPIOA, GPIO6);
    }
    else { gpio_set(GPIOA, GPIO6); }

    if (LM35_average_Tempvalue > trigger_hightemp) {
        gpio_clear(GPIOA, GPIO5);
    }
    else { gpio_set(GPIOA, GPIO5); }
}

static void enter_UserCustomMode(uint8_t command) {
    uint16_t new_Threshvalue = 0;
    sprintf(bufferTX, "Enter the threshold in ºC and press l to load the value  \n ");
    if (command == 'x') {
        sprintf(bufferTX, "Set Max temperature threshold: ");
    }
    else if (command == 'n') {
        sprintf(bufferTX, "Set Min temperature threshold: ");
    }
    USARTSend(bufferTX);
    int i = 1;
    while (bufferRX[0] != 'l') {    //escape sequence to load value
        while (!BM_CHECK(uevent_flags, uart_receiving)); //Wait for user response
        BM_CLEAR(uevent_flags, uart_receiving);
        if (bufferRX[0] != 'l' && bufferRX[0] > 47 && bufferRX[0] < 58) {
            new_Threshvalue *= i;
            new_Threshvalue += (bufferRX[0] - 48);
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
    USARTSend(bufferTX);
    

    BM_CLEAR(uevent_flags, user_customMode);
}

int main(void) {
    gpio_setup();
    usart_setup();
    triggerconversion_timer_setup();
    adc_setup();
    sprintf(bufferTX, "Welcome to your home. Watch your status or   \
        press x to modify Max temperature or n to modify Min temperature \r\n");
    USARTSend(bufferTX);
    for (;;) {
        
        if (timer_get_flag(TIM3, TIM_SR_UIF)) {       //UPDATE EVENT(overflow)
            timer_clear_flag(TIM3, TIM_SR_UIF);
            timer_enable_counter(TIM3);     //re-enable counter 3
            //ADC Read sequence, in this case I only read one channel 
            adc_set_regular_sequence(ADC1, 1, &read_channel);
            adc_start_conversion_direct(ADC1); //start conversion ADON
            status_counter++;  
        }

        /**Mini state-machine to handle interrupt flags */
        if (BM_CHECK(uevent_flags, adc_conversion)) {
            BM_CLEAR(uevent_flags, adc_conversion);
            update_tempon_adc();
        }
        else if (BM_CHECK(uevent_flags, uart_receiving)) {
            BM_CLEAR(uevent_flags, uart_receiving);
            if (bufferRX[0] == 'x' || bufferRX[0] == 'n') {
                BM_SET(uevent_flags, user_customMode);
                enter_UserCustomMode(bufferRX[0]);
            }
            else {
                sprintf(bufferTX, "Please enter valid character \r\n");
                USARTSend(bufferTX);
            }
        }


        //print status to console
        if (5 <= status_counter) {
            status_counter = 0;
            LM35_average_Tempvalue = (LM35_average_Tempvalue * 330) / 4094;
            sprintf(bufferTX, "Temp:%i C, MaxTempTh:%.1f C, MinTempTh:%.1f C\r\n", \
                LM35_average_Tempvalue, (trigger_hightemp * 330) / 4094.0, (trigger_lowtemp * 330) / 4094.0);
            USARTSend(bufferTX);

            
        }

    }

    return 0;
}

/**
    Interrupt routine, declaration found on stm32/f1/nvic.h(cm3>Cortex-M NVIC Defines>user interrupts).
*/
void adc1_2_isr(void) {    
    BM_SET(uevent_flags, adc_conversion);
    LM35_raw_Tempvalue = adc_read_regular(ADC1);     /*!<Read operation clears interrupt flag */
}

/**
    Interrupt routine, declaration found on stm32/f1/nvic.h(cm3>Cortex-M NVIC Defines>user interrupts).
*/
void usart1_isr(void) {
    USARTReceive();
}
