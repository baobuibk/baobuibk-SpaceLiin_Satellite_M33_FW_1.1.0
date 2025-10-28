#ifndef _LWL_H_
#define _LWL_H_

/*
 * @brief Interface declaration of lwl module.
 *
 * See implementation file for information about this module.
 *
 * MIT License
 * 
 * Copyright (c) 2021 Eugene R Schroeder
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Common macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Public (global) externs
////////////////////////////////////////////////////////////////////////////////

// Following variable is global to allow efficient access by macros,
// but it is considered private.
// Macros for logging
#define LWL_1(a) (uint32_t)(a)
#define LWL_2(a) (uint32_t)(a), (uint32_t)(a) >> 8
#define LWL_3(a) (uint32_t)(a), (uint32_t)(a) >> 8, (uint32_t)(a) >> 16
#define LWL_4(a) (uint32_t)(a), (uint32_t)(a) >> 8, (uint32_t)(a) >> 16, (uint32_t)(a) >> 24


////////////////////////////////////////////////////////////////////////////////
// Public (global) function declarations
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    LWL_EXP_INVALID = 0,                  // ID 0: Reserved
    LWL_EXP_TIMESTAMP,                    // ID 1: System timestamp log
    LWL_EXP_BOARD_TEMP,
    LWL_EXP_TEMP_SINGLE_NTC,              // ID 2: Log single NTC value
    LWL_EXP_PROF_STARTED	,            // ID 4:profile started
    LWL_EXP_PROF_ERROR	,              	// ID 5: Profile error
	LWL_EXP_PROF_HEATING,                // ID 6: Cooling active
	LWL_EXP_PROF_STOP,                // ID 7: Cooling active

    LWL_EXP_START_PHOTO_MEASUREMENT,   // ID 8: start Photo-Laser measurement
    LWL_EXP_START_CAPTURE_IMAGE,   // ID 9: start Photo-Laser measurement

	LWL_EXP_SWITCH_ON_SOLENOID,
	LWL_EXP_SWITCH_OFF_SOLENOID,


} log_msg_id_t;


void lwl_start(void);

// Other APIs.
void lwl_rec(uint8_t id, int32_t num_arg_bytes, ...);
void lwl_enable(bool on);
void lwl_dump(void);
uint8_t* lwl_get_buffer(uint32_t* len);
void LWL(uint8_t id, ...) ;
// The special __COUNTER__ macro (not official C but supported by many
// compilers) is used to generate LWL IDs.

void lwl_buffer_full_notify(void);
void lwl_clear_notification(void);

uint16_t * lwl_get_full_buffer_addr(void);
uint32_t lwl_log_send_to_spi(void);
uint32_t lwl_transfer(void);
bool lwl_is_full(void);

#endif // _LWL_H_
