## HOW TO## 
# To build and flash any part of the lab:
# 	make part1
# 	make part2
# 	make part3
# 	make part4
# To figure what the USB device is called:
# 	1. Unplug the Bumbleboi
# 	2. make base_usb
# 	3. Plug in the Bumbleboi
# 	4. make find_usb
# 	5. Set the *your_platform*_DEVICE variable in this file
# To handin:
# 	(Do this on the server)
# 	make handin

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BOARD_LIB = $(SELF_DIR)/library
SOURCE_LIB = $(wildcard $(BOARD_LIB)/*.c)
CLOCK_RATE = 16000000L

CC = avr-gcc
CFLAGS = -Wall -O2 -pipe -std=c99 -pedantic
BFLAGS = -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a $(CFLAGS)

MAC_DEVICE = /dev/tty.usbmodem14201
LINUX_DEVICE = /dev/ttyACM1
UNAME := $(shell uname -s)
LINUX = Linux

HANDIN_FILES = *.c *.h Makefile

lab5: lab5.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf lab5.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

sample: sample.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf sample.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

test: test.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf test.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

lib: $(wildcard $(BOARD_LIB)/*.c)
	$(foreach var,$(SOURCE_LIB), \
        $(CC) $(BFLAGS) -c -o $(var:.c=.o) $(var); )

program_windows: 
	avrdude -F -pm645 -Pcom4 -carduino -u -U flash:w:main.hex

#edit the device to match the device on your computer
# TODO: @Jared Add you Mac device here
program_mac: main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex
program_linux: main.elf
	avrdude -pm645 -P $(LINUX_DEVICE) -c arduino -F -u -U flash:w:main.hex

clean:
	rm -fr *.elf *.hex *.o *.txt

test_hardware: test_hardware.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -O2 -o main.elf test_hardware.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

prep:
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	make program

compiler_compare: lab4_part2.c $(wildcard $(BOARD_LIB)/*.c)
	@echo "-0s"
	@avr-gcc -Wall -Os -pipe -std=c99 -pedantic -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a -o main.elf lab4_part2.c $(wildcard $(BOARD_LIB)/*.c)
	avr-size main.elf
	@echo "\n-01"
	@avr-gcc -Wall -O1 -pipe -std=c99 -pedantic -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a -o main.elf lab4_part2.c $(wildcard $(BOARD_LIB)/*.c)
	avr-size main.elf
	@echo "\n-02"
	@avr-gcc -Wall -O2 -pipe -std=c99 -pedantic -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a -o main.elf lab4_part2.c $(wildcard $(BOARD_LIB)/*.c)
	avr-size main.elf
	@echo "\n-03"
	@avr-gcc -Wall -O3 -pipe -std=c99 -pedantic -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a -o main.elf lab4_part2.c $(wildcard $(BOARD_LIB)/*.c)
	avr-size main.elf

# Select platform for programming
ifeq ($(UNAME),Linux)
program: program_linux
else
program: program_windows
endif

# Detect USB device
# Pipe all devices to file
base_usb:
	ls /dev > dev.txt
	
# Compare devices to file
find_usb:
	ls /dev | diff - dev.txt

valgrind: net
	valgrind --track-origins=yes --leak-check=full ./lab5

# Handin for use on the Unix servers
handin: $(HANDIN_FILES)
	handin jseng CPE416_lab5 $(HANDIN_FILES)
	handin jseng CPE416_lab5

