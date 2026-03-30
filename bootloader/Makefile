# 1. Compiler and Tools
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 2. Compiler Flags
# Added -ICore/Inc so you can #include "defs.h"
CFLAGS  = -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0 -ICore/Inc
LDFLAGS = -T linker.ld -nostartfiles -Wl,-Map=output.map -mcpu=cortex-m0plus -mthumb

# 3. Target and Objects
TARGET  = main
OBJS    = main_bootloader.o startup_bootloader.o

# --- Rules ---

all: $(TARGET).elf $(TARGET).bin

# Link step
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Create a raw binary (essential for flashing your bootloader)
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Compile main_bootloader.c
main_bootloader.o: Core/Src/main_bootloader.c
	$(CC) $(CFLAGS) $< -o $@

# Compile startup_bootloader.c
startup_bootloader.o: Core/Startup/startup_bootloader.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	del /f *.o *.elf *.map *.bin