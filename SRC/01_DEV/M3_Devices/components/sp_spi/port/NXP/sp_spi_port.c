/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "sp_spi_port.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_SIZE(p_spi)              ((p_spi)->max_items)

#define TX_BUFFER_USED(p_spi)              (ring_buffer_get_buffer_count((p_spi)->p_TX_buffer))

#define TX_BUFFER_FREE(p_spi)              (ring_buffer_get_free_space((p_spi)->p_TX_buffer))

#define TX_BUFFER_EMPTY(p_spi)             (ring_buffer_is_empty((p_spi)->p_TX_buffer))

#define TX_BUFFER_FULL(p_spi)              (ring_buffer_is_full((p_spi)->p_TX_buffer))

#define TX_BUFFER_PUSH(p_spi, p_buff)      (ring_buffer_push((p_spi)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_POP(p_spi, p_buff)       (ring_buffer_pop((p_spi)->p_TX_buffer, (void*)(p_buff)))

#define TX_BUFFER_OVERWRITE(p_spi, p_buff) (ring_buffer_overwrite((p_spi)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_PEAK(p_spi, p_buff)      (ring_buffer_peak((p_spi)->p_TX_buffer, (p_buff)))

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_SIZE(p_spi)              ((p_spi)->max_items)

#define RX_BUFFER_USED(p_spi)              (ring_buffer_get_buffer_count((p_spi)->p_RX_buffer))

#define RX_BUFFER_FREE(p_spi)              (ring_buffer_get_free_space((p_spi)->p_RX_buffer))

#define RX_BUFFER_EMPTY(p_spi)             (ring_buffer_is_empty((p_spi)->p_RX_buffer))

#define RX_BUFFER_FULL(p_spi)              (ring_buffer_is_full((p_spi)->p_RX_buffer))

#define RX_BUFFER_PUSH(p_spi, p_buff)      (ring_buffer_push((p_spi)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_POP(p_spi, p_buff)       (ring_buffer_pop((p_spi)->p_RX_buffer, (void*)(p_buff)))

#define RX_BUFFER_OVERWRITE(p_spi, p_buff) (ring_buffer_overwrite((p_spi)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_PEAK(p_spi, p_buff)      (ring_buffer_peak((p_spi)->p_RX_buffer, (p_buff)))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// static uint8_t TX_blocking_buffer[TX_BLOCKING_SIZE] = {0U};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t SP_SPI_build_standard_header(uint8_t reg_addr, sp_spi_command_t command_type);
static void SP_SPI_IRQ_prime_transmit(sp_spi_t* p_spi);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * @brief Initialize the SPI peripheral with the provided configuration
 * @param p_spi Pointer to the SPI_Io_t structure
 * @param init Pointer to the SPI_IoInit_t structure containing initialization parameters
 * @return ERROR_OK on success, or an error code on failure
 */
void SP_SPI_master_blocking_init(sp_spi_t* p_spi, const sp_spi_init_t *init)
{
    if (!p_spi || !init || !init->p_spi_base) 
    {
        return;
    }

    // Copy initialization parameters to the sp_spi_t structure
    p_spi->p_spi_base     = init->p_spi_base;

    p_spi->pcs            = init->pcs; // 0..3
    p_spi->pcsContinuous  = init->pcsContinuous;

    return;
}

void SP_SPI_master_transfer_blocking(sp_spi_t* p_spi, sp_spi_blocking_frame_t* p_frame)
{
    lpspi_transfer_t transfer_handle =
    {
        .txData      = p_frame->p_TX_buffer,
        .rxData      = p_frame->p_RX_buffer,
        .dataSize    = p_frame->data_size,

        .configFlags = ((p_spi->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
                        (p_spi->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0)
    };

    LPSPI_MasterTransferBlocking(p_spi->p_spi_base, &transfer_handle);

	return;
}

void SP_SPI_build_default_blocking_frame(uint8_t addr, uint8_t* p_data_buff, sp_spi_command_t command, sp_spi_blocking_frame_t* p_frame)
{
    // Check for NULL pointer
    if ((p_data_buff == NULL) || (p_frame == NULL))
    {
        return;
    }

    sp_spi_command_t cmd = command & 0x03;

    // Build default header: [reg_addr(6b) | command(2b)]
    p_frame->p_TX_buffer[0] = (uint8_t)(((addr & 0x3F) << 2) | cmd);

    for (uint32_t index = 0; index < p_frame->data_size; index++)
    {
        p_frame->p_TX_buffer[index + 1] = (cmd == SPI_READ) 
                                            ? 0xFF 
                                            : p_data_buff[index];
    }
}

void SP_SPI_master_irq_init(sp_spi_t* p_spi, const sp_spi_init_t *init)
{
    // if (!p_spi || !init || !init->p_spi_base) 
    // {
    //     return;
    // }

    // Copy initialization parameters to the sp_spi_t structure
    p_spi->p_spi_base = init->p_spi_base;
    p_spi->irqn       = init->irqn;

    p_spi->p_master_handle = init->p_master_handle;

    p_spi->p_TX_buffer = init->p_TX_buffer;
    p_spi->p_RX_buffer = init->p_RX_buffer;

    p_spi->pcs           = init->pcs; // 0..3
    p_spi->pcsContinuous = init->pcsContinuous;

    p_spi->is_TX_busy    = false;

    return;
}

void SP_SPI_master_transfer_irq(sp_spi_t* p_spi)
{
    if (p_spi->is_TX_busy == true)
    {
        return;
    }

    SP_SPI_IRQ_prime_transmit(p_spi);
}

void SP_SPI_build_default_irq_frame(sp_spi_t* p_spi, uint8_t addr, uint8_t* p_data_buff, uint32_t data_count,sp_spi_command_t command)
{
    // Check for NULL pointer
    if ((p_spi == NULL) || (p_data_buff == NULL))
    {
        return;
    }

    sp_spi_irq_frame_t irq_frame_temp;
    sp_spi_command_t   cmd = command & 0x03;

    if (addr != 0)
    {
        if (TX_BUFFER_FULL(p_spi))
        {
            irq_frame_temp.data_type = SP_SPI_BYTE_TRAILER;
            return;
        }
        
        // Add the header to the TX buffer
        irq_frame_temp.data_type = SP_SPI_BYTE_HEADER;
        irq_frame_temp.command   = command;
        // irq_frame_temp.mask      = 0xFF;
        irq_frame_temp.data      = SP_SPI_build_standard_header(addr, command);

        TX_BUFFER_PUSH(p_spi, &irq_frame_temp);
    }

    // Loop through data bytes from last to first
    for (int8_t data_idx = (data_count - 1); data_idx >= 0; data_idx--)
    {
        if (TX_BUFFER_FULL(p_spi))
        {
            irq_frame_temp.data_type = SP_SPI_BYTE_TRAILER;
            return;
        }

        // Set data type: END for last byte, DATA for others
        irq_frame_temp.data_type =  (data_idx == 0) 
                                    ? SP_SPI_BYTE_TRAILER
                                    : SP_SPI_BYTE_PAYLOAD;

        irq_frame_temp.command   =   command;
        // irq_frame_temp.mask      =   p_frame->p_data_array[data_idx].mask;

        // For SPI_READ or SPI_READ_TO_TEMP, set data to 0xFF; otherwise use provided data
        irq_frame_temp.data      =  (cmd == SPI_READ) 
                                    ? 0xFF 
                                    : p_data_buff[data_idx];

        TX_BUFFER_PUSH(p_spi, &irq_frame_temp);
    }
}

void SP_SPI_IRQ_subhandle(sp_spi_t* p_spi)
{
    // Cache index & pointer cho nhanh, hạn chế truy RAM nhiều lần
    sp_spi_irq_frame_t *p_current_frame = &p_spi->current_irq_frame;
    uint8_t            *p_RX_temp       = &p_spi->temp_RX_byte;

    if (p_current_frame->command == SPI_READ)
    {
        if (p_current_frame->data_type != SP_SPI_BYTE_HEADER)
        {
            RX_BUFFER_PUSH(p_spi, p_RX_temp);
        }
    }

    // Còn lại (WRITE, WRITE_MODIFY) KHÔNG LÀM GÌ - vì không có nhận data

    // Nếu hết buffer truyền, tắt ngắt
    if(TX_BUFFER_EMPTY(p_spi))
    {
        // Buffer empty, no more queue
        p_spi->is_TX_busy = false;
        return;
    }

    // There is more data in the output buffer. Send the next byte
    p_spi->is_TX_busy = true;
    SP_SPI_IRQ_prime_transmit(p_spi);
    return;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t SP_SPI_build_standard_header(uint8_t reg_addr, sp_spi_command_t command_type)
{
    // Tạo header: [reg_addr(6b) | command(2b)]
    return (uint8_t)(((reg_addr & 0x3F) << 2) | (command_type & 0x03));
}

static void SP_SPI_IRQ_prime_transmit(sp_spi_t* p_spi)
{
    //
    // Do we have any data to transmit?
    //
    lpspi_transfer_t fsl_TX_frame;
    TX_BUFFER_POP(p_spi, &p_spi->current_irq_frame);

    fsl_TX_frame.txData      = &p_spi->current_irq_frame.data;
    fsl_TX_frame.rxData      = &p_spi->temp_RX_byte;
    fsl_TX_frame.dataSize    = 1;
    fsl_TX_frame.configFlags = ((p_spi->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
                            (p_spi->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0);

    LPSPI_MasterTransferNonBlocking(p_spi->p_spi_base, p_spi->p_master_handle, &fsl_TX_frame);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * @brief Initialize the SPI peripheral with the provided configuration
 * @param p_spi Pointer to the SPI_Io_t structure
 * @param init Pointer to the SPI_IoInit_t structure containing initialization parameters
 * @return ERROR_OK on success, or an error code on failure
 */
// void SP_SPI_master_blocking_init(sp_spi_t* p_spi, const sp_spi_init_t *init)
// {
//     if (!p_spi || !init || !init->p_spi_base) 
//     {
//         return;
//     }

//     // Copy initialization parameters to the SPI_Io_t structure
//     p_spi->p_spi_base     = init->p_spi_base;
//     // p_spi->irqn           = init->irqn;

//     // p_spi->srcClockHz     = init->srcClockHz;
//     // p_spi->baudHz         = init->baudHz;

//     // p_spi->mode           = init->mode;
//     // p_spi->bitOrder       = init->bitOrder;
//     // p_spi->csLevel        = init->csLevel;

//     // p_spi->bitsPerFrame   = init->bitsPerFrame ? init->bitsPerFrame : 8;
//     p_spi->pcs            = init->pcs; // 0..3
//     p_spi->pcsContinuous  = init->pcsContinuous;
    
//     // p_spi->bBusy          = false;

//     return;
// }

// void SP_SPI_master_write_blocking(sp_spi_t* p_spi, sp_spi_frame_t* p_frame)
// {
// 	SP_SPI_master_transfer_blocking(p_spi, p_frame, SPI_WRITE);
// }

// void SP_SPI_master_read_blocking(sp_spi_t* p_spi, sp_spi_frame_t* p_frame)
// {
// 	SP_SPI_master_transfer_blocking(p_spi, p_frame, SPI_READ);
// }

// static void SP_SPI_prepare_TX_data(sp_spi_frame_t* p_frame, sp_spi_command_t command_type)
// {
//     TX_blocking_buffer[0] = SP_SPI_make_header(p_frame->addr, command_type);

//     for (uint32_t index = 0; index < p_frame->data_size; index++)
//     {
//         TX_blocking_buffer[index + 1] = ((command_type & 0x03) == SPI_READ) 
//                                         ? 0xFF 
//                                         : p_frame->p_TX_buffer[index];
//     }
// }

// void SP_SPI_master_transfer_blocking(sp_spi_t* p_spi, sp_spi_frame_t* p_frame)
// {
// 	// NVIC_DisableIRQ(p_spi->irqn);
// 	// p_spi->bBusy = true;
// 	// NVIC_EnableIRQ(p_spi->irqn);

//     // SP_SPI_prepare_TX_data(p_frame, command_type);

//     lpspi_transfer_t transfer_handle =
//     {
//         .txData      = p_frame->p_TX_buffer,
//         .rxData      = p_frame->p_RX_buffer,
//         .dataSize    = p_frame->data_size,

//         .configFlags = ((p_spi->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
//                         (p_spi->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0)
//     };

//     // transfer_handle.txData      = TX_blocking_buffer;
//     // transfer_handle.rxData      = p_frame->p_RX_buffer;
//     // transfer_handle.dataSize    = p_frame->data_size + 1; // + 1 for addr

//     // transfer_handle.configFlags = ((p_spi->pcs & 3u) << LPSPI_MASTER_PCS_SHIFT) |
//     //         (p_spi->pcsContinuous ? kLPSPI_MasterPcsContinuous : 0);

//     LPSPI_MasterTransferBlocking(p_spi->p_spi_base, &transfer_handle);

// 	// NVIC_DisableIRQ(p_spi->irqn);
// 	// p_spi->bBusy = false;
// 	// NVIC_EnableIRQ(p_spi->irqn);

// 	return;
// }