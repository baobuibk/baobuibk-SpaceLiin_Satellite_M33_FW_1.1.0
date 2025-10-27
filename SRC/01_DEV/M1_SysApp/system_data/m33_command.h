/*
 * m33_command.h
 *
 *  Created on: Oct 24, 2025
 *      Author: Admin
 */

#ifndef M33_COMMAND_H_
#define M33_COMMAND_H_
#include "stdint.h"
typedef void (*cmdFunction)(uint32_t stdio,uint32_t argc, char *argv[]);



typedef struct {
    const char *name;
    cmdFunction func;
} cmdFunction_entry_t;

void Command_Process(char *input);


/* ==================== Prototypes: TABLE 1 ==================== */
void time_sync_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_ifb_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_io_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_pzp_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_htr_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_sln_tec_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_lda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_pda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_usb_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pwr_usb_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_s_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_s_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_pwm_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_hd4_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_ld_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void i2c_ld_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void tec_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void tec_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void tec_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void tec_3_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void usb_led_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void usb_led_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_0_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_3_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_4_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_5_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_6_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void htr_7_set_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void custom_cmd_cmd(uint32_t stdio, uint32_t argc, char *argv[]);

/* ==================== Prototypes: TABLE 2 ==================== */
void pump_1_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pump_1_volt_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pump_1_freq_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pump_2_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pump_2_volt_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void pump_2_freq_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_0_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_1_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_2_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_3_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_4_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_5_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_6_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_7_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_8_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_9_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_10_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_11_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_valve_1_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sln_valve_2_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);

/* ==================== Prototypes: TABLE 3 ==================== */
void temp_master_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_1_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_2_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_3_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_4_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_5_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_setpoint_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_ntcp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_ntcs_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_htr_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_p_6_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);

/* ==================== Prototypes: TABLE 5 ==================== */
void test_ls_current_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void test_fluidic_seq_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_fluidic_seq_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_mon_start_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_mon_delay_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_mon_interval_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void dls_ls_intensity_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void cam_ls_intensity_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_samp_rate_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_pre_time_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_samp_time_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void exp_post_time_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void custom_ctl_cmd(uint32_t stdio, uint32_t argc, char *argv[]);

/* ==================== Prototypes: TABLE 6 ==================== */
void sys_status_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void boot_cnt_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_exp_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_0_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_1_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_2_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_3_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_4_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_5_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_6_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_7_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_8_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_9_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_10_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void temp_ntc_11_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sen1_data_0_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sen1_data_1_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sen2_data_0_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void sen2_data_1_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_12_tot_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_12_lda_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_12_pda_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_5_io_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_5_tec_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_5_cam_cmd(uint32_t stdio, uint32_t argc, char *argv[]);
void current_5_hd_cmd(uint32_t stdio, uint32_t argc, char *argv[]);




#endif /* M33_COMMAND_H_ */
