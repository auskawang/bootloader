#include <stdint.h>
#include <stdlib.h>
#include "defs.h"

extern void EXTI415_Callback();
extern void TIM14_Callback();

void app_flash_erase(uint8_t);
void app_flash_write(uint8_t, uint8_t* buffer, int len);
void jump_to_slot(int slot);
uint8_t get_new_app_data(uint8_t* buffer, int len);
void send_uart_data(uint8_t data);
uint8_t read_uart_data();
uint8_t validate_app_data(uint8_t *buffer, int);
uint8_t validate_slot_data();
uint8_t buffer[500] = {0};
void Error_Handler();
typedef struct {
	int crc;
	int version;
	int size;
	int unused;
} Slot;

Slot* slot_a = (Slot*)0x08004000;
Slot* slot_b = (Slot*)0x08006000;
int crc_value;

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

	//TODO: create global struct for slot A and B
	slot_a->version = *(int*)0x08004000;
	slot_b->version = *(int*)0x08006000;
	slot_a->crc = *(int*)0x08004004;
	slot_b->crc = *(int*)0x08006004;
	slot_a->size = *(int*)0x08004008;
	slot_b->size = *(int*)0x08004008;
	slot_a->unused = *(int*)0x0800400C;
	slot_b->unused = *(int*)0x0800400C;
	//button press check
	if ((*((int*)GPIOC_IDR) & (1U << 13)) == 0) {
		int buffer_len;
		int chosen_slot;
		*((int*)TIM14_CR1) |= 0x1; //start the counter to trigger tim14 interrupt

		//enable CRC
		*(int*)RCC_AHBENR |= (1U << 12);
		*(volatile uint32_t*)CRC_CR = (1U << 0);
		//'R'
		if (read_uart_data() == HEX_R) {

			//first boot
			if (slot_a->version == 0xFFFFFFFF && slot_b->version == 0xFFFFFFFF) {
				chosen_slot = HEX_A;
			}
			else if (slot_a->version > slot_b->version) {
				//covers cases where version might be FFFF (upon reset or something) or if flash write suddenly stopped
				if (slot_a->version != slot_b->version + 1) chosen_slot = HEX_A;
				else chosen_slot = HEX_B;
			}

			else if (slot_b->version > slot_a->version) {
				if (slot_a->version != slot_b->version + 1) chosen_slot = HEX_B;
				chosen_slot = HEX_A;
			}
			//send slot letter so Python file knows which binary file to send
			send_uart_data(chosen_slot);
			//get payload size
			int payload_sz_lower_byte = read_uart_data();
			int payload_sz_upper_byte = read_uart_data();
			buffer_len = 256 * payload_sz_upper_byte + payload_sz_lower_byte;
			send_uart_data(HEX_R);

			//populate buffer with application data from UART
			if (get_new_app_data(buffer, buffer_len) < 0) Error_Handler();
		}
		else {
			Error_Handler();
		}


		app_flash_erase(chosen_slot);
		app_flash_write(chosen_slot, buffer, buffer_len);
		*((int*)TIM14_CR1) &= ~0x1; //stop the counter to disable tim14 interrupt
	}

	if (slot_a->version > slot_b->version) {
		if (slot_a->version == slot_b->version + 1) {
			if (validate_slot_data(HEX_A) == 1)
				jump_to_slot(HEX_A);
		}
		else {
			if (validate_slot_data(HEX_B) == 1)
				jump_to_slot(HEX_B);
		}
	}

	if (slot_b->version > slot_a->version) {
		if (slot_b->version == slot_a->version + 1) {
			if (validate_slot_data(HEX_B))
				jump_to_slot(HEX_B);
		}
		else {
			if (validate_slot_data(HEX_A))
				jump_to_slot(HEX_A);
		}
	}
	//error handler if it gets to this point



	while(1) {

	}

	return 1;
}
void jump_to_slot(int slot) {
	uint32_t* app_stack_pointer;
	if (slot == HEX_A) app_stack_pointer = (uint32_t*)0x08004200;
	if (slot == HEX_B) app_stack_pointer = (uint32_t*)0x08006200;
	__asm volatile ("cpsid i" : : : "memory");
	//switch stack
	__asm volatile ("MSR msp, %0" : : "r" (*app_stack_pointer) : );
	//switch vector tables
	*(uint32_t*)SCB_VTOR = (uint32_t)app_stack_pointer;
	//jump to app reset handler
	uint32_t jump_address = *(volatile uint32_t*)(app_stack_pointer + 1);

	void (*app_reset_handler)(void) = (void (*)(void))jump_address;

	app_reset_handler();
}
void send_uart_data(uint8_t data) {
	if (*((int*)USART2_ISR) & USART2_ISR_TXE_Msk) {
		*((int*)USART2_TDR) = data;
	}
}

uint8_t read_uart_data() {
	while ((*((int*)USART2_ISR) & USART2_ISR_RXNE_Msk) == 0) {}
	return *((uint8_t*)USART2_RDR);
}

uint8_t validate_slot_data(int slot) {
	uint8_t* buffer;
	int len;
	if (slot == HEX_A) {
		len = slot_a->size;
		buffer = (uint8_t*)0x08004200;
	}
	if (slot == HEX_B) {
		len = slot_b->size;
		buffer = (uint8_t*)0x08006200;
	}

	//check CRC
	//enable CRC
	*(int*)RCC_AHBENR |= (1U << 12);
	*(volatile uint32_t*)CRC_CR = (1U << 0);

	for (int i = 0; i < len; i++) {
		*(uint8_t*)CRC_DR = buffer[i];
	}
	crc_value = *(uint32_t*)CRC_DR;

	if (slot == HEX_A) {
		return slot_a->crc == crc_value;
	}
	if (slot == HEX_B)
		return slot_b->crc == crc_value;
}
uint8_t get_new_app_data(uint8_t* buffer, int len) {

	//get response
	if (read_uart_data() == HEX_R) {
		send_uart_data(HEX_R);
		uint8_t* temp = buffer;
		for (int i = 0; i < len; i++) {
			*temp++ = read_uart_data();
			send_uart_data(HEX_ACK);
		}

		//check CRC
		for (int i = 0; i < len; i++) {
			*(uint8_t*)CRC_DR = buffer[i];
		}
		crc_value = *(uint32_t*)CRC_DR;

		uint8_t server_crc[4];

		for (int i = 0; i < 4; i++) {
			server_crc[i] = read_uart_data();
		}
		int server_crc_calc = server_crc[0] << 24 | server_crc[1] << 16 | server_crc[2] << 8 | server_crc[3] << 0;
		if (server_crc_calc != crc_value) return -1;
		return 0;
	}
	return -1;
}

void app_flash_erase(uint8_t slot) {
	int page_start, page_end;
	//unlock flash
	*(uint32_t*)FLASH_KEYR = KEY1;
	*(uint32_t*)FLASH_KEYR = KEY2;

	if (slot == HEX_A) {
		page_start = 8;
		page_end = 11;
	}
	if (slot == HEX_B) {
		page_start = 12;
		page_end = 15;
	}
	//based on slot erase select pages
	for (int page = page_start; page < page_end; page++) {
		//clear all error bits
		*(int*)FLASH_SR |= (1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1);
		//BSYS1 flag not set
		while (((*(int*)FLASH_SR) & 0x10000) != 0) {};

		//clear all error programming flags
		while (((*(int*)FLASH_SR) & 0x10) != 0) {};

		//CFGBSY flag cleared
		while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

		//enable page erase and select pages 8-15 to erase
		*(int*)FLASH_CR |= 0x2;
		*(int*)FLASH_CR &= ~(0x7F << 3);
		*(int*)FLASH_CR |= (page << 3);

		//start
		*(int*)FLASH_CR |= 0x10000;

		//wait for CFGBSY to clear
		while (((*(int*)FLASH_SR) & 0x40000) != 0) {};
	}
	//disable PE
	*(int*)FLASH_CR &= ~0x2;
	//lock flash
	*(uint32_t*)FLASH_CR |= (1U << 31);
}

void app_flash_write(uint8_t slot, uint8_t* buffer, int len) {
	//unlock flash
	*(uint32_t*)FLASH_KEYR = KEY1;
	*(uint32_t*)FLASH_KEYR = KEY2;

	//BSYS1 flag not set
	while (((*(int*)FLASH_SR) & 0x10000) != 0) {};

	//clear all error programming flags
	while (((*(int*)FLASH_SR) & 0x10) != 0) {};

	//CFGBSY flag cleared
	while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

	//enable PG
	*(int*)FLASH_CR |= 0x1;

	//TODO: set app flash start as a variable
	uint32_t* flash_itr = (slot == HEX_A) ? (uint32_t*)SLOT_A_START : (uint32_t*)SLOT_B_START;
	uint32_t* buffer_itr = (uint32_t*)buffer;

	for (int i = 0; i < len / 4; i+=2) {
		//clear all error bits
		*(int*)FLASH_SR |= (1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1);
		*(flash_itr + i) = *(buffer_itr + i);

		if (i + 1 < len / 4)
			*(flash_itr + i + 1) = *(buffer_itr + i + 1);
		else
			*(flash_itr + i + 1) = 0xFFFFFFFF;

		//CFGBSY flag cleared
		while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

		//clear EOP flag
		*(int*)FLASH_SR |= 0x1;

	}

	if (slot == HEX_A) {
		slot_a->crc = crc_value;
		slot_a->version = slot_b->version + 1;
		slot_a->size = len;
		slot_a->unused = 0xFFFF;
	}
	else {
		slot_b->crc = crc_value;
		slot_b->version = slot_a->version + 1;
		slot_b->size = len;
		slot_b->unused = 0xFFFF;
	}

	//CFGBSY flag cleared
	while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

	//clear EOP flag
	*(int*)FLASH_SR |= 0x1;

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

void Error_Handler() {
	while (1) {}
}
