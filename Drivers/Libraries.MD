## CMSIS layer ##
- Core Peripheral Access Layer: contains name definitions, address definitions and helper functions to access core registers and peripherals. 
	It defines also a device independent interface for RTOS Kernels that includes debug channel definitions. 
- STM32F1xx Device Peripheral Access Layer: provides definitions for all the peripheral register's definitions, 
	bits definitions and memory mapping for STM32F1xx devices. 


## Standard Peripheral Library(SPL) ##
Driver that provides drivers and header files for all the peripherals. It uses CMSIS layer to access STM32F2xx registers. 

## HAL library ##
Offer high-level function-oriented highly-portable APIs.

## LL library ##
Provide low-level APIs at register level, with better optimization but less portability.