ENTRY(Reset_Handler)
MEMORY
{
	BOOTLOADER_FLASH(rx):ORIGIN =0x08000000,LENGTH =16K
	BOOTLOADER_SRAM(rwx):ORIGIN =0x20000000,LENGTH =6K	
}
SECTIONS
{
	.vec_table :
	{
		. = ALIGN(4);
		KEEP(*(.vec_table))
		. = ALIGN(4);
	}>BOOTLOADER_FLASH
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.text))
		. = ALIGN(4);
	}>BOOTLOADER_FLASH
	.data :
	{
		_sdata = .;
		*(.data)
		_edata = .;
	} >BOOTLOADER_SRAM AT>BOOTLOADER_FLASH
	_ld_data = LOADADDR(.data);
	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
		
	} >BOOTLOADER_SRAM
}