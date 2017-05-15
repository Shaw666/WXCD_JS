################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c2000_6.2.0

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./user/zm5168.obj" \
"./user/timer.obj" \
"./user/sim_spi.obj" \
"./user/pid.obj" \
"./user/main.obj" \
"./user/hardiic.obj" \
"./user/gpio.obj" \
"./user/communicationSci.obj" \
"./user/ad.obj" \
"./user/GlobalVariableDefandInits.obj" \
"./user/Exint.obj" \
"./user/DataScope_DP.obj" \
"./user/CANcomm.obj" \
"./DSP2803x_usDelay.obj" \
"./DSP2803x_SysCtrl.obj" \
"./DSP2803x_Sci.obj" \
"./DSP2803x_PieVect.obj" \
"./DSP2803x_PieCtrl.obj" \
"./DSP2803x_I2C.obj" \
"./DSP2803x_Gpio.obj" \
"./DSP2803x_GlobalVariableDefs.obj" \
"./DSP2803x_EPwm.obj" \
"./DSP2803x_DefaultIsr.obj" \
"./DSP2803x_CpuTimers.obj" \
"./DSP2803x_CodeStartBranch.obj" \
"./DSP2803x_Adc.obj" \
"../DSP2803x_Headers_nonBIOS.cmd" \
"../F28035v1.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include user/subdir_vars.mk
-include subdir_rules.mk
-include user/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: WXCD_JS.out

# Tool invocations
WXCD_JS.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"WXCD_JS.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" -i"E:/workspace_v5_5_pfc/WXCD_JS" -i"E:/workspace_v5_5_pfc/WXCD_JS/DSP2803x_common/lib" -i"E:/workspace_v5_5_pfc/WXCD_JS/IQmath/v160/lib" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="WXCD_JS_linkInfo.xml" --rom_model -o "WXCD_JS.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QUOTED) "WXCD_JS.out"
	-$(RM) "DSP2803x_Adc.pp" "DSP2803x_CpuTimers.pp" "DSP2803x_DefaultIsr.pp" "DSP2803x_EPwm.pp" "DSP2803x_GlobalVariableDefs.pp" "DSP2803x_Gpio.pp" "DSP2803x_I2C.pp" "DSP2803x_PieCtrl.pp" "DSP2803x_PieVect.pp" "DSP2803x_Sci.pp" "DSP2803x_SysCtrl.pp" "user\CANcomm.pp" "user\Exint.pp" "user\GlobalVariableDefandInits.pp" "user\ad.pp" "user\communicationSci.pp" "user\gpio.pp" "user\hardiic.pp" "user\main.pp" "user\pid.pp" "user\sim_spi.pp" "user\timer.pp" "user\zm5168.pp" 
	-$(RM) "user\DataScope_DP.pp" 
	-$(RM) "DSP2803x_Adc.obj" "DSP2803x_CodeStartBranch.obj" "DSP2803x_CpuTimers.obj" "DSP2803x_DefaultIsr.obj" "DSP2803x_EPwm.obj" "DSP2803x_GlobalVariableDefs.obj" "DSP2803x_Gpio.obj" "DSP2803x_I2C.obj" "DSP2803x_PieCtrl.obj" "DSP2803x_PieVect.obj" "DSP2803x_Sci.obj" "DSP2803x_SysCtrl.obj" "DSP2803x_usDelay.obj" "user\CANcomm.obj" "user\DataScope_DP.obj" "user\Exint.obj" "user\GlobalVariableDefandInits.obj" "user\ad.obj" "user\communicationSci.obj" "user\gpio.obj" "user\hardiic.obj" "user\main.obj" "user\pid.obj" "user\sim_spi.obj" "user\timer.obj" "user\zm5168.obj" 
	-$(RM) "DSP2803x_CodeStartBranch.pp" "DSP2803x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-@echo 'Create flash image: Intel-HEX'
	-"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/hex2000" -i "WXCD_JS.out" -o "WXCD_JS.hex" -order MS -romwidth 16
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

