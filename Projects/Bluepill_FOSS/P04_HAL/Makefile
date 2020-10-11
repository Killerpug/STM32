######################################################################
#  Project
######################################################################

BINARY		= HAL
SRCFILES	= main.c Interactors/tempsensor_lm35_int.c Interactors/global_system_int.c \
		  Interactors/digital_io_int.c Interactors/console_comm_int.c \
Modules/uc_usart.c  Modules/uc_timer.c  Modules/uc_gpio.c  Modules/uc_commonall.c  Modules/uc_adc.c

#INCLUDES = -I Modules/ Interactors/
all: elf bin    +$(MAKE) -C $$d Modules +$(MAKE) -C $$d Interactors -C $$d 

include ../Makefile.incl

# End
