################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/crc.c 

OBJS += \
./source/crc.o 

C_DEPS += \
./source/crc.d 


# Each subdirectory must supply rules for building sources it contributes
source/crc.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/boards/twrk60d100m/driver_examples/crc/crc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


