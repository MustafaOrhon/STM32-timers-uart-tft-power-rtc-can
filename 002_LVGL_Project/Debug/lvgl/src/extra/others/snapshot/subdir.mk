################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/extra/others/snapshot/lv_snapshot.c 

OBJS += \
./lvgl/src/extra/others/snapshot/lv_snapshot.o 

C_DEPS += \
./lvgl/src/extra/others/snapshot/lv_snapshot.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/extra/others/snapshot/%.o lvgl/src/extra/others/snapshot/%.su lvgl/src/extra/others/snapshot/%.cyclo: ../lvgl/src/extra/others/snapshot/%.c lvgl/src/extra/others/snapshot/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I"D:/STM32Projects/002_LVGL_Project" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-extra-2f-others-2f-snapshot

clean-lvgl-2f-src-2f-extra-2f-others-2f-snapshot:
	-$(RM) ./lvgl/src/extra/others/snapshot/lv_snapshot.cyclo ./lvgl/src/extra/others/snapshot/lv_snapshot.d ./lvgl/src/extra/others/snapshot/lv_snapshot.o ./lvgl/src/extra/others/snapshot/lv_snapshot.su

.PHONY: clean-lvgl-2f-src-2f-extra-2f-others-2f-snapshot

