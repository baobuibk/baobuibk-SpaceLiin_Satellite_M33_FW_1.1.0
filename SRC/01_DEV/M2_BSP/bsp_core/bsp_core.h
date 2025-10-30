#ifndef _BSP_CORE_H_
#define _BSP_CORE_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "i2c_io.h"
#include "spi_io.h"
#include "do.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern i2c_io_t io_expander_i2c;

extern i2c_io_t sensor_i2c;
extern do_t     sensor_en0_gpio;
extern do_t     sensor_en1_gpio;

extern i2c_io_t pump_i2c;
extern do_t     pump_en_gpio;
extern do_t     pump_en2_gpio;

extern SPI_Io_t onboard_adc_spi;
extern do_t     onboard_adc0_cs;
extern do_t     onboard_adc1_cs;

extern do_t     laser_dac_cs;
extern do_t     laser_dac_latch;

extern do_t     laser_sw_int_cs;
extern do_t     laser_sw_ext_cs;

extern SPI_Io_t photo_adc_spi;
extern do_t     photo_adc_cs;
extern do_t     photo_adc_cv;

extern do_t     photo_sw_cs;

extern do_t     usb_en0_gpio;
extern do_t     usb_en1_gpio;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void bsp_core_init(void);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* _BSP_CORE_H_ */