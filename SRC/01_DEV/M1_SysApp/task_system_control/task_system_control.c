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
#define REPORT_INTERVAL 5  // report every 60 seconds

void task_system_control(void *pvParameters)
{
    // Create system control task here
       uint16_t is_start_exp = 0;
    uint16_t exp_remain_time = 0;
    uint8_t  local_counter = 0;
    int16_t board_temperature = 0;
    char msg_buf[256];
    PRINTF("===== [System COntrol Started] =====\r\n");
    
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));    //update system and collect data every 1 second
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
                m33_data_set_u_lock(TABLE_ID_5, exp_mon_delay, exp_remain_time);
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

        snprintf(msg_buf, 256, "update_param 0x%03X=%d\r\n", temp_exp + 0x0600 , (unsigned int) board_temperature);
        rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);

        for (int i = 0; i < 12; i++)   
        {
        snprintf(msg_buf, 256, "update_param 0x%03X=%d\r\n", temp_ntc_0 + 0x0600 + i, (unsigned int) NTC_temps[i]);
        rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);
        PRINTF("%s", msg_buf);
        }

        PRINTF("%s", msg_buf);
        }
    }
}
