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
#include "semphr.h"
#include "Services/cmdline.h"
#include "Interface/SimpleShell/simple_shell.h"
#include "fsl_debug_console.h"
#include "rpmsg/m33_rpmsg.h"
#include "Interface/RemoteCall/remote_call.h"
#include "bsp_core.h"
#include "task_system_control.h"
#include "osSemphr.h"
#include "bsp.h"
#include "task_system_control.h"
#include "m33_data.h"
#include "m33_command.h"
#include "rtos.h"
#include "task_experiment.h"
#include "task_update_onboard_adc.h"
#include "printf.h"
#include "remote_call.h"
#include "app_temperature.h"

#define CREATE_TASK(task_func, task_name, stack, param, priority, handle) \
    if (xTaskCreate(task_func, task_name, stack, param, priority, handle) != pdPASS) { \
        return E_ERROR; \
    }

#define MIN_STACK_SIZE	configMINIMAL_STACK_SIZE
#define ROOT_PRIORITY   3
#define ROOT_STACK_SIZE (configMINIMAL_STACK_SIZE * 3)

static StackType_t root_stack[ROOT_STACK_SIZE];
static StaticTask_t root_tcb;

Std_ReturnType EXP_AppInit(void);

static char app_buf[512];
uint32_t remote_addr;

/*************************************************
 *               TASK DEFINE                     *
 *************************************************/
void EXP_RootTask(void *pvParameters);

static void RPMSG_Task(void *pvParameters);
static void RPMSG_Tx_Task(void *pvParameters);

/*************************************************
 *               	Root Task	                 *
 *************************************************/
void EXP_RootTask(void *pvParameters)
{
    PRINTF("===== i.MX93 Shell started =====\r\n");

    if (EXP_AppInit() != E_OK)
    {

    }
PRINTF("===== start controlling =====\r\n");
    task_system_control();

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
QueueHandle_t experiment_command_queue = NULL;
osSemaphore rptx_mutex = NULL;    //used for rpmsg_send command
osSemaphore command_sem = NULL;
QueueHandle_t remote_message_queue = NULL;
osSemaphore rptx_ram_mutex = NULL;    //mutex to take over the RAM to copy




static void EXP_App_Create_Communication_Queues(void)
{
    // Create communication queues here
    command_sem = xSemaphoreCreateBinary();
    experiment_command_queue = xQueueCreate(6, sizeof(uint16_t));

    rptx_mutex = xSemaphoreCreateMutex();
    rptx_ram_mutex = xSemaphoreCreateMutex();
  remote_message_queue = xQueueCreate(30, sizeof(remote_message_t));

}
Std_ReturnType EXP_AppInit(void)
{

	Std_ReturnType ret = E_ERROR;
    EXP_App_Create_Communication_Queues();
    m33_data_init();
    BSP_Init();   

    PRINTF("growing task\r\n");
    CREATE_TASK(RPMSG_Tx_Task, 		"RPMSG_Tx_Task", 		MIN_STACK_SIZE * 3, 	NULL, 	2, NULL);
    CREATE_TASK(RPMSG_Task, 		"RPMSGTask", 		MIN_STACK_SIZE * 3, 	NULL, 	1, NULL);
    CREATE_TASK(Task_Experiment, 		"Task_Experiment", 		MIN_STACK_SIZE * 5, 	NULL, 	2, NULL);
  // CREATE_TASK(task_temperature_control_profile_type0, 		"task_temperature_control_profile_type0", 		MIN_STACK_SIZE * 5, 	NULL, 	2, NULL);

    // CREATE_TASK(Task_Update_Onboard_ADC, 		"Task_Update_Onboard_ADC", 		MIN_STACK_SIZE *4, 	NULL, 	2, NULL);


    ret = E_OK;
    return ret;
}

/*************************************************
 *               TASK LIST                       *
 *************************************************/


static void RPMSG_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(100));   
    void *rx_buf;
    uint32_t len;

    uint8_t *payload;
    uint32_t payload_len;
    uint8_t target, type;
    uint16_t msg_len;

    PRINTF("\r\n[rpmsg] M33 RPMSG Init [v102]\r\n");


    M33_RPMSG_WaitForLinkUp();

    if (M33_RPMSG_CreateEndpoint(LOCAL_EPT_ADDR) != E_OK)
    {
        PRINTF("[rpmsg] M33_RPMSG_CreateEndpoint failed\r\n");
        M33_RPMSG_Deinit();
        vTaskDelete(NULL);
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

            PRINTF("\r\n");
            PRINTF("\r\nrpmsg] send to shell task");
             PRINTF("\r\n[rpmsg] payload_length = %d \r\n",payload_len);

           char * papp_buff = app_buf;
           while ((0x0A == (*papp_buff)) || (0x0D == (*papp_buff))) papp_buff++;
           if (0 != (*papp_buff)) Command_Process(papp_buff);
           
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

/***************************************
 * Task send telemetry
 * 
 ************************************************/

static void RPMSG_Tx_Task(void *pvParameters)
{
   remote_message_t message;
   uint32_t epoch = 0;

    PRINTF("[RPMSG_Tx_Task] Started\r\n");
    char msg_buf[100] = {0};

    while (1)
    {
         vTaskDelay(300);
        if( xQueueReceive( remote_message_queue,
                           &( message ),
                           ( TickType_t ) 0 ) != pdPASS )
        {
            continue;
        }
        //process the message
        uint32_t msg_type = (message.address & 0xF000);
        
        m33_data_get_epoch_lock(&epoch);
        
        switch (msg_type)
        {
            case UPDATE_PARAM:
                snprintf(msg_buf, 100, "update_param 0x%03X=%u\r\n",(message.address & 0x0FFF),(uint16_t)message.data);
                rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);
                break;
            case DLS_DATA:
                snprintf(msg_buf, 100, "daily_PLDD_%d.dat\r\n", epoch);
                PRINTF("[ RPMSG_Tx_Task]file request with size = %d\r\n",message.data);
                RemoteCall_SendFileRequest(message.data,msg_buf);
                break;
            case TEST_LASER_DATA:
                snprintf(msg_buf, 100, "oneshot_TLPD_%d.dat\r\n", epoch);
                 RemoteCall_SendFileRequest(message.data,msg_buf);

                break;
            case TEST_PUMP_DATA:
                snprintf(msg_buf, 100, "oneshot_TLSR_%d.dat\r\n", epoch);
                RemoteCall_SendFileRequest(message.data,msg_buf);
                break;
            case CAM_CAPTURE:
                snprintf(msg_buf, 100, "capture %d\r\n",message.data);
                rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);
                break;

            case SYS_LOG:
                
                    snprintf(msg_buf, 100, "daily_SYSL_%d.DAT\r\n",epoch);
                    PRINTF("[ RPMSG_Tx_Task]file request with size = %d\r\n",message.data);
                    RemoteCall_SendFileRequest(message.data,msg_buf);

//                rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);
                break;
            default:
                break;
        }
        PRINTF("\r\n[RPMSG_Tx_Task] sent %s\r\n",msg_buf);


    }
}

/*************************************************
 *                    Helper                     *
 *************************************************/

 uint32_t rpmsg_send(uint32_t msg_type, const char *msg)
 {
    int sem_ret = osSemaphoreTake(&rptx_mutex, RPMSG_WAIT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    switch (msg_type)
    {
        case RPMSG_MSG_UPDATE_PARAM:
            RemoteCall_SendCommand(msg);
            break;
        case RPMSG_MSG_CAPTURE:
            RemoteCall_SendCommand(msg);
            break;
        case RPMSG_MSG_RESPONSE_OK:
            RemoteCall_SendResponse("OK\r");
        default:
            break;
    }
    osSemaphoreGiven(&rptx_mutex);
    return sem_ret;
    
 }