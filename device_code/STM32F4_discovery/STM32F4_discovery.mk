
C_SOURCES += \
	$(wildcard $(DEV_HAL)/Drivers/STM32F4xx_HAL_Driver/Src/*.c) \
	$(wildcard $(DEV_HAL)/Src/*.c)

ASM_SOURCES =  \
	$(DEV_HAL)/startup_stm32f407xx.s

C_INC =  \
	$(DEV_HAL)/Inc \
	$(DEV_HAL)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	$(DEV_HAL)/Drivers/CMSIS/Include \
	$(DEV_HAL)/Drivers/STM32F4xx_HAL_Driver/Inc \
	$(DEV_HAL)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy

# link script
LDSCRIPT = \
	$(DEV_HAL)/STM32F407VGTx_FLASH.ld

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
	-DUSE_HAL_DRIVER \
	-DSTM32F407xx