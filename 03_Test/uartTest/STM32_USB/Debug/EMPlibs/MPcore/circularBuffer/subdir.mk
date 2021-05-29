################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EMPlibs/MPcore/circularBuffer/CircularBuffer.cpp 

OBJS += \
./EMPlibs/MPcore/circularBuffer/CircularBuffer.o 

CPP_DEPS += \
./EMPlibs/MPcore/circularBuffer/CircularBuffer.d 


# Each subdirectory must supply rules for building sources it contributes
EMPlibs/MPcore/circularBuffer/CircularBuffer.o: ../EMPlibs/MPcore/circularBuffer/CircularBuffer.cpp EMPlibs/MPcore/circularBuffer/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/alfy/Documents/uniGitHub/EMP/03_Test/uartTest/STM32_USB/EMPlibs" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"EMPlibs/MPcore/circularBuffer/CircularBuffer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

