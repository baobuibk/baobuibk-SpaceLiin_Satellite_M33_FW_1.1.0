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

static uint8_t map_int_heater_position(int x);
static uint8_t map_usb_led_position(int x);

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
	{
		pca9685_set_duty_permille(&pwmDev, channel, 0); //disable all output
	}
	
	pca9685_set_duty_permille(&pwmDev, 4, 1000); //disable usb led
	pca9685_set_duty_permille(&pwmDev, 3, 1000); //disable usb led
	pca9685_set_duty_permille(&pwmDev, 14, 1000); //disable usb led
	pca9685_set_duty_permille(&pwmDev, 15, 1000); //disable usb led

	bsp_heater_enable();
}

void bsp_heater_turnoff(uint32_t channel)
{
	uint8_t real_channel = map_int_heater_position(channel);

	if (real_channel == 50)
	{
		return;
	}	

	pca9685_set_duty_permille(&pwmDev, real_channel, 0);
}

void bsp_heater_turnon(uint32_t channel, uint32_t duty)
{
	uint8_t real_channel = map_int_heater_position(channel);

	if (real_channel == 50)
	{
		return;
	}

	pca9685_set_duty_percent(&pwmDev, real_channel, duty);
}

void bsp_heater_list_turnoff(uint32_t heaters)	//error, turn off all heaters in the list, (0x03 mean heater 0,1)
{
	uint32_t index;


	for (index = 0; index < 8; index++)
	{
		if (heaters & (1 << index))
		{
			bsp_heater_turnoff(index + 1);
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
			bsp_heater_turnon(index + 1, duty);
		}
	}
}

void bsp_usb_led_turnoff(uint32_t channel)
{
	uint8_t real_channel = map_usb_led_position(channel);

	if (real_channel == 50)
	{
		return;
	}	

	pca9685_set_duty_percent(&pwmDev, real_channel, 100);
}

void bsp_usb_led_turnon(uint32_t channel, uint32_t duty)
{
	uint8_t real_channel = map_usb_led_position(channel);
	uint32_t real_duty = 100 - duty;

	if (real_channel == 50)
	{
		return;
	}

	pca9685_set_duty_percent(&pwmDev, real_channel, real_duty);
}

void bsp_usb_led_list_turnoff(uint32_t heaters)	//error, turn off all heaters in the list, (0x03 mean heater 0,1)
{
	uint32_t index;


	for (index = 0; index < 2; index++)
	{
		if (heaters & (1 << index))
		{
			bsp_usb_led_turnoff(index + 1);
		}
	}

}
void bsp_usb_led_list_turnon(uint32_t heaters, uint32_t duty)
{
	uint32_t index;


	for (index = 0; index < 2; index++)
	{
		if (heaters & (1 << index))
		{	
			bsp_usb_led_turnon(index + 1, duty);
		}
	}
}

static uint8_t map_int_heater_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        2, 1, 0, 12, 8, 7, 6, 5
    };

    if (x < 1 || x > 8)
	{
		return 50;
	}

    return map[x];
}

static uint8_t map_usb_led_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        4, 3, 14, 15
    };

    if (x < 1 || x > 4)
	{
		return 50;
	}

    return map[x];
}