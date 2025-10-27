#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "bsp_board.h"
#include "log.h"
#include "lwl.h"
#include "error_codes.h"
#include "m33_data.h"
#include "FreeRTOS.h"
#include "queue.h"


////////////////////////////////////////////////////////////////////////////////
// Common macros
////////////////////////////////////////////////////////////////////////////////

#define LWL_START_BYTE				0xAA // Start byte for each log record

#ifdef CONFIG_LWL_BUF_SIZE
    #define 	LWL_BUF_SIZE 		(CONFIG_LWL_BUF_SIZE)
 	 #define 	LWL_BUF_THRESHOLD 	(CONFIG_LWL_BUF_THRESHOLD)
#else
    #define LWL_BUF_SIZE 			256
	#define LWL_BUF_THRESHOLD 		(LWL_BUF_SIZE - 56)
#endif



////////////////////////////////////////////////////////////////////////////////
// CRC-8 calculation (using CRC-8-ATM polynomial 0x07)
////////////////////////////////////////////////////////////////////////////////
static const uint8_t crc8_table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

static uint8_t calculate_crc8(const uint8_t *data, uint32_t len) {
    uint8_t crc = 0x00; // Initial value
    for (uint32_t i = 0; i < len; i++) {
        crc = crc8_table[crc ^ data[i]];
    }
    return crc;
}

////////////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////////////

// Structure for log message metadata
struct lwl_msg {
    const char *fmt;       // Format string
    uint8_t num_arg_bytes; // Total number of argument bytes
};

// For writing to flash, this structure needs to be a multiple of 8 bytes.
struct lwl_data_buffer {
    uint32_t put_idx;
    uint8_t *p_buf;
};

typedef struct lwl_t{
	uint32_t lwl_working_buf_index;
	uint32_t lwl_full_buf_index;
	bool	 lwl_buf_over_threshold;
	struct lwl_data_buffer lwl_data_buf[2];
}lwl_t;

__attribute__((aligned(4))) static uint8_t lwl_data_buf_0[LWL_BUF_SIZE] = {0};
__attribute__((aligned(4))) static uint8_t lwl_data_buf_1[LWL_BUF_SIZE] = {0};

static lwl_t lwl = {
    .lwl_working_buf_index = 0,
    .lwl_full_buf_index = 1,
    .lwl_buf_over_threshold = false,
    .lwl_data_buf = {
        { .put_idx = 0, .p_buf = (uint8_t *)lwl_data_buf_0 },
        { .put_idx = 0, .p_buf = (uint8_t *)lwl_data_buf_1 }
    }
};

// Log message table (ID is the index)
static const struct lwl_msg lwl_msg_table[] = {
    {NULL, 0},                                       // ID 0: EXP_INVALID
    {"Time: epoch %4d", 4},               // ID 1: EXP_TIMESTAMP
    {"Temperature: NTC[%1d]: %2d", 3},               // ID 2: EXP_TEMP_SINGLE_NTC
    {"Temperature: Board temperature: %2d", 2},               // ID 2: EXP_TEMP_SINGLE_NTC

	{"Temperature: Profile[%1d] started, pri_NTC = %1d sec_NTC=%1d setpoint = %2d", 5},                 // ID 4: EXP_TEMP_MANUAL_MODE
    {"Temperature: Profile[%1d] ERROR pri_temperature=%2d sec_temperature = %2d", 5},                   // ID 5: EXP_TEMP_AUTO_MODE
    {"Temperature: Profile[%1d] enter HEATING pri_temperature=%2d sec_temperature = %2d", 5},                   // ID 5: EXP_TEMP_AUTO_MODE
    {"Temperature: Profile[%1d] enter STOP mode", 1},                   // ID 5: EXP_TEMP_AUTO_MODE

    {"Experiment: Start measurement position %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
    {"Experiment: Start capturing image", 0}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE

    {"Experiment: Switch on solenoid %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
    {"Experiment: Switch off solenoid %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE


};

static const uint8_t lwl_msg_table_size = sizeof(lwl_msg_table) / sizeof(lwl_msg_table[0]);



void lwl_init(lwl_t *lwl) {
    // Xóa toàn bộ cấu trúc về 0 bằng memset
    lwl_t lwl_init = {
    .lwl_working_buf_index = 0,
    .lwl_full_buf_index = 1,
    .lwl_buf_over_threshold = false,
    .lwl_data_buf = {
        { .put_idx = 0, .p_buf = (uint8_t *)lwl_data_buf_0 },
        { .put_idx = 0, .p_buf = (uint8_t *)lwl_data_buf_1 }
        }
    };
    *lwl = lwl_init;
    memset(lwl->lwl_data_buf[0].p_buf, 0, LWL_BUF_SIZE);
    memset(lwl->lwl_data_buf[1].p_buf, 0, LWL_BUF_SIZE);
}

void lwl_start() {
    // Xóa toàn bộ cấu trúc về 0 bằng memset
//	lwl_init(&lwl);
	lwl_clear_notification();
}



/*
 * @brief Record a lightweight log with start byte, length, and CRC.
 *
 * @param[in] id The log message ID (index into lwl_msg_table).
 * @param[in] ... Variable arguments, each 1 byte (from LWL_n macros).
 *
 * Each log record format: [START_BYTE (0xAA)][LENGTH][ID][ARG_BYTES][CRC]
 * LENGTH = 1 (length) + 1 (id) + num_arg_bytes + 1 (CRC)
 * CRC is calculated over [ID][ARG_BYTES]
 */
void LWL(uint8_t id, ...)
{
    va_list ap;
    uint32_t put_idx;

    struct lwl_data_buffer * lwl_data_buf = &lwl.lwl_data_buf[lwl.lwl_working_buf_index];

    // Validate ID
    if (id == 0 || id >= lwl_msg_table_size || lwl_msg_table[id].fmt == NULL) {
        return; // Invalid ID
    }

    const struct lwl_msg *msg = &lwl_msg_table[id];
    uint8_t length = 1 + 1 + msg->num_arg_bytes + 1; // length + id + args + CRC

    va_start(ap, id);


    put_idx = lwl_data_buf->put_idx % LWL_BUF_SIZE;
    lwl_data_buf->put_idx = (put_idx + length + 1) % LWL_BUF_SIZE; // +1 for START_BYTE

    // Write start byte
    *(lwl_data_buf->p_buf + put_idx) = LWL_START_BYTE;
    put_idx = (put_idx + 1) % LWL_BUF_SIZE;

    // Write length
    *(lwl_data_buf->p_buf + put_idx) = length;
    put_idx = (put_idx + 1) % LWL_BUF_SIZE;

    // Write ID
    *(lwl_data_buf->p_buf + put_idx) = id;
    uint8_t crc_data[1 + msg->num_arg_bytes]; // Buffer for CRC calculation
    crc_data[0] = id;
    put_idx = (put_idx + 1) % LWL_BUF_SIZE;

    // Write arguments and collect for CRC
    for (uint8_t i = 0; i < msg->num_arg_bytes; i++) {
        uint32_t arg = va_arg(ap, unsigned);
        *(lwl_data_buf->p_buf + put_idx) = (uint8_t)(arg & 0xFF);
        crc_data[i + 1] = *(lwl_data_buf->p_buf + put_idx);
        put_idx = (put_idx + 1) % LWL_BUF_SIZE;
    }

    // Calculate and write CRC
    uint8_t crc = calculate_crc8(crc_data, 1 + msg->num_arg_bytes);
    *(lwl_data_buf->p_buf + put_idx) = crc;

    if (lwl_data_buf->put_idx > LWL_BUF_THRESHOLD) 		//buffer nearly full
    {
    	lwl.lwl_buf_over_threshold = 1;

    	if (lwl.lwl_working_buf_index == 0)
		{
    		lwl.lwl_working_buf_index = 1;
    		lwl.lwl_full_buf_index = 0;
    		lwl.lwl_data_buf[1].put_idx = 0;
		}
    	else
    	{
    		lwl.lwl_working_buf_index = 0;
    		lwl.lwl_full_buf_index = 1;
    		lwl.lwl_data_buf[0].put_idx = 0;
    	}

    	lwl_buffer_full_notify();
    }
    va_end(ap);
}



// void __attribute__((weak)) lwl_buffer_full_notify()
// {

// }

void __attribute__((weak)) lwl_clear_notification()
{
	lwl.lwl_buf_over_threshold = 0;

}


void lwl_reinit_buffer(struct lwl_data_buffer * lwl_buffer)
{
//    memset(lwl->lwl_data_buf[0].p_buf, 0, LWL_BUF_SIZE);
//    memset(lwl->lwl_data_buf[1].p_buf, 0, LWL_BUF_SIZE);
}

uint16_t * lwl_get_full_buffer_addr(void)
{
	return (uint16_t *)lwl.lwl_data_buf[lwl.lwl_full_buf_index].p_buf;
}

uint32_t __attribute__((weak))  lwl_log_send(void)
{
	// Add log

	lwl_clear_notification();
	// Cấu hình địa chỉ buffer
	return ERROR_OK;
}


QueueHandle_t remote_message_queue; //handle to send message
void lwl_buffer_full_notify()
{
    remote_message_t message = {
        .address = SYS_LOG
    };
    // copy current lwl buffer to RAM

    //trigger write to A55
    xQueueSendToFront(remote_message_queue, &message, 1000);
}

bool lwl_is_full()
{
    return lwl.lwl_buf_over_threshold;
}
uint32_t lwl_transfer(void)
{
    if (!lwl.lwl_buf_over_threshold) return 0;
    
    uint8_t *addr = RAM_TEST_BASE ;
    uint32_t index = lwl.lwl_full_buf_index;
    
    if (index > 1) 
    {
        lwl_init(&lwl);
        return 0; //errror here, should reset the log
    }
    uint32_t length = lwl.lwl_data_buf[index].put_idx;
    memcpy(addr, lwl.lwl_data_buf[index].p_buf, length);
    lwl.lwl_buf_over_threshold = 0; //clear full status
     lwl.lwl_data_buf[index].put_idx = 0;
     return length;
}

/*
LWL(TIMESTAMP, LWL_1(days), LWL_1(hours), LWL_1(minutes), LWL_1(seconds));

*/