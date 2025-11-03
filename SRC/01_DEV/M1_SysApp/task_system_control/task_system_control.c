#include "task_system_control.h"
#include "m33_data.h"
#include "FreeRTOS.h"
#include "osSemphr.h"
#include "printf.h"
#include "simple_shell.h"
#include "rpmsg/m33_rpmsg.h"
#include "Interface/RemoteCall/remote_call.h"
#include "fsl_debug_console.h"
#include "rtos.h"
#include "lwl.h"
#include "task_update_onboard_adc.h"
#include "bsp_onboard_adc.h"
#include "app_temperature.h"

extern QueueHandle_t experiment_command_queue;
extern QueueHandle_t remote_message_queue;
extern osSemaphore rptx_ram_mutex;
#define REPORT_INTERVAL 10  // report every 60 seconds
extern int16_t NTC_temp_C[NTC_CHANNEL_NUM];
void task_system_control()
{
    // Create system control task here

    PRINTF("===== [System Control Started] =====\r\n");

    while (1)
    {
        vTaskDelay(2000);
        if (0 == m33_get_ADC_status()) 
        {
            PRINTF("Use task_temperature_control_profile_type0\r\n");
            task_temperature_control_profile_type0();
        }
        else{
            PRINTF("Use task_temperature_control_use_bmp390\r\n");
            task_temperature_control_use_bmp390();
        }
        
    }
}


