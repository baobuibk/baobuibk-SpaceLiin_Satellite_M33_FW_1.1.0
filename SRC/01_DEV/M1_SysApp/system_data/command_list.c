
#include "m33_command.h"
/* Mảng chứa các hàm trong TABLE 1 */
/* ==================== TABLE 1 ==================== */
cmdFunction_entry_t cmd_table_1[] = {
    { "time_sync_cmd",        (cmdFunction)time_sync_cmd },
    { "pwr_ifb_en_cmd",       (cmdFunction)pwr_ifb_en_cmd },
    { "pwr_io_en_cmd",        (cmdFunction)pwr_io_en_cmd },
    { "pwr_pzp_en_cmd",       (cmdFunction)pwr_pzp_en_cmd },
    { "pwr_htr_en_cmd",       (cmdFunction)pwr_htr_en_cmd },
    { "pwr_sln_tec_en_cmd",   (cmdFunction)pwr_sln_tec_en_cmd },
    { "pwr_lda_en_cmd",       (cmdFunction)pwr_lda_en_cmd },
    { "pwr_pda_en_cmd",       (cmdFunction)pwr_pda_en_cmd },
    { "pwr_usb_0_en_cmd",     (cmdFunction)pwr_usb_0_en_cmd },
    { "pwr_usb_1_en_cmd",     (cmdFunction)pwr_usb_1_en_cmd },
    { "i2c_s_1_en_cmd",       (cmdFunction)i2c_s_1_en_cmd },
    { "i2c_s_2_en_cmd",       (cmdFunction)i2c_s_2_en_cmd },
    { "i2c_pwm_en_cmd",       (cmdFunction)i2c_pwm_en_cmd },
    { "i2c_hd4_en_cmd",       (cmdFunction)i2c_hd4_en_cmd },
    { "i2c_ld_1_en_cmd",      (cmdFunction)i2c_ld_1_en_cmd },
    { "i2c_ld_2_en_cmd",      (cmdFunction)i2c_ld_2_en_cmd },
    { "tec_0_en_cmd",         (cmdFunction)tec_0_en_cmd },
    { "tec_1_en_cmd",         (cmdFunction)tec_1_en_cmd },
    { "tec_2_en_cmd",         (cmdFunction)tec_2_en_cmd },
    { "tec_3_en_cmd",         (cmdFunction)tec_3_en_cmd },
    { "usb_led_1_set_cmd",    (cmdFunction)usb_led_1_set_cmd },
    { "usb_led_2_set_cmd",    (cmdFunction)usb_led_2_set_cmd },
    { "htr_0_set_cmd",        (cmdFunction)htr_0_set_cmd },
    { "htr_1_set_cmd",        (cmdFunction)htr_1_set_cmd },
    { "htr_2_set_cmd",        (cmdFunction)htr_2_set_cmd },
    { "htr_3_set_cmd",        (cmdFunction)htr_3_set_cmd },
    { "htr_4_set_cmd",        (cmdFunction)htr_4_set_cmd },
    { "htr_5_set_cmd",        (cmdFunction)htr_5_set_cmd },
    { "htr_6_set_cmd",        (cmdFunction)htr_6_set_cmd },
    { "htr_7_set_cmd",        (cmdFunction)htr_7_set_cmd },
    { "custom_cmd_cmd",       (cmdFunction)custom_cmd_cmd }
};
/* ==================== TABLE 2 ==================== */
cmdFunction_entry_t cmd_table_2[] = {
    { "pump_1_ctl_cmd",       (cmdFunction)pump_1_ctl_cmd },
    { "pump_1_volt_cmd",      (cmdFunction)pump_1_volt_cmd },
    { "pump_1_freq_cmd",      (cmdFunction)pump_1_freq_cmd },
    { "pump_2_ctl_cmd",       (cmdFunction)pump_2_ctl_cmd },
    { "pump_2_volt_cmd",      (cmdFunction)pump_2_volt_cmd },
    { "pump_2_freq_cmd",      (cmdFunction)pump_2_freq_cmd },

    { "sln_0_ctl_cmd",        (cmdFunction)sln_0_ctl_cmd },
    { "sln_1_ctl_cmd",        (cmdFunction)sln_1_ctl_cmd },
    { "sln_2_ctl_cmd",        (cmdFunction)sln_2_ctl_cmd },
    { "sln_3_ctl_cmd",        (cmdFunction)sln_3_ctl_cmd },
    { "sln_4_ctl_cmd",        (cmdFunction)sln_4_ctl_cmd },
    { "sln_5_ctl_cmd",        (cmdFunction)sln_5_ctl_cmd },
    { "sln_6_ctl_cmd",        (cmdFunction)sln_6_ctl_cmd },
    { "sln_7_ctl_cmd",        (cmdFunction)sln_7_ctl_cmd },
    { "sln_8_ctl_cmd",        (cmdFunction)sln_8_ctl_cmd },
    { "sln_9_ctl_cmd",        (cmdFunction)sln_9_ctl_cmd },
    { "sln_10_ctl_cmd",       (cmdFunction)sln_10_ctl_cmd },
    { "sln_11_ctl_cmd",       (cmdFunction)sln_11_ctl_cmd },

    { "sln_valve_1_ctl_cmd",  (cmdFunction)sln_valve_1_ctl_cmd },
    { "sln_valve_2_ctl_cmd",  (cmdFunction)sln_valve_2_ctl_cmd }
};

/* ==================== TABLE 3 ==================== */
cmdFunction_entry_t cmd_table_3[] = {
    { "temp_master_en_cmd",    (cmdFunction)temp_master_en_cmd },
    { "temp_p_1_en_cmd",       (cmdFunction)temp_p_1_en_cmd },
    { "temp_p_2_en_cmd",       (cmdFunction)temp_p_2_en_cmd },
    { "temp_p_3_en_cmd",       (cmdFunction)temp_p_3_en_cmd },
    { "temp_p_4_en_cmd",       (cmdFunction)temp_p_4_en_cmd },
    { "temp_p_5_en_cmd",       (cmdFunction)temp_p_5_en_cmd },
    { "temp_p_6_en_cmd",       (cmdFunction)temp_p_6_en_cmd },
    { "temp_p_1_setpoint_cmd", (cmdFunction)temp_p_1_setpoint_cmd },
    { "temp_p_1_ntcp_cmd",     (cmdFunction)temp_p_1_ntcp_cmd },
    { "temp_p_1_ntcs_cmd",     (cmdFunction)temp_p_1_ntcs_cmd },
    { "temp_p_1_htr_cmd",      (cmdFunction)temp_p_1_htr_cmd },
    { "temp_p_1_tec_cmd",      (cmdFunction)temp_p_1_tec_cmd },
    { "temp_p_2_setpoint_cmd", (cmdFunction)temp_p_2_setpoint_cmd },
    { "temp_p_2_ntcp_cmd",     (cmdFunction)temp_p_2_ntcp_cmd },
    { "temp_p_2_ntcs_cmd",     (cmdFunction)temp_p_2_ntcs_cmd },
    { "temp_p_2_htr_cmd",      (cmdFunction)temp_p_2_htr_cmd },
    { "temp_p_2_tec_cmd",      (cmdFunction)temp_p_2_tec_cmd },
    { "temp_p_3_setpoint_cmd", (cmdFunction)temp_p_3_setpoint_cmd },
    { "temp_p_3_ntcp_cmd",     (cmdFunction)temp_p_3_ntcp_cmd },
    { "temp_p_3_ntcs_cmd",     (cmdFunction)temp_p_3_ntcs_cmd },
    { "temp_p_3_htr_cmd",      (cmdFunction)temp_p_3_htr_cmd },
    { "temp_p_3_tec_cmd",      (cmdFunction)temp_p_3_tec_cmd },
    { "temp_p_4_setpoint_cmd", (cmdFunction)temp_p_4_setpoint_cmd },
    { "temp_p_4_ntcp_cmd",     (cmdFunction)temp_p_4_ntcp_cmd },
    { "temp_p_4_ntcs_cmd",     (cmdFunction)temp_p_4_ntcs_cmd },
    { "temp_p_4_htr_cmd",      (cmdFunction)temp_p_4_htr_cmd },
    { "temp_p_4_tec_cmd",      (cmdFunction)temp_p_4_tec_cmd },
    { "temp_p_5_setpoint_cmd", (cmdFunction)temp_p_5_setpoint_cmd },
    { "temp_p_5_ntcp_cmd",     (cmdFunction)temp_p_5_ntcp_cmd },
    { "temp_p_5_ntcs_cmd",     (cmdFunction)temp_p_5_ntcs_cmd },
    { "temp_p_5_htr_cmd",      (cmdFunction)temp_p_5_htr_cmd },
    { "temp_p_5_tec_cmd",      (cmdFunction)temp_p_5_tec_cmd },
    { "temp_p_6_setpoint_cmd", (cmdFunction)temp_p_6_setpoint_cmd },
    { "temp_p_6_ntcp_cmd",     (cmdFunction)temp_p_6_ntcp_cmd },
    { "temp_p_6_ntcs_cmd",     (cmdFunction)temp_p_6_ntcs_cmd },
    { "temp_p_6_htr_cmd",      (cmdFunction)temp_p_6_htr_cmd },
    { "temp_p_6_tec_cmd",      (cmdFunction)temp_p_6_tec_cmd }
};



/* ==================== TABLE 5 ==================== */
cmdFunction_entry_t cmd_table_5[] = {
    { "test_ls_current_cmd",   (cmdFunction)test_ls_current_cmd },
    { "test_fluidic_seq_cmd",  (cmdFunction)test_fluidic_seq_cmd },
    { "exp_fluidic_seq_cmd",   (cmdFunction)exp_fluidic_seq_cmd },
    { "exp_mon_start_cmd",     (cmdFunction)exp_mon_start_cmd },
    { "exp_mon_delay_cmd",     (cmdFunction)exp_mon_delay_cmd },
    { "exp_mon_interval_cmd",  (cmdFunction)exp_mon_interval_cmd },
    { "dls_ls_intensity_cmd",  (cmdFunction)dls_ls_intensity_cmd },
    { "cam_ls_intensity_cmd",  (cmdFunction)cam_ls_intensity_cmd },
    { "exp_samp_rate_cmd",     (cmdFunction)exp_samp_rate_cmd },
    { "exp_pre_time_cmd",      (cmdFunction)exp_pre_time_cmd },
    { "exp_samp_time_cmd",     (cmdFunction)exp_samp_time_cmd },
    { "exp_post_time_cmd",     (cmdFunction)exp_post_time_cmd },
    { "custom_ctl_cmd",        (cmdFunction)custom_ctl_cmd }
};
