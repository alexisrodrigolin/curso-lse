################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_miniusart.c 

C_DEPS += \
./component/uart/fsl_adapter_miniusart.d 

OBJS += \
./component/uart/fsl_adapter_miniusart.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/board" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/source" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/component/uart" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/drivers" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/CMSIS" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/CMSIS/m-profile" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/device" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities/debug_console_lite" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities/str" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/device/periph2" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/fsl_adapter_miniusart.d ./component/uart/fsl_adapter_miniusart.o

.PHONY: clean-component-2f-uart

