#ifndef _SP_UART_PORT_H_
#define _SP_UART_PORT_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Standard API for SP SPI STDIO includes. */
#include "sp_uart_api.h"

/* Port specific includes. */
#include "fsl_lpuart.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _sp_uart_t_
{
    LPUART_Type*            handle;
    IRQn_Type		        irqn;

    ring_char_buffer_t*     p_TX_buffer;
    ring_char_buffer_t*     p_RX_buffer;
    volatile    uint8_t     RX_irq_char;
} sp_uart_t;

typedef struct _sp_uart_init_t_
{
    LPUART_Type*            handle;
    IRQn_Type		        irqn;

    ring_char_buffer_t*     p_TX_buffer;
    ring_char_buffer_t*     p_RX_buffer;
    volatile    uint8_t     RX_irq_char;
} sp_uart_init_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif // _SP_UART_PORT_H_
