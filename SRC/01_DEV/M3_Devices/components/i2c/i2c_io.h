/*
 * i2c.h
 *
 *  Created on: Oct 11, 2025
 *      Author: Admin
 */

#ifndef _I2C_IO_H_
#define _I2C_IO_H_
#include "osSemphr.h"


typedef struct i2c_io_t{
	uint32_t 			ui32I2cPort;
	osSemaphore			lock;

}i2c_io_t;

uint32_t i2c_io_init(i2c_io_t *me);
uint32_t i2c_io_send(struct i2c_io_t *me, uint8_t ui8SlaveAddr, const char *buf, int count);
uint32_t i2c_io_recv(struct i2c_io_t *client, uint8_t ui8SlaveAddr,  char *buf, int count);

#endif /* DRIVERS_COMPONENTS_I2C_I2C_H_ */
