# Custom Bootloader

## Overview
The goal of this project is to create a custom bootloader on a STM32. The importance of the bootloader is that it will be able to let you update firmware without having to go through the whole procedure of flashing. Bootloaders also allow phones, smartwatches, etc to be updated with new features and security features safely.

## Specifications
- UART to receive firmware, which is in the form of raw binary
- CRC32 to validate
- Boot trigger: timeout or command


## Log
<img width="372" height="575" alt="image" src="https://github.com/user-attachments/assets/906582c1-8ea1-4f55-b59e-576b6f9d8a02" />






# Previously
This is a side project where I attempt to blink an LED, and hopefully do other things, on the MCU without using vendored code, meaning that I write the code required to run the MCU (linker, startup file, makefile, etc). This readme will also serve as an update page.

## Log
10/29: had issues with code breaking at 0x0. Implemented hardfault function which then it broke there instead. Reviewed SP and saw that it went into reserved space since I set SP wrong in vector table. After setting it right the PC finally accessed the Reset Handler :D

11/10: the map file shows the addresses that the CPU will go to when accessing globals. therefore in startup code its important to copy from flash to the addresses that the map file has
additionally, these addresses are mapped to sram regions, meaning that before the startup code that copies over data from flash to sram, this sram region has trash data. this means that using a global variable in assisting with copying over data does not work, so you must use linker variables or use const globals since the map file links addresses in flash to these variables rather than in sram

11/18: value of the location counter (.) relies on the location of the memory section, whether it starts at >FLASH or >SRAM. If copying over data from FLASH (>SRAM AT>FLASH), then we can access the location of where to start copying from using LOADADDR(section_name)

12/15: implemented on board button interrupt to toggle on board led. learned through reference manual that there are board peripherals such as gpio and core peripherals such as NVIC. activated button gpio as interrupt through EXTI and enabled the interrupt through NVIC. NVIC is kind of like the main interrupt handler while EXTI is an outer peripheral that allows board peripherals to signal to EXTI to send interrupts? also learned that interrupts can be pending and activated. had some hard faulting occuring as a result of wrong bit masking, resulting in me learning about privilege levels to see if that was the issue when i was trying to change core peripheral memory

12/16: implemented a delay function using systick which wasn't too bad given that i had already implemented an exti interrupt. the hard part was figuring out the clock frequency. i saw that systick dependended on hsi48 which was a 48mhz clock so i set the reload accordingly to fire every 1ms. the delay took too long so after further digging systick dependend on hsi/8 so i divided the reload by 8. however it was still to long so after further consulting the reference manual i saw that on startup the register used to control the prescaler of the hsi clock was defaulted to 4, so i dvided the reload by 4 and finally got a decent millisecond delay going.

12/18: used tim14 to trigger its interrupt every couple milliseconds. the interrupt would toggle an led based on a counter in the interrupt that would increment, if the counter was below a certain threshold then the led would be on and above the threshold the led would be off. the threshold is the brightness level. this was basically a pwm clock but more manual and i did it this way because i had no idea how to implement pwm this way in a past interview. at first the call back function for the timer kept triggering which i first found out when the program was constantly getting stuck on the call back function, to which i found out that i needed to clear the interrupt pending flags for the timer through the manual. after getting different brightnesses i hardcoded a sine lookup table to produce a nice pulsing on board led animation.

3/1/26: learned that usart2 is connected to the usb to uart converter and usart1 is not. each pin also has alternate mappings that i had to search for in another manual. most of the time could have been saved if i knew that usart2 was the only usart that was physically connected to the bridge.
