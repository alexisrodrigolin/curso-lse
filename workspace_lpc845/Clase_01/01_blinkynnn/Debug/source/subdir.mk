################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/01_blinkynnn.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/01_blinkynnn.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/01_blinkynnn.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/board" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/source" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/component/uart" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/drivers" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/CMSIS" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/CMSIS/m-profile" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/device" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities/debug_console_lite" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities/str" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/utilities" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/01_blinkynnn/device/periph2" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/01_blinkynnn.d ./source/01_blinkynnn.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

