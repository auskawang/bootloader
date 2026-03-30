#include <stdint.h>
#include "defs.h"

extern void EXTI415_Callback();
extern void TIM14_Callback();

void app_flash_erase();
void app_flash_write(uint8_t* buffer, int len);

int main() {
	//configure onboard button as input (PC13)
	*((int*)RCC_IOPENR) |= 0x4;
	*((int*)GPIOC_MODER) &= ~(0x3 << (2 * BUTTON_PIN));

	//configure onboard led (PA5)
	*((int*)RCC_IOPENR) |= 0x1;
	*((int*)GPIOA_MODER) &= ~(0x3 << (2 * LED_PIN));
	*((int*)GPIOA_MODER) |= 0x1 << (2 * LED_PIN);

	//SysTick setup
	*((int*)STK_RVR) = 0x5DB;
	*((int*)STK_CVR) = 1;
	*((int*)STK_CSR) |= 0x3;

	//setup TIM14 for interrupt based LED blinking
	*((int*)RCC_APBENR2) |= 0x8000; //enable tim14
	*((int*)TIM14_DIER) |= 0x1;	//enable interrupt generation
	*((int*)NVIC_ISER) |= 0x80000; //enable interrupt for NVIC
	*((int*)TIM14_PSC) = 0x320; //PS of 1
	*((int*)TIM14_ARR) = 0x258;

	//VCP USART2 setup
	*((int*)RCC_IOPENR) |= 0x1;
	*((int*)RCC_APBENR1) |= RCC_APBENR1_USART2EN; 	//enable apb clock for usart2 (12mhz)
										//program M bits in usart_cr1 (reset value)
	*((int*)USART2_BRR) = 0x4E2; 		//9600 set baud rate (12000000 (input clock) / 9600 (desired baud)) p.762
	*((int*)USART2_CR1) |= 0x1; 		//enable usart
	*((int*)USART2_CR1) |= 0xc;			//enable usart transmitter and receiver

	*((int*)GPIOA_MODER) &= ~(0xF << 4);   // clear pa2, pa3  p187
	*((int*)GPIOA_MODER) |=  (0xA << 4);   // AF mode pa2, pa3
	*((int*)GPIOA_AFRL) |= 0x1 << 8;		//afsel2 = 0001 p191
	*((int*)GPIOA_AFRL) |= 0x1 << 12;    //afsel3 = 0001

	//print a via usart to terminal to indicate bootloader
	if (*((int*)USART2_ISR) & USART2_ISR_TXE_Msk) {
		*((int*)USART2_TDR) = 0x61;
	}

	//TODO: check for button hold here (active low)
	if ((*((int*)GPIOC_IDR) & (1U << 13)) == 0) {
		//TODO: flash onboard LED rapidly to indicate firmware update
		*((int*)TIM14_CR1) |= 0x1; //start the counter to trigger tim14 interrupt

		//enable CRC
		*(int*)RCC_AHBENR |= (1U << 12);

		*(volatile uint32_t*)CRC_CR = (1U << 0);

		app_flash_erase();

		uint8_t buffer[380];
		uint8_t* temp = buffer;
		for (int i = 0; i < 380; i++) {
			while ((*((int*)USART2_ISR) & USART2_ISR_RXNE_Msk) == 0) {}
			*temp++ = *((uint8_t*)USART2_RDR);
			if (*((int*)USART2_ISR) & USART2_ISR_TXE_Msk) {
				*((int*)USART2_TDR) = 0x1;
			}
		}

		int crc_val;
		//check CRC
		for (int i = 0; i < 380; i++) {
			*(uint8_t*)CRC_DR = buffer[i];
		}
		crc_val = *(uint32_t*)CRC_DR;

		uint8_t server_crc[4];

		for (int i = 0; i < 4; i++) {
			while ((*((int*)USART2_ISR) & USART2_ISR_RXNE_Msk) == 0) {}
			server_crc[i] = *((uint8_t*)USART2_RDR);
		}
		int server_crc_calc = server_crc[0] << 24 | server_crc[1] << 16 | server_crc[2] << 8 | server_crc[3] << 0;
		if (server_crc_calc != crc_val)
			//TODO: keep LED on to indicate failure
			while(1) {}

		app_flash_write(buffer, 380);
		*((int*)TIM14_CR1) &= ~0x1; //start the counter to trigger tim14 interrupt
	}
	//TODO: end if block and turn off onboard LED

	__asm volatile ("cpsid i" : : : "memory");
	//switch stack
	uint32_t app_stack_pointer = *(volatile uint32_t*)0x08004000;
	__asm volatile ("MSR msp, %0" : : "r" (app_stack_pointer) : );
	//switch vector tables
	*(uint32_t*)SCB_VTOR = 0x08004000;
	//jump to app reset handler
	uint32_t jump_address = *(volatile uint32_t*)(0x08004000 + 4);

	void (*app_reset_handler)(void) = (void (*)(void))jump_address;

	app_reset_handler();

	while(1) {

	}

	return 1;
}

void app_flash_erase() {
	//unlock flash
	*(uint32_t*)FLASH_KEYR = KEY1;
	*(uint32_t*)FLASH_KEYR = KEY2;

	for (int page = 8; page < 15; page++) {
		//BSYS1 flag not set
		while (((*(int*)FLASH_SR) & 0x10000) == 1) {};

		//clear all error programming flags
		while (((*(int*)FLASH_SR) & 0x10) == 1) {};

		//CFGBSY flag cleared
		while (((*(int*)FLASH_SR) & 0x40000) == 1) {};

		//enable page erase and select pages 8-15 to erase
		*(int*)FLASH_CR |= 0x2;
		*(int*)FLASH_CR &= ~(0x7F << 3);
		*(int*)FLASH_CR |= (page << 3);

		//start
		*(int*)FLASH_CR |= 0x10000;

		//wait for CFGBSY to clear
		while (((*(int*)FLASH_SR) & 0x40000) == 1) {};
	}
	//disable PE
	*(int*)FLASH_CR &= ~0x2;
	//lock flash
	*(uint32_t*)FLASH_CR |= (1U << 31);
}

void app_flash_write(uint8_t* buffer, int len) {
	//unlock flash
		*(uint32_t*)FLASH_KEYR = KEY1;
		*(uint32_t*)FLASH_KEYR = KEY2;

	//BSYS1 flag not set
	while (((*(int*)FLASH_SR) & 0x10000) == 1) {};

	//clear all error programming flags
	while (((*(int*)FLASH_SR) & 0x10) == 1) {};

	//CFGBSY flag cleared
	while (((*(int*)FLASH_SR) & 0x40000) == 1) {};

	//enable PG
	*(int*)FLASH_CR |= 0x1;

	uint32_t* flash_itr = APP_FLASH_START;
	uint32_t* buffer_itr = (uint32_t*)buffer;
	uint32_t* holder_top;

	for (int i = 0; i < len / 4; i+=2) {
		*(flash_itr + i) = *(buffer_itr + i);

		if (i + 1 < len / 4)
			*(flash_itr + i + 1) = *(buffer_itr + i + 1);
		else
			*(flash_itr + i + 1) = 0xFFFFFFFF;

		//CFGBSY flag cleared
		while (((*(int*)FLASH_SR) & 0x40000) == 1) {};

		//clear EOP flag
		*(int*)FLASH_SR |= 0x1;

	}

	//clear PG
	*(int*)FLASH_CR &= ~0x1;

	//lock flash
		*(uint32_t*)FLASH_CR |= (1U << 31);

}

//called every 5.46ms
void TIM14_Callback() {
	*((int*)GPIOA_ODR) ^= 0x1 << LED_PIN;

	//clear interrupt flags
	*((int*)TIM14_SR) &= ~0x3;
}
