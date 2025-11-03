/*
 * app_temperature.c
 *
 *  Created on: Oct 14, 2025
 *      Author: Admin
 */

#include "config.h"
#include "m33_data.h"
#include "bsp_heater.h"
#include "FreeRTOS.h"
#include "task.h"
// #include "osDelay.h"
// #include "math.h"
#include "fsl_debug_console.h"
#include "bsp_expander.h"
#include "bsp_i2c_sensor.h"
#include "error_codes.h"
#include "lwl.h"
#include "app_temperature.h"



#define 	TEMPERTURE_CONTROL_INTERVAL	 2000
// #define 	TEMPERATURE_CONTROL_WAIT	 10		//when reaching setpoint, wait some cycle to be sure
// #define 	TEMPERATURE_CONTROL_DEADBAND 1		//deadband is 0.1
#define 	TEMPERATURE_DELTA	50				//delta between primary and secondary NTC to detect error

// #define KP	(0.4)
// #define KI	(0.1)
// #define KD	(0.1)

enum
{
	TEMP_PROF0_STOP,
	TEMP_PROF0_HEAT,
	TEMP_PROF0_ERROR
};
#define NUM_NTC 12


/* Hàm điều khiển nhiệt độ cho profile 0 */
void task_temperature_control_profile_type0()
{
	prof_type0_t 	 profile;
	prof_type0_ena_t enaProfile;

	int16_t	 NTC_temperature[NUM_NTC];
	int16_t	 pri_temperature, sec_temperature;
	uint16_t heater_duty[8] = {0};
	
	int16_t	 prof0_ctrl_state[6] = {TEMP_PROF0_STOP};
	uint32_t i = 0;

	// while (1)
	// {
    //     vTaskDelay(TEMPERTURE_CONTROL_INTERVAL); // Suspend task

		data_prof_type0_ena_get(&enaProfile);

		if (!enaProfile.master_ena)
		{
			bsp_expander_ctrl(POW_ONOFF_HEATER, 0);

			bsp_heater_list_turnoff(255);
			m33_sys_status_set_off_all_profile();
			//continue;
			return ;
		}

		m33_data_ntc_temp_get(NTC_temperature);
		for (i = 0; i < 8; i++)
		{
			m33_data_get_u(TABLE_ID_1, htr_0_set + i, &heater_duty[i]);
		}

		for ( i = 0; i < 6; i++)
		{

			data_prof_type0_get(&profile, i);

			if (!enaProfile.prof_ena[i])
			{
				bsp_heater_list_turnoff(profile.heaters_list);
				m33_sys_status_set_profile(i, 0);
				// PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
				if (TEMP_PROF0_STOP != prof0_ctrl_state[i])
				{
					prof0_ctrl_state[i] = TEMP_PROF0_STOP;
					// PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
				}

				continue;
			}

            PRINTF("\r\n[temperature_control] start profile %d\r\n", i);

			if (profile.pri_NTC < NUM_NTC)
			{
				pri_temperature = NTC_temperature[profile.pri_NTC];

				if (profile.sec_NTC < NUM_NTC)
				{
					sec_temperature = NTC_temperature[profile.sec_NTC];
				}
				else
				{
					sec_temperature = pri_temperature;
				}
			}
			else
			{
				continue;
			}

			if ((pri_temperature > (sec_temperature + TEMPERATURE_DELTA)) || (pri_temperature < (sec_temperature - TEMPERATURE_DELTA)))
			{
				// Error, turn off all heaters in the list (0x03 mean heater 0,1)
				bsp_heater_list_turnoff(profile.heaters_list);
				m33_sys_status_set_profile(i, 0);

                if (TEMP_PROF0_ERROR != prof0_ctrl_state[i])
                {
                	prof0_ctrl_state[i] = TEMP_PROF0_ERROR;
                	// pid_init(&PID_Controller[i], KP, KI, KD);
                    PRINTF("\r\n[temperature_control]  profile %d ERROR, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
                }
			}
			else
			{
				if (pri_temperature < profile.setpoint)
				{
					// bsp_heater_list_turnon(profile.heaters_list, heater_duty);
					bsp_expander_ctrl(POW_ONOFF_HEATER, 1);
					for (uint16_t heater_index = 0; heater_index < 8; heater_index++)
						{
							if (profile.heaters_list & (1 << heater_index))
							{	
								PRINTF("\r\n[temperature_control]  heater %d ON, duty = %d\r\n", heater_index + 1, heater_duty[heater_index]);

								bsp_heater_turnon(heater_index + 1, heater_duty[heater_index]);
							}
						}
					PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
					m33_sys_status_set_profile(i, 1);
            		if (TEMP_PROF0_HEAT != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_HEAT;
         				PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
						LWL_SYS_LOG(LWL_PROFILE_HEATER_ON,LWL_1(i));
					}
				}
				else
				{
					bsp_heater_list_turnoff(profile.heaters_list);
					PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
            		m33_sys_status_set_profile(i, 0);
					if (TEMP_PROF0_STOP != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_STOP;
						LWL_SYS_LOG(LWL_PROFILE_HEATER_OFF,LWL_1(i));
         				PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
            		}
				}
			}
		}
//	}
}


void task_temperature_control_use_bmp390()
{
	prof_type0_t 	 profile;
	prof_type0_ena_t enaProfile;

	bmp390_data_t    bmp390_data;
	int16_t			 bmp390_temp;

	// int16_t	 NTC_temperature[NUM_NTC];
	int16_t	 pri_temperature, sec_temperature;
	uint16_t heater_duty[8] = {0};
	// portTick xLastWakeTime = osTaskGetTickCount();
	
	int16_t	 prof0_ctrl_state[6] = {TEMP_PROF0_STOP};
	uint32_t i = 0;

	// for ( i = 0; i < 6; i++)
	// {
	// 	pid_init(&PID_Controller[i], KP, KI, KD);
	// }

	// while (1)
	// {
    //     vTaskDelay(TEMPERTURE_CONTROL_INTERVAL); // Suspend task

		if (ERROR_OK == BMP390_sensor_read(&bmp390_data))
        {
            int16_t sensor_data = (int16_t) (bmp390_data.Pressure / 10.0);
            m33_data_set_i_lock(TABLE_ID_6, sen1_data_1,sensor_data);
            sensor_data = (int16_t)(bmp390_data.Temp * 10.0);
            m33_data_set_i_lock(TABLE_ID_6, sen1_data_0,sensor_data);
        }

		data_prof_type0_ena_get(&enaProfile);

		if (!enaProfile.master_ena)
		{
			bsp_heater_list_turnoff(255);
			m33_sys_status_set_off_all_profile();
			//continue;
			return;
		}

		for (i = 0; i < 8; i++)
		{
			m33_data_get_u(TABLE_ID_1, htr_0_set + i, &heater_duty[i]);
		}

		bmp390_temp = (int16_t)(bmp390_data.Temp * 10.0);

		for ( i = 0; i < 6; i++)
		{
			data_prof_type0_get(&profile, i);

			if (!enaProfile.prof_ena[i])
			{
				bsp_heater_list_turnoff(profile.heaters_list);
				m33_sys_status_set_profile(i, 0);
				// PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
				if (TEMP_PROF0_STOP != prof0_ctrl_state[i])
				{
					prof0_ctrl_state[i] = TEMP_PROF0_STOP;
					// PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
				}

				continue;
			}

            PRINTF("\r\n[temperature_control] start profile %d\r\n", i);

			pri_temperature = bmp390_temp;
			sec_temperature = bmp390_temp;

			if ((pri_temperature > (sec_temperature + TEMPERATURE_DELTA)) || (pri_temperature < (sec_temperature - TEMPERATURE_DELTA)))
			{
				// Error, turn off all heaters in the list (0x03 mean heater 0,1)
				bsp_heater_list_turnoff(profile.heaters_list);

                if (TEMP_PROF0_ERROR != prof0_ctrl_state[i])
                {
                	prof0_ctrl_state[i] = TEMP_PROF0_ERROR;
                	// pid_init(&PID_Controller[i], KP, KI, KD);
                    PRINTF("\r\n[temperature_control]  profile %d ERROR, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
                }
			}
			else
			{
				if (pri_temperature < profile.setpoint)
				{
					// bsp_heater_list_turnon(profile.heaters_list, heater_duty);
					bsp_expander_ctrl(POW_ONOFF_HEATER, 1);

					for (uint16_t heater_index = 0; heater_index < 8; heater_index++)
					{
						if (profile.heaters_list & (1 << heater_index))
						{	
							PRINTF("\r\n[temperature_control]  heater %d ON, duty = %d\r\n", heater_index + 1, heater_duty[heater_index]);

							bsp_heater_turnon(heater_index + 1, heater_duty[heater_index]);
						}
					}

					PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i + 1,pri_temperature,sec_temperature);
					m33_sys_status_set_profile(i, 1);

            		if (TEMP_PROF0_HEAT != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_HEAT;
         				PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i + 1,pri_temperature,sec_temperature);
						LWL_SYS_LOG(LWL_PROFILE_HEATER_ON,LWL_1(i));
            		}
				}
				else
				{
					bsp_heater_list_turnoff(profile.heaters_list);
					PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i + 1,pri_temperature,sec_temperature);
					m33_sys_status_set_profile(i, 0);
            		if (TEMP_PROF0_STOP != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_STOP;
						LWL_SYS_LOG(LWL_PROFILE_HEATER_OFF,LWL_1(i));

         				PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i + 1,pri_temperature,sec_temperature);
            		}
				}
			}
		}
//	}
}