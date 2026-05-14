#include <stdint.h> // C99 standard integers
#include "bsp.h"
#include <stdio.h>  /* for printf */

int main(void) {
    BSP_init();
    while (1) {
        /* Blinky polling state machine */
        static enum {
            INITIAL,
            OFF_STATE,
            ON_STATE
        } state = INITIAL;
        static uint32_t start;
        switch (state) {
            case INITIAL:
                start = BSP_tickCtr();
                state = OFF_STATE; /* initial transition */
                break;
            case OFF_STATE:
                if ((BSP_tickCtr() - start) > BSP_TICKS_PER_SEC * 3U / 4U) {
                    BSP_ledGreenOn();
										printf("LED Green ON\n");
										Uart_Dma_printf ("Hellloo\n\r", 10u);
                    start = BSP_tickCtr();
                    state = ON_STATE; /* state transition */
                }
                break;
            case ON_STATE:
                if ((BSP_tickCtr() - start) > BSP_TICKS_PER_SEC / 4U) {
                    BSP_ledGreenOff();
										printf("LED Green OFF\n");
                    start = BSP_tickCtr();
                    state = OFF_STATE; /* state transition */
                }
                break;
            default:
                //error();
                break;
        }
    }
    //return 0;
}

