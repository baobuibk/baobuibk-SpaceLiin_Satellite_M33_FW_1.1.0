/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdarg.h>
#include <stdio.h>

#include "sp_uart_port.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Tempory buffer for SP_UART_Printf */
#define PRINTF_BUFFER_SIZE 128

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
// static const char * const HEX_reference = "0123456789abcdef";

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint16_t SP_UART_Enqueue(sp_uart_t* p_uart, const char *pcBuf, uint32_t byte_count);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * The function `UARTConfig` initializes a UART configuration with a specified receive buffer size and
 * sets up interrupt-driven reception.
 * 
 * @param uart_p The `uart_p` parameter is a pointer to a structure of type `UART_HandleTypeDef`, which
 * contains configuration settings for a UART (Universal Asynchronous Receiver-Transmitter) peripheral.
 * @param rx_buffer_size The `rx_buffer_size` parameter specifies the size of the receive buffer used
 * for storing incoming data in the UARTConfig function. This buffer will be initialized using the
 * QUEUE_Init function to manage incoming data efficiently.
 */
void SP_UART_Init(sp_uart_t* p_uart, const sp_uart_init_t* p_uart_init)
{
    p_uart->handle  = p_uart_init->handle;
    p_uart->irqn    = p_uart_init->irqn;

    p_uart->p_TX_buffer = p_uart_init->p_TX_buffer;
    p_uart->p_RX_buffer = p_uart_init->p_RX_buffer;

    // Disable the TX IT
    LPUART_DisableInterrupts(p_uart->handle, kLPUART_TxDataRegEmptyInterruptEnable);

    // Start the RX IT, it will only run when there is a data send to the MCU.
    LPUART_EnableInterrupts(p_uart->handle, kLPUART_RxDataRegFullInterruptEnable);
}

//*****************************************************************************
//
//! Send a char to the UART.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void SP_UART_Send_Char(sp_uart_t* p_uart, const char Char)
{
	SP_UART_Enqueue(p_uart, &Char, 1);
}

//*****************************************************************************
//
//! Send the string to the UART.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void SP_UART_Send_String(sp_uart_t* p_uart, const char *pcBuf)
{
	SP_UART_Enqueue(p_uart, pcBuf, strlen(pcBuf));
}

//*****************************************************************************
//
//! Send the string to the UART.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void SP_UART_Send_Buffer(sp_uart_t* p_uart, const char *pcBuf, uint32_t byte_count)
{
	SP_UART_Enqueue(p_uart, pcBuf, byte_count);
}

//*****************************************************************************
//
//! A simple UART based vprintf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pc_string is the format string.
//! \param vaArgP is a variable argument list pointer whose content will depend
//! upon the format string passed in \e pc_string.
//!
//! This function is very similar to the C library <tt>vprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments in the variable arguments list must match the
//! requirements of the format string.  For example, if an integer was passed
//! where a string was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void SP_UART_Printf(sp_uart_t* p_uart, const char *pc_string, ...)
{
    char temp_buffer[PRINTF_BUFFER_SIZE];
    va_list args;

    // Khởi tạo danh sách tham số biến đổi
    va_start(args, pc_string);

    // Định dạng chuỗi vào bộ đệm tạm
    int len = vsnprintf(temp_buffer, PRINTF_BUFFER_SIZE, pc_string, args);

    // Kết thúc danh sách tham số
    va_end(args);

    // Kiểm tra độ dài hợp lệ
    if (len < 0 || len >= PRINTF_BUFFER_SIZE)
    {
        return;
    }

    // Gửi chuỗi đã định dạng qua UART
    SP_UART_Enqueue(p_uart, temp_buffer, (uint32_t)len);
}

//*****************************************************************************
//
//! Read a single character from the UART, blocking if necessary.
//!
//! This function will receive a single character from the UART and store it at
//! the supplied address.
//!
//! In both buffered and unbuffered modes, this function will block until a
//! character is received.  If non-blocking operation is required in buffered
//! mode, a call to UARTRxAvail() may be made to determine whether any
//! characters are currently available for reading.
//!
//! \return Returns the character read.
//
//*****************************************************************************
uint8_t SP_UART_Get_Char(sp_uart_t* p_uart)
{

    uint8_t return_char;

    if (RX_BUFFER_EMPTY(p_uart))
    {
        return 0;
    }
    
    //
    // Read a character from the buffer.
    //
    RX_BUFFER_POP(p_uart, &return_char);

    //
    // Return the character to the caller.
    //
    return(return_char);

}

//*****************************************************************************
//
// Take as many bytes from the transmit buffer as we have space for and move
// them into the UART transmit FIFO.
//
//*****************************************************************************
void SP_UART_Prime_Transmit(sp_uart_t* p_uart)
{
    //
    // Do we have any data to transmit?
    //
    if(!TX_BUFFER_EMPTY(p_uart))
    {
        NVIC_DisableIRQ(p_uart->irqn);

        uint8_t tx_data;
        TX_BUFFER_POP(p_uart, &tx_data);

        LPUART_WriteByte(p_uart->handle, tx_data);
        
        NVIC_EnableIRQ(p_uart->irqn);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ui16Len is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ui16Len parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ui16Len parameter to stop transmitting the string, if
//! a null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
uint16_t SP_UART_Enqueue(sp_uart_t* p_uart, const char *pcBuf, uint32_t byte_count)
{

    uint32_t uIdx;

    //
    // Check for valid arguments.
    //
    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < byte_count; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output. This is for the enter key.
        //
        if(pcBuf[uIdx] == '\n')
        {
            if(!TX_BUFFER_FULL(p_uart))
            {
                TX_BUFFER_PUSH(p_uart, '\r');
            }
            else
            {
                //
                // Buffer is full - discard remaining characters and return.
                //
                break;
            }
        }

        if(!TX_BUFFER_FULL(p_uart))
        {
            TX_BUFFER_PUSH(p_uart, pcBuf[uIdx]);
        }
        else
        {
            //
            // Buffer is full - discard remaining characters and return.
            //
            TX_BUFFER_OVERWRITE(p_uart, '\r');
            break;
        }
    }

    //
    // If the usart txe irq is disable, this mean an usart phase is finished
    // we need to enable the txe irq and kick start the transmit process.
    //
    // uint32_t is_TXE_enable = LPUART_GetEnabledInterrupts(p_uart->handle);
    // if ((kLPUART_TxDataRegEmptyInterruptEnable & is_TXE_enable) == kLPUART_TxDataRegEmptyInterruptEnable)

    // uint32_t ien = LPUART_GetEnabledInterrupts(p_uart->handle);

    // Is TXE interrupt enabled now?
    bool txe_enabled = (LPUART_GetEnabledInterrupts(p_uart->handle) & (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable) != 0U;

    if (!txe_enabled)
    {
        // NOTE: Turn on TXE after prime transmit,
        // if turn on TXE b4 prime transmit create a
        // bug where the index = 2 char don't get
        // send.

        SP_UART_Prime_Transmit(p_uart);
        LPUART_EnableInterrupts(p_uart->handle, kLPUART_TxDataRegEmptyInterruptEnable);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// void SP_UART_Printf(sp_uart_t* p_uart, const char *pc_string, ...)
// {
//     uint16_t ui16Idx, ui16Value, ui16Pos, ui16Count, ui16Base, ui16Neg;
//     char *pcStr, pcBuf[16], cFill;

//     //
//     // Check the arguments.
//     //

//     va_list vaArgP;

//     va_start(vaArgP, pc_string);

//     //
//     // Loop while there are more characters in the string.
//     //
//     while(*pc_string)
//     {
//         //
//         // Find the first non-% character, or the end of the string.
//         //
//         for(ui16Idx = 0;
//             (pc_string[ui16Idx] != '%') && (pc_string[ui16Idx] != '\0');
//             ui16Idx++)
//         {
//         }

//         //
//         // Write this portion of the string.
//         //
//         SP_UART_Enqueue(p_uart, pc_string, ui16Idx);

//         //
//         // Skip the portion of the string that was written.
//         //
//         pc_string += ui16Idx;

//         //
//         // See if the next character is a %.
//         //
//         if(*pc_string == '%')
//         {
//             //
//             // Skip the %.
//             //
//             pc_string++;

//             //
//             // Set the digit count to zero, and the fill character to space
//             // (in other words, to the defaults).
//             //
//             ui16Count = 0;
//             cFill = ' ';

//             //
//             // It may be necessary to get back here to process more characters.
//             // Goto's aren't pretty, but effective.  I feel extremely dirty for
//             // using not one but two of the beasts.
//             //
// again:

//             //
//             // Determine how to handle the next character.
//             //
//             switch(*pc_string++)
//             {
//                 //
//                 // Handle the digit characters.
//                 //
//                 case '0':
//                 case '1':
//                 case '2':
//                 case '3':
//                 case '4':
//                 case '5':
//                 case '6':
//                 case '7':
//                 case '8':
//                 case '9':
//                 {
//                     //
//                     // If this is a zero, and it is the first digit, then the
//                     // fill character is a zero instead of a space.
//                     //
//                     if((pc_string[-1] == '0') && (ui16Count == 0))
//                     {
//                         cFill = '0';
//                     }

//                     //
//                     // Update the digit count.
//                     //
//                     ui16Count *= 10;
//                     ui16Count += pc_string[-1] - '0';

//                     //
//                     // Get the next character.
//                     //
//                     goto again;
//                 }

//                 //
//                 // Handle the %c command.
//                 //
//                 case 'c':
//                 {
//                     //
//                     // Get the value from the varargs.
//                     //
//                     ui16Value = va_arg(vaArgP, int);

//                     //
//                     // Print out the character.
//                     //
//                     SP_UART_Enqueue(p_uart, (char *)&ui16Value, 1);

//                     //
//                     // This command has been handled.
//                     //
//                     break;
//                 }

//                 //
//                 // Handle the %d and %i commands.
//                 //
//                 case 'd':
//                 case 'i':
//                 {
//                     //
//                     // Get the value from the varargs.
//                     //
//                     ui16Value = va_arg(vaArgP, int);

//                     //
//                     // Reset the buffer position.
//                     //
//                     ui16Pos = 0;

//                     //
//                     // If the value is negative, make it positive and indicate
//                     // that a minus sign is needed.
//                     //
//                     if((int16_t)ui16Value < 0)
//                     {
//                         //
//                         // Make the value positive.
//                         //
//                         ui16Value = -(int16_t)ui16Value;

//                         //
//                         // Indicate that the value is negative.
//                         //
//                         ui16Neg = 1;
//                     }
//                     else
//                     {
//                         //
//                         // Indicate that the value is positive so that a minus
//                         // sign isn't inserted.
//                         //
//                         ui16Neg = 0;
//                     }

//                     //
//                     // Set the base to 10.
//                     //
//                     ui16Base = 10;

//                     //
//                     // Convert the value to ASCII.
//                     //
//                     goto convert;
//                 }

//                 //
//                 // Handle the %s command.
//                 //
//                 case 's':
//                 {
//                     //
//                     // Get the string pointer from the varargs.
//                     //
//                     pcStr = va_arg(vaArgP, char *);

//                     //
//                     // Determine the length of the string.
//                     //
//                     for(ui16Idx = 0; pcStr[ui16Idx] != '\0'; ui16Idx++)
//                     {
//                     }

//                     //
//                     // Write the string.
//                     //
//                     SP_UART_Enqueue(p_uart, pcStr, ui16Idx);

//                     //
//                     // Write any required padding spaces
//                     //
//                     if(ui16Count > ui16Idx)
//                     {
//                         ui16Count -= ui16Idx;
//                         while(ui16Count--)
//                         {
//                             SP_UART_Enqueue(p_uart, " ", 1);
//                         }
//                     }

//                     //
//                     // This command has been handled.
//                     //
//                     break;
//                 }

//                 //
//                 // Handle the %u command.
//                 //
//                 case 'u':
//                 {
//                     //
//                     // Get the value from the varargs.
//                     //
//                     ui16Value = va_arg(vaArgP, int);

//                     //
//                     // Reset the buffer position.
//                     //
//                     ui16Pos = 0;

//                     //
//                     // Set the base to 10.
//                     //
//                     ui16Base = 10;

//                     //
//                     // Indicate that the value is positive so that a minus sign
//                     // isn't inserted.
//                     //
//                     ui16Neg = 0;

//                     //
//                     // Convert the value to ASCII.
//                     //
//                     goto convert;
//                 }

//                 //
//                 // Handle the %x and %X commands.  Note that they are treated
//                 // identically; in other words, %X will use lower case letters
//                 // for a-f instead of the upper case letters it should use.  We
//                 // also alias %p to %x.
//                 //
//                 case 'x':
//                 case 'X':
//                 case 'p':
//                 {
//                     //
//                     // Get the value from the varargs.
//                     //
//                     ui16Value = va_arg(vaArgP, int);

//                     //
//                     // Reset the buffer position.
//                     //
//                     ui16Pos = 0;

//                     //
//                     // Set the base to 16.
//                     //
//                     ui16Base = 16;

//                     //
//                     // Indicate that the value is positive so that a minus sign
//                     // isn't inserted.
//                     //
//                     ui16Neg = 0;

//                     //
//                     // Determine the number of digits in the string version of
//                     // the value.
//                     //
// convert:
//                     for(ui16Idx = 1;
//                         (((ui16Idx * ui16Base) <= ui16Value) &&
//                          (((ui16Idx * ui16Base) / ui16Base) == ui16Idx));
//                         ui16Idx *= ui16Base, ui16Count--)
//                     {
//                     }

//                     //
//                     // If the value is negative, reduce the count of padding
//                     // characters needed.
//                     //
//                     if(ui16Neg)
//                     {
//                         ui16Count--;
//                     }

//                     //
//                     // If the value is negative and the value is padded with
//                     // zeros, then place the minus sign before the padding.
//                     //
//                     if(ui16Neg && (cFill == '0'))
//                     {
//                         //
//                         // Place the minus sign in the output buffer.
//                         //
//                         pcBuf[ui16Pos++] = '-';

//                         //
//                         // The minus sign has been placed, so turn off the
//                         // negative flag.
//                         //
//                         ui16Neg = 0;
//                     }

//                     //
//                     // Provide additional padding at the beginning of the
//                     // string conversion if needed.
//                     //
//                     if((ui16Count > 1) && (ui16Count < 16))
//                     {
//                         for(ui16Count--; ui16Count; ui16Count--)
//                         {
//                             pcBuf[ui16Pos++] = cFill;
//                         }
//                     }

//                     //
//                     // If the value is negative, then place the minus sign
//                     // before the number.
//                     //
//                     if(ui16Neg)
//                     {
//                         //
//                         // Place the minus sign in the output buffer.
//                         //
//                         pcBuf[ui16Pos++] = '-';
//                     }

//                     //
//                     // Convert the value into a string.
//                     //
//                     for(; ui16Idx; ui16Idx /= ui16Base)
//                     {
//                         pcBuf[ui16Pos++] =
//                             HEX_reference[(ui16Value / ui16Idx) % ui16Base];
//                     }

//                     //
//                     // Write the string.
//                     //
//                     SP_UART_Enqueue(p_uart, pcBuf, ui16Pos);

//                     //
//                     // This command has been handled.
//                     //
//                     break;
//                 }

//                 //
//                 // Handle the %% command.
//                 //
//                 case '%':
//                 {
//                     //
//                     // Simply write a single %.
//                     //
//                     SP_UART_Enqueue(p_uart, pc_string - 1, 1);

//                     //
//                     // This command has been handled.
//                     //
//                     break;
//                 }

//                 //
//                 // Handle all other commands.
//                 //
//                 default:
//                 {
//                     //
//                     // Indicate an error.
//                     //
//                     SP_UART_Enqueue(p_uart, "ERROR", 5);

//                     //
//                     // This command has been handled.
//                     //
//                     break;
//                 }
//             }
//         }
//     }
//     va_end(vaArgP);
// }