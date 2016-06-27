################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Algorithm.cpp \
../src/RightReference.cpp \
../src/Stack.cpp \
../src/VariadicTemplates.cpp \
../src/binaryTree.cpp \
../src/circle.cpp \
../src/huffmanTree.cpp \
../src/main.cpp \
../src/rollable.cpp \
../src/shape.cpp \
../src/square.cpp \
../src/topK.cpp 

OBJS += \
./src/Algorithm.o \
./src/RightReference.o \
./src/Stack.o \
./src/VariadicTemplates.o \
./src/binaryTree.o \
./src/circle.o \
./src/huffmanTree.o \
./src/main.o \
./src/rollable.o \
./src/shape.o \
./src/square.o \
./src/topK.o 

CPP_DEPS += \
./src/Algorithm.d \
./src/RightReference.d \
./src/Stack.d \
./src/VariadicTemplates.d \
./src/binaryTree.d \
./src/circle.d \
./src/huffmanTree.d \
./src/main.d \
./src/rollable.d \
./src/shape.d \
./src/square.d \
./src/topK.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


