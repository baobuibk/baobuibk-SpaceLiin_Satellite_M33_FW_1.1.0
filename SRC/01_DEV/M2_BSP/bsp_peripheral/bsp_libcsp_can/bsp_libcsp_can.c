/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdio.h>

/* Freescale includes. */
#include "fsl_common.h"

/* Board Support includes. */
#include "bsp_board.h"
#include "bsp_libcsp_can.h"
#include "bsp_debug_console.h"

/* Component includes. */
#include "can_stdio.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)

#define CAN_FRAME_DLC (8)

#define RB_TX_BUFFER_SIZE  2048
#define RB_RX_BUFFER_SIZE  64

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the transmit buffer.
//
//*****************************************************************************
#define TX_BUFFER_SIZE(p_can)              ((p_can)->max_items)

#define TX_BUFFER_USED(p_can)              (ring_buffer_get_buffer_count((p_can)->p_TX_buffer))

#define TX_BUFFER_FREE(p_can)              (ring_buffer_get_free_space((p_can)->p_TX_buffer))

#define TX_BUFFER_EMPTY(p_can)             (ring_buffer_is_empty((p_can)->p_TX_buffer))

#define TX_BUFFER_FULL(p_can)              (ring_buffer_is_full((p_can)->p_TX_buffer))

#define TX_BUFFER_PUSH(p_can, p_buff)      (ring_buffer_push((p_can)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_POP(p_can, p_buff)       (ring_buffer_pop((p_can)->p_TX_buffer, (void*)(p_buff)))

#define TX_BUFFER_OVERWRITE(p_can, p_buff) (ring_buffer_overwrite((p_can)->p_TX_buffer, (p_buff)))

#define TX_BUFFER_PEAK(p_can, p_buff)      (ring_buffer_peak((p_can)->p_TX_buffer, (p_buff)))

//*****************************************************************************
//
// Macros to determine number of free and used bytes in the receive buffer.
//
//*****************************************************************************
#define RX_BUFFER_SIZE(p_can)              ((p_can)->max_items)

#define RX_BUFFER_USED(p_can)              (ring_buffer_get_buffer_count((p_can)->p_RX_buffer))

#define RX_BUFFER_FREE(p_can)              (ring_buffer_get_free_space((p_can)->p_RX_buffer))

#define RX_BUFFER_EMPTY(p_can)             (ring_buffer_is_empty((p_can)->p_RX_buffer))

#define RX_BUFFER_FULL(p_can)              (ring_buffer_is_full((p_can)->p_RX_buffer))

#define RX_BUFFER_PUSH(p_can, p_buff)      (ring_buffer_push((p_can)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_POP(p_can, p_buff)       (ring_buffer_pop((p_can)->p_RX_buffer, (void*)(p_buff)))

#define RX_BUFFER_OVERWRITE(p_can, p_buff) (ring_buffer_overwrite((p_can)->p_RX_buffer, (p_buff)))

#define RX_BUFFER_PEAK(p_can, p_buff)      (ring_buffer_peak((p_can)->p_RX_buffer, (p_buff)))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
flexcan_handle_t        libcsp_can_handle;
flexcan_mb_transfer_t   TX_mb_handle, RX_mb_handle;

flexcan_frame_t         TX_frame, RX_frame;

can_stdio_t     LIBCSP_CAN_stdio;

ring_buffer_t   LIBCSP_CAN_TX_ring_buffer;
flexcan_frame_t LIBCSP_CAN_TX_buffer[RB_TX_BUFFER_SIZE];

ring_buffer_t   LIBCSP_CAN_RX_ring_buffer;
flexcan_frame_t LIBCSP_CAN_RX_buffer[RB_RX_BUFFER_SIZE];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static FLEXCAN_CALLBACK(libcsp_can_callback);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//*****************************************************************************
//
//! Init the bsp libcsp can
//
//*****************************************************************************
void bsp_libcsp_can_init()
{
    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(LIBCSP_CAN_BASE, &libcsp_can_handle, libcsp_can_callback, NULL);

    /* Set RX Message Buffer Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(LIBCSP_CAN_BASE, FLEXCAN_RX_MB_STD_MASK(CAN_RX_IDENTIFIER, 0, 0));

    /* Config RX Message Buffer. */
    flexcan_rx_mb_config_t RX_mb_Config;
    RX_mb_Config.format = kFLEXCAN_FrameFormatStandard;
    RX_mb_Config.type   = kFLEXCAN_FrameTypeData;
    RX_mb_Config.id     = FLEXCAN_ID_STD(CAN_RX_IDENTIFIER);

    FLEXCAN_SetRxMbConfig(LIBCSP_CAN_BASE, RX_MESSAGE_BUFFER_NUM, &RX_mb_Config, true);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(LIBCSP_CAN_BASE, TX_MESSAGE_BUFFER_NUM, true);

    /* Setup TX and RX frame for CAN TX and RX */
    TX_frame.id     = FLEXCAN_ID_STD(CAN_TX_IDENTIFIER);
    TX_frame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    TX_frame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    TX_frame.length = (uint8_t)CAN_FRAME_DLC;

    RX_frame.id     = FLEXCAN_ID_STD(CAN_RX_IDENTIFIER);
    RX_frame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    RX_frame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    RX_frame.length = (uint8_t)CAN_FRAME_DLC;

    /* Setup TX and RX mailbox handle */
    TX_mb_handle.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    TX_mb_handle.frame = &TX_frame;

    RX_mb_handle.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
    RX_mb_handle.frame = &RX_frame;

    ring_buffer_init(&LIBCSP_CAN_TX_ring_buffer, (uint8_t*)LIBCSP_CAN_TX_buffer, (uint32_t)sizeof(LIBCSP_CAN_TX_buffer), RB_TX_BUFFER_SIZE, (uint32_t)sizeof(flexcan_frame_t));
    ring_buffer_init(&LIBCSP_CAN_RX_ring_buffer, (uint8_t*)LIBCSP_CAN_RX_buffer, (uint32_t)sizeof(LIBCSP_CAN_RX_buffer), RB_RX_BUFFER_SIZE, (uint32_t)sizeof(flexcan_frame_t));

    CAN_stdio_Init(
                &LIBCSP_CAN_stdio,
                LIBCSP_CAN_BASE,
                LIBCSP_CAN_IRQn,
                &libcsp_can_handle,
                &TX_mb_handle,
                &RX_mb_handle,
                &LIBCSP_CAN_TX_ring_buffer,
                &LIBCSP_CAN_RX_ring_buffer);
}

//*****************************************************************************
//
//! LIBCSP CAN IRQHandler
//
//*****************************************************************************
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(libcsp_can_callback)
{
    switch (status)
    {
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                CAN_stdio_TX_idle_subhandle(&LIBCSP_CAN_stdio);
            }
            break;

        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                CAN_stdio_RX_idle_subhandle(&LIBCSP_CAN_stdio);
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            ;
            break;

        default:
            break;
    }
}

//*****************************************************************************
//
//! Send a char to the debug console.
//
//*****************************************************************************
void bsp_libcsp_can_send_frame(const flexcan_frame_t* p_frame)
{
	CAN_Send_Frame(&LIBCSP_CAN_stdio, p_frame);
}

//*****************************************************************************
//
//! Send the string to the debug console.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//
//*****************************************************************************
void bsp_libcsp_can_send_buffer(const flexcan_frame_t *p_buff, uint32_t frame_count)
{
	CAN_Send_Buffer(&LIBCSP_CAN_stdio, p_buff, frame_count);
}

//*****************************************************************************
//
//! Get a char from the buffer.
//
//*****************************************************************************
void bsp_libcsp_can_get_frame(flexcan_frame_t* p_return_frame)
{
	CAN_Get_Frame(&LIBCSP_CAN_stdio, p_return_frame);
}

//*****************************************************************************
//
//! Check wheter RX buffer empty or not.
//
//*****************************************************************************
bool bsp_libcsp_RX_buffer_empty(void)
{
	return RX_BUFFER_EMPTY(&LIBCSP_CAN_stdio);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
