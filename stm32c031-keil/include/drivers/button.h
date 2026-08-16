/**
 * @file    button.h
 * @brief   Push-button driver public interface utilizing EXTI interrupts.
 * @details Defines the API for user button initialization and interrupt management.
 * 
 * @satisfies{SWR_BUTTON_xx}
 */
#ifndef DRIVERS_BUTTON_H
#define DRIVERS_BUTTON_H

/**
 * @brief Initializes User Button B1 (PC.13) and its EXTI hardware.
 * 
 * @post  GPIOC is clocked, PC.13 is set as input, and EXTI line 13 is 
 *        configured to trigger on a falling edge.
 * 
 * @satisfies{SWR_BUTTON_xx}
 */
void Button_InitB1(void);

#endif // DRIVERS_BUTTON_H