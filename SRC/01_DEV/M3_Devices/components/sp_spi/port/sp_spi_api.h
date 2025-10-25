#ifndef _SP_SPI_API_H_
#define _SP_SPI_API_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	SPI_CPHA	0x01
#define	SPI_CPOL	0x02
#define SPI_MAX_BUS_NUMBER 8

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// /**
//  * @enum spi_mode
//  * @brief SPI configuration for clock phase and polarity.
//  */
// typedef enum _sp_spi_cs_level_t_
// {
// 	/* CS active low */
// 	SPI_CS_ACTIVE_LOW = 0,
// 	/* CS active high */
// 	SPI_CS_ACTIVE_HIGH = 1,
// } sp_spi_cs_level_t;

// /**
//  * @enum spi_mode
//  * @brief SPI configuration for clock phase and polarity.
//  */
// typedef enum _sp_spi_mode_t_
// {
// 	/** Data on rising, shift out on falling */
// 	SPI_MODE_0 = (0 | 0),
// 	/** Data on falling, shift out on rising */
// 	SPI_MODE_1 = (0 | SPI_CPHA),
// 	/** Data on rising, shift out on falling */
// 	SPI_MODE_2 = (SPI_CPOL | 0),
// 	/** Data on falling, shift out on rising */
// 	SPI_MODE_3 = (SPI_CPOL | SPI_CPHA)
// } sp_spi_mode_t;

// /**
//  * @enum spi_bit_order
//  * @brief SPI configuration for bit order (MSB/LSB).
//  */
// typedef enum _sp_spi_bit_order_t_
// {
// 	/** Most-significant bit (MSB) first */
// 	SPI_BIT_ORDER_MSB_FIRST = 0,
// 	/** Least-significant bit (LSB) first */
// 	SPI_BIT_ORDER_LSB_FIRST = 1,
// } sp_spi_bit_order_t;

// /**
//  * @enum spi_lanes
//  * @brief SPI configuration for number of lanes.
//  */
// typedef enum _sp_spi_lanes_t_
// {
// 	/** Single Lane */
// 	SPI_SINGLE_LANE,
// 	/** Dual Lane */
// 	DUAL_LANE,
// 	/** Quad Lane */
// 	SPI_QUAD_LANE,
// 	/** Octo Lane */
// 	SPI_OCTO_LANE,
// } sp_spi_lanes_t;

/**
 * Forward declaration, go to respective port file to
 * read definition
 */
typedef struct _sp_spi_t_ 	 	sp_spi_t;
typedef struct _sp_spi_init_t_  sp_spi_init_t;

typedef enum   _sp_spi_command_t_ sp_spi_command_t;
typedef enum _sp_spi_byte_type_t_ sp_spi_byte_type_t;

typedef struct _sp_spi_blocking_frame_t_ sp_spi_blocking_frame_t;
typedef struct _sp_spi_irq_frame_t_      sp_spi_irq_frame_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void SP_SPI_master_blocking_init(sp_spi_t* p_spi, const sp_spi_init_t* p_init);
void SP_SPI_master_transfer_blocking(sp_spi_t* p_spi, sp_spi_blocking_frame_t* p_frame);
void SP_SPI_build_default_blocking_frame(uint8_t addr, uint8_t* p_data_buff, sp_spi_command_t command, sp_spi_blocking_frame_t* p_frame);

void SP_SPI_master_irq_init(sp_spi_t* p_spi, const sp_spi_init_t *init);
void SP_SPI_master_transfer_irq(sp_spi_t* p_spi);
void SP_SPI_build_default_irq_frame(sp_spi_t* p_spi, uint8_t addr, uint8_t* p_data_buff, uint32_t data_count,sp_spi_command_t command);
void SP_SPI_IRQ_subhandle(sp_spi_t* p_spi);

// void SP_SPI_master_write_blocking(sp_spi_t* p_spi, sp_spi_frame_t* p_frame);
// void SP_SPI_master_read_blocking(sp_spi_t* p_spi, sp_spi_frame_t* p_frame);

// void SP_SPI_master_write_dma(sp_spi_t* p_spi, sp_spi_frame_t* p_frame);
// void SP_SPI_master_read_dma(sp_spi_t* p_spi, sp_spi_frame_t* p_frame);

// void sp_spi_write_and_read_sync(sp_spi_t* me, uint8_t* pui8TxBuff, uint32_t ui32TxLength, uint8_t* pui8RxBuff,uint32_t ui32RxLength);

// uint32_t sp_spi_read_async(sp_spi_t *me, uint8_t	*pui8RxBuff, uint32_t ui32Length);
// uint32_t sp_spi_write_async(sp_spi_t *me, uint8_t	*pui8TxBuff, uint32_t ui32Length);
// uint32_t sp_spi_transfer_async(sp_spi_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length);
// uint32_t sp_spi_write_and_read_async(sp_spi_t *me, uint8_t	*pui8TxBuff, uint32_t ui32TxLength, uint8_t	*pui8RxBuff,uint32_t ui32RxLength);

// uint32_t sp_spi_read_dma(sp_spi_t *me, uint8_t	*pui8RxBuff, uint32_t ui32Length);
// uint32_t sp_spi_write_dma(sp_spi_t *me, uint8_t	*pui8TxBuff, uint32_t ui32Length);
// uint32_t sp_spi_transfer_dma(sp_spi_t *me, uint8_t	*pui8TxBuff, uint8_t	*pui8RxBuff, uint32_t ui32Length);
// uint32_t sp_spi_write_and_read_dma(sp_spi_t *me, uint8_t	*pui8TxBuff, uint32_t ui32TxLength, uint8_t	*pui8RxBuff,uint32_t ui32RxLength);

// bool SP_SPI_is_busy(sp_spi_t* me);

#endif /* _SP_SPI_API_H_ */