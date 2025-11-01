/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "timers.h"

/* Board Support includes. */
#include "bsp_core.h"
#include "bsp_i2c_sensor.h"

/* Component includes. */
#include "i2c_io.h"
#include "do.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint8_t bmp390_is_init = 0;
uint8_t slf3s_is_init = 0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void bsp_i2c_sensor_init()
{
	do_set(&sensor_en0_gpio);
	do_set(&sensor_en1_gpio);

	TickType_t last_delay = xTaskGetTickCount();
	vTaskDelayUntil(&last_delay, pdMS_TO_TICKS(200));

	if (BMP390_init(&sensor_i2c) == 1)
	{
		bmp390_is_init = 1;
	}
	else
	{
		bmp390_is_init = 0;
	}
	
	if (slf3s_init(&sensor_i2c, true) == 0)
	{
		slf3s_is_init = 1;
	}
	else
	{
		slf3s_is_init = 0;
	}
}

uint32_t Flow_sensor_read(slf3s_readings_t* p_value)
{
	if (slf3s_is_init == 1)
	{
		return slf3s_read_all(&sensor_i2c, p_value);
	}
	
	if (slf3s_init(&sensor_i2c, true) == 0)
	{
		slf3s_is_init = 1;
		return slf3s_read_all(&sensor_i2c, p_value);
	}
	else
	{
		slf3s_is_init = 0;
		return ERROR_I2C_NOT_INIT;
	}
}

uint32_t BMP390_sensor_read(bmp390_data_t* p_data)
{
	if (bmp390_is_init == 1)
	{
		if (!BMP390_read_value(&sensor_i2c, p_data))
		{
			return ERROR_FAIL;
		}
	}
	else
	{
		if (BMP390_init(&sensor_i2c) == 1)
		{
			bmp390_is_init = 1;

			if (!BMP390_read_value(&sensor_i2c, p_data))
			{
				return ERROR_FAIL;
			}
		}
		else
		{
			bmp390_is_init = 0;
			return ERROR_FAIL;
		}
	}

	return ERROR_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */