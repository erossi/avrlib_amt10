# Copyright (C) 2019 Enrico Rossi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Microcontroller definition
#
MCU = atmega328p
FCPU = 16000000UL

# Uncomment if git is in use
GIT_TAG = $(shell git describe --tags --long --always)
TARGET = $@_$(MCU)_$(GIT_TAG)

OPTLEV = 2

PWD = $(shell pwd)
LIBDIR = $(wildcard ../lib/*/src)
VPATH += $(wildcard ../lib/*/src)
BUILD_DIR = ./build
INC = /usr/lib/avr/include
CINC = $(addprefix -I, $(INC) $(LIBDIR))
CXXINC = $(CINC)

CC = avr-gcc
CXX = avr-g++
CFLAGS = $(CINC) \
	 -Wall \
	 -Wstrict-prototypes \
	 -pedantic \
	 -std=c11 \
	 -mmcu=$(MCU) \
	 -O$(OPTLEV) \
	 -D F_CPU=$(FCPU)
CXXFLAGS := $(CXXINC) \
	-Wall \
	-Wextra \
	-pedantic \
	-std=c++14 \
	-mmcu=$(MCU) \
	-O$(OPTLEV) \
	-fshort-enums \
	-D F_CPU=$(FCPU)

LFLAGS = -lm
AR = avr-ar

## Programming device
DUDEPORT = /dev/ttyACM0
DUDEDEV = arduino

# Use sudo for USB avrispmkII
DUDE = sudo avrdude -p $(MCU) -e -U flash:w:$(TARGET).hex
DUDEE = $(DUDE) -U eeprom:w:eeprom.dump:r
DUMPEE = sudo avrdude -p $(MCU) -U eeprom:r:eeprom.dump:r

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size --format=avr --mcu=$(MCU) $(TARGET).elf

REMOVE = rm -f

SRC_FULLPATH = $(foreach dir, $(LIBDIR), $(wildcard $(dir)/*.cpp))
SRC = $(notdir $(SRC_FULLPATH))
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .cpp, .c, .o

## Export variables used in sub-make
#
.EXPORT_ALL_VARIABLES: doc

# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif

# Default target.
all: $(BUILD_DIR)

# Creates the object directory
$(BUILD_DIR):
	$P mkdir -p $(BUILD_DIR)

### Libraries
common_o := \
	amt10.o \
	arduino_led.o

### Tests
# in dependancy order
#
test: \
	test_arduino_led

## Power Test
# depends:
#
test_arduino_led: $(common_o)
	$(Pecho) "  CXX   $@"
	$P $(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@.elf $@.cpp \
		$(addprefix $(BUILD_DIR)/, $(common_o)) \
		$(LFLAGS)

# Standard build
%.o: %.c
	$(Pecho) "  CC    $<"
	$P $(CC) -MMD -c $(CFLAGS) $< -o $(BUILD_DIR)/$@

%.o: %.cpp
	$(Pecho) "  CXX   $<"
	$P $(CXX) -MMD -c $(CXXFLAGS) $< -o $(BUILD_DIR)/$@

# HEX build
%.hex: %.elf
	$(OBJCOPY) %.elf %.hex

## Programming part
#
program:
	$(DUDE) -c $(DUDEDEV) -P $(DUDEPORT)

program_ee:
	$(DUDEE) -c $(DUDEDEV) -P $(DUDEPORT)

dump_eeprom:
	$(DUMPEE) -c $(DUDEDEV) -P $(DUDEPORT)

## CleanUp and tools
#
clean:
	$(REMOVE) $(BUILD_DIR)/*

version:
	# Last Git tag: $(GIT_TAG)

doc:
	$(MAKE) -C ../doc doc

size:
	$(SIZE)
