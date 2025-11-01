
#include "m33_command.h"
#include "rtos.h"
/* Mảng chứa các hàm trong TABLE 1 */
/* ==================== TABLE 1 ==================== */
cmdFunction_entry_t cmd_table_1[] = {
    { "time_sync",        (cmdFunction)time_sync_cmd },
    { "pwr_ifb_en",       (cmdFunction)pwr_ifb_en_cmd },
    { "pwr_io_en",        (cmdFunction)pwr_io_en_cmd },
    { "pwr_pzp_en",       (cmdFunction)pwr_pzp_en_cmd },
    { "pwr_htr_en",       (cmdFunction)pwr_htr_en_cmd },
    { "pwr_sln_tec_en",   (cmdFunction)pwr_sln_tec_en_cmd },
    { "pwr_lda_en",       (cmdFunction)pwr_lda_en_cmd },
    { "pwr_pda_en",       (cmdFunction)pwr_pda_en_cmd },
    { "pwr_usb_0_en",     (cmdFunction)pwr_usb_0_en_cmd },
    { "pwr_usb_1_en",     (cmdFunction)pwr_usb_1_en_cmd },
    { "i2c_s_1_en",       (cmdFunction)i2c_s_1_en_cmd },
    { "i2c_s_2_en",       (cmdFunction)i2c_s_2_en_cmd },
    { "i2c_pwm_en",       (cmdFunction)i2c_pwm_en_cmd },
    { "i2c_hd4_en",       (cmdFunction)i2c_hd4_en_cmd },
    { "i2c_ld_1_en",      (cmdFunction)i2c_ld_1_en_cmd },
    { "i2c_ld_2_en",      (cmdFunction)i2c_ld_2_en_cmd },
    { "tec_0_en",         (cmdFunction)tec_0_en_cmd },
    { "tec_1_en",         (cmdFunction)tec_1_en_cmd },
    { "tec_2_en",         (cmdFunction)tec_2_en_cmd },
    { "tec_3_en",         (cmdFunction)tec_3_en_cmd },
    { "usb_led_1_set",    (cmdFunction)usb_led_1_set_cmd },
    { "usb_led_2_set",    (cmdFunction)usb_led_2_set_cmd },
    { "htr_0_set",        (cmdFunction)htr_0_set_cmd },
    { "htr_1_set",        (cmdFunction)htr_1_set_cmd },
    { "htr_2_set",        (cmdFunction)htr_2_set_cmd },
    { "htr_3_set",        (cmdFunction)htr_3_set_cmd },
    { "htr_4_set",        (cmdFunction)htr_4_set_cmd },
    { "htr_5_set",        (cmdFunction)htr_5_set_cmd },
    { "htr_6_set",        (cmdFunction)htr_6_set_cmd },
    { "htr_7_set",        (cmdFunction)htr_7_set_cmd },
    { "custom_cmd",       (cmdFunction)custom_cmd_cmd },

/* ==================== TABLE 2 ==================== */

    { "pump_1_ctl",       (cmdFunction)pump_1_ctl_cmd },
    { "pump_1_volt",      (cmdFunction)pump_1_volt_cmd },
    { "pump_1_freq",      (cmdFunction)pump_1_freq_cmd },
    { "pump_2_ctl",       (cmdFunction)pump_2_ctl_cmd },
    { "pump_2_volt",      (cmdFunction)pump_2_volt_cmd },
    { "pump_2_freq",      (cmdFunction)pump_2_freq_cmd },

    { "sln_0_ctl",        (cmdFunction)sln_0_ctl_cmd },
    { "sln_1_ctl",        (cmdFunction)sln_1_ctl_cmd },
    { "sln_2_ctl",        (cmdFunction)sln_2_ctl_cmd },
    { "sln_3_ctl",        (cmdFunction)sln_3_ctl_cmd },
    { "sln_4_ctl",        (cmdFunction)sln_4_ctl_cmd },
    { "sln_5_ctl",        (cmdFunction)sln_5_ctl_cmd },
    { "sln_6_ctl",        (cmdFunction)sln_6_ctl_cmd },
    { "sln_7_ctl",        (cmdFunction)sln_7_ctl_cmd },
    { "sln_8_ctl",        (cmdFunction)sln_8_ctl_cmd },
    { "sln_9_ctl",        (cmdFunction)sln_9_ctl_cmd },
    { "sln_10_ctl",       (cmdFunction)sln_10_ctl_cmd },
    { "sln_11_ctl",       (cmdFunction)sln_11_ctl_cmd },

    { "sln_valve_1_ctl",  (cmdFunction)sln_valve_1_ctl_cmd },
    { "sln_valve_2_ctl",  (cmdFunction)sln_valve_2_ctl_cmd },


/* ==================== TABLE 3 ==================== */

    { "temp_master_en",    (cmdFunction)temp_master_en_cmd },
    { "temp_p_1_en",       (cmdFunction)temp_p_1_en_cmd },
    { "temp_p_2_en",       (cmdFunction)temp_p_2_en_cmd },
    { "temp_p_3_en",       (cmdFunction)temp_p_3_en_cmd },
    { "temp_p_4_en",       (cmdFunction)temp_p_4_en_cmd },
    { "temp_p_5_en",       (cmdFunction)temp_p_5_en_cmd },
    { "temp_p_6_en",       (cmdFunction)temp_p_6_en_cmd },
    { "temp_p_1_setpoint", (cmdFunction)temp_p_1_setpoint_cmd },
    { "temp_p_1_ntcp",     (cmdFunction)temp_p_1_ntcp_cmd },
    { "temp_p_1_ntcs",     (cmdFunction)temp_p_1_ntcs_cmd },
    { "temp_p_1_htr",      (cmdFunction)temp_p_1_htr_cmd },
    { "temp_p_1_tec",      (cmdFunction)temp_p_1_tec_cmd },
    { "temp_p_2_setpoint", (cmdFunction)temp_p_2_setpoint_cmd },
    { "temp_p_2_ntcp",     (cmdFunction)temp_p_2_ntcp_cmd },
    { "temp_p_2_ntcs",     (cmdFunction)temp_p_2_ntcs_cmd },
    { "temp_p_2_htr",      (cmdFunction)temp_p_2_htr_cmd },
    { "temp_p_2_tec",      (cmdFunction)temp_p_2_tec_cmd },
    { "temp_p_3_setpoint", (cmdFunction)temp_p_3_setpoint_cmd },
    { "temp_p_3_ntcp",     (cmdFunction)temp_p_3_ntcp_cmd },
    { "temp_p_3_ntcs",     (cmdFunction)temp_p_3_ntcs_cmd },
    { "temp_p_3_htr",      (cmdFunction)temp_p_3_htr_cmd },
    { "temp_p_3_tec",      (cmdFunction)temp_p_3_tec_cmd },
    { "temp_p_4_setpoint", (cmdFunction)temp_p_4_setpoint_cmd },
    { "temp_p_4_ntcp",     (cmdFunction)temp_p_4_ntcp_cmd },
    { "temp_p_4_ntcs",     (cmdFunction)temp_p_4_ntcs_cmd },
    { "temp_p_4_htr",      (cmdFunction)temp_p_4_htr_cmd },
    { "temp_p_4_tec",      (cmdFunction)temp_p_4_tec_cmd },
    { "temp_p_5_setpoint", (cmdFunction)temp_p_5_setpoint_cmd },
    { "temp_p_5_ntcp",     (cmdFunction)temp_p_5_ntcp_cmd },
    { "temp_p_5_ntcs",     (cmdFunction)temp_p_5_ntcs_cmd },
    { "temp_p_5_htr",      (cmdFunction)temp_p_5_htr_cmd },
    { "temp_p_5_tec",      (cmdFunction)temp_p_5_tec_cmd },
    { "temp_p_6_setpoint", (cmdFunction)temp_p_6_setpoint_cmd },
    { "temp_p_6_ntcp",     (cmdFunction)temp_p_6_ntcp_cmd },
    { "temp_p_6_ntcs",     (cmdFunction)temp_p_6_ntcs_cmd },
    { "temp_p_6_htr",      (cmdFunction)temp_p_6_htr_cmd },
    { "temp_p_6_tec",      (cmdFunction)temp_p_6_tec_cmd },




/* ==================== TABLE 5 ==================== */

    { "test_ls_current",   (cmdFunction)test_ls_current_cmd },
    { "test_fluidic_seq",  (cmdFunction)test_fluidic_seq_cmd },
    { "exp_fluidic_seq",   (cmdFunction)exp_fluidic_seq_cmd },
    { "exp_mon_start",     (cmdFunction)exp_mon_start_cmd },
    { "exp_mon_delay",     (cmdFunction)exp_mon_delay_cmd },
    { "exp_mon_interval",  (cmdFunction)exp_mon_interval_cmd },
    { "dls_ls_intensity",  (cmdFunction)dls_ls_intensity_cmd },
    { "cam_ls_intensity",  (cmdFunction)cam_ls_intensity_cmd },
    { "exp_samp_rate",     (cmdFunction)exp_samp_rate_cmd },
    { "exp_pre_time",      (cmdFunction)exp_pre_time_cmd },
    { "exp_samp_time",     (cmdFunction)exp_samp_time_cmd },
    { "exp_post_time",     (cmdFunction)exp_post_time_cmd },
    { "custom_ctl",        (cmdFunction)custom_ctl_cmd },
    { "table_val_get",     (cmdFunction)table_val_get }

};
const int bee_table1_count = sizeof(cmd_table_1) / sizeof(cmd_table_1[0]);
