################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/board.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/clock_config.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/board.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/board.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

board/clock_config.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/clock_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

board/pin_mux.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/pin_mux.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


