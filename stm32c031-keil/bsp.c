/* Board Support Package (BSP) for the STM32 NUCLEO-C031C6 board */
#include <stdint.h>  /* Standard integers. WG14/N843 C99 Standard */
#include <stdio.h>  /* for FILE, printf */


#include "bsp.h"
#include "stm32c0xx.h"  // CMSIS-compliant header file for the MCU used
/* add other drivers if necessary... */

// Local-scope defines -----------------------------------------------------
// LED pins available on the board (just one user LED LD4--Green on PA.5)
#define LD4_PIN  5U

// external LED to be inserted between GND (short leg) and
// D12 (longer leg) on the CN9 connector
#define LD5_PIN  6U

// Button pins available on the board (just one user Button B1 on PC.13)
#define B1_PIN   13U

static uint32_t volatile l_tickCtr;

/* buffer for uart_dma printf */
static volatile uint8_t UartDmaTransmitBuffer[50];

/* ISRs  ===============================================*/
void SysTick_Handler(void) {
    ++l_tickCtr;
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
	
		BSP_ledGreenInit();
	
    BSP_buttonInitB1();
		
	
		uart2_init();
	
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    __enable_irq();
}

uint32_t BSP_tickCtr(void) {
    uint32_t tickCtr;

    __disable_irq();
    tickCtr = l_tickCtr;
    __enable_irq();

    return tickCtr;
}

void BSP_delay(uint32_t ticks) {
    uint32_t start = BSP_tickCtr();
    while ((BSP_tickCtr() - start) < ticks) {
    }
}

/*..........................................................................*/
void BSP_ledRedOn(void) {
    GPIOA->BSRR = (1U << LD5_PIN);  // turn LD5 on
}

void BSP_ledRedOff(void) {
    GPIOA->BSRR = (1U << (LD5_PIN + 16U));  // turn LD5 off
}

void BSP_ledBlueOn(void) {
    //GPIOA->BSRR = (1U << LD4_PIN);  // turn LD4 on
}

void BSP_ledBlueOff(void) {
    //GPIOA->BSRR = (1U << (LD4_PIN + 16U));  // turn LD4 off
}

void BSP_ledGreenOn(void) {
    GPIOA->BSRR = (1U << LD4_PIN); // turn LD4 on
}

void BSP_ledGreenOff(void) {
    GPIOA->BSRR = (1U << (LD4_PIN + 16U)); // turn LD4 off
}
void BSP_ledGreenInit(void) {
		// enable GPIOA clock port for the LED LD4
    RCC->IOPENR |= (1U << 0U);

    // NUCLEO-C031C6 board has LED LD4 on GPIOA pin LD4_PIN
    // and external LED LD5 on GPIO LD5_PIN
    // set the LED pins as push-pull output, no pull-up, pull-down
    GPIOA->MODER   &= ~((3U << 2U*LD4_PIN) | (3U << 2U*LD5_PIN));
    GPIOA->MODER   |=  ((1U << 2U*LD4_PIN) | (1U << 2U*LD5_PIN));
    GPIOA->OTYPER  &= ~((1U <<    LD4_PIN) | (1U <<    LD5_PIN));
    GPIOA->OSPEEDR &= ~((3U << 2U*LD4_PIN) | (3U << 2U*LD5_PIN));
    GPIOA->OSPEEDR |=  ((1U << 2U*LD4_PIN) | (1U << 2U*LD5_PIN));
    GPIOA->PUPDR   &= ~((3U << 2U*LD4_PIN) | (3U << 2U*LD5_PIN));
}

void BSP_buttonInitB1(void) {
		
	  // enable GPIOC clock port for the Button B1
    RCC->IOPENR |= (1U << 2U);

    // configure Button B1 (PC.13) pins as input, no pull-up, pull-down
    GPIOC->MODER   &= ~(3U << 2*B1_PIN);
    GPIOC->OSPEEDR &= ~(3U << 2*B1_PIN);
    GPIOC->OSPEEDR |=  (1U << 2*B1_PIN);
    GPIOC->PUPDR   &= ~(3U << 2*B1_PIN);

    // configure Button B1 interrupt as falling edge
    EXTI->EMR1 &= ~(1U << B1_PIN);
    EXTI->IMR1 |= (1U << B1_PIN);
    EXTI->RTSR1 &= ~(1U << B1_PIN);
    EXTI->FTSR1 |= (1U << B1_PIN);
    EXTI->EXTICR[3] &= ~(7U << 8); // EXTI port C line 13
    EXTI->EXTICR[3] |= (2U << 8);  // EXTI port C line 13
	
		NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void uart2_init(void) {
		// enable GPIOA clock port for the UART2  --> already enabled
    // RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
		// enable APB clock to UART1 
		RCC->APBENR1 |= RCC_APBENR1_USART2EN;
	
		// set PA2 as Tx and PA3 as Rx for UART1, using GPIOB MODER and  AFRL(contain the first 8 ports PB0 till PB7)
		// Pin PA2
		GPIOA->MODER &= ~GPIO_MODER_MODE2;
		GPIOA->MODER |= GPIO_MODER_MODE2_1;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
		// Pin PA3
		GPIOA->MODER &= ~GPIO_MODER_MODE3;
		GPIOA->MODER |= GPIO_MODER_MODE3_1;
		GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
		GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
		
		// set baud rate for UART1 using UART2 BRR register
		// upon checking RCC register in the watch window (RCC_TypeDef*)0x40021000
		// the UART2 is clocked using HSISYS (12 MHz) -> APB (no division) 
		// from the data sheet with 16 bit oversamlping BRR = ClkSrc / baudrate 
		USART2->BRR = 0x4E2UL;
	
		// configure CR1 for oversampling rate, character size (8 bit), and enabling transmit, no parity		8-N-1
		USART2->CR1 &= ~USART_CR1_M0;
		USART2->CR1 &= ~USART_CR1_M1;	//this makes 1 start bit, 8 data bits and N stop bit
		USART2->CR1 &= ~USART_CR1_OVER8;	//makes oversampling by 16
		//USART2->CR1 |= USART_CR1_TE;
		
		// configure CR2 for number of stop bits and ..etc
		USART2->CR2 &= ~USART_CR2_STOP_0;
		USART2->CR2 &= ~USART_CR2_STOP_1;
		
		// enable UART2 
		USART2->CR1 |= USART_CR1_UE;
		
		// select DMA enable DMAT in CR3
		USART2->CR3 |= USART_CR3_DMAT;
		
		/********** DMA CONFIGURATION START	**********/
		
		// enable DMA1 clock
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		
		// Map USART2_TX (Request 53) to DMA1 Channel1 using DMAMUX1 Channel0
		DMAMUX1_Channel0->CCR = (53U & DMAMUX_CxCR_DMAREQ_ID);

		// set the peripheral address in the DMA_CPARx register
		DMA1_Channel1->CPAR = (uint32_t)&USART2->TDR;
		// set the memory address in the DMA_CMARx register
		DMA1_Channel1->CMAR = (uint32_t)&UartDmaTransmitBuffer;
		// configure total number of bytes to transfare --> NOT HERE
		
		// configure channel priority, data transfare direction
		// circular mode disabled, memory increment mode enabled, peripheral increment mode disabled
		// peripheral and memory data size, interrupts disabled
		DMA1_Channel1->CCR = 0;
		DMA1_Channel1->CCR |= DMA_CCR_MINC;
		DMA1_Channel1->CCR |= DMA_CCR_DIR;
		// activate the channel by setting EN bit in CCRx register --> NOT HERE  DMA1_Channel1->CCR |= DMA_CCR_EN;
		
		
		/********** DMA CONFIGURATION END	**********/
		
		// enable TE transmission enable register, sets an idle frame as first transmission
		USART2->CR1 |= USART_CR1_TE;
}
void Uart_Dma_printf (uint8_t *string, uint8_t BufferSize) {
		while (!( USART2->ISR & USART_ISR_TXE_TXFNF ) & !(USART2->ISR & USART_ISR_TC)) 
		{ 
		 /* DO NOTHING */		/* wait for Transmit Data Register Empty/TXFIFO Not Full & Transmission Complete*/
		}
		// deactivate the channel by resetting EN bit in CCRx register  
		DMA1_Channel1->CCR &= ~DMA_CCR_EN;
		// sets the data 
	  for (uint8_t i = 0; i < BufferSize; i++) {
        UartDmaTransmitBuffer[i] = string[i];
    }
		// configure buffer size
		DMA1_Channel1->CNDTR = BufferSize;
		// clear TC
		USART2->ICR |= USART_ICR_TCCF;
		// activate the channel by setting EN bit in CCRx register  
		DMA1_Channel1->CCR |= DMA_CCR_EN;
}
void Uart2_SendChar(uint8_t c) {
//		while (!( USART2->ISR & USART_ISR_TXE_TXFNF ) & !(USART2->ISR & USART_ISR_TC)) 
//		{ 
//		 /* DO NOTHING */		/* wait for Transmit Data Register Empty/TXFIFO Not Full & Transmission Complete*/
//		}
//		USART2->TDR = (c & USART_TDR_TDR);
}

int fputc(int c, FILE *stream){
		(void)stream;		/* unused parameter */
		if (c == '\n') {
        Uart2_SendChar('\r');  // Send carriage return first
    }
		Uart2_SendChar(c);
		return c;
}

void EXTI4_15_IRQHandler(void){
	// Clear the pending flag
  EXTI->FPR1 |= EXTI_FPR1_FPIF13;
	printf("Button Pressed\n");
	/* counter for ISR */
	static uint16_t button_int_ctr;
	button_int_ctr++;
	
}

//............................................................................
_Noreturn void assert_failed(char const * const module, int const id);
_Noreturn void assert_failed(char const * const module, int const id) {
    (void)module; // unused parameter
    (void)id;     // unused parameter
#ifndef NDEBUG
    GPIOA->BSRR = (1U << LD4_PIN); // turn LD4 on
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#endif
    NVIC_SystemReset();
}
