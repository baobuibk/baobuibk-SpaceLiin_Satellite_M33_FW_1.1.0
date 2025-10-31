/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "MIMX9352_cm33.h"   // Provides SAR_ADC_Type and ADC1

#include "bsp_laser.h"
#include "bsp.h"

// #include "do.h"

#include "fsl_tpm.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ---- Minimal bits we need ---- */
// #define SAR_ADC_MCR_NSTART_MASK      (1u << 24)   /* start normal conversion */
// #define SAR_ADC_ISR_ECH_MASK         (1u << 0)    /* end of chain */
// #define SAR_ADC_ISR_EOC_MASK         (1u << 1)    /* end of conversion */
#define SAR_ADC_ISR_ALL              (0xFFFFFFFFu)
#define SAR_ADC_CEOCFR_ALL           (0xFFFFFFFFu)

/* i.MX93 SAR ADC precision data are 12-bit right aligned in PCDR[x] */
#define SAR_ADC_PCDR_MASK_12B        (0x0FFFu)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
adg1414_dev_t laser_int_dev =
{
		.spi = &onboard_adc_spi,
		.cs  = { .port = 4, .pin = 25, .bStatus = true },
		.num_of_sw = 3
};

adg1414_dev_t laser_ext_dev =
{
		.spi = &onboard_adc_spi,
		.cs  = { .port = 4, .pin = 26, .bStatus = true },
		.num_of_sw = 1
};

mcp4902_dev_t laser_dac_dev =
{
		.spi = &onboard_adc_spi,
		.cs = { .port = 4, .pin = 27, .bStatus = true },
		.latch = { .port = 4, .pin = 24, .bStatus = true },
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_int_LD_position(int x);
static int8_t map_ext_LD_position(int x);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint16_t laser_current[32 * 2] = {0};

uint32_t laser_adc_set_count;
uint32_t laser_adc_count;

uint8_t  is_laser_tim_run = 0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void bsp_laser_init(void)
{
	adg1414_chain_init(&laser_int_dev, &onboard_adc_spi, &laser_int_dev.cs, laser_int_dev.num_of_sw);
	adg1414_chain_init(&laser_ext_dev, &onboard_adc_spi, &laser_ext_dev.cs, laser_ext_dev.num_of_sw);

	mcp4902_dev_init(&laser_dac_dev, &onboard_adc_spi, &laser_dac_dev.cs, &laser_dac_dev.latch);
}

void bsp_laser_int_set_dac(uint8_t code)
{
	spi_io_set_mode(laser_dac_dev.spi, 0);
	mcp4902_set_dac(&laser_dac_dev, MCP4902_CHA, code);
}

uint8_t bsp_laser_int_get_dac(void)
{
	return laser_dac_dev.dac_channel[MCP4902_CHA];
}

void bsp_laser_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_on(&laser_int_dev, real_channel);
}

void bsp_laser_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_sw_off(&laser_int_dev, real_channel);
}

void bsp_laser_int_sw_on_manual(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_manual_sw_on(&laser_int_dev, real_channel);
}

void bsp_laser_int_sw_off_manual(uint8_t channel)
{
	uint8_t real_channel = map_int_LD_position(channel);
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_manual_sw_off(&laser_int_dev, real_channel);
}

void bsp_laser_int_all_sw_off(void)
{
	spi_io_set_mode(laser_int_dev.spi, 1);
	adg1414_chain_all_sw_off(&laser_int_dev);
}

void bsp_laser_ext_set_dac(uint8_t code)
{
	spi_io_set_mode(laser_dac_dev.spi, 0);
	mcp4902_set_dac(&laser_dac_dev, MCP4902_CHB, code);
}

uint8_t bsp_laser_ext_get_dac(void)
{
	return laser_dac_dev.dac_channel[MCP4902_CHB];
}

void bsp_laser_ext_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_sw_on(&laser_ext_dev, real_channel);
}

void bsp_laser_ext_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_sw_off(&laser_ext_dev, real_channel);
}

void bsp_laser_ext_sw_on_manual(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_manual_sw_on(&laser_ext_dev, real_channel);
}

void bsp_laser_ext_sw_off_manual(uint8_t channel)
{
	uint8_t real_channel = map_ext_LD_position(channel);
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_manual_sw_off(&laser_ext_dev, real_channel);
}

void bsp_laser_ext_all_sw_off(void)
{
	spi_io_set_mode(laser_ext_dev.spi, 1);
	adg1414_chain_all_sw_off(&laser_ext_dev);
}


/* ============================================================
 * 1) TRIGGER: select channel and start a one-shot conversion
 *    - Channel 0 -> ADC1_IN0
 *    - Channel 1 -> ADC1_IN1
 * ============================================================ */
void bsp_laser_int_current_trigger_adc(void)
{
    /* Select only channel 1 in the normal sequence */
    ADC1->NCMR0 = (1u << 1);
    ADC1->NCMR1 = 0u;
    ADC1->JCMR0 = 0u;
    ADC1->JCMR1 = 0u;

    /* Clear any previous status/pending, then start */
    ADC1->ISR     = SAR_ADC_ISR_ALL;      /* W1C */
    ADC1->CEOCFR0 = SAR_ADC_CEOCFR_ALL;   /* W1C channel-pending */
    ADC1->CEOCFR1 = SAR_ADC_CEOCFR_ALL;
    ADC1->MCR    |= SAR_ADC_MCR_NSTART_MASK;
}

void bsp_laser_ext_current_trigger_adc(void) //ext
{
    /* Select only channel 0 in the normal sequence */
    ADC1->NCMR0 = (1u << 0);
    ADC1->NCMR1 = 0u;
    ADC1->JCMR0 = 0u;
    ADC1->JCMR1 = 0u;

    /* Clear any previous status/pending, then start */
    ADC1->ISR     = SAR_ADC_ISR_ALL;      /* W1C */
    ADC1->CEOCFR0 = SAR_ADC_CEOCFR_ALL;   /* W1C channel-pending */
    ADC1->CEOCFR1 = SAR_ADC_CEOCFR_ALL;
    ADC1->MCR    |= SAR_ADC_MCR_NSTART_MASK;
}

/* ============================================================
 * 2) READ DATA (assumes conversion already finished)
 *    Reads PCDR[x] that corresponds to the selected channel.
 * ============================================================ */
uint16_t bsp_laser_int_current_read_adc_data(void)
{
    uint16_t v = (uint16_t)(ADC1->PCDR[1] & SAR_ADC_PCDR_MASK_12B);

    /* Clear EOC/ECH (W1C) */
    ADC1->ISR = (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK);

    return v;

	// return 0;
}

uint16_t bsp_laser_ext_current_read_adc_data(void)
{
    uint16_t v = (uint16_t)(ADC1->PCDR[0] & SAR_ADC_PCDR_MASK_12B);

    /* Clear EOC/ECH (W1C) in case caller wants to chain calls */
    ADC1->ISR = (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK);

    return v;
	// return 0;
}

/* ============================================================
 * 3) POLLING READ (trigger -> wait -> read)
 *    Busy-waits on EOC/ECH with no timeout per your request.
 *    If you want a timeout later, add a countdown in the loop.
 * ============================================================ */
uint16_t bsp_laser_int_current_adc_polling(void)
{
    bsp_laser_int_current_trigger_adc();

	uint32_t timeout = 2000;
    while ((ADC1->ISR & (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK)) == 0u)
	{
		timeout--;

		if (timeout == 0)
		{
			return 0x0;
		}
	}

	uint16_t raw_adc = bsp_laser_int_current_read_adc_data();

	// float temp = ((float)raw_adc * 1800.0) / (4095.0);	//mV x 1 times
	// temp /= (5.97 * 30.0);

	float temp = (float)raw_adc * 2.454273811;
	return (uint16_t)(temp);

    // return raw_adc;

	// return 0;
}

uint16_t bsp_laser_ext_current_adc_polling(void)
{
    bsp_laser_ext_current_trigger_adc();

	uint16_t timeout = 2000;
    while ((ADC1->ISR & (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK)) == 0u)
	{
		timeout--;

		if (timeout == 0)
		{
			return 0;
		}
	}

	// uint16_t raw_adc = bsp_laser_ext_current_read_adc_data();

	// float temp = ((float)raw_adc * 1800.0) / (4095.0);	//mV x 1 times
	// temp /= (5.97 * 30.0);
	// return (uint16_t)(temp);

	return bsp_laser_ext_current_read_adc_data();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_int_LD_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        1, 2, 3, 4, 8, 7, 6, 5,
        9, 10, 11, 12, 16, 15, 14, 13,
        17, 18, 19, 20, 24, 23, 22, 21
    };

    if (x < 1 || x > 24)
	{
		return -1;
	}

    return map[x];
}

static int8_t map_ext_LD_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        1, 2, 3, 4, 5, 6, 7, 8
    };

    if (x < 1 || x > 8)
	{
		return -1;
	}

    return map[x];
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */