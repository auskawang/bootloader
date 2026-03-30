#include <stdint.h>

extern int _sdata;
extern int _edata;
extern int _sbss;
extern int _ebss;
extern int _ld_data;

void Reset_Handler();
void HardFault_Handler();
void SysTick_Handler();
void TIM14_Callback();
void delay();
int main();

int volatile counter = 0;

uint32_t vectors[] __attribute__((section(".vec_table")))= {
	0x200017FF,
	(uint32_t)Reset_Handler,
	0,
	(uint32_t)HardFault_Handler,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0, //SV call handler
	0,
	0,
	0, //PendSV
	(uint32_t)SysTick_Handler, //SysTick
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	(uint32_t)TIM14_Callback
};

void Reset_Handler() {
	int* src = &_ld_data;
	int* dst_data = &_sdata;
	int* end_data = &_edata;

	int* dst_bss = &_sbss;
	int* end_bss = &_ebss;

	//copy data from FLASH to SRAM
	for (int* i = dst_data; i < end_data; i++) {
		*(i) = *(src++);
	}

	//copy from FLASH TO SRAM for bss
	for (int *i = dst_bss; i < end_bss; i++) {
		*(i) = 0;
	}

	//call main
	main();
}

void HardFault_Handler() {
	while(1) {};
}

void SysTick_Handler() {
	counter++;
}




