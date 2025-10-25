/*
 * bsp_photo.h
 *
 *  Created on: Oct 18, 2025
 *      Author: HTSANG
 */

#ifndef BSP_BSP_PHOTO_H_
#define BSP_BSP_PHOTO_H_

#include "adg1414.h"

void bsp_photo_init(void);

void bsp_photo_int_sw_on(uint8_t channel);
void bsp_photo_int_sw_off(uint8_t channel);
void bsp_photo_int_all_sw_off(void);

void bsp_photo_setup_timmer(uint32_t sampling_rate_khz);
void bsp_photo_start_timer(void);
void bsp_photo_stop_timer(void);

uint32_t bsp_photo_spi_irq_init(void);
uint32_t bsp_photo_spi_irq_deinit(void);

#endif /* BSP_BSP_PHOTO_H_ */
