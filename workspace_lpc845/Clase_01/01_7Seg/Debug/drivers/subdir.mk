################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_swm.c \
../drivers/fsl_usart.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_swm.d \
./drivers/fsl_usart.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_swm.o \
./drivers/fsl_usart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/board" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/source" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/component/uart" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/drivers" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/CMSIS" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/CMSIS/m-profile" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/device" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities/debug_console_lite" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities/str" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/device/periph2" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_power.d ./drivers/fsl_power.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_swm.d ./drivers/fsl_swm.o ./drivers/fsl_usart.d ./drivers/fsl_usart.o

.PHONY: clean-drivers

