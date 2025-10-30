/*
 * spi_io.c
 *
 *  Created on: Oct 11, 2025
 *      Author: Admin
 */

#include "spi_io.h"
#include "osSemphr.h"
uint32_t spi_io_init(SPI_Io_t *me)
{
	return osSemaphoreCreateMutex(&me->lock);
}

