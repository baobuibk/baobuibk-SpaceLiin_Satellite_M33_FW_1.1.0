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


uint32_t m33_data_init()
{
    int sem_ret = osSemaphoreCreate(&m33_data_sem);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    sem_ret = osSemaphoreCreate(&m33_data_sem);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
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

// uint32_t m33_data_get_f(uint32_t table_id,uint32_t index, float * value)
// {
//     if (table_id >= TABLE_ID_TOTAL_COUNT) return 1; // tránh out of range
//     if (index >= tables[table_id].size) return 1;   // tránh vượt giới hạn bảng

//     *value = tables[table_id].data[index].f;
//     return 0;
// }

uint32_t m33_data_update_NTC(int16_t* p_data)
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
uint32_t m33_data_update_board_temp(int16_t data)
{
    int sem_ret = osSemaphoreTake(&m33_data_sem, M33_DATA_SEMAPHOR_TIMEOUT);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }
    m33_data_set_i(TABLE_ID_6, temp_board, data);   
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
    sem_ret = m33_data_get_i(TABLE_ID_6, temp_board, p_data);
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
    sem_ret += m33_data_get_u(TABLE_ID_5, cam_ls_intensity, &profile->laser_intensity);
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