#include "board.h"

#include "bsp_expander.h"
// #include "bsp_power.h"

// #include "osDelay.h"

#include "tca6416.h"
#include "error_codes.h"

extern i2c_io_t io_expander_i2c;

tca6416a_t expander0 = {
		.bus = &io_expander_i2c,
		.addr7 = 0x21,	//ARRR = 1
};

tca6416a_t expander1 = {
		.bus = &io_expander_i2c,
		.addr7 = 0x20,	//ARRR = 1
};




uint32_t bsp_expander_init(void)
{
	uint32_t rc = 0;
	// bsp_power_IO_ctrl(POWER_ON);
	// osDelay(10); //wait for the output to be stable

	rc = tca6416a_init(&expander0, &io_expander_i2c,0x21);
	rc += tca6416a_init(&expander1, &io_expander_i2c,0x20);
	rc += tca6416a_write_outputs(&expander0, 0); //make all output of u11 zero
	rc += tca6416a_write_outputs(&expander1, 0xE000); //RAM_SPI_nCS, PWM_I2C_nOE, HD4_I2C_EN are set, others are cleared
	rc += tca6416a_write_modes(&expander0, 0); //make all output
	rc += tca6416a_write_modes(&expander1, 0);

	return rc;
}

uint32_t bsp_expander_ctrl(uint32_t index, uint32_t status)
{
	uint32_t rc = 1;
	if (index < 16)		//expander 0
	{
		rc = tca6416a_pin_write(&expander0,index,status);
	}
	else if (index < 31)
	{
		rc = tca6416a_pin_write(&expander1,index - 16,status);
	}
	return rc;
}