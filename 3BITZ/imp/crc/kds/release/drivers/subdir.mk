################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_clock.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_common.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_crc.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_flash.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_gpio.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_smc.c \
C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_crc.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_crc.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/fsl_clock.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_clock.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_common.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_common.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_crc.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_crc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_flash.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_gpio.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_smc.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_smc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/fsl_uart.o: C:/Freescale/SDK_2.2_MK60DN512xxx10/devices/MK60D10/drivers/fsl_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall -DNDEBUG -DCPU_MK60DN512VMD10 -DTWR_K60D100M -DTOWER -I../../../../../../CMSIS/Include -I../../../../../../devices -I../.. -I../../../.. -I../../../../../../devices/MK60D10/drivers -I../../../../../../devices/MK60D10/utilities -I../../../../../../devices/MK60D10 -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


