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


extern osSemaphore exp_task_sem;
extern QueueHandle_t remote_message_queue;
#define REPORT_INTERVAL 10  // report every 60 seconds

void task_system_control()
{
    // Create system control task here
       uint16_t is_start_exp = 0;
    uint16_t exp_remain_time = 0;
    uint8_t  local_counter = 0;
    int16_t board_temperature = 0;
    char msg_buf[256];
    remote_message_t message;
    PRINTF("===== [System COntrol Started] =====\r\n");

    TickType_t xLastWakeTime;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        vTaskDelayUntil( &xLastWakeTime, 1000 );   //update system and collect data every 1 second
        local_counter++;
        //check if experiment is enabled
        m33_data_get_u_lock(TABLE_ID_5, exp_mon_start, &is_start_exp);
        m33_data_get_u_lock(TABLE_ID_5, exp_mon_delay, &exp_remain_time);
        if (is_start_exp == 1)  // experiment started                     
        {
            if (exp_remain_time > 0)    // still in delay period
            {
                exp_remain_time--;
                m33_data_set_u_lock(TABLE_ID_5, exp_mon_delay, exp_remain_time);
            }
            else    // delay period finished, reset delay time for next cycle, trigger experiment task
            {
                m33_data_get_u_lock(TABLE_ID_5, exp_mon_interval, &exp_remain_time);
                //m33_data_set_u_lock(TABLE_ID_5, exp_mon_delay, exp_remain_time);
                m33_data_set_u_lock(TABLE_ID_5, exp_mon_delay, 3600);
                //notify experiment task to start experiment
                osSemaphoreGiven(&exp_task_sem);
            }
        }
        if (local_counter >= REPORT_INTERVAL)
        {
            local_counter = 0;
            int16_t NTC_temps[12];
            m33_data_ntc_temp_get(NTC_temps);
            m33_data_get_i_lock(TABLE_ID_6, temp_board, &board_temperature);
            message.address = temp_exp + 0x0600;
            message.data = board_temperature;
            xQueueSend(remote_message_queue, &message, (TickType_t)0);

            for (int i = 0; i < 12; i++)   
            {
                message.address = temp_ntc_0 + 0x0600 + i;
                message.data = NTC_temps[i];
                xQueueSend(remote_message_queue, &message, (TickType_t)0);         
            }

            PRINTF("\r\n[task_system_control] sent to remote messae tx task\r\n", msg_buf);
        }
    }
}
