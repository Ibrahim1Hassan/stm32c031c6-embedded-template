#include <stdint.h> // C99 standard integers
#include "bsp.h"
#include <stdio.h>  /* for printf */
#include <stdbool.h>
#include "stm32c0xx.h"  // CMSIS-compliant header file for the MCU used
extern volatile event_t event_signal;
extern volatile char global_char;
volatile static state_t main_state = INITIAL;
static char string[] = "Received Char --> [x}\n\r";
static void clear_event_signal (void);
static void clear_event_signal (void)
{
		if (event_signal != NONE){
			event_signal = NONE;
		}
}

int main(void) {
    BSP_init();
    while (1) {
        /* Blinky polling state machine */
				__disable_irq();
        switch (main_state) {
						static bool led_state = 0u;
            case INITIAL:
                main_state = IDLE; /* initial transition */
                break;
						
            case TIMER_EVENT:			/* Handle Timer Event */
								if (led_state == 0u)
								{
									BSP_ledGreenOn();
									led_state = 1u;
									Uart_Dma_printf("LED Green ON\n\r");
									Uart_Dma_printf ("Hellloo\n\r");
								}
								else 
								{
									BSP_ledGreenOff();
									led_state = 0u;
									Uart_Dma_printf("LED Green OFF\n\r");
								}
								main_state = IDLE; /* reset state machine to idle */
                break;
								
						case UART_EVENT:			 /* Handle UART Event */
								string[19] = global_char;
								Uart_Dma_printf(string);
                main_state = IDLE; /* reset state machine to idle */
                break;		
						
						case BUTTON_EVENT:		 /* Handle BUTTON Event */
								Uart_Dma_printf("Button Pressed\n\r");
                main_state = IDLE; /* reset state machine to idle */
                break;
						
						case IDLE:						/* Check for event signals and switch the state machine if found */
								if (event_signal == UART)
								{
									clear_event_signal();
									main_state = UART_EVENT;
								}
								else if (event_signal == TIMER)
								{
									clear_event_signal();
									main_state = TIMER_EVENT;
								}
								else if (event_signal == BUTTON)
								{
									clear_event_signal();
									main_state = BUTTON_EVENT;
								}
								else { /* DO NOTHING */}
								break;
						
            default:
								while(1){/* ERROR */}
                break;
        }
				__enable_irq();
    }
    //return 0;
}
