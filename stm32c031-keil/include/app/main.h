/**
 * @file    main.h
 * @brief   Application public definitions, state machine types, and event structures.
 * @details Declares global variables, state enumerations, and event signals.
 * 
 * @satisfies{SWR_MAIN_xx}
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

/**
 * @brief Application string and buffer dimension constants.
 */
enum {
  APP_INPUT_PROMPT_LEN = sizeof("Received Input --> ") - 1U,
  APP_INPUT_STRING_LEN = 53U
};

/**
 * @brief Application state machine states.
 */
typedef enum {
  INITIAL,
  TIMER_EVENT,
  UART_EVENT,
  BUTTON_EVENT,
  UART_ERROR_EVENT,
  IDLE
} state_t;

/**
 * @brief Asynchronous event signals triggered by interrupts.
 */
typedef enum {
  TIMER,
  UART,
  UART_BUFFER_OVERFLOW,
  BUTTON,
  NONE
} event_t;

extern volatile event_t event_signal;

extern char input_string[APP_INPUT_STRING_LEN];

#endif // MAIN_H