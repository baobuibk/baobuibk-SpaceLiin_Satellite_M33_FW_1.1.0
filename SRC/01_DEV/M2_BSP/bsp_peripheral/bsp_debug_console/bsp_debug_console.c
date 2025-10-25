/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdio.h>

/* Freescale includes. */
#include "fsl_common.h"
#include "fsl_lpuart.h"

/* Board Support includes. */
#include "bsp_board.h"
#include "bsp_debug_console.h"

/* Component includes. */
#include "sp_uart.h"
#include "ring_char_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DEBUG_DB9_PRINTF_BUFFER_SIZE    128
#define DEBUG_DB9_IRQHandler            LPUART2_IRQHandler

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_SIZE(p_uart)              ((p_uart)->max_items)

#define TX_BUFFER_USED(p_uart)              (ring_char_buffer_get_buffer_count((p_uart)->p_TX_buffer))

#define TX_BUFFER_FREE(p_uart)              (ring_char_buffer_get_free_space((p_uart)->p_TX_buffer))

#define TX_BUFFER_EMPTY(p_uart)             (ring_char_buffer_is_empty((p_uart)->p_TX_buffer))

#define TX_BUFFER_FULL(p_uart)              (ring_char_buffer_is_full((p_uart)->p_TX_buffer))

#define TX_BUFFER_PUSH(p_uart, char)        (ring_char_buffer_push((p_uart)->p_TX_buffer, (char)))

#define TX_BUFFER_POP(p_uart, p_buff)       (ring_char_buffer_pop((p_uart)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_OVERWRITE(p_uart, char)   (ring_char_buffer_overwrite((p_uart)->p_TX_buffer, (char)))

#define TX_BUFFER_PEAK(p_uart, p_buff)      (ring_char_buffer_peak((p_uart)->p_TX_buffer, (p_buff)))

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_SIZE(p_uart)              ((p_uart)->max_items)

#define RX_BUFFER_USED(p_uart)              (ring_char_buffer_get_buffer_count((p_uart)->p_RX_buffer))

#define RX_BUFFER_FREE(p_uart)              (ring_char_buffer_get_free_space((p_uart)->p_RX_buffer))

#define RX_BUFFER_EMPTY(p_uart)             (ring_char_buffer_is_empty((p_uart)->p_RX_buffer))

#define RX_BUFFER_FULL(p_uart)              (ring_char_buffer_is_full((p_uart)->p_RX_buffer))

#define RX_BUFFER_PUSH(p_uart, char)        (ring_char_buffer_push((p_uart)->p_RX_buffer, (char)))

#define RX_BUFFER_POP(p_uart, p_buff)       (ring_char_buffer_pop((p_uart)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_OVERWRITE(p_uart, char)   (ring_char_buffer_overwrite((p_uart)->p_RX_buffer, (char)))

#define RX_BUFFER_PEAK(p_uart, p_buff)      (ring_char_buffer_peak((p_uart)->p_RX_buffer, (p_buff)))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
// A mapping from an integer between 0 and 15 to its ASCII character
// equivalent.
//
//*****************************************************************************
static const char * const HEX_reference = "0123456789abcdef";

sp_uart_t           DEBUG_DB9_UART;

ring_char_buffer_t  DEBUG_DB9_UART_TX_ring_buffer;
uint8_t             g_DEBUG_DB9_UART_TX_buffer[2048];

ring_char_buffer_t  DEBUG_DB9_UART_RX_ring_buffer;
uint8_t             g_DEBUG_DB9_UART_RX_buffer[64];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
//! Init the bsp debug console
//
//*****************************************************************************
void bsp_debug_console_init()
{
    ring_char_buffer_init(&DEBUG_DB9_UART_TX_ring_buffer, g_DEBUG_DB9_UART_TX_buffer, (uint32_t)sizeof(g_DEBUG_DB9_UART_TX_buffer));
    ring_char_buffer_init(&DEBUG_DB9_UART_RX_ring_buffer, g_DEBUG_DB9_UART_RX_buffer, (uint32_t)sizeof(g_DEBUG_DB9_UART_RX_buffer));

    sp_uart_init_t debug_uart_init =
    {
        .handle = DEBUG_DB9_LPUART_BASE,
        .irqn   = DEBUG_DB9_LPUART_IRQn,
        .p_TX_buffer = &DEBUG_DB9_UART_TX_ring_buffer,
        .p_RX_buffer = &DEBUG_DB9_UART_RX_ring_buffer,
    };

    SP_UART_Init(&DEBUG_DB9_UART, &debug_uart_init);

    EnableIRQ(DEBUG_DB9_UART.irqn);
}

//*****************************************************************************
//
//! Debug DB9 LPUART IRQHandler
//
//*****************************************************************************
void DEBUG_DB9_IRQHandler(void)
{
    // if((kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE)) == kLPUART_TxDataRegEmptyFlag)
    if(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE))
    {
        if(TX_BUFFER_EMPTY(&DEBUG_DB9_UART))
        {
            // Buffer empty, so disable interrupts
            LPUART_DisableInterrupts(DEBUG_DB9_LPUART_BASE, kLPUART_TxDataRegEmptyInterruptEnable);
        }
        else
        {
            // There is more data in the output buffer. Send the next byte
            SP_UART_Prime_Transmit(&DEBUG_DB9_UART);
        }
    }

    // if((kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE)) == kLPUART_RxDataRegFullFlag)
    if(kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(DEBUG_DB9_LPUART_BASE))
    {
        DEBUG_DB9_UART.RX_irq_char = (char)LPUART_ReadByte(DEBUG_DB9_LPUART_BASE);

        // NOTE: On win 10, default PUTTY when hit enter only send back '\r',
        // while on default HERCULES when hit enter send '\r\n' in that order.
        // The code bellow is modified so that it can work on PUTTY and HERCULES.
        if((!RX_BUFFER_FULL(&DEBUG_DB9_UART)) && (DEBUG_DB9_UART.RX_irq_char != '\n'))
        {
            if (DEBUG_DB9_UART.RX_irq_char == '\r')
            {
                RX_BUFFER_PUSH(&DEBUG_DB9_UART, '\n');
            }
            else
            {
                RX_BUFFER_PUSH(&DEBUG_DB9_UART, DEBUG_DB9_UART.RX_irq_char);
            }
        }
    }

    SDK_ISR_EXIT_BARRIER;
}

//*****************************************************************************
//
//! Print string with format to the debug console.
//
//*****************************************************************************
void bsp_debug_console_printf(const char * format, ...)
{
    uint16_t ui16Idx, ui16Value, ui16Pos, ui16Count, ui16Base, ui16Neg;
    char *pcStr, pcBuf[16], cFill;
    
    //
    // Check the arguments.
    //

    va_list vaArgP;

    va_start(vaArgP, format);

    //
    // Loop while there are more characters in the string.
    //
    while(*format)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui16Idx = 0;
            (format[ui16Idx] != '%') && (format[ui16Idx] != '\0');
            ui16Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        SP_UART_Send_Buffer(&DEBUG_DB9_UART, format, ui16Idx);

        //
        // Skip the portion of the string that was written.
        //
        format += ui16Idx;

        //
        // See if the next character is a %.
        //
        if(*format == '%')
        {
            //
            // Skip the %.
            //
            format++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui16Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*format++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((format[-1] == '0') && (ui16Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui16Count *= 10;
                    ui16Count += format[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui16Value = va_arg(vaArgP, int);

                    //
                    // Print out the character.
                    //
                    SP_UART_Send_Buffer(&DEBUG_DB9_UART, (char *)&ui16Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui16Value = va_arg(vaArgP, int);

                    //
                    // Reset the buffer position.
                    //
                    ui16Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int16_t)ui16Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui16Value = -(int16_t)ui16Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui16Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui16Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui16Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui16Idx = 0; pcStr[ui16Idx] != '\0'; ui16Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    SP_UART_Send_Buffer(&DEBUG_DB9_UART, pcStr, ui16Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui16Count > ui16Idx)
                    {
                        ui16Count -= ui16Idx;
                        while(ui16Count--)
                        {
                            SP_UART_Send_Buffer(&DEBUG_DB9_UART, " ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui16Value = va_arg(vaArgP, int);

                    //
                    // Reset the buffer position.
                    //
                    ui16Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui16Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui16Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui16Value = va_arg(vaArgP, int);

                    //
                    // Reset the buffer position.
                    //
                    ui16Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui16Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui16Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui16Idx = 1;
                        (((ui16Idx * ui16Base) <= ui16Value) &&
                         (((ui16Idx * ui16Base) / ui16Base) == ui16Idx));
                        ui16Idx *= ui16Base, ui16Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui16Neg)
                    {
                        ui16Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui16Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui16Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui16Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui16Count > 1) && (ui16Count < 16))
                    {
                        for(ui16Count--; ui16Count; ui16Count--)
                        {
                            pcBuf[ui16Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui16Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui16Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui16Idx; ui16Idx /= ui16Base)
                    {
                        pcBuf[ui16Pos++] =
                            HEX_reference[(ui16Value / ui16Idx) % ui16Base];
                    }

                    //
                    // Write the string.
                    //
                    SP_UART_Send_Buffer(&DEBUG_DB9_UART, pcBuf, ui16Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    SP_UART_Send_Buffer(&DEBUG_DB9_UART, format - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    SP_UART_Send_Buffer(&DEBUG_DB9_UART, "ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
    va_end(vaArgP);
}

//*****************************************************************************
//
//! Send a char to the debug console.
//
//*****************************************************************************
void bsp_debug_console_send_char(const char Char)
{
	SP_UART_Send_Char(&DEBUG_DB9_UART, Char);
}

//*****************************************************************************
//
//! Send the string to the debug console.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void bsp_debug_console_send_string(const char *pcBuf)
{
	SP_UART_Send_String(&DEBUG_DB9_UART, pcBuf);
}

//*****************************************************************************
//
//! Get a char from the buffer.
//
//*****************************************************************************
uint8_t bsp_debug_console_get_char(void)
{
	return SP_UART_Get_Char(&DEBUG_DB9_UART);
}

//*****************************************************************************
//
//! Check wheter RX buffer empty or not.
//
//*****************************************************************************
bool bsp_debug_console_RX_buffer_empty(void)
{
	return RX_BUFFER_EMPTY(&DEBUG_DB9_UART);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// void bsp_debug_console_printf(const char * format, ...)
// {
//     char temp_buffer[DEBUG_DB9_PRINTF_BUFFER_SIZE];
//     va_list args;

//     // Khởi tạo danh sách tham số biến đổi
//     va_start(args, format);

//     // Định dạng chuỗi vào bộ đệm tạm
//     int len = vsnprintf(temp_buffer, DEBUG_DB9_PRINTF_BUFFER_SIZE, format, args);

//     // Kết thúc danh sách tham số
//     va_end(args);

//     // Kiểm tra độ dài hợp lệ
//     if (len < 0 || len >= DEBUG_DB9_PRINTF_BUFFER_SIZE)
//     {
//         return ;
//     }

//     // Gửi chuỗi đã định dạng qua UART
//     SP_UART_Send_Buffer(&DEBUG_DB9_UART, temp_buffer, (uint32_t)len);
// }