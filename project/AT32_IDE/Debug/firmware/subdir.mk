################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_acc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_adc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_can.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_crc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_crm.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_debug.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_dma.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_ertc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_exint.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_flash.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_gpio.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_i2c.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_misc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_pwc.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_qspi.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_scfg.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_spi.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_tmr.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_usart.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_usb.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_wdt.c \
D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_wwdt.c 

OBJS += \
./firmware/at32f402_405_acc.o \
./firmware/at32f402_405_adc.o \
./firmware/at32f402_405_can.o \
./firmware/at32f402_405_crc.o \
./firmware/at32f402_405_crm.o \
./firmware/at32f402_405_debug.o \
./firmware/at32f402_405_dma.o \
./firmware/at32f402_405_ertc.o \
./firmware/at32f402_405_exint.o \
./firmware/at32f402_405_flash.o \
./firmware/at32f402_405_gpio.o \
./firmware/at32f402_405_i2c.o \
./firmware/at32f402_405_misc.o \
./firmware/at32f402_405_pwc.o \
./firmware/at32f402_405_qspi.o \
./firmware/at32f402_405_scfg.o \
./firmware/at32f402_405_spi.o \
./firmware/at32f402_405_tmr.o \
./firmware/at32f402_405_usart.o \
./firmware/at32f402_405_usb.o \
./firmware/at32f402_405_wdt.o \
./firmware/at32f402_405_wwdt.o 

C_DEPS += \
./firmware/at32f402_405_acc.d \
./firmware/at32f402_405_adc.d \
./firmware/at32f402_405_can.d \
./firmware/at32f402_405_crc.d \
./firmware/at32f402_405_crm.d \
./firmware/at32f402_405_debug.d \
./firmware/at32f402_405_dma.d \
./firmware/at32f402_405_ertc.d \
./firmware/at32f402_405_exint.d \
./firmware/at32f402_405_flash.d \
./firmware/at32f402_405_gpio.d \
./firmware/at32f402_405_i2c.d \
./firmware/at32f402_405_misc.d \
./firmware/at32f402_405_pwc.d \
./firmware/at32f402_405_qspi.d \
./firmware/at32f402_405_scfg.d \
./firmware/at32f402_405_spi.d \
./firmware/at32f402_405_tmr.d \
./firmware/at32f402_405_usart.d \
./firmware/at32f402_405_usb.d \
./firmware/at32f402_405_wdt.d \
./firmware/at32f402_405_wwdt.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/at32f402_405_acc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_acc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_adc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_adc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_can.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_can.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_crc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_crc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_crm.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_crm.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_debug.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_debug.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_dma.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_dma.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_ertc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_ertc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_exint.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_exint.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_flash.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_flash.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_gpio.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_gpio.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_i2c.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_i2c.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_misc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_misc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_pwc.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_pwc.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_qspi.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_qspi.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_scfg.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_scfg.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_spi.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_spi.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_tmr.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_tmr.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_usart.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_usart.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_usb.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_usb.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_wdt.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_wdt.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/at32f402_405_wwdt.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/drivers/src/at32f402_405_wwdt.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


