/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdint.h"

#include "bsp_onboard_adc.h"
#include "fsl_debug_console.h"

#include "spi_io.h"
#include "do.h"
#include "ntc.h"


#include "ad4114.h"

#include "fsl_lpspi.h"
#include "MIMX9352_cm33.h"

#include "delay.h"
#include "m33_data.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TEC_CHANNEL_NUM 4U
#define NTC_CHANNEL_NUM	12U
#define EFUSE_CHANNEL_NUM 9U

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef enum
{
    PIN_TEC_CHANNEL_1,
    PIN_TEC_CHANNEL_3,
    PIN_TEC_CHANNEL_4,
    PIN_TEC_CHANNEL_2,
    PIN_NTC_CHANNEL_12,
    PIN_NTC_CHANNEL_10,
    PIN_NTC_CHANNEL_5,
    PIN_NTC_CHANNEL_4,
    PIN_NTC_CHANNEL_1,
    PIN_NTC_CHANNEL_8,
    PIN_NTC_CHANNEL_2,
    PIN_NTC_CHANNEL_3,
    PIN_NTC_CHANNEL_7,
    PIN_NTC_CHANNEL_6,
    PIN_NTC_CHANNEL_11,
    PIN_NTC_CHANNEL_9,
} Onboard_ADC0_Channel;

typedef enum
{
    ONBOARD_ADC1_0,
    ONBOARD_ADC1_1,
    PIN_EFUSE_12V_PHOTO,
    PIN_EFUSE_5V_CAM,
    PIN_TEMP_SENSOR,
    PIN_EFUSE_12V_LASER,
    PIN_EFUSE_5V_HD4,
    PIN_EFUSE_12V_HEADER,
    PIN_EFUSE_12V, //EFUSE_12V_IN
    PIN_EFUSE_5V_TEC,
    PIN_EFUSE_5V_IO,
    PIN_EFUSE_12V_SOLENOID,
} Onboard_ADC1_Channel;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static  uint32_t adc0_raw[16] = {0};
static  uint32_t adc1_raw[16] = {0};

static 	float    adc0_volt_mv[16] = {0.0};
static  float	 adc1_volt_mv[16] = {0.0};
int16_t NTC_temp_C[NTC_CHANNEL_NUM] = {0};
int32_t t_dC = 0;

//static  uint8_t  TEC_channel_map[TEC_CHANNEL_NUM] = {0, 2, 3, 1};
static  uint8_t  NTC_channel_map[NTC_CHANNEL_NUM] = {11, 9, 4, 3, 0, 7, 1, 2, 6, 5, 10, 8};
uint16_t eFUSE_current_ma[EFUSE_CHANNEL_NUM] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint32_t bsp_onboard_adc_config_channels(ad4114_t* p_dev, uint16_t enable_mask);
static uint32_t bsp_onboard_adc_get_vin_mv(uint32_t raw24, float vref_mv, float* vin_out_mv);
static uint32_t spi_io_onboard_adc_config(SPI_Io_t *me, uint8_t is_flip);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern SPI_Io_t onboard_adc_spi;

extern do_t 	onboard_adc0_cs;
extern do_t 	onboard_adc1_cs;

ad4114_t onboard_adc_dev0 =
{
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc0_cs
};

ad4114_t onboard_adc_dev1 =
{
		.spi = &onboard_adc_spi,
		.cs  = &onboard_adc1_cs
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t bsp_onboard_adc_init()
{
	uint32_t ret;


	spi_io_onboard_adc_config(onboard_adc_dev0.spi, 1);


    for (uint16_t i = 0; i < 2000; i++)
    {
        __NOP();
    }
    
	ret = ad4114_init(&onboard_adc_dev0, &onboard_adc_spi, &onboard_adc0_cs);
   
	if (ret != ERROR_OK)
	{
		spi_io_onboard_adc_config(onboard_adc_dev0.spi, 0);
		return ret;
	}

	// 0xFFFF Enable 16 pin
	ret = bsp_onboard_adc_config_channels(&onboard_adc_dev0, 0xFFFF);

	if (ret != ERROR_OK)
	{
		spi_io_onboard_adc_config(onboard_adc_dev0.spi, 0);
		return ret;
	}

	ret = ad4114_init(&onboard_adc_dev1, &onboard_adc_spi, &onboard_adc1_cs);

	if (ret != ERROR_OK)
	{
		spi_io_onboard_adc_config(onboard_adc_dev0.spi, 0);
		return ret;
	}

    uint16_t id = 0;
    ad4114_read_id(&onboard_adc_dev0, &id);

	// ((1 << 10) - 1) << 2: Enable 10 pin, start at pin 2
    //start at channel 2 to channel 11
	ret = bsp_onboard_adc_config_channels(&onboard_adc_dev1, (((1 << 10) - 1) << 2));

	spi_io_onboard_adc_config(onboard_adc_dev1.spi, 0);

	if (ret != ERROR_OK)
	{
		return ret;
	}

    return ERROR_OK;
}

uint32_t bsp_onboard_adc_update_raw()
{
	uint16_t out_mask_adc0, out_mask_adc1 = 0;
	uint32_t ret;

	spi_io_onboard_adc_config(onboard_adc_dev0.spi, 1);


    ret = ad4114_read_all(&onboard_adc_dev0, 500u, &out_mask_adc0, adc0_raw);


	if (ret != ERROR_OK)
	{
		spi_io_onboard_adc_config(onboard_adc_dev0.spi, 0);
		return ret;
	}

	ret = ad4114_read_all(&onboard_adc_dev1, 500u, &out_mask_adc1, adc1_raw);

    if (ret != ERROR_OK)
	{
		spi_io_onboard_adc_config(onboard_adc_dev0.spi, 0);
		return ret;
	}

	spi_io_onboard_adc_config(onboard_adc_dev1.spi, 0);

	if (ret != ERROR_OK)
	{
		return ret;
	}

    return ERROR_OK;
}

void bsp_onboard_adc_update_volt()
{
	for (uint8_t index = 0; index < 16; index++)
	{
		bsp_onboard_adc_get_vin_mv(adc0_raw[index], 5000.0, &adc0_volt_mv[index]);
		bsp_onboard_adc_get_vin_mv(adc1_raw[index], 1800.0, &adc1_volt_mv[index]);
	}
}

void bsp_convert_TEC()
{
	;
}

void bsp_convert_NTC()
{
	//int16_t NTC_temp_C[NTC_CHANNEL_NUM] = {0};
	
	for (uint8_t index = 0; index < NTC_CHANNEL_NUM; index++)
	{
		NTC_temp_C[NTC_channel_map[index]] = ntc_convert_from_volt(adc0_volt_mv[PIN_NTC_CHANNEL_12 + index], 5000.0);
        PRINTF("channel %i value %d\n",index,NTC_temp_C[NTC_channel_map[index]]);
    }
   // m33_data_update_NTC(NTC_temp_C[]);

	//system_data_update_NTC(NTC_temp_C);
}

void bsp_convert_eFUSE_Current()
{
	eFUSE_current_ma[0] = (uint16_t)(adc1_volt_mv[PIN_EFUSE_12V_PHOTO] * 3.343);
	eFUSE_current_ma[1] = (uint16_t)(adc1_volt_mv[PIN_EFUSE_12V_PHOTO + 1] * 3.343);

	for (uint8_t index = 2; index < EFUSE_CHANNEL_NUM; index++)
	{
		eFUSE_current_ma[index] = (uint16_t)(adc1_volt_mv[PIN_EFUSE_5V_CAM + index] * 3.343);
	}
}

void bsp_convert_onboard_temp()
{
	t_dC = (uint16_t)adc1_volt_mv[PIN_TEMP_SENSOR] - 500;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Use this once right after ad4114_init() for each ADC.
static uint32_t bsp_onboard_adc_config_channels(ad4114_t* p_dev, uint16_t channel_mask)
{
    if (!p_dev)
	{
		return (uint32_t)ERROR_INVALID_PARAM;
	}

    uint32_t rc;

	// 1) Disable all channel
	ad4114_channels_disable_mask(p_dev, 0xFFFF);

    // 2) SETUPx:
    uint16_t setupcon = 0 | AD4114_SETUP_BI_UNIPOLAR(0) | AD4114_SETUP_REFBUF_PN(3) | AD4114_SETUP_INBUF(3) | AD4114_SETUP_REF_SEL(0);
    rc = ad4114_setup_write(p_dev, 0, setupcon);
    if (rc) return rc;

	// 3) GAINx:
	ad4114_gain_write(p_dev, 0, (uint32_t)0x400000);
	if (rc) return rc;

	// 4) FILTERx:

    // 5) Link channel, pin map to the specific setup
	for (uint8_t i = 0; i < 16; i++)
	{
		// (i << 5): pin i as positive
		// (1 << 4): VINCOM as negative
		uint16_t input_map = (1 << 4) | (i << 5);
		rc = ad4114_config_channel(p_dev, i, false, input_map, 0);
    	if (rc) return rc;
	}

    // 6) Continuous conversions, REF_EN = 0 (because external reference)
    rc = ad4114_set_mode_continuous(p_dev, /*ref_en=*/false);
    if (rc) return rc;

	// 7) Enable channel follow enable mask
	ad4114_channels_enable_mask(p_dev, channel_mask);
	if (rc) return rc;

    return (uint32_t)ERROR_OK;
}

// Simplify version of it to make thing easier to calculate, this only use
// for channel that has the same setup
static uint32_t bsp_onboard_adc_get_vin_mv(uint32_t raw24, float vref_mv, float* vin_out_mv)
{
	if (!vin_out_mv || vref_mv <= 0.0f)
	{
		return (uint32_t)ERROR_INVALID_PARAM;
	}

    /* chặn 24-bit, tránh tràn */
    raw24    &= 0xFFFFFFu;

    /* Hằng số theo datasheet */
    const float K_ATT   = 0.075f;           /* hệ số suy giảm nội trước hiệu chuẩn */
    // const float TWO23   = 8388608.0f;       /* 2^23 */
	const float TWO24   = 16777216.0f;       /* 2^24 = TWO23 * 2.0 */
    // const float G_NORM  = 4194304.0f;       /* 0x400000 = 2^22 */

    /* Chuẩn hoá gain */
	/* Do gain = 0x400000 nên cho G = 1 */
    // float G = 1.0;

	// term_off = 0 do offset = 0x800000
    // float term_off = 0.0;
    float vin;
	
	// This board í uinipolar, if yours not change accordingly
	/* Unipolar: VIN = vref_mv/0.075 * [ Data/(G*2*2^23) + (OFFSET-0x800000)/2^23 ] */
	// float term_data = ((float)raw24) / (G * 2.0f * TWO23);
	// vin = (vref_mv / K_ATT) * (term_data + term_off);

	float term_data = ((float)raw24) / (TWO24);
	vin = (vref_mv / K_ATT) * (term_data);

    *vin_out_mv = vin;
    return (uint32_t)ERROR_OK;
}

static LPSPI_Type* const spi_periph[8 + 1] =
{
    NULL,
    LPSPI1,
    LPSPI2,
    LPSPI3,
    LPSPI4,
    LPSPI5,
    LPSPI6,
    LPSPI7,
    LPSPI8,
};

/**
 * FUNCTION BELOW HERE IS NON STANDARD API
 * THESE API ONLY APPLY FOR THIS BOARD VERSION 1.1.0, FOR BOARD SATELITE_EXP
 * FOR OTHER APPLICATION AND OTHER BOARD PLEASE DISCARD (DELETE) THOSE FUNCTION
 */
static uint32_t spi_io_onboard_adc_config(SPI_Io_t *me, uint8_t is_flip)
{
    if (me == NULL || me->ui32SpiPort == 0 || me->ui32SpiPort > 8)
    {
        return ERROR_INVALID_PARAM;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    LPSPI_Type *base = spi_periph[me->ui32SpiPort];

    if (!base)
    {
        return ERROR_INVALID_PARAM;
    }

    delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        return ERROR_TIMEOUT;
    }

    /* Disable SPI before modifying configuration */
    base->CR &= ~LPSPI_CR_MEN_MASK;

    // Update TCR: CPOL/CPHA bits (preserve the rest)
    uint32_t temp_cfgr1 = base->CFGR1;

    /* Clear CPOL and CPHA bits first */
    temp_cfgr1 &= ~(LPSPI_CFGR1_PINCFG_MASK);

    // Implement cpol, cpha accordingly
    if (is_flip == 0)
    {
        temp_cfgr1 |= (LPSPI_CFGR1_PINCFG(kLPSPI_SdiInSdoOut));
    }
    else
    {
        temp_cfgr1 |= (LPSPI_CFGR1_PINCFG(kLPSPI_SdoInSdiOut));
    }
    
    base->CFGR1 = temp_cfgr1;

    // Update TCR: CPOL/CPHA bits (preserve the rest)
    uint32_t temp_tcr = base->TCR;

    /* Clear CPOL and CPHA bits first */
    temp_tcr &= ~(LPSPI_TCR_CPOL_MASK | LPSPI_TCR_CPHA_MASK);

    // Implement cpol, cpha accordingly
    temp_tcr |=  (LPSPI_TCR_CPOL(1) | LPSPI_TCR_CPHA(1));
    base->TCR = temp_tcr;

    /* Enable SPI again */
    base->CR |= (LPSPI_CR_MEN_MASK);

     osSemaphoreGiven(&me->lock);

    return ERROR_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */