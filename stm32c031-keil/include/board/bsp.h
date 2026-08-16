/**
 * @file    bsp.h
 * @brief   Board Support Package (BSP) public interface for STM32 NUCLEO-C031C6.
 * @details Defines the API for low-level hardware initialization and LED control.
 * 
 * @satisfies{SWR_BSP_xx}
 */
#ifndef BOARD_BSP_H
#define BOARD_BSP_H

#include <stdint.h>

/* system clock tick [Hz] */
#define BSP_TICKS_PER_SEC 100U

#define LD4_PIN  5U
#define LD5_PIN  6U

// Local-scope defines -----------------------------------------------------
// LED pins available on the board (just one user LED LD4--Green on PA.5)
#define LD4_PIN  5U

// external LED to be inserted between GND (short leg) and
// D12 (longer leg) on the CN9 connector
#define LD5_PIN  6U

/**
 * @brief Initializes board peripherals (LEDs, Button, UART, SysTick) and enables interrupts.
 * @satisfies{SWR_BSP_xx}
 */
void BSP_init(void);

/**
 * @brief Turns on the Red LED (LD5).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledRedOn(void);

/**
 * @brief Turns off the Red LED (LD5).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledRedOff(void);

/**
 * @brief Turns on the Blue LED (Stub/Not implemented).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledBlueOn(void);

/**
 * @brief Turns off the Blue LED (Stub/Not implemented).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledBlueOff(void);

/**
 * @brief Turns on the Green LED (LD4).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledGreenOn(void);

/**
 * @brief Turns off the Green LED (LD4).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledGreenOff(void);

#endif // BOARD_BSP_H