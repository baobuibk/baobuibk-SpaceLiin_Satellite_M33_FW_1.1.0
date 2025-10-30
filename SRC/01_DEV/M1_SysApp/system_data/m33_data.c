/*
 * m33_data.c
 *
 *  Created on: Oct 24, 2025
 *      Author: Admin
 */


#include "m33_data.h"
#include "osSemphr.h"

static osSemaphore m33_data_sem = NULL;
#define M33_DATA_SEMAPHOR_TIMEOUT 2000

uint64_t    epoch_time = 0;

 value16_t table1_data[TABLE1_TOTAL_COUNT];
 value16_t table2_data[TABLE2_TOTAL_COUNT];
 value16_t table3_data[TABLE3_TOTAL_COUNT];
 value16_t table5_data[TABLE5_TOTAL_COUNT];
 value16_t table6_data[TABLE6_TOTAL_COUNT];

table_info_t tables[TABLE_ID_TOTAL_COUNT] = {
    [TABLE_ID_1] = { table1_data, TABLE1_TOTAL_COUNT },
    [TABLE_ID_2] = { table2_data, TABLE2_TOTAL_COUNT },
    [TABLE_ID_3] = { table3_data, TABLE3_TOTAL_COUNT },
    [TABLE_ID_5] = { table5_data, TABLE5_TOTAL_COUNT },
    [TABLE_ID_6] = { table6_data, TABLE6_TOTAL_COUNT },
};

/* ==================== TABLE 1 INITIAL DATA ==================== */
const int16_t table1_data_init[TABLE1_TOTAL_COUNT] = {
    0x00,  // time_sync
    0x00,  // pwr_ifb_en
    0x00,  // pwr_io_en
    0x00,  // pwr_pzp_en
    0x00,  // pwr_htr_en
    0x00,  // pwr_sln_tec_en
    0x00,  // pwr_lda_en
    0x00,  // pwr_pda_en
    0x00,  // pwr_usb_0_en
    0x00,  // pwr_usb_1_en
    0x00,  // i2c_s_1_en
    0x00,  // i2c_s_2_en
    0x00,  // i2c_pwm_en
    0x00,  // i2c_hd4_en
    0x00,  // i2c_ld_1_en
    0x00,  // i2c_ld_2_en
    0x00,  // tec_0_en
    0x00,  // tec_1_en
    0x00,  // tec_2_en
    0x00,  // tec_3_en
    0x00,  // usb_led_1_set
    0x00,  // usb_led_2_set
    0x00,  // htr_0_set
    0x00,  // htr_1_set
    0x00,  // htr_2_set
    0x00,  // htr_3_set
    0x00,  // htr_4_set
    0x00,  // htr_5_set
    0x00,  // htr_6_set
    0x00,  // htr_7_set
    0x00   // custom_cmd
};

/* ==================== TABLE 2 INITIAL DATA ==================== */
const int16_t table2_data_init[TABLE2_TOTAL_COUNT] = {
    0x00,  // pump_1_ctl
    100,  // pump_1_volt
    100,  // pump_1_freq
    0x00,  // pump_2_ctl
    0x00,  // pump_2_volt
    0x00,  // pump_2_freq

    0x00,  // sln_0_ctl
    0x00,  // sln_1_ctl
    0x00,  // sln_2_ctl
    0x00,  // sln_3_ctl
    0x00,  // sln_4_ctl
    0x00,  // sln_5_ctl
    0x00,  // sln_6_ctl
    0x00,  // sln_7_ctl
    0x00,  // sln_8_ctl
    0x00,  // sln_9_ctl
    0x00,  // sln_10_ctl
    0x00,  // sln_11_ctl

    0x00,  // sln_valve_1_ctl
    0x00   // sln_valve_2_ctl
};

/* ==================== TABLE 3 INITIAL DATA ==================== */
const int16_t table3_data_init[] = {
    0x01,  // temp_master_en

    0x00,  // temp_p_1_en
    0x00,  // temp_p_2_en
    0x00,  // temp_p_3_en
    0x00,  // temp_p_4_en
    0x00,  // temp_p_5_en
    0x00,  // temp_p_6_en

    200,  // temp_p_1_setpoint
    0x00,  // temp_p_1_ntcp
    1,  // temp_p_1_ntcs
    0x01,  // temp_p_1_htr
    0x00,  // temp_p_1_tec

    200,  // temp_p_2_setpoint
    4,  // temp_p_2_ntcp
    5,  // temp_p_2_ntcs
    0x02,  // temp_p_2_htr
    0x00,  // temp_p_2_tec

    250,  // temp_p_3_setpoint
    6,  // temp_p_3_ntcp
    7,  // temp_p_3_ntcs
    (1 << 3),  // temp_p_3_htr
    0x00,  // temp_p_3_tec

    0x00,  // temp_p_4_setpoint
    0xFFFF,  // temp_p_4_ntcp
    0xFFFF,  // temp_p_4_ntcs
    0x00,  // temp_p_4_htr
    0x00,  // temp_p_4_tec

    0x00,  // temp_p_5_setpoint
    0xFFFF,  // temp_p_5_ntcp
    0xFFFF,  // temp_p_5_ntcs
    0x00,  // temp_p_5_htr
    0x00,  // temp_p_5_tec

    0x00,  // temp_p_6_setpoint
    0xFFFF,  // temp_p_6_ntcp
    0xFFFF,  // temp_p_6_ntcs
    0x00,  // temp_p_6_htr
    0x00   // temp_p_6_tec
};

/* ==================== TABLE 5 INITIAL DATA ==================== */
const int16_t table5_data_init[] = {
    0x00,  // test_ls_current
    0x00,  // test_fluidic_seq

    0x00,  // exp_fluidic_seq
    0x00,  // exp_mon_start
    0x00,  // exp_mon_delay
    28800,  // exp_mon_interval

    25,  // dls_ls_intensity
    15,  // cam_ls_intensity

    100,  // exp_samp_rate
    1,  // exp_pre_time
    100,  // exp_samp_time
    1,  // exp_post_time

    0x00   // custom_ctl
};


uint32_t m33_data_init()
{
    int sem_ret = osSemaphoreCreateMutex(&m33_data_sem);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    uint16_t i;
    for (i=0; i < TABLE1_TOTAL_COUNT; i++) 
        {
            table1_data[i].i = table1_data_init[i];
        }
    for (i=0; i < TABLE2_TOTAL_COUNT; i++) 
        {
            table2_data[i].i = table2_data_init[i];
        }
    for (i=0; i < TABLE3_TOTAL_COUNT; i++) 
        {
            table3_data[i].i = table3_data_init[i];
        }  
    for (i=0; i < TABLE5_TOTAL_COUNT; i++) 
        {
            table5_data[i].i = table5_data_init[i];
        }    
    return (uint32_t)sem_ret;
}

uint32_t m33_data_set_u(uint16_t table_id,uint16_t index, uint16_t value)
{
    if (table_id >= TABLE_ID_TOTAL_COUNT) return 1; // tránh out of range
    if (index >= tables[table_id].size) return 1;   // tránh vượt giới hạn bảng

    tables[table_id].data[index].u = value;
    return 0;
}

uint32_t m33_data_set_epoch_lock(uint32_t value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    epoch_time = value;
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}

uint32_t m33_data_get_epoch_lock(uint32_t * value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
   * value = epoch_time;
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}

uint32_t m33_data_set_u_lock(uint16_t table_id,uint16_t index, uint16_t value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_set_u(table_id, index, value);
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}


uint32_t m33_data_set_i(uint16_t table_id,uint16_t index, int16_t value)
{
    if (table_id >= TABLE_ID_TOTAL_COUNT) return 1; // tránh out of range
    if (index >= tables[table_id].size) return 1;   // tránh vượt giới hạn bảng

    tables[table_id].data[index].i = value;
    return 0;
}
uint32_t m33_data_set_i_lock(uint16_t table_id,uint16_t index, int16_t value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_set_i(table_id, index, value);
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}

uint32_t m33_data_get_i(uint16_t table_id,uint16_t index, int16_t * value)
{
    if (table_id >= TABLE_ID_TOTAL_COUNT) return 1; // tránh out of range
    if (index >= tables[table_id].size) return 1;   // tránh vượt giới hạn bảng

    *value = tables[table_id].data[index].i;
    return 0;
}
uint32_t m33_data_get_i_lock(uint16_t table_id,uint16_t index, int16_t * value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_get_i(table_id, index, value);
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}


uint32_t m33_data_get_u(uint16_t table_id,uint16_t index, uint16_t * value)
{
    if (table_id >= TABLE_ID_TOTAL_COUNT) return 1; // tránh out of range
    if (index >= tables[table_id].size) return 1;   // tránh vượt giới hạn bảng
    *value = tables[table_id].data[index].u;
    return 0;
  
}
uint32_t m33_data_get_u_lock(uint16_t table_id,uint16_t index, uint16_t * value)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_get_u(table_id, index, value);
    osSemaphoreGiven(&m33_data_sem);
    return 0;
}

uint32_t m33_data_update_NTC(int16_t *p_data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

 //   memcpy((void*)NTC_temp_C, (void*)p_data, sizeof(NTC_temp_C));
    m33_data_set_i(TABLE_ID_6, temp_ntc_0, p_data[0]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_1, p_data[1]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_2, p_data[2]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_3, p_data[3]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_4, p_data[4]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_5, p_data[5]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_6, p_data[6]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_7, p_data[7]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_8, p_data[8]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_9, p_data[9]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_10, p_data[10]);
    m33_data_set_i(TABLE_ID_6, temp_ntc_11, p_data[11]);
    
    osSemaphoreGiven(&m33_data_sem);

    return sem_ret;
}

uint32_t m33_data_update_EFUSE(uint16_t *p_data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    m33_data_set_u(TABLE_ID_6, current_12_pda, p_data[0]);
    m33_data_set_u(TABLE_ID_6, current_5_cam, p_data[1]);
    m33_data_set_u(TABLE_ID_6, current_12_lda, p_data[2]);
    m33_data_set_u(TABLE_ID_6, current_5_hd, p_data[3]);
    m33_data_set_u(TABLE_ID_6, current_12_tot, p_data[5] + p_data[4] + p_data[8]);
    m33_data_set_u(TABLE_ID_6, current_5_tec, p_data[6]);
    m33_data_set_u(TABLE_ID_6, current_5_io, p_data[7]);
    
    osSemaphoreGiven(&m33_data_sem);

    return sem_ret;
}

uint32_t m33_data_update_board_temp(int16_t data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_set_i(TABLE_ID_6, temp_exp, data);   
    osSemaphoreGiven(&m33_data_sem);
    return sem_ret;
}

uint32_t     m33_data_ntc_temp_get(int16_t* p_data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret = m33_data_get_i(TABLE_ID_6, temp_ntc_0, &p_data[0]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_1, &p_data[1]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_2, &p_data[2]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_3, &p_data[3]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_4, &p_data[4]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_5,&p_data[5]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_6, &p_data[6]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_7, &p_data[7]);
    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_8, &p_data[8]);

    sem_ret += m33_data_get_i(TABLE_ID_6, temp_ntc_9, &p_data[9]);
    sem_ret +=  m33_data_get_i(TABLE_ID_6, temp_ntc_10, &p_data[10]);
    sem_ret +=  m33_data_get_i(TABLE_ID_6, temp_ntc_11, &p_data[11]);
    osSemaphoreGiven(&m33_data_sem);
    return (uint32_t)sem_ret;
}
uint32_t m33_data_get_board_temp(int16_t* p_data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret = m33_data_get_i(TABLE_ID_6, temp_exp, p_data);
    osSemaphoreGiven(&m33_data_sem);
    return (uint32_t)sem_ret;
}

uint32_t data_prof_type0_ena_get(prof_type0_ena_t *enaProfile)
{
        int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_master_en, &enaProfile->master_ena);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_1_en, &enaProfile->prof_ena[0]);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_2_en, &enaProfile->prof_ena[1]);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_3_en, &enaProfile->prof_ena[2]);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_4_en, &enaProfile->prof_ena[3]);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_5_en, &enaProfile->prof_ena[4]);
    sem_ret +=  m33_data_get_u(TABLE_ID_3, temp_p_6_en, &enaProfile->prof_ena[5]);
    osSemaphoreGiven(&m33_data_sem);
    return (uint32_t)sem_ret;
}

uint32_t data_prof_type0_get(prof_type0_t *profile, uint16_t index)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    switch (index)
    {
        case 0:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_1_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_1_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_1_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_1_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_1_setpoint, &profile->setpoint);
            break;
        case 1:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_2_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_2_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_2_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_2_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_2_setpoint, &profile->setpoint);
            break;
        case 2:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_3_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_3_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_3_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_3_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_3_setpoint, &profile->setpoint);
            break;
        case 3:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_4_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_4_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_4_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_4_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_4_setpoint, &profile->setpoint);
            break;
        case 4:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_5_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_5_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_5_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_5_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_5_setpoint, &profile->setpoint);
            break;
        case 5:
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_6_ntcp, &profile->pri_NTC);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_6_ntcs, &profile->sec_NTC);
            profile->duty = 0; // not used
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_6_htr, &profile->heaters_list);
            sem_ret += m33_data_get_u(TABLE_ID_3, temp_p_6_tec, &profile->tecs_list);
            sem_ret += m33_data_get_i(TABLE_ID_3, temp_p_6_setpoint, &profile->setpoint);
            break;
        default:
            break;
    }
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}

uint32_t m33_data_exp_profile_get(exp_profile_t *profile)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_pre_time, &profile->pre_time_ms);
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_samp_time, &profile->sampling_time_ms);
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_post_time, &profile->post_time_ms);
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_samp_rate, &profile->sampling_rate_khz);
    sem_ret += m33_data_get_u(TABLE_ID_5, dls_ls_intensity, &profile->laser_intensity);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}

uint32_t m33_get_is_start_exp(uint16_t *is_start)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_mon_start, is_start);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}
uint32_t m33_get_get_mon_delay(uint16_t *mon_delay)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_mon_delay, mon_delay);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}
uint32_t m33_get_set_mon_delay(uint16_t mon_delay)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_set_u(TABLE_ID_5, exp_mon_delay, mon_delay);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}
uint32_t m33_get_get_mon_interval(uint16_t *mon_interval)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_get_u(TABLE_ID_5, exp_mon_interval, mon_interval);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}
uint32_t m33_get_set_mon_interval(uint16_t mon_interval)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    sem_ret += m33_data_set_u(TABLE_ID_5, exp_mon_interval, mon_interval);
    osSemaphoreGiven(&m33_data_sem);   
    return (uint32_t)sem_ret; 
}