/*
 * defs.h
 *
 *  Created on: Mar 28, 2026
 *      Author: auska
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

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
#define GPIOC_IDR (GPIOC + 0x10)

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

#define LED_PIN 5
#define BUTTON_PIN 13

#endif /* INC_DEFS_H_ */
