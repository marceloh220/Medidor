#Marcelo H Moraes
#marceloh220@hotmail.com
#
#Used with avr
#Dependences:	avr-gcc 7.1
#				avr-libc 2.0
#				avr-binutils 2.28
#
#
#The MIT License (MIT)
#
#Copyright (c) 2017 Marcelo Henrique Moraes Dermones
#
#Permission is hereby granted, free of charge, to any person obtaining a copy of
#this software and associated documentation files (the "Software"), to deal in
#the Software without restriction, including without limitation the rights to
#use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
#the Software, and to permit persons to whom the Software is furnished to do so,
#subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
#COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#options of sketch
sketch = medidor.cpp
mcu = atmega88
clock = 16000000UL

#options of compiler
debug = -ffunction-sections -fdata-sections
debug = -g2 -gdwarf-2
warnings = -Wall
optmization = -Os -MMD -flto -felide-constructors
CPPflags = -std=gnu++14 -fpermissive -fno-threadsafe-statics -fno-exceptions -x c++
LINKflags = -fuse-linker-plugin -Wl,--gc-sections

#binaries
compilerCPP = avr-g++
compilerC = avr-gcc
pack = avr-objcopy

all:
	
	$(compilerCPP) $(optmization) $(CPPflags) $(debug) $(warnings) -mmcu=$(mcu) -DF_CPU=$(clock) "-Iclass/" "-Idefines/" -c $(sketch) -o $(sketch).o
	
	$(compilerCPP) $(optmization) $(LINKflags) $(debug) $(warnings) -mmcu=$(mcu) $(sketch).o -o $(sketch).elf
	
	$(pack) -O ihex $(sketch).elf $(sketch).hex
	
	avr-size -C  -x --mcu=$(mcu) $(sketch).elf

	rm -f *.o
	rm -f *.d
