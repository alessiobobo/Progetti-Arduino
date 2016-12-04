###############################################################################
# Makefile for the project 20130212_mega8_usbisp
###############################################################################

## General Flags
PROJECT = 20130212_mega8_usbisp
MCU = atmega8
TARGET = 20130212_mega8_usbisp.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=20130212_mega8_usbisp.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = 20130212_mega8_usbisp.o clock.o isp.o oddebug.o usbdrv.o usbdrvasm.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) 20130212_mega8_usbisp.hex 20130212_mega8_usbisp.eep 20130212_mega8_usbisp.lss size

## Compile
20130212_mega8_usbisp.o: ../20130212_mega8_usbisp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

isp.o: ../isp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

oddebug.o: ../oddebug.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usbdrv.o: ../usbdrv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usbdrvasm.o: ../usbdrvasm.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) 20130212_mega8_usbisp.elf dep/* 20130212_mega8_usbisp.hex 20130212_mega8_usbisp.eep 20130212_mega8_usbisp.lss 20130212_mega8_usbisp.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

