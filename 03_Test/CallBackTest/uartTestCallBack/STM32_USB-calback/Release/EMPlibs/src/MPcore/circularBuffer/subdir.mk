################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EMPlibs/src/MPcore/circularBuffer/CircularBuffer.cpp 

OBJS += \
./EMPlibs/src/MPcore/circularBuffer/CircularBuffer.o 

CPP_DEPS += \
./EMPlibs/src/MPcore/circularBuffer/CircularBuffer.d 


# Each subdirectory must supply rules for building sources it contributes
EMPlibs/src/MPcore/circularBuffer/CircularBuffer.o: ../EMPlibs/src/MPcore/circularBuffer/CircularBuffer.cpp EMPlibs/src/MPcore/circularBuffer/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"/home/alfy/Documents/uniGitHub/EMP/03_Test/CallBackTest/uartTestCallBack/STM32_USB-calback/EMPlibs/src" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"EMPlibs/src/MPcore/circularBuffer/CircularBuffer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

