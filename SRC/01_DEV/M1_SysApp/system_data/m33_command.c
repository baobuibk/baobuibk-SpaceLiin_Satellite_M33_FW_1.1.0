/*
 * m33_command.c
 *
 *  Created on: Oct 24, 2025
 *      Author: Admin
 */


#include "m33_data.h"
#include "m33_command.h"
#include "cmdline.h"
#include <stdio.h>
#include "simple_shell.h"
#include "config.h"
#include <stdint.h>
#include "printf.h"

int str2int(char *str, int32_t *ret) {

    if (!str || !ret) return 1;
    if (str[0] == '\0') return 1;  // chuỗi rỗng


    int neg = 0;
    const char *p = str;

    // Kiểm tra dấu âm
    if (*p == '-') {
        neg = 1;
        p++;
        if (*p == '\0') return 1;  // chỉ có dấu '-'
    }

    // Giới hạn 8 chữ số (không tính dấu)
    size_t digit_count = 0;
    const char *q = p;
    while (*q) {
        if (*q < '0' || *q > '9') return 1; // ký tự không phải số
        digit_count++;
        if (digit_count > 20) return 1;      // vượt quá 8 chữ số
        q++;
    }

    const uint32_t LIMIT = neg ? 2147483648u : 2147483647u;
    const uint32_t LIM_DIV10 = LIMIT / 10u;
    const uint32_t LIM_MOD10 = LIMIT % 10u;

    uint32_t mag = 0;

    while (*p) {
        uint32_t digit = (uint32_t)(*p++ - '0');
        if (mag > LIM_DIV10) return 1;
        if (mag == LIM_DIV10 && digit > LIM_MOD10) return 1;
        mag = mag * 10u + digit;
    }

    if (neg) {
        if (mag == 2147483648u)
            *ret = (int32_t)0x80000000; // INT32_MIN
        else
            *ret = -(int32_t)mag;
    } else {
        *ret = (int32_t)mag;
    }

    return 0;
}

void stdio_out(uint32_t stdio, char * buffer)
{
    if (stdio == UART_CONSOLE)
        {Shell_WriteString(buffer);}
    else if (stdio == TTY_CONSOLE)   {}
}
/* ==================== TABLE 1: implementations ==================== */
/*
 * format time_sync epoch 1761286946
 */
static char response_buffer[256];
void __attribute__((weak)) stdio_out(uint32_t stdio, char * buffer);
void time_sync_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{ 	int32_t time ;
	uint32_t ret = str2int(argv[1], &time);
	if (ret) return;
	m33_data_set_u(TABLE_ID_1,time_sync,(uint32_t)time);
	sprintf(response_buffer,"OK\r\n");
	stdio_out(stdio,response_buffer);
	return;
}

void pwr_ifb_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("pwr_ifb_en\n"); }
void pwr_io_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("pwr_io_en\n"); }
void pwr_pzp_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("pwr_pzp_en\n"); }
void pwr_htr_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("pwr_htr_en\n"); }
void pwr_sln_tec_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]) { (void)stdio;(void)argc;(void)argv; printf("pwr_sln_tec_en\n"); }
void pwr_lda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("pwr_lda_en\n"); }
void pwr_pda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("pwr_pda_en\n"); }
void pwr_usb_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])   { (void)stdio;(void)argc;(void)argv; printf("pwr_usb_0_en\n"); }
void pwr_usb_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])   { (void)stdio;(void)argc;(void)argv; printf("pwr_usb_1_en\n"); }
void i2c_s_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("i2c_s_1_en\n"); }
void i2c_s_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("i2c_s_2_en\n"); }
void i2c_pwm_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("i2c_pwm_en\n"); }
void i2c_hd4_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("i2c_hd4_en\n"); }
void i2c_ld_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])    { (void)stdio;(void)argc;(void)argv; printf("i2c_ld_1_en\n"); }
void i2c_ld_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])    { (void)stdio;(void)argc;(void)argv; printf("i2c_ld_2_en\n"); }
void tec_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       { (void)stdio;(void)argc;(void)argv; printf("tec_0_en\n"); }
void tec_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       { (void)stdio;(void)argc;(void)argv; printf("tec_1_en\n"); }
void tec_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       { (void)stdio;(void)argc;(void)argv; printf("tec_2_en\n"); }
void tec_3_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       { (void)stdio;(void)argc;(void)argv; printf("tec_3_en\n"); }
void usb_led_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])  { (void)stdio;(void)argc;(void)argv; printf("usb_led_1_set\n"); }
void usb_led_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])  { (void)stdio;(void)argc;(void)argv; printf("usb_led_2_set\n"); }
void htr_0_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_0_set\n"); }
void htr_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_1_set\n"); }
void htr_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_2_set\n"); }
void htr_3_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_3_set\n"); }
void htr_4_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_4_set\n"); }
void htr_5_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_5_set\n"); }
void htr_6_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_6_set\n"); }
void htr_7_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      { (void)stdio;(void)argc;(void)argv; printf("htr_7_set\n"); }
void custom_cmd_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; printf("custom_cmd\n"); }

/* ==================== TABLE 2: implementations ==================== */
void pump_1_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_1_ctl\n");}
void pump_1_volt_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_1_volt\n");}
void pump_1_freq_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_1_freq\n");}
void pump_2_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_2_ctl\n");}
void pump_2_volt_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_2_volt\n");}
void pump_2_freq_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("pump_2_freq\n");}
void sln_0_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_0_ctl\n");}
void sln_1_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_1_ctl\n");}
void sln_2_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_2_ctl\n");}
void sln_3_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_3_ctl\n");}
void sln_4_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_4_ctl\n");}
void sln_5_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_5_ctl\n");}
void sln_6_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_6_ctl\n");}
void sln_7_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_7_ctl\n");}
void sln_8_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_8_ctl\n");}
void sln_9_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_9_ctl\n");}
void sln_10_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_10_ctl\n");}
void sln_11_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_11_ctl\n");}
void sln_valve_1_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_valve_1_ctl\n");}
void sln_valve_2_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;printf("sln_valve_2_ctl\n");}

/* ==================== TABLE 3: implementations ==================== */
void temp_master_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_master_en\n");}
void temp_p_1_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_en\n");}
void temp_p_2_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_en\n");}
void temp_p_3_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_en\n");}
void temp_p_4_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_en\n");}
void temp_p_5_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_en\n");}
void temp_p_6_en_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_en\n");}
void temp_p_1_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_setpoint\n");}
void temp_p_1_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_ntcp\n");}
void temp_p_1_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_ntcs\n");}
void temp_p_1_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_htr\n");}
void temp_p_1_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_1_tec\n");}
void temp_p_2_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_setpoint\n");}
void temp_p_2_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_ntcp\n");}
void temp_p_2_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_ntcs\n");}
void temp_p_2_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_htr\n");}
void temp_p_2_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_2_tec\n");}
void temp_p_3_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_setpoint\n");}
void temp_p_3_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_ntcp\n");}
void temp_p_3_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_ntcs\n");}
void temp_p_3_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_htr\n");}
void temp_p_3_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_3_tec\n");}
void temp_p_4_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_setpoint\n");}
void temp_p_4_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_ntcp\n");}
void temp_p_4_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_ntcs\n");}
void temp_p_4_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_htr\n");}
void temp_p_4_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_4_tec\n");}
void temp_p_5_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_setpoint\n");}
void temp_p_5_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_ntcp\n");}
void temp_p_5_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_ntcs\n");}
void temp_p_5_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_htr\n");}
void temp_p_5_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_5_tec\n");}
void temp_p_6_setpoint_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_setpoint\n");}
void temp_p_6_ntcp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_ntcp\n");}
void temp_p_6_ntcs_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_ntcs\n");}
void temp_p_6_htr_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_htr\n");}
void temp_p_6_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_p_6_tec\n");}

/* ==================== TABLE 5: implementations ==================== */
void test_ls_current_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("test_ls_current\n");}
void test_fluidic_seq_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("test_fluidic_seq\n");}
void exp_fluidic_seq_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_fluidic_seq\n");}
void exp_mon_start_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_mon_start\n");}
void exp_mon_delay_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_mon_delay\n");}
void exp_mon_interval_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_mon_interval\n");}
void dls_ls_intensity_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("dls_ls_intensity\n");}
void cam_ls_intensity_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("cam_ls_intensity\n");}
void exp_samp_rate_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_samp_rate\n");}
void exp_pre_time_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_pre_time\n");}
void exp_samp_time_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_samp_time\n");}
void exp_post_time_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("exp_post_time\n");}
void custom_ctl_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("custom_ctl\n");}

/* ==================== TABLE 6: implementations ==================== */
void sys_status_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("sys_status\n");}
void boot_cnt_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("boot_cnt\n");}
void temp_exp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_exp\n");}
void temp_ntc_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_0\n");}
void temp_ntc_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_1\n");}
void temp_ntc_2_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_2\n");}
void temp_ntc_3_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_3\n");}
void temp_ntc_4_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_4\n");}
void temp_ntc_5_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_5\n");}
void temp_ntc_6_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_6\n");}
void temp_ntc_7_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_7\n");}
void temp_ntc_8_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_8\n");}
void temp_ntc_9_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_9\n");}
void temp_ntc_10_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_10\n");}
void temp_ntc_11_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("temp_ntc_11\n");}
void sen1_data_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("sen1_data_0\n");}
void sen1_data_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("sen1_data_1\n");}
void sen2_data_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("sen2_data_0\n");}
void sen2_data_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("sen2_data_1\n");}
void current_12_tot_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_12_tot\n");}
void current_12_lda_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_12_lda\n");}
void current_12_pda_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_12_pda\n");}
void current_5_io_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_5_io\n");}
void current_5_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_5_tec\n");}
void current_5_cam_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_5_cam\n");}
void current_5_hd_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;printf("current_5_hd\n");}
