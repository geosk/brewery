################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include libs/SoftwareSerial/subdir.mk
-include libs/OneWire/subdir.mk
-include libs/EEPROM/subdir.mk
-include libs/DallasTemperature/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AVRBrewery.lss \

FLASH_IMAGE += \
AVRBrewery.hex \

EEPROM_IMAGE += \
AVRBrewery.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: AVRBrewery.elf secondary-outputs

# Tool invocations
AVRBrewery.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,AVRBrewery.map,--cref -L"D:\Work\eclipse\ArduinoCore\Release" -mmcu=atmega328p -o "AVRBrewery.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

AVRBrewery.lss: AVRBrewery.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AVRBrewery.elf  >"AVRBrewery.lss"
	@echo 'Finished building: $@'
	@echo ' '

AVRBrewery.hex: AVRBrewery.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex AVRBrewery.elf  "AVRBrewery.hex"
	@echo 'Finished building: $@'
	@echo ' '

AVRBrewery.eep: AVRBrewery.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex AVRBrewery.elf  "AVRBrewery.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AVRBrewery.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p AVRBrewery.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) AVRBrewery.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
