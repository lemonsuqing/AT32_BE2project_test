################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/work/LPMS-BE/BE2test/BE2test/libraries/cmsis/cm4/device_support/system_at32f402_405.c 

OBJS += \
./cmsis/system_at32f402_405.o 

C_DEPS += \
./cmsis/system_at32f402_405.d 


# Each subdirectory must supply rules for building sources it contributes
cmsis/system_at32f402_405.o: D:/work/LPMS-BE/BE2test/BE2test/libraries/cmsis/cm4/device_support/system_at32f402_405.c cmsis/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections  -g -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F405RCT7_7 -DUSE_STDPERIPH_DRIVER -I"../../../libraries/drivers/inc" -I"../../../libraries/cmsis/cm4/core_support/" -I"../../../libraries/cmsis/cm4/device_support/" -I"../../inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


