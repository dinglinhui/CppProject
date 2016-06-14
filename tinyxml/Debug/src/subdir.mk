################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/tinystr.cpp \
../src/tinyxml.cpp \
../src/tinyxmlerror.cpp \
../src/tinyxmlparser.cpp 

OBJS += \
./src/main.o \
./src/tinystr.o \
./src/tinyxml.o \
./src/tinyxmlerror.o \
./src/tinyxmlparser.o 

CPP_DEPS += \
./src/main.d \
./src/tinystr.d \
./src/tinyxml.d \
./src/tinyxmlerror.d \
./src/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


