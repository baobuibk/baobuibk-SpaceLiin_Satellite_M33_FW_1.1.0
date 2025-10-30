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

extern QueueHandle_t experiment_command_queue;
extern QueueHandle_t remote_message_queue;
extern osSemaphore rptx_ram_mutex;
#define REPORT_INTERVAL 10  // report every 60 seconds
extern int16_t NTC_temp_C[NTC_CHANNEL_NUM];
void task_system_control()
{
    // Create system control task here
       uint16_t is_start_exp = 0;
    uint16_t exp_remain_time = 0;
    uint8_t  local_counter = 0;
    int16_t board_temperature = 0;
    uint16_t command;
    char msg_buf[256];
    uint32_t epoch ;
    remote_message_t message;
    PRINTF("===== [System COntrol Started] =====\r\n");

    TickType_t xLastWakeTime;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        vTaskDelayUntil( &xLastWakeTime, 1000 );   //update system and collect data every 1 second

        m33_data_get_epoch_lock(&epoch);
        epoch ++;
        m33_data_set_epoch_lock(epoch);

    //    ADC_update();
        local_counter++;
        //check if log is full
        if (lwl_is_sys_log_full())
        {
            xSemaphoreTake(rptx_ram_mutex, portMAX_DELAY);
            uint32_t length = lwl_sys_log_transfer();

            message.address = SYS_LOG;
            message.data = length;
           // xQueueSendToFront(remote_message_queue, &message, 1000);//send notification for log
            xQueueSend(remote_message_queue, &message, 1000);//send notification for log
            vTaskDelay(2000); //make sure data is read
            xSemaphoreGive(rptx_ram_mutex);
            PRINTF("\r\n[task_system_control] sent syslog notìication\r\n", msg_buf);

        }
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
            command = SLD_RUN;
            xQueueSend(experiment_command_queue, &command, 100);
            }
        }
        //check if test laser is enabled
        m33_data_get_u_lock(TABLE_ID_5, test_ls_current, &is_start_exp);
        if (1 == is_start_exp)
        {
            command = LASER_TEST;
            xQueueSend(experiment_command_queue, &command, 100);//send notification for log
            m33_data_set_u_lock(TABLE_ID_5, test_ls_current, 0);
        }
        //check if test pump is enabled
        m33_data_get_u_lock(TABLE_ID_5, test_fluidic_seq, &is_start_exp);
        if (1 == is_start_exp)
        {
            command = FLUIDIC_TEST;
            xQueueSend(experiment_command_queue, &command, 100);//send notification for log
            m33_data_set_u_lock(TABLE_ID_5, test_fluidic_seq, 0);
        }

        m33_data_get_u_lock(TABLE_ID_5, exp_fluidic_seq, &is_start_exp);
        if (1 == is_start_exp)
        {
            command = FLUIDIC_SEQ;
            xQueueSend(experiment_command_queue, &command, 100);//send notification for log
            m33_data_set_u_lock(TABLE_ID_5, exp_fluidic_seq, 0);
        }

        if (local_counter >= REPORT_INTERVAL)
        {
            
            m33_data_get_epoch_lock(&epoch);
            //log time
            LWL_SYS_LOG(LWL_EXP_TIMESTAMP, LWL_4(epoch));

            local_counter = 0;

            //m33_data_ntc_temp_get(NTC_temps);
            m33_data_get_i_lock(TABLE_ID_6, temp_board, &board_temperature);
            message.address = temp_exp + 0x0600;
            message.data = board_temperature;


            xQueueSend(remote_message_queue, &message, (TickType_t)0);
            for (int i = 0; i < 12; i++)   
            {
                 message.address = 3 +  0x0600 + i;
                 message.data =  NTC_temp_C[i];

                xQueueSend(remote_message_queue, &message, (TickType_t)0);  
                PRINTF("NTC value [i]= %d\r\n", NTC_temp_C[i])   ;    
            }
            PRINTF("\r\n[task_system_control] sent to remote messae tx task\r\n", msg_buf);
        }
    }
}

