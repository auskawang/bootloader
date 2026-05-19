#include <stdint.h>
#include "defs.h"

extern void delay();
void send_uart_data(uint8_t data);
uint8_t read_uart_data();
uint8_t flash_new_app_data(uint32_t start_address, int total_len);
void app_flash_erase(uint8_t);
void app_flash_write(uint32_t start_address, uint8_t* buffer, int len);
uint8_t firmware_updater();
void flash_header_data(int chosen_slot, int len, int crc_value);
void Error_Handler();
void trigger_reset(void);
extern void TIM14_Handler();

typedef struct {
	int crc;
	int version;
	int size;
	int unused;
} Slot;

Slot* slot_a = (Slot*)SLOT_A_HEADER_START;
Slot* slot_b = (Slot*)SLOT_B_HEADER_START;
int crc_value;
uint8_t buffer[BUFFER_LEN] = {0};
extern UpdateState_t current_update_state;
int main() {
	asm volatile ("cpsie i" : : : "memory");
	*((int*)USART2_CR1) |= 0x20;		//enable interrupts on receiving
	*(int*)NVIC_ISER |= 0x10000000;		//enable USART2 interrupts

	//turn on LSI for IWDG
	RCC_CSR2 |= RCC_CSR2_LSION;
	while (!(RCC_CSR2 & RCC_CSR2_LSIRDY)) {}

	//WDG
	*(int*)IWDG_KR = 0x5555;	//unlock wdg
	*(int*)IWDG_PR = 0x3; //PS 16 32khz clock (~4 seconds)
	*(int*)IWDG_RLR = 0xFFF; //start value
	*(int*)IWDG_KR = 0xCCCC; //start countdown

	while(1) {
		if (*((int*)USART2_ISR) & USART2_ISR_TXE_Msk) {
			*((int*)USART2_TDR) = 0x71;
		}
		delay(1000);

		// Refresh the watchdog counter
		*(int*)IWDG_KR = 0xAAAA;
		if (current_update_state == UPDATE_TRIGGERED) {
			*((int*)USART2_CR1) &= ~0x20;	//disable receiving interrupts
			*(int*)NVIC_ISER &= ~0x10000000;		//enable USART2 interrupts
			//setup TIM14 for interrupt based LED blinking
			*((int*)RCC_APBENR2) |= 0x8000; //enable tim14
			*((int*)TIM14_DIER) |= 0x1;	//enable interrupt generation
			*((int*)NVIC_ISER) |= 0x80000; //enable interrupt for NVIC
			*((int*)TIM14_PSC) = 0x320; //PS of 1
			*((int*)TIM14_ARR) = 0x258;
			if (firmware_updater() == 0) {
				*((int*)GPIOA_ODR) &= ~(1 << LED_PIN);
				*((int*)TIM14_CR1) &= ~0x1; //stop the counter to disable tim14 interrupt
				trigger_reset();
			}
			*((int*)GPIOA_ODR) &= ~(1 << LED_PIN);
			*((int*)TIM14_CR1) &= ~0x1; //stop the counter to disable tim14 interrupt

		}
	}

	return 1;
}

void trigger_reset(void) {
    // Ensure all data is synchronized before reset
    __asm volatile ("dsb" : : : "memory");

    // Write the Key and the Reset Request bit
    *(int*)SCB_AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_SYSRESETREQ;

    // Ensure the CPU waits for the reset to occur
    __asm volatile ("isb");
    while(1);
}

uint8_t firmware_updater() {


	int payload_len;
	int chosen_slot;
	*((int*)TIM14_CR1) |= 0x1; //start the counter to trigger tim14 interrupt

	//enable CRC
	*(int*)RCC_AHBENR |= (1U << 12);
	*(volatile uint32_t*)CRC_CR = (1U << 0);
	send_uart_data(HEX_ACK);
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
		payload_len = 256 * payload_sz_upper_byte + payload_sz_lower_byte;

		if (payload_len > APP_MAX_SIZE) {
			send_uart_data(HEX_NACK);
			return -1;
		}

		send_uart_data(HEX_ACK);
	}
	else {
		return -1;
	}
	app_flash_erase(chosen_slot);
	//populate buffer with application data from UART
	if (flash_new_app_data(chosen_slot, payload_len) < 0) return -1;
	// Refresh the watchdog counter
	*(int*)IWDG_KR = 0xAAAA;
	return 0;
}

uint8_t flash_new_app_data(uint32_t chosen_slot, int total_len) {
	uint8_t buffer[BUFFER_LEN];
	int remaining_bytes = total_len;
	uint32_t current_flash_addr = (chosen_slot == HEX_A) ? SLOT_A_APP_START : SLOT_B_APP_START;
	int chunk_size = BUFFER_LEN;

	if (read_uart_data() != HEX_R) {
		return -1;
	}

	send_uart_data(HEX_R);

	while (remaining_bytes > 0) {
		for (int i = 0; i < chunk_size; i++) {
			buffer[i] = read_uart_data();
		}
		uint8_t server_crc_bytes[4] = {0};
		for (int i = 0; i < 4; i++) {
			server_crc_bytes[i] = read_uart_data();
		}
		uint32_t server_crc_val = (server_crc_bytes[0] << 24) | (server_crc_bytes[1] << 16) |
								  (server_crc_bytes[2] << 8)  | (server_crc_bytes[3]);

		*(uint32_t*)CRC_CR |= 0x1;
		for (int i = 0; i < chunk_size; i++) {
			*(uint8_t*)CRC_DR = buffer[i];
		}
		uint32_t local_crc_val = *(uint32_t*)CRC_DR;

		if (local_crc_val == server_crc_val) {
			app_flash_write(current_flash_addr, buffer, chunk_size);
			send_uart_data(HEX_ACK);
			current_flash_addr += chunk_size;
			remaining_bytes -= chunk_size;
		} else {
			send_uart_data(HEX_NACK);
			return -1;
		}
	}

	uint8_t server_crc_bytes[4] = {0};
	for (int i = 0; i < 4; i++) {
		server_crc_bytes[i] = read_uart_data();
	}
	uint32_t server_crc_val = (server_crc_bytes[0] << 24) | (server_crc_bytes[1] << 16) |
							  (server_crc_bytes[2] << 8)  | (server_crc_bytes[3]);
	flash_header_data(chosen_slot, total_len, server_crc_val);
	return 0;
}

void app_flash_erase(uint8_t slot) {
	int page_start, page_end;
	//unlock flash
	*(uint32_t*)FLASH_KEYR = KEY1;
	*(uint32_t*)FLASH_KEYR = KEY2;

	if (slot == HEX_A) {
		page_start = 2;
		page_end = 8;	//includes erasing this page
	}
	if (slot == HEX_B) {
		page_start = 9;
		page_end = 15;
	}
	//based on slot erase select pages
	for (int page = page_start; page <= page_end; page++) {
		//clear all error bits
		*(int*)FLASH_SR |= (1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1);
		//BSYS1 flag not set
		while (((*(int*)FLASH_SR) & 0x10000) != 0) {};

		//clear all error programming flags
		while (((*(int*)FLASH_SR) & 0x10) != 0) {};

		//CFGBSY flag cleared
		while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

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

void app_flash_write(uint32_t start_address, uint8_t* buffer, int len) {
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

	uint32_t* flash_itr = (uint32_t*)start_address;
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

	//CFGBSY flag cleared
	while (((*(int*)FLASH_SR) & 0x40000) != 0) {};

	//clear EOP flag
	*(int*)FLASH_SR |= 0x1;

	//clear PG
	*(int*)FLASH_CR &= ~0x1;

	//lock flash
	*(uint32_t*)FLASH_CR |= (1U << 31);
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

void flash_header_data(int chosen_slot, int len, int crc_value) {
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

	//clear all error bits
	*(int*)FLASH_SR |= (1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1);

	if (chosen_slot == HEX_A) {
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

void Error_Handler() {
	while (1) {}
}

//called every 5.46ms
void TIM14_Handler() {
	*((int*)GPIOA_ODR) ^= 0x1 << LED_PIN;

	//clear interrupt flags
	*((int*)TIM14_SR) &= ~0x3;
}

