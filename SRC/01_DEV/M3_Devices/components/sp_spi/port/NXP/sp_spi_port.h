#ifndef _SP_SPI_PORT_H_
#define _SP_SPI_PORT_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Standard API for SP SPI STDIO includes. */
#include "sp_spi_api.h"

/* Port specific includes. */
#include "fsl_lpspi.h"

/* User depency includes. */
#include "ring_buffer.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Only the 2 LSB are used for SPI header (00: WRITE, 01: READ)
// Higher bits can be used for internal logic.
typedef enum _sp_spi_command_t_
{
    SPI_WRITE = 0,                          // DO NOT MOVE OR CHANGE POSITION
    SPI_READ  = 1,                          // DO NOT MOVE OR CHANGE POSITION
} sp_spi_command_t;

typedef enum _sp_spi_byte_type_t_
{
    SP_SPI_BYTE_HEADER,   // frame/preamble bytes, addr/cmd, etc.
    SP_SPI_BYTE_PAYLOAD,  // data bytes
    SP_SPI_BYTE_TRAILER,  // footer: CRC, checksum, EOT marker
} sp_spi_byte_type_t;

typedef struct _sp_spi_blocking_frame_t_
{
    uint8_t*    p_TX_buffer;
    uint8_t*    p_RX_buffer;

    uint32_t    data_size;
} sp_spi_blocking_frame_t;

typedef struct _sp_spi_irq_frame_t_
{
    sp_spi_command_t    command;
    sp_spi_byte_type_t  data_type;

    uint8_t             data;
    // uint8_t             mask;
} sp_spi_irq_frame_t;

typedef struct _sp_spi_t_
{
    /* SPI bus address */
    LPSPI_Type*         p_spi_base;
    IRQn_Type           irqn;

    lpspi_master_handle_t* p_master_handle;

    /* Software queues (frames, not bytes) */
    ring_buffer_t*      p_TX_buffer;
    sp_spi_irq_frame_t  current_irq_frame;

    ring_buffer_t*      p_RX_buffer;
    uint8_t             temp_RX_byte;

    // uint32_t            srcClockHz;
    // uint32_t            baudHz;
    
    // sp_spi_mode_t       mode;
    // sp_spi_bit_order_t  bitOrder;
    // sp_spi_cs_level_t   csLevel;
    
    // uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;

    bool                is_TX_busy;
} sp_spi_t;

typedef struct _sp_spi_init_t_
{
    LPSPI_Type*         p_spi_base;
    IRQn_Type           irqn;

    lpspi_master_handle_t* p_master_handle;

    /* Software queues (frames, not bytes) */
    ring_buffer_t*      p_TX_buffer;
    sp_spi_irq_frame_t  current_irq_frame;

    ring_buffer_t*      p_RX_buffer;
    uint8_t             temp_RX_byte;

    // uint32_t            srcClockHz;
    // uint32_t            baudHz;
    
    // sp_spi_mode_t       mode;
    // sp_spi_bit_order_t  bitOrder;
    // sp_spi_cs_level_t   csLevel;

    // uint8_t             bitsPerFrame;
    uint8_t             pcs;
    bool                pcsContinuous;
} sp_spi_init_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* _SP_SPI_PORT_H_ */
