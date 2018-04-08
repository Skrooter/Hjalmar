##########################################################################
# This file is part of Hjalmar.                                          #
#                                                                        #
# Hjalmar is free software: you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by   #
# the Free Software Foundation, either version 3 of the License, or      #
# (at your option) any later version.                                    #
#                                                                        #
# Hjalmar is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU General Public License      #
# along with Hjalmar.  If not, see <http://www.gnu.org/licenses/>.       #
##########################################################################

######################################
# target
######################################
TARGET = hjalmar
PLATFORM ?= STM32F4_discovery

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -g3 -O0

ifeq ($(PLATFORM), STM32F4_discovery)
DEV_HAL=STM32F4_discovery
else ifeq ($(PLATFORM), STM32F7_nucleo)
DEV_HAL=STM32F7_nucleo
else ifeq ($(PLATFORM), STM32F4_nucleo)
DEV_HAL=STM32F4_nucleo
else
$(error "Unknown platform")
endif

include $(DEV_HAL)/$(DEV_HAL).mk

#######################################
# paths
#######################################

# firmware library path
PERIFLIB_PATH =

# Build path
BUILD_DIR = out/$(PLATFORM)
OBJ_DIR = $(BUILD_DIR)/obj

######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# binaries
#######################################
PREFIX = $(ARM_NONE_BINPATH)arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

######################################
# source
######################################
# C sources
C_SOURCES +=  \
$(wildcard */src/*.c) \

# AS includes
AS_INCLUDES =

# C includes
C_INC +=  \
	$(wildcard */inc/) \


NULL=
SPACE=$(NULL) $(NULL)
C_INCLUDES = $(subst $(SPACE),$(SPACE)-I, $(C_INC))

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Wextra -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Wextra -fdata-sections -ffunction-sections -fshort-wchar

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--no-wchar-size-warning

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(OBJ_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(OBJ_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(OBJ_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo building $@
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(OBJ_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(OBJ_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo building $@
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo building $@
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo building $@
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo building $@
	@$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@
	mkdir $(OBJ_DIR)

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
