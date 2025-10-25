/*
 * bsp_heater.h
 *
 *  Created on: Oct 16, 2025
 *      Author: Admin
 */

#ifndef BSP_BSP_HEATER_H_
#define BSP_BSP_HEATER_H_

enum
{
	HEATER_PWM_3 = 0,
	HEATER_PWM_2,
	HEATER_PWM_1,
	USB_LED_2,
	USB_LED_1,
	HEATER_PWM_8,
	HEATER_PWM_7,
	HEATER_PWM_6,
	HEATER_PWM_5 ,
	RESERVE_0,
	RESERVE_1,
	RESERVE_2,
	HEATER_PWM_4 ,
	RESERVE_3,
	USB_LED_3,
	USB_LED_4,
};

void bsp_heater_enable();
void bsp_heater_disable();

void bsp_heater_init();
void bsp_heater_turnoff(uint32_t channel);
void bsp_heater_turnon(uint32_t channel, uint32_t duty);


void bsp_heater_list_turnoff(uint32_t heaters);	//error, turn off all heaters in the list, (0x03 mean heater 0,1)
void bsp_heater_list_turnon(uint32_t heaters, uint32_t duty);

#endif /* BSP_BSP_HEATER_H_ */
