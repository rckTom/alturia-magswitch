TARGET = alturia-magswitch

MCU ?= attiny4
F_OSC ?= 1000000UL
AVRDUDE_MCU ?= t4

PROGRAMMER=usbasp

OBJS = main.o

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS := -g -Os -mmcu=$(MCU)
CFLAGS += -Wall -Wextra -Wstrict-prototypes
CFLAGS += -DF_OSC=$(F_OSC) -DF_CPU=F_OSC -DMCU=$(MCU)

all: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET).elf $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $^ $@
	avr-size -C --mcu=$(MCU) $(TARGET).elf

flash: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -U flash:w:$^:i

fuse:
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -U fuse:w:0xFF:m

clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf $(TARGET).hex
