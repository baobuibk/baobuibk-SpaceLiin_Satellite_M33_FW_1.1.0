/*
 * m33_data.h
 *
 *  Created on: Oct 24, 2025
 *      Author: Admin
 */

#ifndef MIDDLEWARE_FLIGHTSYSTEM_SYSTEM_M33_DATA_H_
#define MIDDLEWARE_FLIGHTSYSTEM_SYSTEM_M33_DATA_H_
#include "stdint.h"
/**
 * A 32 bit variable that can be interpreted as int, uint or float
 */
typedef union value16_t{
    int16_t i;
    uint16_t u;
} value16_t;
typedef enum {
    TABLE_ID_1 = 0,
    TABLE_ID_2,
    TABLE_ID_3,
    TABLE_ID_5,
    TABLE_ID_6,
    TABLE_ID_TOTAL_COUNT    // tổng số bảng
} table_id_t;

typedef struct {
    value16_t *data;
    uint16_t   size;
} table_info_t;


typedef struct prof_type0_t
{
	uint16_t	pri_NTC;
	uint16_t	sec_NTC;
	uint16_t	duty;
	uint16_t	heaters_list;
    uint16_t	tecs_list;
	int16_t		setpoint;
} prof_type0_t;


typedef struct prof_type0_ena_t
{
	uint16_t	master_ena;
	uint16_t	prof_ena[6];
} prof_type0_ena_t;


typedef struct exp_profile_t
{
    uint16_t pre_time_ms;
    uint16_t sampling_time_ms;
    uint16_t post_time_ms;

    uint16_t sampling_rate_khz;
    uint16_t laser_intensity;
} exp_profile_t;

typedef enum {
    time_sync,
    pwr_ifb_en,
    pwr_io_en,
    pwr_pzp_en,
    pwr_htr_en,
    pwr_sln_tec_en,
    pwr_lda_en,
    pwr_pda_en,
    pwr_usb_0_en,
    pwr_usb_1_en,
    i2c_s_1_en,
    i2c_s_2_en,
    i2c_pwm_en,
    i2c_hd4_en,
    i2c_ld_1_en,
    i2c_ld_2_en,
    tec_0_en,
    tec_1_en,
    tec_2_en,
    tec_3_en,
    usb_led_1_set,
    usb_led_2_set,
    htr_0_set,
    htr_1_set,
    htr_2_set,
    htr_3_set,
    htr_4_set,
    htr_5_set,
    htr_6_set,
    htr_7_set,
    custom_cmd,

    TABLE1_TOTAL_COUNT   // dùng để biết tổng số phần tử, tiện cho việc lặp
} table1_id_t;

typedef enum {
    pump_1_ctl ,
    pump_1_volt,
    pump_1_freq,
    pump_2_ctl,
    pump_2_volt,
    pump_2_freq,

    sln_0_ctl,
    sln_1_ctl,
    sln_2_ctl,
    sln_3_ctl,
    sln_4_ctl,
    sln_5_ctl,
    sln_6_ctl,
    sln_7_ctl,
    sln_8_ctl,
    sln_9_ctl,
    sln_10_ctl,
    sln_11_ctl,

    sln_valve_1_ctl,
    sln_valve_2_ctl,

    TABLE2_TOTAL_COUNT  // tổng số phần tử enum, tiện cho debug/iteration
} table2_id_t;

typedef enum {
    temp_master_en ,

    temp_p_1_en,
    temp_p_2_en,
    temp_p_3_en,
    temp_p_4_en,
    temp_p_5_en,
    temp_p_6_en,

    temp_p_1_setpoint,
    temp_p_1_ntcp,
    temp_p_1_ntcs,
    temp_p_1_htr,
    temp_p_1_tec,

    temp_p_2_setpoint,
    temp_p_2_ntcp,
    temp_p_2_ntcs,
    temp_p_2_htr,
    temp_p_2_tec,

    temp_p_3_setpoint,
    temp_p_3_ntcp,
    temp_p_3_ntcs,
    temp_p_3_htr,
    temp_p_3_tec,

    temp_p_4_setpoint,
    temp_p_4_ntcp,
    temp_p_4_ntcs,
    temp_p_4_htr,
    temp_p_4_tec,

    temp_p_5_setpoint,
    temp_p_5_ntcp,
    temp_p_5_ntcs,
    temp_p_5_htr,
    temp_p_5_tec,

    temp_p_6_setpoint,
    temp_p_6_ntcp,
    temp_p_6_ntcs,
    temp_p_6_htr,
    temp_p_6_tec,

    TABLE3_TOTAL_COUNT   // tổng số phần tử trong enum, tiện dùng cho vòng lặp hoặc logging
} table3_id_t;

typedef enum {
    test_ls_current,
    test_fluidic_seq,

    exp_fluidic_seq,
    exp_mon_start,
    exp_mon_delay,
    exp_mon_interval,

    dls_ls_intensity,
    cam_ls_intensity,

    exp_samp_rate,
    exp_pre_time,
    exp_samp_time,
    exp_post_time,

    custom_ctl,

    TABLE5_TOTAL_COUNT   // tổng số phần tử, tiện cho debug/iteration
} table5_id_t;

typedef enum {
    sys_status,
    boot_cnt,

    temp_exp,
    temp_ntc_0,
    temp_ntc_1,
    temp_ntc_2,
    temp_ntc_3,
    temp_ntc_4,
    temp_ntc_5,
    temp_ntc_6,
    temp_ntc_7,
    temp_ntc_8,
    temp_ntc_9,
    temp_ntc_10,
    temp_ntc_11,
    temp_board,

    sen1_data_0,
    sen1_data_1,
    sen2_data_0,
    sen2_data_1,

    current_12_tot,
    current_12_lda,
    current_12_pda,
    current_5_io,
    current_5_tec,
    current_5_cam,
    current_5_hd,

    TABLE6_TOTAL_COUNT   // tổng số phần tử, tiện cho việc lặp hoặc debug
} table6_id_t;

uint32_t m33_data_init(void);

uint32_t m33_data_set_u(uint16_t table_id,uint16_t index, uint16_t value);
uint32_t m33_data_set_i(uint16_t table_id,uint16_t index, int16_t value);
uint32_t m33_data_set_u_lock(uint16_t table_id,uint16_t index, uint16_t value);
uint32_t m33_data_set_i_lock(uint16_t table_id,uint16_t index, int16_t value);

uint32_t m33_data_get_i(uint16_t table_id,uint16_t index, int16_t * value);
uint32_t m33_data_get_u(uint16_t table_id,uint16_t index, uint16_t * value);
uint32_t m33_data_get_i_lock(uint16_t table_id,uint16_t index, int16_t * value);
uint32_t m33_data_get_u_lock(uint16_t table_id,uint16_t index, uint16_t * value);

uint32_t m33_data_update_NTC(int16_t* p_data);
uint32_t m33_data_ntc_temp_get(int16_t* p_data);
uint32_t m33_data_update_board_temp(int16_t data);
uint32_t m33_data_get_board_temp(int16_t* p_data);
uint32_t data_prof_type0_get(prof_type0_t *profile, uint16_t index);
uint32_t data_prof_type0_ena_get(prof_type0_ena_t *enaProfile);
uint32_t m33_data_exp_profile_get(exp_profile_t *profile);
uint32_t m33_get_is_start_exp(uint16_t *is_start);
uint32_t m33_get_get_mon_delay(uint16_t *mon_delay);
uint32_t m33_get_set_mon_delay(uint16_t mon_delay);
uint32_t m33_get_get_mon_interval(uint16_t *mon_interval);
uint32_t m33_get_set_mon_interval(uint16_t mon_interval);


#endif /* MIDDLEWARE_FLIGHTSYSTEM_SYSTEM_M33_DATA_H_ */
