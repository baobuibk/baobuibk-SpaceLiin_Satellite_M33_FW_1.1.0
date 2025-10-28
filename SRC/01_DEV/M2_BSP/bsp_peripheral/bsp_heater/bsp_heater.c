/*
 * bsp_heater.c
 *
 *  Created on: Oct 16, 2025
 *      Author: Admin
 */
#include "pca9685.h"
#include "tca6416.h"

// #include "i2c.h"
#include "do.h"

#include "bsp_expander.h"
#include "bsp_heater.h"

extern i2c_io_t io_expander_i2c;

pca9685_t pwmDev = {
		.bus = &io_expander_i2c,
		.addr7 = 0x40
};

void bsp_heater_enable()
{
	bsp_expander_ctrl(PWM_I2C_nOE, LOW);
}
void bsp_heater_disable()
{
	bsp_expander_ctrl(PWM_I2C_nOE, HIGH);
}

void bsp_heater_init()
{

	pca9685_init_min(&pwmDev,&io_expander_i2c,0x40);
	pca9685_set_freq_hz(&pwmDev, 100);
	for (uint32_t channel = 0;channel < 16; channel++)
		pca9685_set_duty_permille(&pwmDev, channel, 0); //disable all output
	bsp_heater_enable();

}
void bsp_heater_turnoff(uint32_t channel)
{
	pca9685_set_duty_permille(&pwmDev, channel, 0);
}
void bsp_heater_turnon(uint32_t channel, uint32_t duty)
{
	pca9685_set_duty_percent(&pwmDev, channel, duty);
}

void bsp_heater_set_freq_hz(uint16_t freq_hz)
{
	pca9685_set_freq_hz(&pwmDev, freq_hz);
}

void bsp_heater_list_turnoff(uint32_t heaters)	//error, turn off all heaters in the list, (0x03 mean heater 0,1)
{
	uint32_t index;
	for (index = 0; index < 8; index++)
	{
		if (heaters & (1 << index))
		{
			bsp_heater_turnoff(index);
		}
	}

}
void bsp_heater_list_turnon(uint32_t heaters, uint32_t duty)
{
	uint32_t index;
	for (index = 0; index < 8; index++)
	{
		if (heaters & (1 << index))
		{
			bsp_heater_turnon(index, duty);
		}
	}
}
