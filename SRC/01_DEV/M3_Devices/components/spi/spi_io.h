/*
 * SPI_io.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#ifndef DRIVER_COMPONENTS_SPI_SPI_IO_H_
#define DRIVER_COMPONENTS_SPI_SPI_IO_H_
#include "os.h"
#include "do.h"
#include "osSemphr.h"
#include "error_codes.h"

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/

#define	SPI_CPHA	0x01
#define	SPI_CPOL	0x02

#define SPI_WAIT_TIME	250

/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/

/**
 * @enum spi_mode
 * @brief SPI configuration for clock phase and polarity.
 */
enum spi_mode {
	/** Data on rising, shift out on falling */
	SPI_MODE_0 = (0 | 0),
	/** Data on falling, shift out on rising */
	SPI_MODE_1 = (0 | SPI_CPHA),
	/** Data on rising, shift out on falling */
	SPI_MODE_2 = (SPI_CPOL | 0),
	/** Data on falling, shift out on rising */
	SPI_MODE_3 = (SPI_CPOL | SPI_CPHA)
};

/**
 * @enum no_os_spi_bit_order
 * @brief SPI configuration for bit order (MSB/LSB).
 */
enum spi_bit_order {
	/** Most-significant bit (MSB) first */
	SPI_BIT_ORDER_MSB_FIRST = 0,
	/** Least-significant bit (LSB) first */
	SPI_BIT_ORDER_LSB_FIRST = 1,
};

/**
 * @enum spi_lanes
 * @brief SPI configuration for number of lanes.
 */
enum spi_lanes {
	/** Single Lane */
	SPI_SINGLE_LANE,
	/** Dual Lane */
	DUAL_LANE,
	/** Quad Lane */
	SPI_QUAD_LANE,
	/** Octo Lane */
	SPI_OCTO_LANE,
};


typedef struct
{
	/** SPI bus address */
	uint32_t 			ui32SpiPort;
	osSemaphore			lock;

}SPI_Io_t;

uint32_t spi_io_set_mode(SPI_Io_t *me, uint8_t spi_mode);

uint32_t spi_io_init(SPI_Io_t *me);
uint32_t spi_io_read_sync(SPI_Io_t *me, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_sync(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32Length);
uint32_t spi_io_transfer_sync(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_and_read_sync(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32TxLength, uint8_t	*pui8RxBuff,uint32_t ui32RxLength);

uint32_t spi_io_read_async(SPI_Io_t *me, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_async(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32Length);
uint32_t spi_io_transfer_async(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_and_read_async(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32TxLength, uint8_t	*pui8RxBuff,uint32_t ui32RxLength);

uint32_t spi_io_read_dma(SPI_Io_t *me, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_dma(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32Length);
uint32_t spi_io_transfer_dma(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length);
uint32_t spi_io_write_and_read_dma(SPI_Io_t *me, uint8_t	*pui8TxBuff, uint32_t ui32TxLength, uint8_t	*pui8RxBuff,uint32_t ui32RxLength);

#endif /* DRIVER_COMPONENTS_SPI_SPI_IO_H_ */
