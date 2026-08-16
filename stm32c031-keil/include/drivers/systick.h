/**
 * @file    systick.h
 * @brief   SysTick timer driver public interface.
 * @details Defines the API for system tick initialization, blocking delays, and periodic event generation.
 * 
 * @satisfies{SWR_SYSTICK_xx}
 */
#ifndef DRIVERS_SYSTICK_H
#define DRIVERS_SYSTICK_H

#include <stdint.h>

/**
 * @brief Initializes the SysTick peripheral.
 * 
 * @pre   System core clock must be configured before calling.
 * @post  SysTick interrupts fire at the rate defined by BSP_TICKS_PER_SEC.
 * 
 * @satisfies{SWR_SYSTICK_xx}
 */
void SysTick_Init(void);

/**
 * @brief Safely retrieves the current tick counter value.
 * 
 * @return uint32_t Current system tick count.
 * 
 * @note  Disables global interrupts temporarily to ensure an atomic read.
 * @satisfies{SWR_SYSTICK_xx}
 */
uint32_t SysTick_TickCtr(void);

/**
 * @brief Provides a blocking delay for a specified number of ticks.
 * 
 * @param[in] ticks  Number of system ticks to wait.
 * 
 * @note  Reentrancy: Reentrant, but halts execution thread.
 * @satisfies{SWR_SYSTICK_xx}
 */
void SysTick_Delay(uint32_t ticks);

#endif // DRIVERS_SYSTICK_H