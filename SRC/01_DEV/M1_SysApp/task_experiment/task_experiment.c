/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Component includes. */

/* USER include. */
#include "task_experiment.h"
#include "m33_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_laser.h"
#include "bsp_photo.h"
#include "simple_shell.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define EXP_SEM_TIMEOUT 2000
#define WAIT_SPI_TIMEOUT 100

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static exp_profile_t s_exp_profile =
{
    .pre_time_ms = 50,
    .sampling_time_ms = 1900,
    .post_time_ms = 50,

    .sampling_rate_khz = 100,
    .laser_intensity = 50,
};

static uint32_t    DAC_code_from_percent = 0;
static TickType_t exp_last_delay, pre_delay, sample_delay, post_delay, wait_delay;

static uint8_t    wait_spi_timeout = 0;

static uint16_t is_start_exp = 0;
static uint16_t mon_delay = 3600;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uint32_t photo_spi_set_count;
extern uint32_t photo_spi_count;
extern uint32_t current_channel;
extern uint32_t  is_spi_counter_finish;

extern osSemaphore exp_task_sem;
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Experiment Task ::::::::::::: */
void Task_Experiment(void *pvParameters)
{
    exp_last_delay = xTaskGetTickCount();
    Shell_WriteString("Experiment Control...\r\n");

    for(;;)
    {
    // SET_UP_STATE:
 //       vTaskDelayUntil(&exp_last_delay, 1000);
        osSemaphoreTake(&exp_task_sem, portMAX_DELAY); //wait for notification from root task to start experiment
        Shell_WriteString("Experiment Starting ...\r\n");

        // TODO: system_data_get_exp_profile(&s_exp_profile);

        pre_delay    = pdMS_TO_TICKS(s_exp_profile.pre_time_ms);
        sample_delay = pdMS_TO_TICKS(s_exp_profile.sampling_time_ms);
        post_delay   = pdMS_TO_TICKS(s_exp_profile.post_time_ms);
        wait_delay   = pdMS_TO_TICKS(5);


        // TODO: Comment for test
        // photo_spi_set_count = s_exp_profile.sampling_rate_khz * (s_exp_profile.pre_time_ms + s_exp_profile.sampling_time_ms + s_exp_profile.post_time_ms);
        photo_spi_set_count = 128;
        photo_spi_count = 0;
        current_channel = 0;
        is_spi_counter_finish = 0;

        bsp_laser_int_all_sw_off();
        bsp_photo_int_all_sw_off();

        // TODO: Guard intensity = 0
        DAC_code_from_percent = (uint8_t)((((float)s_exp_profile.laser_intensity * 256.0) / 100.0) - 1.0);

        bsp_photo_setup_timmer(s_exp_profile.sampling_rate_khz);

        for (current_channel = 0; current_channel < 24; current_channel++)
        {
    // PRE_TIME_STATE:
            bsp_laser_int_sw_on(current_channel + 1);
            bsp_photo_int_sw_on(current_channel + 1);

            bsp_photo_spi_irq_init();
            bsp_photo_start_timer();

            vTaskDelayUntil(&exp_last_delay, pre_delay);

    // SAMPLING_STATE:
            bsp_laser_int_set_dac(DAC_code_from_percent);

            vTaskDelayUntil(&exp_last_delay, sample_delay);

    // POST_TIME_STATE:
            bsp_laser_int_set_dac(0);

            vTaskDelayUntil(&exp_last_delay, post_delay);

            wait_spi_timeout = WAIT_SPI_TIMEOUT;

            while (is_spi_counter_finish == 0)
            {
                vTaskDelayUntil(&exp_last_delay, wait_delay);

                wait_spi_timeout--;

                if(wait_spi_timeout == 0)
                {
                    bsp_photo_stop_timer();
                    bsp_photo_spi_irq_deinit();
                    photo_spi_count = 0;
                    is_spi_counter_finish = 0;
                    bsp_laser_int_sw_off(current_channel + 1);
                    bsp_photo_int_sw_off(current_channel + 1);

                    is_spi_counter_finish = 1;
                }
            }

            is_spi_counter_finish = 0;
            bsp_photo_spi_irq_deinit();
            bsp_laser_int_sw_off(current_channel + 1);
            bsp_photo_int_sw_off(current_channel + 1);
        }

    // OFFLOAD_STATE:

        // MAKE SURE to already deinit spi from irq mode to polling mode
        // OFFLOAD the data to core A55
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
