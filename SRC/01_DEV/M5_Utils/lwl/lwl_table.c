#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "lwl.h"



////////////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////////////

// Structure for log message metadata
struct lwl_msg_full {
    uint32_t lwlID;
    const char *fmt;       // Format string
    uint8_t num_arg_bytes; // Total number of argument bytes
};

// For writing to flash, this structure needs to be a multiple of 8 bytes.
struct lwl_data_buffer {
    uint32_t put_idx;
    uint8_t *p_buf;
}lwl_data_buffer_t;

typedef struct lwl_t{
	struct lwl_data_buffer lwl_data_buf      ;
	uint16_t	 lwl_buf_over_threshold;
}lwl_t;



// Log message table (ID is the index)
static const struct lwl_msg_full lwl_msg_table[LWL_MAX_NUM] = {
    {LWL_EXP_INVALID, NULL, 0},                                       // ID 0: EXP_INVALID
    {LWL_EXP_TIMESTAMP, "Time: epoch %4d", 4},               // ID 1: EXP_TIMESTAMP
    {LWL_EXP_BOARD_TEMP,"Temperature: Board temperature: %2d", 2},               // ID 2: EXP_TEMP_SINGLE_NTC
    {LWL_EXP_TEMP_SINGLE_NTC,"Temperature: NTC[%1d]: %2d", 3},               // ID 2: EXP_TEMP_SINGLE_NTC

	{LWL_EXP_PROF_STARTED,"Temperature: Profile[%1d] started, pri_NTC = %1d sec_NTC=%1d setpoint = %2d", 5},                 // ID 4: EXP_TEMP_MANUAL_MODE
    {LWL_EXP_PROF_ERROR, "Temperature: Profile[%1d] ERROR pri_temperature=%2d sec_temperature = %2d", 5},                   // ID 5: EXP_TEMP_AUTO_MODE
    {LWL_EXP_PROF_HEATING,"Temperature: Profile[%1d] enter HEATING pri_temperature=%2d sec_temperature = %2d", 5},                   // ID 5: EXP_TEMP_AUTO_MODE
    {LWL_EXP_PROF_STOP,"Temperature: Profile[%1d] enter STOP mode", 1},                   // ID 5: EXP_TEMP_AUTO_MODE

   { LWL_EXP_START_PHOTO_MEASUREMENT,"Experiment: Start measurement position %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
    {LWL_EXP_START_CAPTURE_IMAGE,"Experiment: Start capturing image", 0}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE

    {LWL_EXP_SWITCH_ON_SOLENOID,"Experiment: Switch on solenoid %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
    {LWL_EXP_SWITCH_OFF_SOLENOID,"Experiment: Switch off solenoid %1d", 1}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
   { LWL_EXP_TURN_ON_PUMP,"Experiment: Turn on Pump ", 0}, // ID 9: EXP_TEMP_TEC_OVERRIDE_PROFILE
    {LWL_EXP_TURN_OFF_PUMP,"Experiment: Turn off Pump ", 0},
    {FLOW_VALUE,"Experiment: Flow Value %4d ", 4},
    {LWL_EXP_SWITCH_VALVE,"Experiment: Valve swwitch to %1d",1},
   {LWL_EXP_PUMP_SET_FREQ, "Experiment: pump set freq to %2d",2},
    {LWL_EXP_PUMP_SET_VOLT,"Experiment: pump set volt to %2d",2},
    {LWL_EXP_PUMP_FLOW_TEMP,"Experiment: Pump flow = %4d temp =%4d",8},
    {LWL_EXP_PUMP_OFF,"Experiment: Pump off",0}
   
};




