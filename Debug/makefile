################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include hal/CortexM3/GNU/subdir.mk
-include hal/CortexM3/subdir.mk
-include drivers_config/sys_config/subdir.mk
-include drivers/mss_uart/subdir.mk
-include drivers/mss_sys_services/subdir.mk
-include drivers/mss_gpio/subdir.mk
-include CMSIS/startup_gcc/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Assisted_Boot.hex \

SECONDARY_SIZE += \
Assisted_Boot.siz \


# All Target
all: Assisted_Boot.elf secondary-outputs

# Tool invocations
Assisted_Boot.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "D:\Projects\sf2_m3_bootloader\risc-v-assisted-boot.ld" -Xlinker --gc-sections -Wl,-Map,"Assisted_Boot.map" --specs=nano.specs -o "Assisted_Boot.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Assisted_Boot.hex: Assisted_Boot.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Assisted_Boot.elf"  "Assisted_Boot.hex"
	@echo 'Finished building: $@'
	@echo ' '

Assisted_Boot.siz: Assisted_Boot.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "Assisted_Boot.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Assisted_Boot.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
