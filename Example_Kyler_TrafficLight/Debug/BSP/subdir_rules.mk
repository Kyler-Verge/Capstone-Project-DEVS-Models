################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BSP/%.o: ../BSP/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fexceptions -D__MSP432P401R__ -DNO_LOGGING -DEMBED -DCCS -DTARGET_IS_MSP432P4XX -Dgcc -D__TI_COMPILER_VERSION__ -I"C:/Users/james/OneDrive/Desktop/flash_cadmium/July18_AnalogInput_ExternalSensors_MissingIOFiles/BSP" -I"C:/Users/james/OneDrive/Desktop/flash_cadmium/July18_AnalogInput_ExternalSensors_MissingIOFiles/DEVS_Models" -I"C:/Users/james/OneDrive/Desktop/msp432_driverlib/driverlib/MSP432P4xx" -I"C:/ti/ccs1230/ccs/ccs_base/arm/include" -I"C:/Users/james/OneDrive/Desktop/flash_cadmium/July18_AnalogInput_ExternalSensors_MissingIOFiles/cadmium" -I"C:/ti/ccs1230/ccs/ccs_base/arm/include/CMSIS" -I"C:/Users/james/OneDrive/Desktop/flash_cadmium/July18_AnalogInput_ExternalSensors_MissingIOFiles" -I"C:/ti/ccs1230/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"BSP/$(basename $(<F)).d_raw" -MT"$(@)" -std=c17 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


