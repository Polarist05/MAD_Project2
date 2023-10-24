################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/dma.c \
../Core/Src/endGamePage.c \
../Core/Src/gameUtility.c \
../Core/Src/gpio.c \
../Core/Src/icons.c \
../Core/Src/lcd.c \
../Core/Src/main.c \
../Core/Src/map.c \
../Core/Src/rng.c \
../Core/Src/spi.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/utiliity.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/dma.o \
./Core/Src/endGamePage.o \
./Core/Src/gameUtility.o \
./Core/Src/gpio.o \
./Core/Src/icons.o \
./Core/Src/lcd.o \
./Core/Src/main.o \
./Core/Src/map.o \
./Core/Src/rng.o \
./Core/Src/spi.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/utiliity.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/dma.d \
./Core/Src/endGamePage.d \
./Core/Src/gameUtility.d \
./Core/Src/gpio.d \
./Core/Src/icons.d \
./Core/Src/lcd.d \
./Core/Src/main.d \
./Core/Src/map.d \
./Core/Src/rng.d \
./Core/Src/spi.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/utiliity.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/vorapolpromwan/Desktop/MicroCon/Project/KMITL-2023-S1-Microcontroller/MAD_Project2/Drivers/ILI9341" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/endGamePage.cyclo ./Core/Src/endGamePage.d ./Core/Src/endGamePage.o ./Core/Src/endGamePage.su ./Core/Src/gameUtility.cyclo ./Core/Src/gameUtility.d ./Core/Src/gameUtility.o ./Core/Src/gameUtility.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/icons.cyclo ./Core/Src/icons.d ./Core/Src/icons.o ./Core/Src/icons.su ./Core/Src/lcd.cyclo ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/map.cyclo ./Core/Src/map.d ./Core/Src/map.o ./Core/Src/map.su ./Core/Src/rng.cyclo ./Core/Src/rng.d ./Core/Src/rng.o ./Core/Src/rng.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/utiliity.cyclo ./Core/Src/utiliity.d ./Core/Src/utiliity.o ./Core/Src/utiliity.su

.PHONY: clean-Core-2f-Src

