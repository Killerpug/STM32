// Defining the exact width used types
#define int8_t          char
#define int16_t         short
#define int32_t         int
#define uint8_t         unsigned char
#define uint16_t        unsigned short
#define uint32_t        unsigned int

// Base peripheral addresses on STM32F1
#define PERIPH_BASE     ((uint32_t) 0x40000000) 	//!< 0x40000000 - 0xA000 0FFF
#define SRAM_BASE       ((uint32_t) 0x20000000)		//!< Lenght = 20K (0x20000000 - 0x20004FFF).
#define NVIC_BASE       ((uint32_t) 0xE000E100)		//!< Cortex-m3 Nestes vectored interrupt controller standard location.
#define STACKINIT       0x20004FFF					//!< Stack starts on Last direction of RAM as the processor uses Full-descending stack. 

// APB2 Modules address initialization
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define GPIOA_BASE      (APB2PERIPH_BASE + 0x0800) //!<  GPIOA base address is 0x40010800
#define GPIOB_BASE      (APB2PERIPH_BASE + 0x0C00) //!<  GPIOB base address is 0x40010C00
#define GPIOC_BASE      (APB2PERIPH_BASE + 0x1000) //!<  GPIOC base address is 0x40011000
#define USART1_BASE		(APB2PERIPH_BASE + 0X3800) //!< USART1 base address is 0x40013800
// AHB ModuleS address initialization
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)
#define RCC_BASE        ( AHBPERIPH_BASE + 0x1000) //!<   RCC base address is 0x40021000
#define FLASH_BASE      ( AHBPERIPH_BASE + 0x2000) //!< FLASH base address is 0x40022000

// Structure addresses initialization.
#define GPIOA 			((GPIO_type  *) GPIOA_BASE)
#define GPIOB 			((GPIO_type  *) GPIOB_BASE)
#define GPIOC 			((GPIO_type  *) GPIOC_BASE)
#define USART1 			((USART_type *) USART1_BASE)
#define RCC             ((RCC_type   *)   RCC_BASE)
#define NVIC            ((NVIC_type  *)  NVIC_BASE)


/*
 * Macros
 */
#define BM_SET(uWord, uMask)	(uWord |= (uMask))		//Sets the masked bits in an specified word
#define BM_CLEAR(uWord, uMask)	(uWord &= ~(uMask))		//Disable the masked bits in a word
#define BM_CHECK(uWord, uMask)	(uWord & (uMask))	    //Verifies masked bits in a word

/*
 * Register unwrap
 */
typedef struct
{
	uint32_t CRL;      //!< GPIO port configuration register low(0-7) 
	uint32_t CRH;      //!< GPIO port configuration register high(8-15) 
	uint32_t IDR;      //!< GPIO port input data register		
	uint32_t ODR;      //!< GPIO port output data register            
	uint32_t BSRR;     //!< GPIO port bit set/reset register         
	uint32_t BRR;      //!< GPIO port bit reset register             
	uint32_t LCKR;     //!< GPIO port configuration lock register
} GPIO_type;


// Reset an Clock Control registers
typedef struct
{
	uint32_t CR;       //!< RCC clock control register              
	uint32_t CFGR;     //!< RCC clock configuration register       
	uint32_t CIR;      //!< RCC clock interrupt register           
	uint32_t APB2RSTR; //!< RCC APB2 peripheral reset register      
	uint32_t APB1RSTR; //!< RCC APB1 peripheral reset register      
	uint32_t AHBENR;   //!< RCC AHB peripheral clock enable register
	uint32_t APB2ENR;  //!< Enables clock on peripherals connected to APB2(e.g. -GPIOA-GPIOC    -AFIO   -USART1     -SPI1   -ADC1-3    -TIM1)
	uint32_t APB1ENR;  //!< RCC APB1 peripheral clock enable register
	uint32_t BDCR;     //!< RCC backup domain control register      
	uint32_t CSR;      //!< RCC control/status register             
	uint32_t AHBRSTR;  //!< RCC AHB peripheral clock reset register 
	uint32_t CFGR2;    //!< RCC clock configuration register 2   
} RCC_type;


typedef struct  {
	uint32_t SR;		//!< Status Register
	uint32_t DR;		//!< Data Register (1 byte at a time)
	uint32_t BRR;		//!< Baudrate (mantisa)
	uint32_t CR1;		//!< configuration: usart enable, parity, word lenght, interrupts 
	uint32_t CR2;
	uint32_t CR3;		//!< flow control
	uint32_t GTPR;			

} USART_type;


// Standard addresses of NVIC cortex-m3. Check: https://developer.arm.com/documentation/dui0552/a/cortex-m3-peripherals/nested-vectored-interrupt-controller?lang=en
// or for specific used NVIC registers check page 128 of PM0056 (Programming Manual for stm32f10).
// Note: r/w to these registers requires Privileged access-level.
typedef struct
{
	uint32_t   ISER[8];     /*!< Interrupt Set-Enable Register, set the bit according to IRQn enum to enable interrupts on that module.  Address offset: 0x000 - 0x01C  */
	uint32_t  RES0[24];     /*!< Reserved. Address offset: 0x020 - 0x07C */
	uint32_t   ICER[8];     /*!< Interrupt Clear-Enable Register, disable interrupts. Address offset: 0x080 - 0x09C */
	uint32_t  RES1[24];     /*!< Reserved. Address offset: 0x0A0 - 0x0FC */
	uint32_t   ISPR[8];     /*!< Interrupt Set-Pending Register, force interrupts into pending state. Address offset: 0x100 - 0x11C */
	uint32_t  RES2[24];     /*!< Reserved. Address offset: 0x120 - 0x17C */
	uint32_t   ICPR[8];     /*!< Interrupt Clear-Pending Register, remove pending state from interrupts. Address offset: 0x180 - 0x19C */
	uint32_t  RES3[24];     /*!< Reserved. Address offset: 0x1A0 - 0x1FC */
	uint32_t   IABR[8];     /*!< Interrupt Active Bit Registers, indicate which interrupts are active. Address offset: 0x200 - 0x21C */
	uint32_t  RES4[56];     /*!< Reserved. Address offset: 0x220 - 0x2FC */
	char   	  IPR[240];     /*!< Interrupt Priority Registers, 4-bit priority field for each interrupt. Address offset: 0x300 - 0x3EC */
	uint32_t RES5[644];     /*!< Reserved. Address offset: 0x3F0 - 0xEFC */
	uint32_t      STIR;     /*!< Software Trigger Interrupt. Located in a separate block (0XE000EF00). Address offset: 0xF00 */
} NVIC_type;


/*
 * STM32F103 Interrupt Number Definition. check page 198 of RM0008(Reference manual stm32f103).
 * Also, for generic information of vector table check cortex-m3 Decives Generic User Guide.
 */
typedef enum IRQn
{
	NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
	MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
	BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
	UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
	SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
	DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
	PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
	SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */
	WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
	PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
	TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
	RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
	FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
	RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
	EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
	EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
	EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
	EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
	EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
	DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
	DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
	DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
	DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
	DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
	DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
	DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */
	ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
	CAN1_TX_IRQn                = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
	CAN1_RX0_IRQn               = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
	CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
	CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
	EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
	TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
	TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
	TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
	TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
	TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
	TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
	TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
	I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
	I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
	I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
	I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
	SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
	SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
	USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
	USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
	USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
	EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
	RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
	OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS WakeUp from suspend through EXTI Line Int */
	TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
	SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
	UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
	UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
	TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
	TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
	DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
	DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
	DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
	DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                      */
	DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                      */
	ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                            */
	ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt          */
	CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                    */
	CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                   */
	CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                   */
	CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                   */
	OTG_FS_IRQn                 = 67      /*!< USB OTG FS global Interrupt                          */
} IRQn_type;