/*
 * defs.h
 *
 *  Common hardware register and system definitions
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#include <stdint.h>

/* RCC Base and Registers */
#define RCC                     0x40021000UL
#define RCC_CR                  *((volatile uint32_t *)(RCC + 0x00))
#define RCC_IOPENR              *((volatile uint32_t *)(RCC + 0x34))
#define RCC_AHBENR              *((volatile uint32_t *)(RCC + 0x38))
#define RCC_APBENR1             *((volatile uint32_t *)(RCC + 0x3C))
#define RCC_APBENR2             *((volatile uint32_t *)(RCC + 0x40))
#define RCC_CCIPR               *((volatile uint32_t *)(RCC + 0x54))
#define RCC_CSR2                *((volatile uint32_t *)(RCC + 0x60))

/* RCC Bit Masks */
#define RCC_CSR2_LSION          (1 << 0)  // LSI Oscillator Enable
#define RCC_CSR2_LSIRDY         (1 << 1)  // LSI Ready Flag

#define RCC_APBENR1_USART2EN_Pos (17U)
#define RCC_APBENR1_USART2EN_Msk (1U << RCC_APBENR1_USART2EN_Pos)
#define RCC_APBENR1_USART2EN     RCC_APBENR1_USART2EN_Msk

/* GPIO Base and Registers */
#define GPIOA                   0x50000000UL
#define GPIOA_MODER             *((volatile uint32_t *)(GPIOA + 0x00))
#define GPIOA_ODR               *((volatile uint32_t *)(GPIOA + 0x14))
#define GPIOA_AFRL              *((volatile uint32_t *)(GPIOA + 0x20))
#define GPIOA_BRR               *((volatile uint32_t *)(GPIOA + 0x28))

#define GPIOB                   0x50000400UL
#define GPIOB_MODER             *((volatile uint32_t *)(GPIOB + 0x00))
#define GPIOB_ODR               *((volatile uint32_t *)(GPIOB + 0x14))
#define GPIOB_AFRL              *((volatile uint32_t *)(GPIOB + 0x20))
#define GPIOB_BRR               *((volatile uint32_t *)(GPIOB + 0x28))

#define GPIOC                   0x50000800UL
#define GPIOC_MODER             *((volatile uint32_t *)(GPIOC + 0x00))
#define GPIOC_PUPDR             *((volatile uint32_t *)(GPIOC + 0x0C))
#define GPIOC_IDR               *((volatile uint32_t *)(GPIOC + 0x10))

/* EXTI Base and Registers */
#define EXTI                    0x40021800UL
#define EXTI_EXTICR_4           *((volatile uint32_t *)(EXTI + 0x060 + 0x4 * (4 - 1)))
#define EXTI_FTSR1              *((volatile uint32_t *)(EXTI + 0x004))
#define EXTI_FPR1               *((volatile uint32_t *)(EXTI + 0x010))
#define EXTI_IMR1               *((volatile uint32_t *)(EXTI + 0x080))

/* Cortex-M Core Registers */
#define NVIC_ISER               *((volatile uint32_t *)0xE000E100UL)
#define NVIC_IPR4               *((volatile uint32_t *)0xE000E410UL)

#define MPU_CTRL                *((volatile uint32_t *)0xE000ED94UL)

#define STK_CSR                 *((volatile uint32_t *)0xE000E010UL)
#define STK_RVR                 *((volatile uint32_t *)0xE000E014UL)
#define STK_CVR                 *((volatile uint32_t *)0xE000E018UL)

#define SHPR3                   *((volatile uint32_t *)0xE000ED20UL)
#define SCB_VTOR                *((volatile uint32_t *)0xE000ED08UL)
#define SCB_AIRCR               *((volatile uint32_t *)0xE000ED0CUL)
#define SCB_AIRCR_VECTKEY       (0x05FAUL << 16)
#define SCB_AIRCR_SYSRESETREQ   (1UL << 2)

/* TIM14 Base and Registers */
#define TIM14                   0x40002000UL
#define TIM14_CR1               *((volatile uint32_t *)(TIM14 + 0x00))
#define TIM14_SR                *((volatile uint32_t *)(TIM14 + 0x10))
#define TIM14_DIER              *((volatile uint32_t *)(TIM14 + 0x0C))
#define TIM14_CNT               *((volatile uint32_t *)(TIM14 + 0x24))
#define TIM14_PSC               *((volatile uint32_t *)(TIM14 + 0x28))
#define TIM14_ARR               *((volatile uint32_t *)(TIM14 + 0x2C))

/* USART1 Base and Registers */
#define USART1                  0x40013800UL
#define USART1_CR1              *((volatile uint32_t *)(USART1 + 0x00))
#define USART1_BRR              *((volatile uint32_t *)(USART1 + 0x0C))
#define USART1_ISR              *((volatile uint32_t *)(USART1 + 0x1C))
#define USART1_ISR_TXE_POS      7
#define USART1_ISR_TXE_Msk      (1U << 7)
#define USART1_TDR              *((volatile uint32_t *)(USART1 + 0x28))

/* USART2 Base and Registers */
#define USART2                  0x40004400UL
#define USART2_CR1              *((volatile uint32_t *)(USART2 + 0x00))
#define USART2_CR3              *((volatile uint32_t *)(USART2 + 0x08))
#define USART2_BRR              *((volatile uint32_t *)(USART2 + 0xC))
#define USART2_RQR              *((volatile uint32_t *)(USART2 + 0x18))
#define USART2_ISR              *((volatile uint32_t *)(USART2 + 0x1C))
#define USART2_ISR_TXE_POS      7
#define USART2_ISR_TXE_Msk      (1U << USART2_ISR_TXE_POS)
#define USART2_ISR_RXNE_POS     5
#define USART2_ISR_RXNE_Msk     (1U << USART2_ISR_RXNE_POS)
#define USART2_ICR              *((volatile uint32_t *)(USART2 + 0x20))
#define USART2_TDR              *((volatile uint8_t *)(USART2 + 0x28))
#define USART2_RDR              *((volatile uint8_t *)(USART2 + 0x24))
#define USART2_RDR_ADDR         (USART2 + 0x24)
#define USART2_RDR8             *((volatile uint8_t *)(USART2 + 0x24))

/* IWDG Base and Registers */
#define IWDG_BASE               0x40003000UL
#define IWDG_KR                 *((volatile uint32_t *)(IWDG_BASE + 0x00))
#define IWDG_PR                 *((volatile uint32_t *)(IWDG_BASE + 0x04))
#define IWDG_RLR                *((volatile uint32_t *)(IWDG_BASE + 0x08))
#define IWDG_SR                 *((volatile uint32_t *)(IWDG_BASE + 0x0C))
#define IWDG_WINR               *((volatile uint32_t *)(IWDG_BASE + 0x10))

/* FLASH Base and Registers */
#define FLASH                   0x40022000UL
#define FLASH_KEYR              *((volatile uint32_t *)(FLASH + 0x008))
#define FLASH_CR                *((volatile uint32_t *)(FLASH + 0x014))
#define FLASH_SR                *((volatile uint32_t *)(FLASH + 0x010))
#define KEY1                    0x45670123
#define KEY2                    0xCDEF89AB

/* CRC Base and Registers */
#define CRC                     0x40023000UL
#define CRC_DR                  *((volatile uint32_t *)(CRC + 0x00))
#define CRC_DR8                 *((volatile uint8_t *)(CRC + 0x00))
#define CRC_CR                  *((volatile uint32_t *)(CRC + 0x08))
#define CRC_INIT                *((volatile uint32_t *)(CRC + 0x10))
#define CRC_POL                 *((volatile uint32_t *)(CRC + 0x14))

/* DMA1 Base and Registers */
#define DMA1                    0x40020000UL
#define DMA1_ISR                *((volatile uint32_t *)(DMA1 + 0x00))
#define DMA1_IFCR               *((volatile uint32_t *)(DMA1 + 0x04))
#define DMA1_CCR1               *((volatile uint32_t *)(DMA1 + 0x08))
#define DMA1_CNDTR1             *((volatile uint32_t *)(DMA1 + 0x0C))
#define DMA1_CPAR1              *((volatile uint32_t *)(DMA1 + 0x10))
#define DMA1_CMAR1              *((volatile uint32_t *)(DMA1 + 0x14))

/* DMAMUX1 Base and Registers */
#define DMAMUX1                 0x40020800UL
#define DMAMUX1_C0CR            *((volatile uint32_t *)(DMAMUX1 + 0x00))

/* Board & Pin Definitions */
#define LED_PIN                 5
#define BUTTON_PIN              13

/* Protocol & Magic Constants */
#define HEX_A                   0x61
#define HEX_B                   0x62
#define HEX_R                   0x52
#define HEX_ACK                 0x1
#define HEX_NACK                0x2

#define MAGIC_BYTE_1            0xA5
#define MAGIC_BYTE_2            0x5A
#define MAGIC_BYTE_3            0x7E

/* Memory Layout Constants */
#define SLOT_A_HEADER_START     0x08001000
#define SLOT_B_HEADER_START     0x08004800
#define SLOT_A_APP_START        0x08001800
#define SLOT_B_APP_START        0x08005000
#define APP_MAX_SIZE            (SLOT_B_HEADER_START - SLOT_A_APP_START)

#define APP_FLASH_START         ((uint8_t*)0x08004000)
#define APP_FLASH_SIZE          (1U << 14)

#define BUFFER_LEN              504

/* Update State */
typedef enum {
    UPDATE_IDLE,          // Waiting for the first magic byte
    UPDATE_BYTE1_FOUND,   // MAGIC_BYTE_1 matched, waiting for 2nd
    UPDATE_BYTE2_FOUND,   // MAGIC_BYTE_2 matched, waiting for 3rd
    UPDATE_TRIGGERED      // Full sequence detected, ready to update
} UpdateState_t;

#endif /* INC_DEFS_H_ */
