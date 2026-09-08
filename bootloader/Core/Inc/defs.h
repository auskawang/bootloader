/*
 * defs.h
 *
 *  Created on: Mar 28, 2026
 *      Author: auska
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#include <stdint.h>

#define RCC 0x40021000
#define RCC_CR *((volatile uint32_t *)(RCC))
#define RCC_IOPENR *((volatile uint32_t *)(RCC + 0x34))
#define RCC_AHBENR *((volatile uint32_t *)(RCC + 0x38))
#define RCC_APBENR1 *((volatile uint32_t *)(RCC + 0x3C))
#define RCC_APBENR2 *((volatile uint32_t *)(RCC + 0x40))
#define RCC_CCIPR *((volatile uint32_t *)(RCC + 0x54))
#define RCC_CSR2      *((volatile uint32_t *)(RCC + 0x60))

#define RCC_APBENR1_USART2EN_Pos    (17U)
#define RCC_APBENR1_USART2EN_Msk    (1U << RCC_APBENR1_USART2EN_Pos)
#define RCC_APBENR1_USART2EN        RCC_APBENR1_USART2EN_Msk

#define GPIOA 0x50000000
#define GPIOA_MODER *((volatile uint32_t *)(GPIOA + 0x00))
#define GPIOA_ODR *((volatile uint32_t *)(GPIOA + 0x14))
#define GPIOA_AFRL *((volatile uint32_t *)(GPIOA + 0x20))
#define GPIOA_BRR *((volatile uint32_t *)(GPIOA + 0x28))

#define GPIOB 0x50000400
#define GPIOB_MODER *((volatile uint32_t *)(GPIOB + 0x00))
#define GPIOB_ODR *((volatile uint32_t *)(GPIOB + 0x14))
#define GPIOB_AFRL *((volatile uint32_t *)(GPIOB + 0x20))
#define GPIOB_BRR *((volatile uint32_t *)(GPIOB + 0x28))

#define GPIOC 0x50000800
#define GPIOC_MODER *((volatile uint32_t *)(GPIOC + 0x00))
#define GPIOC_PUPDR *((volatile uint32_t *)(GPIOC + 0x0C))
#define GPIOC_IDR *((volatile uint32_t *)(GPIOC + 0x10))

#define EXTI 0x40021800
#define EXTI_EXTICR_4 *((volatile uint32_t *)(EXTI + 0x060 + 0x4 * (4 - 1)))
#define EXTI_FTSR1 *((volatile uint32_t *)(EXTI + 0x004))
#define EXTI_FPR1 *((volatile uint32_t *)(EXTI + 0x010))
#define EXTI_IMR1 *((volatile uint32_t *)(EXTI + 0x080))

#define NVIC_ISER *((volatile uint32_t *)0xE000E100)

#define MPU_CTRL *((volatile uint32_t *)0xE000ED94)

#define STK_CSR *((volatile uint32_t *)0xE000E010)
#define STK_RVR *((volatile uint32_t *)0xE000E014)
#define STK_CVR *((volatile uint32_t *)0xE000E018)

#define TIM14 0x40002000
#define TIM14_CR1 *((volatile uint32_t *)(TIM14 + 0x0))
#define TIM14_SR *((volatile uint32_t *)(TIM14 + 0x10))
#define TIM14_DIER *((volatile uint32_t *)(TIM14 + 0xC))
#define TIM14_CNT *((volatile uint32_t *)(TIM14 + 0x24))
#define TIM14_PSC *((volatile uint32_t *)(TIM14 + 0x28))
#define TIM14_ARR *((volatile uint32_t *)(TIM14 + 0x2C))



#define USART1 0x40013800
#define USART1_CR1 *((volatile uint32_t *)(USART1 + 0x0))
#define USART1_BRR *((volatile uint32_t *)(USART1 + 0xC))
#define USART1_ISR *((volatile uint32_t *)(USART1 + 0x1C))
#define USART1_ISR_TXE_POS 7
#define USART1_ISR_TXE_Msk (1U << 7)
#define USART1_TDR *((volatile uint32_t *)(USART1 + 0x28))

#define USART2 0x40004400
#define USART2_CR1 *((volatile uint32_t *)(USART2 + 0x0))
#define USART2_BRR *((volatile uint32_t *)(USART2 + 0xC))
#define USART2_ISR *((volatile uint32_t *)(USART2 + 0x1C))
#define USART2_ISR_TXE_POS 7
#define USART2_ISR_TXE_Msk (1U << USART2_ISR_TXE_POS)
#define USART2_ISR_RXNE_POS 5
#define USART2_ISR_RXNE_Msk (1U << USART2_ISR_RXNE_POS)
#define USART2_TDR *((volatile uint8_t *)(USART2 + 0x28))
#define USART2_RDR *((volatile uint8_t *)(USART2 + 0x24))


#define SHPR3 *((volatile uint32_t *)0xE000ED20)
#define NVIC_IPR4 *((volatile uint32_t *)0xE000E410)

#define SCB_VTOR *((volatile uint32_t *)0xE000ED08)
#define APP_FLASH_START (uint8_t*)0x08004000
#define APP_FLASH_SIZE (1U << 14)

#define FLASH 0x40022000
#define FLASH_KEYR *((volatile uint32_t*)(FLASH + 0x008))
#define FLASH_CR *((volatile uint32_t*)(FLASH + 0x014))
#define FLASH_SR *((volatile uint32_t*)(FLASH + 0x010))
#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

#define CRC  0x40023000
#define CRC_DR *((volatile uint32_t *)(CRC + 0x00))
#define CRC_DR8 *((volatile uint8_t *)(CRC + 0x00))
#define CRC_CR       *((volatile uint32_t *)(CRC + 0x08))
#define CRC_INIT *((volatile uint32_t *)(CRC + 0x10))
#define CRC_POL *((volatile uint32_t *)(CRC + 0x14))

#define LED_PIN 5
#define BUTTON_PIN 13

#define HEX_A 0x61
#define HEX_B 0x62
#define HEX_R 0x52
#define HEX_ACK 0x1
#define HEX_NACK 0x2

#define SLOT_A_HEADER_START 0x08001000
#define SLOT_B_HEADER_START 0x08004800
#define SLOT_A_APP_START 0x08001800
#define SLOT_B_APP_START 0x08005000
#define APP_MAX_SIZE SLOT_B_HEADER_START - SLOT_A_APP_START

#define SCB_AIRCR *((volatile uint32_t *)0xE000ED0C)
#define SCB_AIRCR_VECTKEY (0x05FA << 16)
#define SCB_AIRCR_SYSRESETREQ (1 << 2)

#define BUFFER_LEN 504
#endif /* INC_DEFS_H_ */
