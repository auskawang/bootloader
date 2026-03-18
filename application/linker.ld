ENTRY(Reset_Handler)
MEMORY
{
	FLASH(rwx):ORIGIN =0x08004000,LENGTH =16K
	SRAM(rwx):ORIGIN =0x20001800,LENGTH =6K
	
}
SECTIONS
{
	.vec_table :
	{
		. = ALIGN(4);
		KEEP(*(.vec_table))
		. = ALIGN(4);
	}>FLASH
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.text))
		. = ALIGN(4);
	}>FLASH
	.data :
	{
		_sdata = .;
		*(.data)
		_edata = .;
	} >SRAM AT>FLASH
	_ld_data = LOADADDR(.data);
	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
		
	} >SRAM
}