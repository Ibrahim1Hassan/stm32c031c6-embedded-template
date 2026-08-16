/**
 * @file    uart.h
 * @brief   UART driver public interface for STM32C0xx.
 * @details Defines the API for UART initialization, polling transmission, 
 *          and DMA-based asynchronous string transmission.
 * 
 * @satisfies{SWR_UART_DRV_xx}
 */
 
#ifndef DRIVERS_UART_H
#define DRIVERS_UART_H

#include <stdint.h>

/**
 * @brief Initializes UART2 peripheral with 8-N-1 configuration.
 *        Configures GPIOs, Baudrate, and DMA1 Channel 1 for transmission.
 * 
 * @pre   System clock and GPIOA/DMA1 clocks must be enabled.
 * @post  UART2 and associated DMA channel are active and ready for transmission.
 * 
 * @note  Reentrancy: Non-reentrant. Must be called sequentially during startup.
 * @satisfies{SWR_UART_INIT_xx}
 */
void UART2_Init(void);

/**
 * @brief Transmits a single character using polling (blocking mode).
 * 
 * @param[in] c  Byte to be transmitted.
 * 
 * @pre   UART2 initialized and TXE flag is functional.
 * @post  Character is loaded into the hardware transmit data register (TDR).
 * 
 * @note  Reentrancy: Reentrant, but byte streams may interleave if preempted.
 */
void UART2_SendChar(uint8_t c);

/**
 * @brief Asynchronously transmits a null-terminated string via DMA.
 *        Blocks if a previous DMA transfer is still actively running.
 * 
 * @param[in] string  Pointer to the null-terminated string to transmit. Must not be NULL.
 * 
 * @pre   UART2_Init() must have been executed.
 * @post  String data is copied to the internal buffer and DMA transfer is triggered.
 * 
 * @note  Reentrancy: Non-reentrant. Concurrent calls will block the CPU.
 * @satisfies{SWR_UART_TX_xx}
 */
void UART2_PrintDma(char const *string);

#endif // DRIVERS_UART_H