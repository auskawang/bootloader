all: app.bin
app.bin: app.elf
	arm-none-eabi-objcopy -O binary app.elf app.bin
app.elf: main_app.o startup_app.o
	arm-none-eabi-gcc -T linker.ld -nostartfiles -Wl,-Map=output.map -o app.elf main_app.o startup_app.o
startup_app.o: Core/Startup/startup_app.c
	arm-none-eabi-gcc -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0 Core/Startup/startup_app.c -o startup_app.o
main_app.o: Core/Src/main_app.c
	arm-none-eabi-gcc -g -c -mcpu=cortex-m0plus -mthumb -std=gnu11 -Wall -O0 Core/Src/main_app.c -o main_app.o
clean:
	del *.o *.elf