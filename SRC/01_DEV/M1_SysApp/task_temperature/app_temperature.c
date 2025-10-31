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
// Struct để lưu trạng thái PID (tránh biến toàn cục)
// typedef struct PID_Controller{
//     float Kp;      // Proportional gain
//     float Ki;      // Integral gain
//     float Kd;      // Derivative gain
//     float integral; // Tích phân lỗi hiện tại
//     float prev_error; // Lỗi trước đó
//     uint32_t prev_output;
// } PID_Controller_t;


// PID_Controller_t PID_Controller[6];

// Hàm khởi tạo PID với tham số đầu vào
// void pid_init(PID_Controller_t * pid, double kp, double ki, double kd) ;
// void pid_init(PID_Controller_t * pid, double kp, double ki, double kd) {

//     pid->Kp = kp;
//     pid->Ki = ki;
//     pid->Kd = kd;
//     pid->integral = 0.0;
//     pid->prev_error = 0.0;
//     return ;
// }

// Hàm cập nhật PID: nhận pointer đến PID struct, setpoint, current_temp, dt
// Trả về output (0-100), cập nhật trạng thái bên trong struct
// float pid_update(PID_Controller_t *pid, int32_t setpoint, int32_t current_temp, uint32_t dt) ;

// float pid_update(PID_Controller_t *pid, int32_t setpoint, int32_t current_temp, uint32_t dt) {
//     float error = setpoint - current_temp;

//     if (fabs(error) <= TEMPERATURE_CONTROL_DEADBAND) {

//             return pid->prev_output;  // Giả sử lưu prev_output trong struct
//         }
//     // Tích phân lỗi
//     pid->integral += error * dt;

//     // Đạo hàm lỗi
//     float derivative = (error - pid->prev_error) / dt;

//     // Tính output PID
//     float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
//     pid->prev_error = error;

//     // Giới hạn output trong 0-100 với anti-windup cơ bản
//     if (output > 100.0) {
//         output = 100.0;
//         // Anti-windup: không tích lũy integral nếu saturated và error cùng dấu
//         if (error > 0.0) pid->integral -= error * dt;
//     } else if (output < 0.0) {
//         output = 0.0;
//         if (error < 0.0) pid->integral -= error * dt;
//     }
//     pid->prev_output = output;
//     return output;
// }


// Định nghĩa hàm CLAMP để giới hạn giá trị trong khoảng [lo, hi]
// #define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

/* Hàm điều khiển nhiệt độ cho profile 0 */
void task_temperature_control_profile_type0(void *param)
{
	prof_type0_t 	 profile;
	prof_type0_ena_t enaProfile;

	int16_t	 NTC_temperature[NUM_NTC];
	int16_t	 pri_temperature, sec_temperature;
	// portTick xLastWakeTime = osTaskGetTickCount();
	
	int16_t	 prof0_ctrl_state[6] = {TEMP_PROF0_STOP};
	uint32_t i = 0;

	// for ( i = 0; i < 6; i++)
	// {
	// 	pid_init(&PID_Controller[i], KP, KI, KD);
	// }

	while (1)
	{
        vTaskDelay(TEMPERTURE_CONTROL_INTERVAL); // Suspend task

		data_prof_type0_ena_get(&enaProfile);

		if (!enaProfile.master_ena)
		{
			bsp_heater_list_turnoff(255);
			continue;
		}

		m33_data_ntc_temp_get(NTC_temperature);

		for ( i = 0; i < 6; i++)
		{

			data_prof_type0_get(&profile, i);

			if (!enaProfile.prof_ena[i])
			{
				bsp_heater_list_turnoff(profile.heaters_list);
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

                if (TEMP_PROF0_ERROR != prof0_ctrl_state[i])
                {
                	prof0_ctrl_state[i] = TEMP_PROF0_ERROR;
                	// pid_init(&PID_Controller[i], KP, KI, KD);
                    PRINTF("\r\n[temperature_control]  profile %d ERROR, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
                }
			}
			else
			{
				uint32_t heater_duty = 50;
				if (pri_temperature < profile.setpoint)
				{
					// bsp_heater_list_turnon(profile.heaters_list, heater_duty);
					bsp_expander_ctrl(POW_ONOFF_HEATER, 1);
					bsp_heater_list_turnon(profile.heaters_list, heater_duty);

					PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);

            		if (TEMP_PROF0_HEAT != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_HEAT;
         				PRINTF("\r\n[temperature_control]  profile %d heater ON, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
            		}
				}
				else
				{
					bsp_expander_ctrl(POW_ONOFF_HEATER, 0);
					bsp_heater_list_turnoff(profile.heaters_list);
					PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
            		if (TEMP_PROF0_STOP != prof0_ctrl_state[i])
            		{
            			prof0_ctrl_state[i] = TEMP_PROF0_STOP;
         				PRINTF("\r\n[temperature_control]  profile %d heater OFF, pri = %d sec=%d\r\n", i,pri_temperature,sec_temperature);
            		}
				}
			}
		}
	}
}