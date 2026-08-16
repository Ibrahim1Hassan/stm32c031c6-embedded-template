/**
 * @file    bsp.c
 * @brief   Board Support Package (BSP) for STM32 NUCLEO-C031C6.
 * @details Handles low-level hardware initialization and LED control.
 * 
 * @satisfies{SWR_BSP_xx}
 */

#include <stdint.h>
#include "bsp.h"
#include "button.h"
#include "systick.h"
#include "uart.h"
#include "stm32c0xx.h"

static void BSP_ledGreenInit(void);

/**
 * @brief Initializes board peripherals (LEDs, Button, UART, SysTick) and enables interrupts.
 * @satisfies{SWR_BSP_xx}
 */
void BSP_init(void) {
    /* BSP initialization sequence */
    BSP_ledGreenInit();
    Button_InitB1();
    UART2_Init();
    SysTick_Init();

    __enable_irq();
}

/**
 * @brief Turns on the Red LED (LD5).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledRedOn(void) {
    /* turn LD5 on */
    GPIOA->BSRR = (1U << LD5_PIN);
}

/**
 * @brief Turns off the Red LED (LD5).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledRedOff(void) {
    /* turn LD5 off */
    GPIOA->BSRR = (1U << (LD5_PIN + 16U));
}

/**
 * @brief Turns on the Blue LED (Stub/Not implemented).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledBlueOn(void) {
}

/**
 * @brief Turns off the Blue LED (Stub/Not implemented).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledBlueOff(void) {
}

/**
 * @brief Turns on the Green LED (LD4).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledGreenOn(void) {
    /* turn LD4 on */
    GPIOA->BSRR = (1U << LD4_PIN);
}

/**
 * @brief Turns off the Green LED (LD4).
 * @satisfies{SWR_BSP_xx}
 */
void BSP_ledGreenOff(void) {
    /* turn LD4 off */
    GPIOA->BSRR = (1U << (LD4_PIN + 16U));
}

/**
 * @brief Configures GPIOA pins for both LD4 (Green) and LD5 (Red).
 * @satisfies{SWR_BSP_xx}
 */
static void BSP_ledGreenInit(void) {
    /* enable GPIOA clock port for the LED LD4 */
    RCC->IOPENR |= (1U << 0U);

    /* NUCLEO-C031C6 board has LED LD4 on GPIOA pin LD4_PIN
    // and external LED LD5 on GPIO LD5_PIN
    // set the LED pins as push-pull output, no pull-up, pull-down */
    GPIOA->MODER   &= ~((3U << 2U * LD4_PIN) | (3U << 2U * LD5_PIN));
    GPIOA->MODER   |=  ((1U << 2U * LD4_PIN) | (1U << 2U * LD5_PIN));
    GPIOA->OTYPER  &= ~((1U << LD4_PIN) | (1U << LD5_PIN));
    GPIOA->OSPEEDR &= ~((3U << 2U * LD4_PIN) | (3U << 2U * LD5_PIN));
    GPIOA->OSPEEDR |=  ((1U << 2U * LD4_PIN) | (1U << 2U * LD5_PIN));
    GPIOA->PUPDR   &= ~((3U << 2U * LD4_PIN) | (3U << 2U * LD5_PIN));
}

_Noreturn void assert_failed(char const * const module, int const id);

/**
 * @brief System assertion failure handler.
 * @details Hangs in an infinite loop for debugging, or resets the system in release builds.
 * 
 * @param[in] module  Pointer to the module name string where assertion failed.
 * @param[in] id      Line number or ID where the assertion occurred.
 * 
 * @satisfies{SWR_BSP_xx}
 */
_Noreturn void assert_failed(char const * const module, int const id) {
    (void)module;
    (void)id;
#ifndef NDEBUG
    /* for debugging, hang on in an endless loop... */
    GPIOA->BSRR = (1U << LD4_PIN);
    for (;;) {
    }
#endif
    NVIC_SystemReset();
}