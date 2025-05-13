#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

typedef void (*InterruptFnPtr)(void);

/**
 * Interrupt vector table
 */
extern InterruptFnPtr interrupt_vector_table[128];

void hardware_interrupt_mode(void);

typedef enum
{
    INT_LINE = 0,
    INT_UART0_RX = 1,
    INT_UART1_RX = 2,
    INT_CTC_CHANNEL_0 = 3,
    INT_CTC_channel_1 = 4,
    INT_CTC_CHANNEL_2 = 5,
    INT_CTC_CHANNEL_3 = 6,
    // INT_CTC_CHANNEL_4 = 7,
    // INT_CTC_CHANNEL_5 = 8,
    // INT_CTC_CHANNEL_6 = 9,
    // INT_CTC_CHANNEL_7 = 10,
    INT_ULA = 11,
    INT_UART0_TX = 12,
    INT_UART1_TX = 13
} Interrupt;

#endif