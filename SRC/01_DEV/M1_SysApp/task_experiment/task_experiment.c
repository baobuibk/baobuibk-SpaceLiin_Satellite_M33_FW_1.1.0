/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Component includes. */

/* USER include. */
#include "task_experiment.h"
#include "task_update_onboard_adc.h"
#include "m33_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_laser.h"
#include "bsp_photo.h"
#include "simple_shell.h"
#include "fsl_debug_console.h"
#include "bsp_expander.h"
#include "bsp_pump.h"
#include "bsp_solenoid.h"
#include "lwl.h"

/* Driver inlcudes */
#include "slf3s_flow.h"
#include "bsp_i2c_sensor.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define EXP_SEM_TIMEOUT 2000
#define WAIT_SPI_TIMEOUT 100

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void task_experiment_DLS(void);
static void fluidic_test_flow(void);
static void main_exp_fluidic_flow();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uint32_t photo_spi_set_count;
extern uint32_t photo_spi_count;
extern uint32_t current_channel;
extern uint32_t  is_spi_counter_finish;

extern QueueHandle_t experiment_command_queue;
extern QueueHandle_t remote_message_queue;
extern osSemaphore rptx_ram_mutex;
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Experiment Task ::::::::::::: */
void Task_Experiment(void *pvParameters)
{
    static uint16_t command = 0;

    // exp_last_delay = xTaskGetTickCount();
    PRINTF("Experiment Control...\r\n");

    for(;;)
    {
        vTaskDelay(2000);
        Update_Onboard_ADC();

        if(xQueueReceive(experiment_command_queue, &command, 0) != pdPASS)
        {
            continue;
        }

        PRINTF("Experiment Starting ...\r\n");

        // TODO: system_data_get_exp_profile(&s_exp_profile);
        switch (command)
        {
            case SLD_RUN:
            {
              //  task_experiment_DLS();
                break;
            }
                
            case FLUIDIC_TEST:
            {
                fluidic_test_flow();
                break;
            }
    
            case FLUIDIC_SEQ:
            {
                main_exp_fluidic_flow();
                break;
            }

            default:
                break;
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void task_experiment_DLS()
{
    static uint32_t    DAC_code_from_percent = 0;
    static TickType_t  pre_delay, sample_delay, post_delay, wait_delay;
    static uint8_t    wait_spi_timeout = 0;
    remote_message_t message;

    static exp_profile_t s_exp_profile =
    {
        .pre_time_ms = 500,
        .sampling_time_ms = 9000,
        .post_time_ms = 500,

        .sampling_rate_khz = 100,
        .laser_intensity = 50,
    };

    m33_data_exp_profile_get(&s_exp_profile);
    pre_delay    = pdMS_TO_TICKS(s_exp_profile.pre_time_ms);
    sample_delay = pdMS_TO_TICKS(s_exp_profile.sampling_time_ms);
    post_delay   = pdMS_TO_TICKS(s_exp_profile.post_time_ms);
    wait_delay   = pdMS_TO_TICKS(5);
    bsp_expander_ctrl(POW_ONOFF_LASER,1);
    bsp_expander_ctrl(POW_ONOFF_PHOTO,1);

    // TODO: Comment for test
    photo_spi_set_count = s_exp_profile.sampling_rate_khz * (s_exp_profile.pre_time_ms + s_exp_profile.sampling_time_ms + s_exp_profile.post_time_ms);
    PRINTF("[task_experiment_DLS] photo set count = %d\r\n",photo_spi_set_count);
    //photo_spi_set_count = 128;
    photo_spi_count = 0;
    current_channel = 0;
    is_spi_counter_finish = 0;

    bsp_laser_int_all_sw_off();
    bsp_photo_int_all_sw_off();

    // TODO: Guard intensity = 0
    DAC_code_from_percent = (uint8_t)((((float)s_exp_profile.laser_intensity * 256.0) / 100.0) - 1.0);
    PRINTF("laser intensity = %d\r\n", DAC_code_from_percent);

    bsp_photo_setup_timmer(s_exp_profile.sampling_rate_khz);

    for (current_channel = 0; current_channel < 24; current_channel++)
    {
        xSemaphoreTake(rptx_ram_mutex, portMAX_DELAY); // claim RAM

    // PRE_TIME_STATE:
        PRINTF("\r\n channel %d started",current_channel + 1);
        is_spi_counter_finish = 0;
        photo_spi_count = 0;
        bsp_laser_int_sw_on(current_channel + 1);
        bsp_photo_int_sw_on(current_channel + 1);

        bsp_photo_spi_irq_init();
        bsp_photo_start_timer();

        vTaskDelay(pre_delay);
        PRINTF("\r\n channel %d start sampling, wait for %d",current_channel + 1, sample_delay);
        
    // SAMPLING_STATE:
        bsp_laser_int_set_dac(DAC_code_from_percent);

        vTaskDelay(sample_delay);

    // POST_TIME_STATE:
        PRINTF("\r\n channel %d stopped sampling",current_channel + 1);
        bsp_laser_int_set_dac(0);

        vTaskDelay(post_delay);

        wait_spi_timeout = WAIT_SPI_TIMEOUT;

        while (is_spi_counter_finish == 0)
        {
            vTaskDelay(wait_delay);

            wait_spi_timeout--;

            if(wait_spi_timeout == 0)
            {
                bsp_photo_stop_timer();
                bsp_photo_spi_irq_deinit();
                //photo_spi_count = 0;
                is_spi_counter_finish = 0;
                bsp_laser_int_sw_off(current_channel + 1);
                bsp_photo_int_sw_off(current_channel + 1);

                is_spi_counter_finish = 1;
            }
        }

        bsp_photo_spi_irq_deinit();
        bsp_laser_int_sw_off(current_channel + 1);
        bsp_photo_int_sw_off(current_channel + 1);
        PRINTF("\r\n channel %d trigger writing to file",current_channel + 1);

        message.address = DLS_DATA;
        message.data = photo_spi_count * 2;
        // xQueueSendToFront(remote_message_queue, &message, 1000);//send notification for log
        xQueueSend(remote_message_queue, &message, 1000);//send notification for log

        PRINTF("\r\n[task_system_control] sent DLS notification with size:%d\r\n", message.data);
        vTaskDelay(2000);
        xSemaphoreGive(rptx_ram_mutex);
        vTaskDelay(1000);

    }

    for (current_channel = 0; current_channel < 3; current_channel++)
    {
        bsp_laser_ext_sw_on(2*current_channel);
        bsp_laser_ext_sw_on(2*current_channel + 1);
        vTaskDelay(500);
        message.address = CAM_CAPTURE;
        message.data = current_channel;
        xQueueSend(remote_message_queue, &message, 1000);//send notification for log
        vTaskDelay(5000);
        bsp_laser_ext_all_sw_off();
    }

    bsp_expander_ctrl(POW_ONOFF_LASER,0);
    bsp_expander_ctrl(POW_ONOFF_PHOTO,0);
}

static void fluidic_test_flow()
{
    uint8_t sec_count = 0;
    slf3s_readings_t flow_data;
    
    // 0. capture USBcam image
    // TODO: capture
    remote_message_t message;
    message.address = CAM_CAPTURE;
    message.data = 4;
    xQueueSend(remote_message_queue, &message, 1000);//send notification for log
    vTaskDelay(5000);
    lwl_data_log_init();

    // 1. pump on
    // make sure valve dir is on dummy
    Valve_switch(VALVE_DIRECTION_DUMMY);
    LWL_DATA_LOG(LWL_EXP_SWITCH_VALVE,LWL_1(VALVE_DIRECTION_DUMMY));

    I2C_HD_Pump_Set_Freq(100);
    LWL_DATA_LOG(LWL_EXP_PUMP_SET_FREQ,LWL_2(100));
   
    I2C_HD_Pump_set_Voltage(100); //<-- After this line, pump on.
    LWL_DATA_LOG(LWL_EXP_PUMP_SET_VOLT,LWL_2(100));

    // 2. measure flow in every seccond
    for (sec_count = 0; sec_count < 5; sec_count++)
    {
	    Flow_sensor_read(&flow_data);
        LWL_DATA_LOG(LWL_EXP_PUMP_FLOW_TEMP,LWL_4(flow_data.flow), LWL_4(flow_data.temp));

        // TODO: Implement pass data to system data

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    I2C_HD_Pump_set_Voltage(0); //<-- After this line, pump off.
    LWL_DATA_LOG(LWL_EXP_PUMP_OFF);

    // 3. capture USBcam image after 5s
    message.address = CAM_CAPTURE;
    message.data = 4;
    // xQueueSendToFront(remote_message_queue, &message, 1000);//send notification for log
    xQueueSend(remote_message_queue, &message, 1000);//send notification for log
    PRINTF("\r\n[task_system_control] sent DLS notification with size:%d\r\n", message.data);
    vTaskDelay(2000);

    // TODO: capture
    xSemaphoreTake(rptx_ram_mutex, portMAX_DELAY); // claim RAM
    message.data = lwl_sys_log_transfer();
    
    message.address = TEST_PUMP_DATA;
    xQueueSend(remote_message_queue, &message, 1000);//send notification for log
    vTaskDelay(2000);

    xSemaphoreGive(rptx_ram_mutex);
    
    // 4. pump off
    
}

static void main_exp_fluidic_flow()
{
    remote_message_t message;
    uint16_t sec_count = 0;
    slf3s_readings_t flow_data;
    float volumn = 0.0;
    lwl_data_log_init();

    // 1. pump on
    // make sure valve dir is on dummy
    Valve_switch(VALVE_DIRECTION_DUMMY);
    LWL_DATA_LOG(LWL_EXP_SWITCH_VALVE,LWL_1(VALVE_DIRECTION_DUMMY));
    I2C_HD_Pump_Set_Freq(100);
    LWL_DATA_LOG(LWL_EXP_PUMP_SET_FREQ,LWL_2(100));

    I2C_HD_Pump_set_Voltage(100); //<-- After this line, pump on.
    LWL_DATA_LOG(LWL_EXP_PUMP_SET_VOLT,LWL_2(100));

    // 2. after 1s, switch valve to MAIN_EXP, measure flow in every second
    vTaskDelay(pdMS_TO_TICKS(1000));
    Valve_switch(VALVE_DIRECTION_MAIN_EXP);
    LWL_DATA_LOG(LWL_EXP_SWITCH_VALVE,LWL_1(VALVE_DIRECTION_MAIN_EXP));

    // Flow_sensor_read(&flow_data);
    Flow_sensor_read(&flow_data);
    volumn += (flow_data.flow / 60.0);
    
    // measure flow in every seccond, timeout after 6 minutes
    // if the volumn still unable to reach 5000uL, then exit
    // TODO: add a printf or statement to warn about this.
    for (sec_count = 0; (volumn < 5000.0) || (sec_count < 360); sec_count++)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        Flow_sensor_read(&flow_data);
        LWL_DATA_LOG(LWL_EXP_PUMP_FLOW_TEMP,LWL_4(flow_data.flow), LWL_4(flow_data.temp));
        volumn += (flow_data.flow / 60.0);
    }

    // TODO: if flow under 5mL after 360 secs warning or smth

    // 3. when volumn reach 5mL (5s at 1.5mL/min), pump off
    I2C_HD_Pump_set_Voltage(0); //<-- After this line, pump off.
    LWL_DATA_LOG(LWL_EXP_PUMP_SET_VOLT,LWL_2(0));

    // 4. switch valve to DUMMY
    Valve_switch(VALVE_DIRECTION_DUMMY);
    Valve_switch(VALVE_DIRECTION_DUMMY);

    xSemaphoreTake(rptx_ram_mutex, portMAX_DELAY); // claim RAM
    message.data = lwl_sys_log_transfer();
    message.address = TEST_PUMP_DATA;
    xQueueSend(remote_message_queue, &message, 1000);//send notification for log

    vTaskDelay(2000);
    xSemaphoreGive(rptx_ram_mutex);
}