// #ifndef _BSP_LIBCSP_CAN_H_
// #define _BSP_LIBCSP_CAN_H_

// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// #include <stdarg.h>
// #include <stdbool.h>

// #include "fsl_flexcan.h"

// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// #define CAN_TX_IDENTIFIER   0x123
// #define CAN_RX_IDENTIFIER   0x321

// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// void bsp_libcsp_can_init();

// void bsp_libcsp_can_send_frame(const flexcan_frame_t* p_frame);

// void bsp_libcsp_can_send_buffer(const flexcan_frame_t* p_buff, uint32_t frame_count);

// void bsp_libcsp_can_get_frame(flexcan_frame_t* p_return_frame);

// bool bsp_libcsp_RX_buffer_empty(void);

// #endif /* _BSP_LIBCSP_CAN_H_ */
