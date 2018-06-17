################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/system_MK60D10.c 

S_UPPER_SRCS += \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/gcc/startup_MK60D10.S 

OBJS += \
./startup/startup_MK60D10.o \
./startup/system_MK60D10.o 

C_DEPS += \
./startup/system_MK60D10.d 

S_UPPER_DEPS += \
./startup/startup_MK60D10.d 


# Each subdirectory must supply rules for building sources it contributes
startup/startup_MK60D10.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/gcc/startup_MK60D10.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -x assembler-with-cpp -D__STARTUP_CLEAR_BSS -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/system_MK60D10.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/system_MK60D10.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


