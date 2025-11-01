/*
 * m33_command.c
 *
 *  Created on: Oct 24, 2025
 *      Author: Admin
 */

#include "bsp_core.h"
#include "m33_data.h"
#include "m33_command.h"
#include "cmdline.h"
#include <stdio.h>
#include "simple_shell.h"
#include "config.h"
#include <stdint.h>
#include "fsl_debug_console.h"
#include "bsp_expander.h"
#include "bsp_heater.h"
#include "bsp_solenoid.h"
#include "bsp_pump.h"
#include "bsp_laser.h"
#include "do.h"

#include "rtos_tasks.h"
#include "rtos.h"


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
extern cmdFunction_entry_t cmd_table_1[];

extern const int bee_table1_count;
void Command_Process(char *input) {
    char buffer_copy[SHELL_BUFFER_SIZE];
    cmdFunction_entry_t * p_cmd;
    strncpy(buffer_copy, input, SHELL_BUFFER_SIZE);
    buffer_copy[SHELL_BUFFER_SIZE - 1] = '\0';

    char *args[SHELL_MAX_ARGS + 1];
    int argc = 0;

    args[argc] = strtok(buffer_copy, " ");
    while (args[argc] != NULL && argc < SHELL_MAX_ARGS - 1) {
        args[++argc] = strtok(NULL, " ");
    }
    if (argc > 0) {
        for (uint32_t i = 0; i < bee_table1_count; i++)
        {
            PRINTF("\r\nsearch for %s", args[0]);
            p_cmd = &cmd_table_1[i];
            if (!strcmp(args[0], p_cmd->name))
            {
                 PRINTF("\r\nsFOUND at %d\r\n", i);
                 p_cmd->func(0,argc, args);
                return;
            }
       
        }


    }
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
//static char response_buffer[256];
void __attribute__((weak)) stdio_out(uint32_t stdio, char * buffer);
void time_sync_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{ 	int32_t time ;
	uint32_t ret = str2int(argv[1], &time);
	if (ret) return;
	m33_data_set_epoch((uint32_t)time);
	return;
}

void pwr_ifb_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])    
 { (void)stdio;(void)argc;(void)argv;  }//PRINTF("pwr_ifb_en\r\n"); }

void pwr_io_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("\r\npwr_io_en\r\n");
}

void pwr_pzp_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])    
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_pzp_en\n");
    
    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }
    
    bsp_expander_ctrl(POW_ONOFF_TEC,status);
}

void pwr_htr_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])   
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_htr_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }
    
    bsp_expander_ctrl(POW_ONOFF_HEATER,status);
}

void pwr_sln_tec_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_sln_tec_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(POW_ONOFF_TEC,status);
}

void pwr_lda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_lda_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(POW_ONOFF_LASER,status);
    bsp_core_init_laser_dac_gpio();
}

void pwr_pda_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_pda_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(POW_ONOFF_PHOTO,status);
}

void pwr_usb_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_usb_0_en\n");

    // int32_t status;
    // uint32_t ret = str2int(argv[1], &status);

	// if (ret)
    // {
    //     return;
    // }

    // if (status)
    // {
    //     do_set(&usb_en0_gpio);
    // }
    // else
    // {
    //     do_reset(&usb_en0_gpio);
    // }
}

void pwr_usb_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[]) 
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("pwr_usb_1_en\n");

    // int32_t status;
    // uint32_t ret = str2int(argv[1], &status);

	// if (ret)
    // {
    //     return;
    // }

    // if (status)
    // {
    //     do_set(&usb_en1_gpio);
    // }
    // else
    // {
    //     do_reset(&usb_en1_gpio);
    // }
}

void i2c_s_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_s_1_en\n");
    
    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    if (status)
    {
        do_set(&sensor_en0_gpio);
    }
    else
    {
        do_reset(&sensor_en0_gpio);
    }
}

void i2c_s_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_s_2_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    if (status)
    {
        do_set(&sensor_en1_gpio);
    }
    else
    {
        do_reset(&sensor_en1_gpio);
    }
}

void i2c_pwm_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_pwm_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    uint32_t en = (status == 0) ? 1 : 0;
    bsp_expander_ctrl(PWM_I2C_nOE, en);
}

void i2c_hd4_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_hd4_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(HD4_I2C_EN, status);
}

void i2c_ld_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_ld_1_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    if (status)
    {
        do_set(&pump_en_gpio);
    }
    else
    {
        do_reset(&pump_en_gpio);
    }
}
void i2c_ld_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("i2c_ld_2_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    if (status)
    {
        do_set(&pump_en2_gpio);
    }
    else
    {
        do_reset(&pump_en2_gpio);
    }
}

void tec_0_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("tec_0_en\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(TEC_EN_1, status);
}

void tec_1_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("tec_1_en\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(TEC_EN_2, status);
}

void tec_2_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("tec_2_en\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(TEC_EN_3, status);
}

void tec_3_en_cmd(uint32_t stdio, uint32_t argc, char *argv[])       
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("tec_3_en\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_expander_ctrl(TEC_EN_4, status);
}

void usb_led_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])  
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("usb_led_1_set\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(USB_LED_1, status);
}

void usb_led_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])  
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("usb_led_2_set\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(USB_LED_2, status);
}

void htr_0_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_0_set\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_1, status);
}

void htr_1_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_1_set\n");

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_2, status);
}

void htr_2_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_2_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_3, status);
}

void htr_3_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_3_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_4, status);
}

void htr_4_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_4_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_5, status);
}

void htr_5_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_5_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_6, status);
}

void htr_6_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_6_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_7, status);
}

void htr_7_set_cmd(uint32_t stdio, uint32_t argc, char *argv[])      
{ 
    (void)stdio;(void)argc;(void)argv; //PRINTF("htr_7_set\n"); 

    int32_t status;
    uint32_t ret = str2int(argv[1], &status);

	if (ret)
    {
        return;
    }

    bsp_heater_turnon(HEATER_PWM_8, status);
}

void custom_cmd_cmd(uint32_t stdio, uint32_t argc, char *argv[])     { (void)stdio;(void)argc;(void)argv; }//PRINTF("custom_cmd\n"); }

/* ==================== TABLE 2: implementations ==================== */
void pump_1_ctl_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("pump_1_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    I2C_HD_Pump_set_enable(status);
}

void pump_1_volt_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("pump_1_volt\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    I2C_HD_Pump_set_Voltage(status);
}

void pump_1_freq_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("pump_1_freq\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    I2C_HD_Pump_Set_Freq(status);
}

void pump_2_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("pump_2_ctl\n");}
void pump_2_volt_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("pump_2_volt\n");}
void pump_2_freq_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("pump_2_freq\n");}

void sln_0_ctl_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;//PRINTF("sln_0_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    uint32_t en = (status == 0) ? 1 : 0;
    bsp_expander_ctrl(SOLENOID_CTR_1, en);
}

void sln_1_ctl_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;//PRINTF("sln_1_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    uint32_t en = (status == 0) ? 1 : 0;
    bsp_expander_ctrl(SOLENOID_CTR_2, en);
}

void sln_2_ctl_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;//PRINTF("sln_2_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    uint32_t en = (status == 0) ? 1 : 0;
    bsp_expander_ctrl(SOLENOID_CTR_3, en);
}

void sln_3_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_3_ctl\n");}
void sln_4_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_4_ctl\n");}
void sln_5_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_5_ctl\n");}
void sln_6_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_6_ctl\n");}
void sln_7_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_7_ctl\n");}
void sln_8_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_8_ctl\n");}
void sln_9_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_9_ctl\n");}
void sln_10_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_10_ctl\n");}
void sln_11_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_11_ctl\n");}

void sln_valve_1_ctl_cmd(uint32_t s, uint32_t a, char *v[])
{
    (void)s;(void)a;(void)v;//PRINTF("sln_valve_1_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    Valve_switch(status);
}
void sln_valve_2_ctl_cmd(uint32_t s, uint32_t a, char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sln_valve_2_ctl\n");}

/* ==================== TABLE 3: implementations ==================== */
void temp_master_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_master_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_master_en,(uint32_t)status);
	return;
}//PRINTF("temp_master_en\n");}
void temp_p_1_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_en\n");}
void temp_p_2_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_en\n");}
void temp_p_3_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_en\n");}
void temp_p_4_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_en\n");}
void temp_p_5_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_en\n");}
void temp_p_6_en_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_en\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_en,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_en\n");}
void temp_p_1_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_setpoint\n");}
void temp_p_1_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_ntcp\n");}
void temp_p_1_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_ntcs\n");}
void temp_p_1_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_htr\n");}
void temp_p_1_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_1_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_1_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_1_tec\n");}
void temp_p_2_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_setpoint\n");}
void temp_p_2_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_ntcp\n");}
void temp_p_2_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_ntcs\n");}
void temp_p_2_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_htr\n");}
void temp_p_2_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_2_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_2_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_2_tec\n");}
void temp_p_3_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_setpoint\n");}
void temp_p_3_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_ntcp\n");}
void temp_p_3_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_ntcs\n");}
void temp_p_3_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_htr\n");}
void temp_p_3_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_3_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_3_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_3_tec\n");}
void temp_p_4_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_setpoint\n");}
void temp_p_4_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_ntcp\n");}
void temp_p_4_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_ntcs\n");}
void temp_p_4_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_htr\n");}
void temp_p_4_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_4_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_4_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_4_tec\n");}
void temp_p_5_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_setpoint\n");}
void temp_p_5_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_ntcp\n");}
void temp_p_5_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_ntcs\n");}
void temp_p_5_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_htr\n");}
void temp_p_5_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_5_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_5_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_5_tec\n");}
void temp_p_6_setpoint_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_setpoint\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_setpoint,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_setpoint\n");}
void temp_p_6_ntcp_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_ntcp\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_ntcp,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_ntcp\n");}
void temp_p_6_ntcs_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_ntcs\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_ntcs,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_ntcs\n");}
void temp_p_6_htr_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_htr\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_htr,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_htr\n");}
void temp_p_6_tec_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("temp_p_6_tec\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_3,temp_p_6_tec,(uint32_t)status);
	return;
}//PRINTF("temp_p_6_tec\n");}

/* ==================== TABLE 5: implementations ==================== */
void test_ls_current_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("test_ls_current----------------------------------------------------------->\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,test_ls_current,(uint32_t)status);
	return;
}
void test_fluidic_seq_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("test_fluidic_seq\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,test_fluidic_seq,(uint32_t)status);
	return;
}
void exp_fluidic_seq_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("exp_fluidic_seq\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_fluidic_seq,(uint32_t)status);
	return;
}
void exp_mon_start_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("exp_mon_start\n");
    int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_mon_start,(uint32_t)status);
	return;
}
void exp_mon_delay_cmd(uint32_t s,uint32_t a,char *v[]){
    (void)s;(void)a;(void)v;PRINTF("exp_mon_delay\n");
       int32_t status ;
	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_mon_delay,(uint32_t)status);
	return;
}
void exp_mon_interval_cmd(uint32_t s,uint32_t a,char *v[]){
    (void)s;(void)a;(void)v;PRINTF("exp_mon_interval\n");
    int32_t status ;
    	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_mon_interval,(uint32_t)status);
}
void dls_ls_intensity_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;
    PRINTF("dls_ls_intensity\n");
    int32_t status ;
        	uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,dls_ls_intensity,(uint32_t)status);
}
void cam_ls_intensity_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;PRINTF("cam_ls_intensity\n");}
void exp_samp_rate_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;PRINTF("exp_samp_rate\n");
    int32_t status ;
    uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_samp_rate,(uint32_t)status);
}
void exp_pre_time_cmd(uint32_t s,uint32_t a,char *v[]){
    (void)s;(void)a;(void)v;PRINTF("exp_pre_time\n");
    int32_t status ;    
    uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_pre_time,(uint32_t)status);
}
void exp_samp_time_cmd(uint32_t s,uint32_t a,char *v[]){
    (void)s;(void)a;(void)v;PRINTF("exp_samp_time\n");
    int32_t status ;
    uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_samp_time,(uint32_t)status);
}
void exp_post_time_cmd(uint32_t s,uint32_t a,char *v[]){
    (void)s;(void)a;(void)v;PRINTF("exp_post_time\n");
    int32_t status ;
    uint32_t ret = str2int(v[1], &status);
	if (ret) return;
	m33_data_set_u_lock(TABLE_ID_5,exp_post_time,(uint32_t)status);
}
void custom_ctl_cmd(uint32_t s,uint32_t a,char *v[])
{
    (void)s;(void)a;(void)v;PRINTF("custom_ctl\n");

    int32_t status;
    uint32_t ret = str2int(v[1], &status);

	if (ret)
    {
        return;
    }

    if (status == 0)
    {
        // bsp_expander_ctrl(POW_ONOFF_LASER, 0);
        // bsp_laser_int_set_dac(0);
        // bsp_laser_int_sw_off(1);
        bsp_expander_ctrl(POW_ONOFF_HEATER, 0);
        bsp_heater_turnon(1, 0);
    }
    else
    {
        // bsp_core_init_laser_dac_gpio();

        // vTaskDelay(10);

        // bsp_expander_ctrl(POW_ONOFF_LASER, 1);

        // vTaskDelay(10);
        
        // bsp_laser_int_set_dac(127);
        // bsp_laser_int_sw_on(1);
        // bsp_heater_enable();
        bsp_expander_ctrl(POW_ONOFF_HEATER, 1);
        bsp_heater_turnon(1, 50);
    }
}

/*
    format: table_val_get table index
*/
void table_val_get (uint32_t s,uint32_t a,char *v[])
{
    (void)s;PRINTF("table_val_get\n");

    int32_t tableNum;
    int32_t tableIndex;
    uint16_t value;
    char msg_buf[100] = {0};

    uint32_t tableNumMap[] = {TABLE_ID_1, TABLE_ID_1, TABLE_ID_2,TABLE_ID_3,TABLE_ID_5, TABLE_ID_5, TABLE_ID_6};
    if (a != 3) return;
    uint32_t ret = str2int(v[1], &tableNum);
    if (ret)    return;
    if (tableNum > 6) return;

    tableNum = tableNumMap[tableNum];
    ret = str2int(v[1], &tableIndex);
    if (ret)    return;
    m33_data_get_u_lock(tableNum, tableIndex, &value);
    snprintf(msg_buf, 100, "table_val_get table_id[%d] index [%d] = %d\r\n",(int)tableNum,(int)tableIndex,(int)value);
    rpmsg_send(RPMSG_MSG_UPDATE_PARAM,msg_buf);

	if (ret)
    {
        return;
    }
}


/* ==================== TABLE 6: implementations ==================== */
void sys_status_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sys_status\n");}
void boot_cnt_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("boot_cnt\n");}
void temp_exp_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_exp\n");}
void temp_ntc_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_0\n");}
void temp_ntc_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_1\n");}
void temp_ntc_2_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_2\n");}
void temp_ntc_3_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_3\n");}
void temp_ntc_4_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_4\n");}
void temp_ntc_5_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_5\n");}
void temp_ntc_6_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_6\n");}
void temp_ntc_7_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_7\n");}
void temp_ntc_8_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_8\n");}
void temp_ntc_9_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_9\n");}
void temp_ntc_10_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_10\n");}
void temp_ntc_11_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("temp_ntc_11\n");}
void sen1_data_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sen1_data_0\n");}
void sen1_data_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sen1_data_1\n");}
void sen2_data_0_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sen2_data_0\n");}
void sen2_data_1_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("sen2_data_1\n");}
void current_12_tot_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_12_tot\n");}
void current_12_lda_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_12_lda\n");}
void current_12_pda_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_12_pda\n");}
void current_5_io_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_5_io\n");}
void current_5_tec_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_5_tec\n");}
void current_5_cam_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_5_cam\n");}
void current_5_hd_cmd(uint32_t s,uint32_t a,char *v[]){(void)s;(void)a;(void)v;}//PRINTF("current_5_hd\n");}