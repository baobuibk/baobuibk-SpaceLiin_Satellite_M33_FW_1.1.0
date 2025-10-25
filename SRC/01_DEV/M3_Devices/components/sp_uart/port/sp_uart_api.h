#ifndef _SP_UART_API_H_
#define _SP_UART_API_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

#include "ring_char_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * Forward declaration, go to respective port file to
 * read definition
 */
typedef struct _sp_uart_t_ 		sp_uart_t;
typedef struct _sp_uart_init_t_ sp_uart_init_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void SP_UART_Init           (sp_uart_t* p_uart, const sp_uart_init_t* p_uart_init);

void SP_UART_Send_Char	    (sp_uart_t* p_uart, const char Char);
void SP_UART_Send_String    (sp_uart_t* p_uart, const char *pcBuf);
void SP_UART_Send_Buffer    (sp_uart_t* p_uart, const char *pcBuf, uint32_t byte_count);
void SP_UART_Printf 	    (sp_uart_t* p_uart, const char *pc_string, ...);

uint8_t SP_UART_Get_Char	(sp_uart_t* p_uart);

void SP_UART_Prime_Transmit	(sp_uart_t* p_uart);

#endif /* _SP_UART_API_H_ */