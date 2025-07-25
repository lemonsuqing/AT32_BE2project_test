################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/Serial.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/at32f402_405_int.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/at32f402_405_wk_config.c \
../user/be2_spi.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/main.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_spi.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_system.c \
D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_usart.c 

OBJS += \
./user/Serial.o \
./user/at32f402_405_int.o \
./user/at32f402_405_wk_config.o \
./user/be2_spi.o \
./user/main.o \
./user/wk_spi.o \
./user/wk_system.o \
./user/wk_usart.o 

C_DEPS += \
./user/Serial.d \
./user/at32f402_405_int.d \
./user/at32f402_405_wk_config.d \
./user/be2_spi.d \
./user/main.d \
./user/wk_spi.d \
./user/wk_system.d \
./user/wk_usart.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/at32f402_405_int.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/at32f402_405_int.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/at32f402_405_wk_config.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/at32f402_405_wk_config.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/main.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/main.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/wk_spi.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_spi.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/wk_system.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_system.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

user/wk_usart.o: D:/work/LPMS-BE/BE2test/BE2test/project/src/wk_usart.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


