################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/EEPROM/EEPROM.cpp 

OBJS += \
./libs/EEPROM/EEPROM.o 

CPP_DEPS += \
./libs/EEPROM/EEPROM.d 


# Each subdirectory must supply rules for building sources it contributes
libs/EEPROM/%.o: ../libs/EEPROM/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Work\eclipse\AVRBrewery\libs\SoftwareSerial" -I"D:\Work\eclipse\AVRBrewery\libs\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard" -I"D:\Work\eclipse\AVRBrewery\libs\OneWire" -I"D:\Work\eclipse\AVRBrewery\libs\DallasTemperature" -DARDUINO=200 -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


