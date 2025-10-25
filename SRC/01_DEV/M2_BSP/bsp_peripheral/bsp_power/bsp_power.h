/*
 * bsp_power.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Admin
 */

#ifndef BSP_BSP_POWER_H_
#define BSP_BSP_POWER_H_


void bsp_power_IO_ctrl(uint32_t status);
void bsp_power_12V_ctrl(uint32_t status);

uint32_t bsp_power_12V_stat();
uint32_t bsp_power_12V_current();
#endif /* BSP_BSP_POWER_H_ */
