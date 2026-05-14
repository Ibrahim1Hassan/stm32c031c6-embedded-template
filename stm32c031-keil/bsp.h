#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h> // C99 standard integers
/* system clock tick [Hz] */
#define BSP_TICKS_PER_SEC 100U

void BSP_init(void);

/* get the current value of the clock tick counter (returns immedately) */
uint32_t BSP_tickCtr(void);

/* delay for a specified number of system clock ticks (polling) */
void BSP_delay(uint32_t ticks);

void BSP_ledRedOn(void);
void BSP_ledRedOff(void);

void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);

void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);

void BSP_ledGreenInit(void);
void BSP_buttonInitB1(void);
void uart2_init(void);
void Uart2_SendChar(uint8_t c);
void Uart_Dma_printf (uint8_t *string);

#endif // __BSP_H__
