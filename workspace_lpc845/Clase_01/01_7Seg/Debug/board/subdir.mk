################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/board" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/source" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/component/uart" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/drivers" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/CMSIS" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/CMSIS/m-profile" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/device" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities/debug_console_lite" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities/str" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/utilities" -I"/Users/rok/Documents/curso-lse/workspace_lpc845/LPC845_Project/device/periph2" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

