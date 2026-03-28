#include <stdint.h>
#define RCC 0x40021000
#define RCC_CR (RCC)
#define RCC_IOPENR (RCC + 0x34)
#define RCC_APBENR2 (RCC + 0x40)

#define RCC_APBENR1 (RCC + 0x3C)
#define RCC_APBENR2 (RCC + 0x40)
#define RCC_CCIPR (RCC + 0x54)

#define RCC_APBENR1_USART2EN_Pos    (17U)
#define RCC_APBENR1_USART2EN_Msk    (1U << RCC_APBENR1_USART2EN_Pos)
#define RCC_APBENR1_USART2EN        RCC_APBENR1_USART2EN_Msk

#define RCC_AHBENR (RCC + 0x38)

#define GPIOA 0x50000000
#define GPIOA_MODER (GPIOA + 0x00)
#define GPIOA_ODR (GPIOA + 0x14)
#define GPIOA_BRR (GPIOA + 0x28)


#define GPIOA_AFRL (GPIOA + 0x20)

#define GPIOB 0x50000400
#define GPIOB_MODER (GPIOB + 0x00)
#define GPIOB_ODR (GPIOB + 0x14)
#define GPIOB_BRR (GPIOB + 0x28)
#define GPIOB_AFRL (GPIOB + 0x20)


#define GPIOC 0x50000800
#define GPIOC_MODER (GPIOC + 0x00)
#define GPIOC_PUPDR (GPIOC + 0x0C)

#define EXTI 0x40021800
#define EXTI_EXTICR_4 (EXTI + 0x060 + 0x4 * (4 - 1))
#define EXTI_FTSR1 (EXTI + 0x004)
#define EXTI_FPR1 (EXTI + 0x010)
#define EXTI_IMR1 (EXTI + 0x080)

#define NVIC_ISER 0xE000E100

#define MPU_CTRL 0xE000ED94

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

#define TIM14 0x40002000
#define TIM14_CR1 (TIM14 + 0x0)
#define TIM14_SR (TIM14 + 0x10)
#define TIM14_DIER (TIM14 + 0xC)
#define TIM14_CNT (TIM14 + 0x24)
#define TIM14_PSC (TIM14 + 0x28)
#define TIM14_ARR (TIM14 + 0x2C)



#define USART1 0x40013800
#define USART1_CR1 (USART1 + 0x0)
#define USART1_BRR (USART1 + 0xC)
#define USART1_ISR (USART1 + 0x1C)
#define USART1_ISR_TXE_POS 7
#define USART1_ISR_TXE_Msk (1U << 7)
#define USART1_TDR (USART1 + 0x28)

#define USART2 0x40004400
#define USART2_CR1 (USART2 + 0x0)
#define USART2_BRR (USART2 + 0xC)
#define USART2_ISR (USART2 + 0x1C)
#define USART2_ISR_TXE_POS 7
#define USART2_ISR_TXE_Msk (1U << USART2_ISR_TXE_POS)
#define USART2_ISR_RXNE_POS 5
#define USART2_ISR_RXNE_Msk (1U << USART2_ISR_RXNE_POS)
#define USART2_TDR (USART2 + 0x28)
#define USART2_RDR (USART2 + 0x24)


#define SHPR3 0xE000ED20
#define NVIC_IPR4 0xE000E410

#define SCB_VTOR 0xE000ED08
#define APP_FLASH_START (uint8_t*)0x08004000
#define APP_FLASH_SIZE (1U << 14)
#define FLASH 0x40022000
#define FLASH_KEYR (FLASH + 0x008)
#define FLASH_CR (FLASH + 0x014)
#define FLASH_SR (FLASH + 0x010)
#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

#define CRC  0x40023000
#define CRC_DR (CRC + 0x00)
#define CRC_CR       (CRC + 0x08)
#define CRC_INIT (CRC + 0x10)
#define CRC_POL (CRC + 0x14)

extern void EXTI415_Callback();
extern void TIM14_Callback();

void app_flash_erase();
void app_flash_write(uint8_t* buffer, int len);

int main() {
	uint32_t final_result = 0;
	//SysTick setup
	*((int*)STK_RVR) = 0x5DB;
	*((int*)STK_CVR) = 1;
	*((int*)STK_CSR) |= 0x3;

	//enable CRC
	*(int*)RCC_AHBENR |= (1U << 12);

	*(volatile uint32_t*)CRC_CR = (1U << 0);



	//final_result = (*(volatile uint32_t*)CRC_DR) ^ 0xFFFFFFFF;

	//VCP USART2 setup
	*((int*)RCC_IOPENR) = 0x1;
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
		while(1) {}

	app_flash_write(buffer, 380);


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
