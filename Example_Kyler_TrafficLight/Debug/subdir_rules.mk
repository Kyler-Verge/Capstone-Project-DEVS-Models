################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-gcc-10.3.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fexceptions -D__MSP432P401R__ -DNO_LOGGING -DEMBED -DCCS -DTARGET_IS_MSP432P4XX -Dgcc -D__TI_COMPILER_VERSION__ -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/Example_Kyler_TrafficLight/DEVS_Models" -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/MSP432P4xx" -I"C:/ti/ccs1250/ccs/ccs_base/arm/include" -I"S:/UNI PROGRAMMING/SYSC 4907 Year Project/cadmium_v2_rt_msp432-dev-rt/include/cadmium" -I"C:/ti/ccs1250/ccs/ccs_base/arm/include/CMSIS" -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/Example_Kyler_TrafficLight" -I"C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c++17  -fno-threadsafe-statics -Wno-register $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-gcc-10.3.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fexceptions -D__MSP432P401R__ -DNO_LOGGING -DEMBED -DCCS -DTARGET_IS_MSP432P4XX -Dgcc -D__TI_COMPILER_VERSION__ -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/Example_Kyler_TrafficLight/DEVS_Models" -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/MSP432P4xx" -I"C:/ti/ccs1250/ccs/ccs_base/arm/include" -I"S:/UNI PROGRAMMING/SYSC 4907 Year Project/cadmium_v2_rt_msp432-dev-rt/include/cadmium" -I"C:/ti/ccs1250/ccs/ccs_base/arm/include/CMSIS" -I"C:/Users/kyler/workspace_v12/cadmium_v2_rt_msp432-dev-rt/example/rt_msp432/Example_Kyler_TrafficLight" -I"C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c17 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


