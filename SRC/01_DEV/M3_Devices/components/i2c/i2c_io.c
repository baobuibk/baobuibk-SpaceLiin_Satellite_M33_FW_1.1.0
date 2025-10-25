/*
 * i2c.c
 *
 *  Created on: Oct 16, 2025
 *      Author: Admin
 */

#include "i2c_io.h"

uint32_t i2c_io_init(i2c_io_t *me)
{
	return osSemaphoreCreate(&me->lock);
}
