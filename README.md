# Dual Slot IAP Bootloader
## Features
- application side handles flashing of new application data, allowing for less downtime for useful work
- ~13KB available for each application slot
- UART driven communication between Python script (flash.py) and microcontroller for new application bytes, allowing for simple setup
- CRC checks before application jumps to ensure data integrity
- very lightweight as all of the code does not depend on HAL libraries


# Potential Improvements
## Watchdog Implementation
System has a failure point where if an application unknowingly runs into a fault, there is no way for the system to reset itself to get out of this fault. Watchdog will be able to reset the system and revert back to a known working application slot
## Wireless Implementation
To send over UART board needs to be physically connected via a COM port which makes firmware updates inconvenient
## Reduce the padding
padding is currently used to align the vector table to 512 byte boundary. a potential solution is in the application slot, put the application flash at lower addresses and the header bytes at higher addresses since the header bytes don't require any alignment requirements.

## Memory Boundaries
![](flash_diagram.png)
![](sram_diagram.png)
