#define RCC 0x40021000
#define RCC_CR (RCC)
#define RCC_IOPENR (RCC + 0x34)
#define RCC_APBENR2 (RCC + 0x40)

#define RCC_APBENR1 (RCC + 0x3C)
#define RCC_APBENR2 (RCC + 0x40)
#define RCC_CCIPR (RCC + 0x54)

// Define RCC base address for STM32G0
#define RCC_CSR2      (RCC + 0x60)

// Define Bit Masks
#define RCC_CSR2_LSION    (1 << 0)  // LSI Oscillator Enable
#define RCC_CSR2_LSIRDY   (1 << 1)  // LSI Ready Flag

#define RCC_APBENR1_USART2EN_Pos    (17U)
#define RCC_APBENR1_USART2EN_Msk    (1U << RCC_APBENR1_USART2EN_Pos)
#define RCC_APBENR1_USART2EN        RCC_APBENR1_USART2EN_Msk

#define RCC_AHBENR (RCC + 0x38)

#define RCC_APBENR1_USART2EN_Pos    (17U)
#define RCC_APBENR1_USART2EN_Msk    (1U << RCC_APBENR1_USART2EN_Pos)
#define RCC_APBENR1_USART2EN        RCC_APBENR1_USART2EN_Msk

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
#define USART2_RQR (USART2 + 0x18)
#define USART2_ISR (USART2 + 0x1C)
#define USART2_ISR_TXE_POS 7
#define USART2_ISR_TXE_Msk (1U << USART2_ISR_TXE_POS)
#define USART2_ICR (USART2 + 0x20)
#define USART2_TDR (USART2 + 0x28)
#define USART2_RDR (USART2 + 0x24)
#define USART2_ISR_RXNE_POS 5
#define USART2_ISR_RXNE_Msk (1U << USART2_ISR_RXNE_POS)

#define IWDG_BASE  0x40003000
#define IWDG_KR    (IWDG_BASE + 0x00) // Key Register
#define IWDG_PR    (IWDG_BASE + 0x04) // Prescaler Register
#define IWDG_RLR   (IWDG_BASE + 0x08) // Reload Register
#define IWDG_SR    (IWDG_BASE + 0x0C) // Status Register
#define IWDG_WINR  (IWDG_BASE + 0x10) // Window Register

#define CRC  0x40023000
#define CRC_DR (CRC + 0x00)
#define CRC_CR       (CRC + 0x08)
#define CRC_INIT (CRC + 0x10)
#define CRC_POL (CRC + 0x14)

#define SCB_VTOR 0xE000ED08
#define APP_FLASH_START (uint8_t*)0x08004000
#define APP_FLASH_SIZE (1U << 14)
#define FLASH 0x40022000
#define FLASH_KEYR (FLASH + 0x008)
#define FLASH_CR (FLASH + 0x014)
#define FLASH_SR (FLASH + 0x010)
#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

#define SHPR3 0xE000ED20
#define NVIC_IPR4 0xE000E410

#define MAGIC_BYTE_1 0xA5
#define MAGIC_BYTE_2 0x5A
#define MAGIC_BYTE_3 0x7E
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

#define BUFFER_LEN 504

#define SCB_AIRCR 0xE000ED0C
#define SCB_AIRCR_VECTKEY (0x05FA << 16)
#define SCB_AIRCR_SYSRESETREQ (1 << 2)

typedef enum {
    UPDATE_IDLE,          // Waiting for the first magic byte
    UPDATE_BYTE1_FOUND,   // MAGIC_BYTE_1 matched, waiting for 2nd
    UPDATE_BYTE2_FOUND,   // MAGIC_BYTE_2 matched, waiting for 3rd
    UPDATE_TRIGGERED      // Full sequence detected, ready to update
} UpdateState_t;
#define LED_PIN 5


