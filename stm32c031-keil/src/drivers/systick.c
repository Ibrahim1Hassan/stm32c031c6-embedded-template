/**
 * @file    systick.c
 * @brief   SysTick timer driver implementation.
 * @details Handles system tick initialization, blocking delays, and periodic event generation.
 * 
 * @satisfies{SWR_SYSTICK_xx}
 */

#include "systick.h"
#include "main.h"
#include "bsp.h"
#include "stm32c0xx.h"

/**
 * @brief Volatile global counter incremented every SysTick interrupt.
 */
static uint32_t volatile l_tickCtr;

/**
 * @brief SysTick interrupt service routine.
 * @details Increments system tick counter and triggers a TIMER event periodically.
 * 
 * @note  Executed in ISR context. No blocking delays permitted.
 * @satisfies{SWR_SYSTICK_xx}
 */
void SysTick_Handler(void) {
    static uint32_t start = 0;
    ++l_tickCtr;
    /* timer signal */
    if ((l_tickCtr - start) > BSP_TICKS_PER_SEC * 3U / 4U) {
        start = l_tickCtr;
        event_signal = TIMER;
    }
}

/**
 * @brief Initializes the SysTick peripheral.
 * 
 * @pre   System core clock must be configured before calling.
 * @post  SysTick interrupts fire at the rate defined by BSP_TICKS_PER_SEC.
 * 
 * @satisfies{SWR_SYSTICK_xx}
 */
void SysTick_Init(void) {
    /* initialize SysTick */
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}

/**
 * @brief Safely retrieves the current tick counter value.
 * 
 * @return uint32_t Current system tick count.
 * 
 * @note  Disables global interrupts temporarily to ensure an atomic read.
 * @satisfies{SWR_SYSTICK_xx}
 */
uint32_t SysTick_TickCtr(void) {
    uint32_t tickCtr;

    __disable_irq();
    tickCtr = l_tickCtr;
    __enable_irq();

    return tickCtr;
}

/**
 * @brief Provides a blocking delay for a specified number of ticks.
 * 
 * @param[in] ticks  Number of system ticks to wait.
 * 
 * @note  Reentrancy: Reentrant, but halts execution thread.
 * @satisfies{SWR_SYSTICK_xx}
 */
void SysTick_Delay(uint32_t ticks) {
    uint32_t start = SysTick_TickCtr();
    while ((SysTick_TickCtr() - start) < ticks) {
    }
}