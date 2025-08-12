################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/kaush/Desktop/LFU BSL/slaaec9/MSPM0 LFU Bootloader Implementation release v1.2/Target_MSPM0G3507_demos/LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang" -I"C:/ti/mspm0_sdk_1_00_01_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_00_01_03/kernel/freertos/Source/include" -I"C:/ti/mspm0_sdk_1_00_01_03/source" -I"C:/ti/mspm0_sdk_1_00_01_03/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"C:/ti/mspm0_sdk_1_00_01_03/source/ti/posix/ticlang" -I"C:/Users/kaush/Desktop/LFU BSL/slaaec9/MSPM0 LFU Bootloader Implementation release v1.2/Target_MSPM0G3507_demos/freertos_builds_LP_MSPM0G3507_release_ticlang" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


