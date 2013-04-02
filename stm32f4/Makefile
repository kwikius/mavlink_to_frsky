
# Copyright (c) 2012-2013 Andy Little 

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>
 
############################################################
#  Invoke with 'make Discovery' to build for STM32F4Discovery board
# **** you will need to modify these to your own paths ***
TOOLCHAIN_PREFIX = /home/andy/arm/arm-cortex-m4-hardfloat-toolchain/
STM32F4_INCLUDE_PATH = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/
MAVLINK_INCLUDE_PATH = /home/andy/website/fpv/mavlink/include/mavlink/v1.0/
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/
INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)lib/gcc/arm-none-eabi/4.7.1/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
############################################################

CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump

STM32F4_LINKER_SCRIPT = stm32f4.ld

INCLUDES = -I$(STM32F4_INCLUDE_PATH)CMSIS/Include/ \
-I$(STM32F4_INCLUDE_PATH)CMSIS/Device/ST/STM32F4xx/Include/ \
-I$(STM32F4_INCLUDE_PATH)STM32F4xx_StdPeriph_Driver/inc/ \
-I$(MAVLINK_INCLUDE_PATH) \
-I$(MAVLINK_INCLUDE_PATH)ardupilotmega/ \
-I$(QUAN_INCLUDE_PATH)

CFLAG_EXTRAS = -fno-math-errno -DQUAN_STM32F4 -DQUAN_NO_EXCEPTIONS 

PROCESSOR_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

CFLAGS = -std=c++11 -fno-rtti -fno-exceptions -c -O2 -g $(INCLUDES) $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS)

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

LFLAGS = -T$(STM32F4_LINKER_SCRIPT) -O2 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) $(INIT_LIBS) -nodefaultlibs -nostartfiles

CPFLAGS = -Obinary
ODFLAGS = -d

STARTUP = startup.s

.PHONY : all Debug Discovery clean

all : test

Debug : test

Discovery : test

ifeq ($(MAKECMDGOALS),Discovery)
CFLAG_EXTRAS += -DMAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD
OBJECT_PREFIX = discovery_
else
CFLAG_EXTRAS += -DMAVLINK_TO_FRSKY_TARGET_BOARD
OBJECT_PREFIX = target_
endif

sources =  main.cpp mavlink.cpp frsky.cpp serial_port.cpp system_init.cpp timer.cpp setup.cpp request_mavlink_rates.cpp

objects =  $(patsubst %.cpp,$(OBJECT_PREFIX)%.o,$(sources))

clean:
	-rm -rf *.o *.elf *.lst *.bin

test: $(OBJECT_PREFIX)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(OBJECT_PREFIX)main.elf $(OBJECT_PREFIX)main.bin
	$(OD) $(ODFLAGS) $(OBJECT_PREFIX)main.elf > $(OBJECT_PREFIX)main.lst

$(OBJECT_PREFIX)main.elf: startup.o $(objects) $(STM32F4_LINKER_SCRIPT)
	@ echo "..linking"
	$(LD) $(LFLAGS) -o $(OBJECT_PREFIX)main.elf $(objects) startup.o

$(objects): $(OBJECT_PREFIX)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 
	
