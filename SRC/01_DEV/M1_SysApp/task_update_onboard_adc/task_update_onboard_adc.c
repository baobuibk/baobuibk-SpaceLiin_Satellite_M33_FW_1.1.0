/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Task common includes. */

#include "m33_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_onboard_adc.h"
#include "simple_shell.h"
#include "remote_call.h"
#include "fsl_debug_console.h"
/* Component includes. */

/* USER include. */
#include "task_update_onboard_adc.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TASK_DELAY_MS   1000
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Test CAN Task ::::::::::::: */
void Task_Update_Onboard_ADC(void *pvParameters)
{

    // flexcan_frame_t RX_frame;
    PRINTF("[update on board ADC]\r\n");

    for(;;)
    {
        PRINTF("update ADC\r\n");
        bsp_onboard_adc_update_raw();
        PRINTF("update ADC_->1\r\n");
        bsp_onboard_adc_update_volt();

        bsp_convert_TEC();
        bsp_convert_NTC();
        bsp_convert_eFUSE_Current();
        bsp_convert_onboard_temp();

        // wake up exactly every 1000 ms
        vTaskDelay( 5000);
    }
}

void ADC_update()
{
     bsp_onboard_adc_update_raw();
        bsp_onboard_adc_update_volt();

        bsp_convert_TEC();
        bsp_convert_NTC();
        bsp_convert_eFUSE_Current();
        bsp_convert_onboard_temp();
}

//static void CMD_send_splash()
//{
//    for(uint8_t i = 0 ; i < 21 ; i++)
//    {
//		TASK_CMD_LINE_SEND_STRING((&SPLASH[i][0]);
//	}
//	TASK_CMD_LINE_SEND_STRING(("> ");
//}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
