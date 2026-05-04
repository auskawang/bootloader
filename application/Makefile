APP_A_START = 0x08001200
APP_A_END = 0x080047FF
APP_A_LEN   = $(shell echo $$(($(APP_A_END) - $(APP_A_START))))

APP_B_START = 0x08004A00
APP_B_END = 0x08007FFF
APP_B_LEN   = $(shell echo $$(($(APP_B_END) - $(APP_B_START))))

# Define Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Common Flags
CFLAGS = -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0
LDFLAGS = -T linker.ld -nostartfiles

# Source Files
SRCS = Core/Src/main_app.c Core/Startup/startup_app.c

all: app_slot_a.bin app_slot_b.bin

# Logic for Slot A
app_slot_a.bin: $(SRCS)
	$(CC) $(CFLAGS) -DAPP_START_ADDR=0x08004010 Core/Src/main_app.c -o main_a.o
	$(CC) $(CFLAGS) Core/Startup/startup_app.c -o startup_a.o
	$(CC) -T linker.ld -nostartfiles -Wl,--defsym=_app_start=$(APP_A_START) -Wl,--defsym=_flash_len=$(APP_A_LEN) -Wl,-Map=slot_a.map -o app_slot_a.elf main_a.o startup_a.o
	$(OBJCOPY) -O binary app_slot_a.elf app_slot_a.bin

# Logic for Slot B
app_slot_b.bin: $(SRCS)
	$(CC) $(CFLAGS) -DAPP_START_ADDR=0x08006010 Core/Src/main_app.c -o main_b.o
	$(CC) $(CFLAGS) Core/Startup/startup_app.c -o startup_b.o
	$(CC) -T linker.ld -nostartfiles -Wl,--defsym=_app_start=$(APP_B_START) -Wl,--defsym=_flash_len=$(APP_B_LEN) -Wl,-Map=slot_b.map -o app_slot_b.elf main_b.o startup_b.o
	$(OBJCOPY) -O binary app_slot_b.elf app_slot_b.bin

clean:
	del /Q *.o *.elf *.bin *.map 2>nul