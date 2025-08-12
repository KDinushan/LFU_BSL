################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./BSL_CI.o" \
"./dl_flashctl.o" \
"./main-tasks-create.o" \
"./main_freertos.o" \
"./ti_msp_dl_config.o" \
"./uart.o" \
"./freertos/ticlang/startup_mspm0g3507_ticlang.o" \
"../mspm0gxx_boot.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-l"C:/ti/mspm0_sdk_1_00_01_03/source/ti/drivers/lib/ticlang/m0p/drivers_mspm0g150x_g350x_nortos.a" \
-Wl,-l"C:/ti/mspm0_sdk_1_00_01_03/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
-Wl,-lfreertos_builds_LP_MSPM0G3507_release_ticlang.lib \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include freertos/ticlang/subdir_vars.mk
-include subdir_rules.mk
-include freertos/ticlang/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.out \

EXE_OUTPUTS__QUOTED += \
"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.out" \

BIN_OUTPUTS += \
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt \
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex \

BIN_OUTPUTS__QUOTED += \
"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt" \
"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex" \

BIN_OUTPUTS += \
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt \
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex \

BIN_OUTPUTS__QUOTED += \
"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt" \
"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.out" secondary-outputs

dependents:
	-cd C:/Users/kaush/Desktop/LFU\ BSL/slaaec9/MSPM0\ LFU\ Bootloader\ Implementation\ release\ v1.2/Target_MSPM0G3507_demos/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug && $(MAKE) all

# Tool invocations
LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/Users/kaush/Desktop/LFU\ BSL/slaaec9/MSPM0\ LFU\ Bootloader\ Implementation\ release\ v1.2/Target_MSPM0G3507_demos/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug/freertos_builds_LP_MSPM0G3507_release_ticlang.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmclang.exe" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -D__MSPM0G3507__ -gdwarf-3 -Wl,-m"LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.map" -Wl,--stack_size=512 -Wl,-i"C:/Users/kaush/Desktop/LFU BSL/slaaec9/MSPM0 LFU Bootloader Implementation release v1.2/Target_MSPM0G3507_demos/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug" -Wl,-i"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang_linkInfo.xml" -Wl,--rom_model -o "LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmhex.exe" --diag_wrap=off --ti_txt -o "LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Objcopy Utility'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmobjcopy.exe"  $(EXE_OUTPUTS__QUOTED) "LFU_Bootloader_LP_MSPM0G3507_freertos_ticlang.hex"
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "BSL_CI.o" "dl_flashctl.o" "main-tasks-create.o" "main_freertos.o" "ti_msp_dl_config.o" "uart.o" "freertos\ticlang\startup_mspm0g3507_ticlang.o" 
	-$(RM) "BSL_CI.d" "dl_flashctl.d" "main-tasks-create.d" "main_freertos.d" "ti_msp_dl_config.d" "uart.d" "freertos\ticlang\startup_mspm0g3507_ticlang.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS) $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/kaush/Desktop/LFU\ BSL/slaaec9/MSPM0\ LFU\ Bootloader\ Implementation\ release\ v1.2/Target_MSPM0G3507_demos/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug/freertos_builds_LP_MSPM0G3507_release_ticlang.lib:

-include ../makefile.targets

