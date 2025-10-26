/************************************************
 *  @file     : rtos_task.c
 *  @author   : CAO HIEU
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/
#include "rtos_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Services/cmdline.h"
#include "Interface/SimpleShell/simple_shell.h"
#include "fsl_debug_console.h"
#include "rpmsg/m33_rpmsg.h"
#include "Interface/RemoteCall/remote_call.h"
#include "task_update_onboard_adc.h"
#include  "app_temperature.h"
#include "task_experiment.h"
#include "bsp_core.h"
#include "m33_data.h"
#include "bsp.h"
#include "printf.h"

#define CREATE_TASK(task_func, task_name, stack, param, priority, handle) \
    if (xTaskCreate(task_func, task_name, stack, param, priority, handle) != pdPASS) { \
        return E_ERROR; \
    }

#define MIN_STACK_SIZE	configMINIMAL_STACK_SIZE
#define ROOT_PRIORITY   1
#define ROOT_STACK_SIZE (configMINIMAL_STACK_SIZE * 20)
#define REPORT_INTERVAL 5

static StackType_t root_stack[ROOT_STACK_SIZE];
static StaticTask_t root_tcb;

Std_ReturnType EXP_AppInit(void);

static char app_buf[512];
uint32_t remote_addr;

osSemaphore exp_task_sem;

/*************************************************
 *               TASK DEFINE                     *
 *************************************************/
void EXP_RootTask(void *pvParameters);

static void Shell_Task(void *pvParameters);
static void RPMSG_Task(void *pvParameters);

/*************************************************
 *               	Root Task	                 *
 *************************************************/
void EXP_RootTask(void *pvParameters)
{
    PRINTF("===== i.MX93 Shell started =====\r\n");
    bsp_core_init();
    m33_data_init();
            spi_io_init(&onboard_adc_spi);
        spi_io_init(&photo_adc_spi);

        i2c_io_init(&io_expander_i2c);

        /* Init board peripheral. */
        bsp_debug_console_init();
        bsp_expander_init();
        bsp_heater_init();
        bsp_onboard_adc_init();
        bsp_laser_init();
        bsp_photo_init();

        // Pull up RAM SPI nCS
        bsp_expander_ctrl(RAM_SPI_nCS, 1);
        osSemaphoreCreate(&exp_task_sem);   //semaphore to notify experiment task to start experiment

    if (EXP_AppInit() != E_OK)
    {

    }
    PRINTF("===== i.MX93 Shell started SUCCESSFULLY=====\r\n");
    uint16_t is_start_exp = 0;
    uint16_t exp_remain_time = 0;
    uint8_t  local_counter = 0;
    int16_t board_temperature = 0;
    char msg_buf[256];
    
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

        snprintf(msg_buf, 256,
        "NTC1: %04X, NTC2: %04X, NTC3: %04X, NTC4: %04X, "
        "NTC5: %04X, NTC6: %04X, NTC7: %04X, NTC8: %04X, "
        "NTC9: %04X, NTC10: %04X, NTC11: %04X, NTC12: %04X\r\n",
        (unsigned int) NTC_temps[0], (unsigned int) NTC_temps[1], (unsigned int) NTC_temps[2], (unsigned int) NTC_temps[3],
        (unsigned int) NTC_temps[4], (unsigned int) NTC_temps[5], (unsigned int) NTC_temps[6], (unsigned int) NTC_temps[7],
        (unsigned int) NTC_temps[8], (unsigned int) NTC_temps[9], (unsigned int) NTC_temps[10], (unsigned int) NTC_temps[11]);
       
        RemoteCall_SendCommand(msg_buf);
        PRINTF("%s", msg_buf);
        snprintf(msg_buf, 256, "BoardTemp: %04X", board_temperature);
        RemoteCall_SendCommand(msg_buf);
        PRINTF("%s", msg_buf);
        }
    }

    vTaskDelete(NULL);
    while(1){

    }
}

void EXP_RootGrowUp(void)
{
    TaskHandle_t task_handle = xTaskCreateStatic(
        EXP_RootTask,
        "EXP_RootTask",
        ROOT_STACK_SIZE,
        NULL,
        ROOT_PRIORITY,
        root_stack,
        &root_tcb
    );

    if (task_handle == NULL)
    {
        while (1);
    }

    vTaskStartScheduler();
}

/*************************************************
 *               	TASK INIT	                 *
 *************************************************/

Std_ReturnType EXP_AppInit(void)
{

	Std_ReturnType ret = E_ERROR;

    CREATE_TASK(Shell_Task, 		"ShellTask", 		MIN_STACK_SIZE * 2, 	NULL, 	1, NULL);

    CREATE_TASK(RPMSG_Task, 		"RPMSGTask", 		MIN_STACK_SIZE * 10, 	NULL, 	1, NULL);
    CREATE_TASK(Task_Update_Onboard_ADC, 		"Task_Update_Onboard_ADC", 		MIN_STACK_SIZE * 2, 	NULL, 	1, NULL);
     CREATE_TASK(task_temperature_control_profile_type0, 		"task_temperature_control_profile_type0", 		MIN_STACK_SIZE * 2, 	NULL, 	1, NULL);

     CREATE_TASK(Task_Experiment, 		"Task_Experiment", 		MIN_STACK_SIZE * 2, 	NULL, 	1, NULL);

    ret = E_OK;
    return ret;
}

/*************************************************
 *               TASK LIST                       *
 *************************************************/
static void Shell_Task(void *pvParameters)
{
    setup_dynamic_shell();

    while (1)
    {
        // char c = GETCHAR();     
        // Shell_ReceiveChar(c);   
        vTaskDelay(pdMS_TO_TICKS(5000));   
        Shell_WriteString("Sending ping to A55...\r\n");
        if (RemoteCall_SendCommand("a55_ping\n") == E_OK)
        {
            Shell_WriteString("Ping sent\r\n");
        }
    }
}

static void RPMSG_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(100));   
    void *rx_buf;
    uint32_t len;
    // void *tx_buf;
    // uint32_t tx_size;
    // struct cmd_header *hdr;
    uint8_t *payload;
    uint32_t payload_len;
    uint8_t target, type;
    uint16_t msg_len;

    PRINTF("\r\n[rpmsg] M33 RPMSG Init [v102]\r\n");

    // if (M33_RPMSG_Init() != E_OK)
    // {
    //     PRINTF("[rpmsg] M33_RPMSG_Init failed\r\n");
    //     vTaskDelete(NULL);
    //     return;
    // }

    M33_RPMSG_WaitForLinkUp();

    if (M33_RPMSG_CreateEndpoint(LOCAL_EPT_ADDR) != E_OK)
    {
        PRINTF("[rpmsg] M33_RPMSG_CreateEndpoint failed\r\n");
        M33_RPMSG_Deinit();
        // vTaskDelete(NULL);
        return;
    }

    
    PRINTF("[rpmsg] Ready for incoming messages...\r\n");

    while (1)
    {

        if (M33_RPMSG_ReceiveNoCopy(&remote_addr, &rx_buf, &len) != E_OK)
        {
            PRINTF("[rpmsg] Wrong! \r\n");
            continue;
        }

        RemoteCall_UpdateRemoteAddr(remote_addr);

        if (len < sizeof(struct cmd_header))
        {
            PRINTF("[rpmsg] Invalid message (too short)\r\n");
            M33_RPMSG_ReleaseRxBuffer(rx_buf);
            continue;
        }

        payload = (uint8_t *)rx_buf;
        target   = payload[0];
        type     = payload[1];
        // uint8_t reserved = payload[2];
        msg_len  = payload[3] | (payload[4] << 8);

        if (target != REMOTE_CALL_MY_ADDR)
        {
            PRINTF("[rpmsg] Message for unknown target: 0x%02X\r\n", target);
            M33_RPMSG_ReleaseRxBuffer(rx_buf);
            continue;
        }
        
        if (len < 5 + msg_len)
        {
            PRINTF("[rpmsg] Message length mismatch (len=%lu, expected=%u)\r\n", len, 5 + msg_len);
            M33_RPMSG_ReleaseRxBuffer(rx_buf);
            continue;
        }

        payload_len = msg_len;
        if (payload_len >= sizeof(app_buf))
            payload_len = sizeof(app_buf) - 1;

        memcpy(app_buf, &payload[5], payload_len);
        app_buf[payload_len] = 0;

        if (type == CMD_TYPE_NORMAL_REQ)
        {
            PRINTF("\r\n[rpmsg] Command: \"%s\" [len: %u]\r\n", app_buf, (unsigned)payload_len);

            PRINTF("[rpmsg] Raw data:");
            for (uint32_t i = 0; i < payload_len; i++)
            {
                PRINTF(" 0x%02X", (unsigned char)app_buf[i]);
            }
            PRINTF("\r\n");

            for (uint32_t i = 0; i < payload_len; i++)
            {
                Shell_ReceiveChar(app_buf[i]);
            }
        
            // /* Build response with header */
            // if (M33_RPMSG_AllocTxBuffer(&tx_buf, &tx_size) != E_OK)
            // {
            //     M33_RPMSG_ReleaseRxBuffer(rx_buf);
            //     continue;
            // }

            // if (tx_size < sizeof(struct cmd_header) + payload_len)
            // {
            //     payload_len = tx_size - sizeof(struct cmd_header);
            // }

            // /* Response header */
            // struct cmd_header *resp_hdr = (struct cmd_header *)tx_buf;
            // resp_hdr->type = CMD_TYPE_NORMAL_RESP;
            // resp_hdr->reserved = 0;
            // resp_hdr->length = payload_len;

            // /* Response payload (echo back) */
            // memcpy((char *)tx_buf + sizeof(struct cmd_header), app_buf, payload_len);

            // if (M33_RPMSG_SendNoCopy(remote_addr, tx_buf, 
            //                          sizeof(struct cmd_header) + payload_len) != E_OK)
            // {
            //     PRINTF("[rpmsg] SendNoCopy failed\r\n");
            // }
        }
        else if (type == CMD_TYPE_FILE_RESP)
        {
            PRINTF("[rpmsg] File response received from A55\r\n");
        }
        else if (type == CMD_TYPE_FILE_REQ)
        {
            PRINTF("[rpmsg] File transfer request (not implemented)\r\n");
        }
        else
        {
            PRINTF("[rpmsg] Unknown message type: 0x%x\r\n", type);
        }

        M33_RPMSG_ReleaseRxBuffer(rx_buf);
    }
}

/*************************************************
 *                    Helper                     *
 *************************************************/
