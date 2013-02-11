
############################################################
# **** you will need to modify these to your own paths ***
TOOLCHAIN_PREFIX = /home/andy/arm/arm-cortex-m4-hardfloat-toolchain/bin/
STM32F4_INCLUDE_PATH = /opt/stm32f4/STM32F4xx_DSP_StdPeriph_Lib_V1.0.0/Libraries/
MAVLINK_INCLUDE_PATH = /home/andy/website/fpv/mavlink/include/mavlink/v1.0/
QUAN_INCLUDE_PATH = /home/andy/website/quan-trunk/
############################################################

CC      = $(TOOLCHAIN_PREFIX)arm-none-eabi-g++
CC1     = $(TOOLCHAIN_PREFIX)arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)arm-none-eabi-objdump

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

LFLAGS = -T$(STM32F4_LINKER_SCRIPT) -O2 $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS)

CPFLAGS = -Obinary
ODFLAGS = -d

STARTUP = startup.s

all: test

Debug : test

objects =  main.o mavlink.o frsky.o serial_port.o system_init.o timer.o setup.o request_mavlink_rates.o

clean:
	-rm  $(objects) startup.o main.elf main.lst main.bin

test: main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.lst

main.elf: startup.o $(objects) $(STM32F4_LINKER_SCRIPT)
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.elf $(objects) startup.o

$(objects): %.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -o startup.o $(STARTUP) 
