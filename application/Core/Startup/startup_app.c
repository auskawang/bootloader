#include <stdint.h>
#include "defs.h"

extern int _sdata;
extern int _edata;
extern int _sbss;
extern int _ebss;
extern int _ld_data;


void x();
void delay();
int main();
void Reset_Handler();
void HardFault_Handler();
void USART2_Handler();
void SysTick_Handler();
void TIM14_Handler();

int volatile counter = 0;
UpdateState_t current_update_state = UPDATE_IDLE;

void Default_Handler(void) { while(1); }

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void WWDG_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void PVM_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void RTC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void FLASH_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void RCC_CRS_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void EXTI0_1_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void EXTI2_3_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void EXTI4_15_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void USB_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DMA1_Ch1_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void DMA1_Ch2_3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMAMUX_DMA1_Ch4_7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_UP_TRG_COM_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void TIM2_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM3_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM15_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM16_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void TIM17_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void I2C1_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void I2C2_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SPI1_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SPI2_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void USART1_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void USART3_4_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void FDCAN_IT0_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void FDCAN_IT1_Handler(void)  __attribute__((weak, alias("Default_Handler")));

uint32_t vectors[] __attribute__((section(".vec_table"))) = {
    0x20002FFF,                       // 0x00: Initial Stack Pointer
    (uint32_t)Reset_Handler,          // 0x04: Reset
    (uint32_t)NMI_Handler,            // 0x08: NMI
    (uint32_t)HardFault_Handler,      // 0x0C: HardFault
    0,                                // 0x10: Reserved
    0,                                // 0x14: Reserved
    0,                                // 0x18: Reserved
    0,                                // 0x1C: Reserved
    0,                                // 0x20: Reserved
    0,                                // 0x24: Reserved
    0,                                // 0x28: Reserved
    (uint32_t)SVCall_Handler,         // 0x2C: SVCall
    0,                                // 0x30: Reserved
    0,                                // 0x34: Reserved
    (uint32_t)PendSV_Handler,         // 0x38: PendSV
    (uint32_t)SysTick_Handler,        // 0x3C: SysTick
    (uint32_t)WWDG_Handler,           // 0x40: WWDG
    (uint32_t)PVM_Handler,            // 0x44: PVM
    (uint32_t)RTC_Handler,            // 0x48: RTC
    (uint32_t)FLASH_Handler,          // 0x4C: Flash
    (uint32_t)RCC_CRS_Handler,        // 0x50: RCC/CRS
    (uint32_t)EXTI0_1_Handler,        // 0x54: EXTI 0 & 1
    (uint32_t)EXTI2_3_Handler,        // 0x58: EXTI 2 & 3
    (uint32_t)EXTI4_15_Handler,       // 0x5C: EXTI 4 to 15
    (uint32_t)USB_Handler,            // 0x60: USB
    (uint32_t)DMA1_Ch1_Handler,       // 0x64: DMA1 Ch 1
    (uint32_t)DMA1_Ch2_3_Handler,     // 0x68: DMA1 Ch 2 & 3
    (uint32_t)DMAMUX_DMA1_Ch4_7_Handler, // 0x6C: DMAMUX / DMA1 Ch 4-7
    (uint32_t)ADC_Handler,            // 0x70: ADC
    (uint32_t)TIM1_BRK_UP_TRG_COM_Handler, // 0x74: TIM1 BRK/UP/TRG/COM
    (uint32_t)TIM1_CC_Handler,        // 0x78: TIM1 CC
    (uint32_t)TIM2_Handler,           // 0x7C: TIM2
    (uint32_t)TIM3_Handler,           // 0x80: TIM3
    0,                                // 0x84: Reserved
    0,                                // 0x88: Reserved
    (uint32_t)TIM14_Handler,          // 0x8C: TIM14
    (uint32_t)TIM15_Handler,          // 0x90: TIM15
    (uint32_t)TIM16_Handler,          // 0x94: TIM16
    (uint32_t)TIM17_Handler,          // 0x98: TIM17
    (uint32_t)I2C1_Handler,           // 0x9C: I2C1
    (uint32_t)I2C2_Handler,           // 0xA0: I2C2
    (uint32_t)SPI1_Handler,           // 0xA4: SPI1
    (uint32_t)SPI2_Handler,           // 0xA8: SPI2
    (uint32_t)USART1_Handler,         // 0xAC: USART1
    (uint32_t)USART2_Handler,         // 0xB0: USART2
    (uint32_t)USART3_4_Handler,       // 0xB4: USART3/4
    (uint32_t)FDCAN_IT0_Handler,      // 0xB8: FDCAN IT0
    (uint32_t)FDCAN_IT1_Handler       // 0xBC: FDCAN IT1
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

void delay(int milliseconds) {
	counter = 0;
	while (counter < milliseconds) {
		__asm volatile ("nop");
	}
}

void USART2_Handler() {
	if ((*(int*)USART2_ICR) & (1 << 3)) {
		*(int*)USART2_ICR |= 0x8;
	}

	if ((*(int*)USART2_ISR) & USART2_ISR_RXNE_Msk) {
		uint8_t received_byte = *(uint8_t*)USART2_RDR;

		switch (current_update_state) {
			case UPDATE_IDLE:
				if (received_byte == MAGIC_BYTE_1) {
					current_update_state = UPDATE_BYTE1_FOUND;
				}
				break;

			case UPDATE_BYTE1_FOUND:
				if (received_byte == MAGIC_BYTE_2) {
					current_update_state = UPDATE_BYTE2_FOUND;
				} else {
					current_update_state = UPDATE_IDLE; // Reset if sequence is broken
				}
				break;

			case UPDATE_BYTE2_FOUND:
				if (received_byte == MAGIC_BYTE_3) {
					current_update_state = UPDATE_TRIGGERED;
				} else {
					current_update_state = UPDATE_IDLE;
				}
				break;

			default:
				current_update_state = UPDATE_IDLE;
				break;
		}
	}
}




