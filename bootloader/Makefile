all: link
link: main_bootloader.o startup_bootloader.o
	arm-none-eabi-gcc -T linker.ld -nostartfiles -Wl,-Map=output.map -o main.elf main_bootloader.o startup_bootloader.o
startup_bootloader.o: Core/Startup/startup_bootloader.c
	arm-none-eabi-gcc -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0 Core/Startup/startup_bootloader.c -o startup_bootloader.o
main_bootloader.o: Core/Src/main_bootloader.c
	arm-none-eabi-gcc -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0 Core/Src/main_bootloader.c -o main_bootloader.o
clean:
	del *.o *.elf