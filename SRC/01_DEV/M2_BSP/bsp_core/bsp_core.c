/* Freescale includes. */
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "fsl_flexcan.h"

#include "fsl_lpspi.h"
#include "fsl_rgpio.h"

#include "fsl_lpi2c.h"

#include "fsl_tpm.h"

/* USER includes. */
#include "bsp_core.h"
#include "bsp_board.h"
#include "bsp_expander.h"
#include "bsp_heater.h"
#include "bsp_i2c_sensor.h"
#include "bsp_onboard_adc.h"
#include "bsp_laser.h"
#include "bsp_photo.h"
#include "bsp_pump.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void bsp_core_init_uart(void);
static void bsp_core_init_can(void);

//static void bsp_core_init_usb_pwr_en_gpio(void);

static void bsp_core_init_io_expander_i2c(void);
static void bsp_core_init_tec_cs_gpio(void);

static void bsp_core_init_sensor_i2c(void);
static void bsp_core_init_sensor_en_gpio(void);

static void bsp_core_init_pump_i2c(void);
static void bsp_core_init_pump_en_gpio(void);

static void bsp_core_init_onboard_adc_spi(void);
static void bsp_core_init_onboard_adc_cs_gpio(void);

static void bsp_core_init_laser_switch_gpio(void);

static void bsp_core_init_photo_adc_spi(void);
static void bsp_core_init_photo_adc_cs_gpio(void);

static void bsp_core_init_photo_switch_gpio(void);

static void bsp_core_init_dsub_i2c_en_gpio(void);

// static void bsp_core_init_gpio(void);
static void bsp_core_init_photo_adc_tim(void);

static int bsp_core_init_laser_adc(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
// const volatile uint32_t temp[1*1024] = {0xAA};
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief bsp_core_init
 */
void bsp_core_init(void)
{
    bsp_core_init_io_expander_i2c();
    bsp_core_init_tec_cs_gpio();

    bsp_core_init_sensor_i2c();
    bsp_core_init_sensor_en_gpio();

    bsp_core_init_pump_i2c();
    bsp_core_init_pump_en_gpio();

    bsp_core_init_laser_dac_gpio();
    bsp_core_init_laser_switch_gpio();
    bsp_core_init_onboard_adc_cs_gpio();
    bsp_core_init_onboard_adc_spi();

    bsp_core_init_photo_adc_spi();
    bsp_core_init_photo_adc_cs_gpio();

    bsp_core_init_photo_switch_gpio();

    bsp_core_init_photo_adc_tim();

    bsp_core_init_laser_adc();
 
    spi_io_init(&onboard_adc_spi);
    spi_io_init(&photo_adc_spi);

    i2c_io_init(&io_expander_i2c);
    i2c_io_init(&sensor_i2c);
    i2c_io_init(&pump_i2c);

    /* Init board peripheral. */
    bsp_expander_init();

    // Pull up RAM SPI nCS
    bsp_expander_ctrl(RAM_SPI_nCS, 1);

    for (uint16_t i = 0; i < 10000; i++)
    {
        __NOP();
    }

    bsp_expander_ctrl(POW_ONOFF_LASER,0);
    bsp_expander_ctrl(POW_ONOFF_PHOTO,0);

    for (uint16_t i = 0; i < 30000; i++)
    {
        __NOP();
    }

    bsp_onboard_adc_init();

    for (uint32_t i = 0; i < 30000; i++)
    {
        __NOP();
    }
    
   // TODO: Check I2C lib
    bsp_i2c_sensor_init();

    bsp_heater_init();
    // bsp_pump_init();
    bsp_core_init_dsub_i2c_en_gpio();
    
    for (uint32_t i = 0; i < 30000; i++)
    {
        __NOP();
    }
}

/*!
 * @brief bsp_core_init_uart
 */
__attribute__((unused)) static void bsp_core_init_uart(void)
{
    lpuart_config_t config;
    
    /* clang-format off */

    const clock_root_config_t lpuartClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(DEBUG_DB9_LPUART_CLOCK_ROOT, &lpuartClkCfg);
    CLOCK_EnableClock(DEBUG_DB9_LPUART_CLOCK_GATE);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = DEBUG_DB9_LPUART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DEBUG_DB9_LPUART_BASE, &config, DEBUG_DB9_LPUART_CLK_FREQ);
}

__attribute__((unused)) static void bsp_core_init_can(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_timing_config_t flexcan_timing_config;

    /* clang-format off */

    const clock_root_config_t flexcanClkCfg =
    {
        .clockOff = false,
	    .mux = 2,
	    .div = 10
    };
    /* clang-format on */

    CLOCK_SetRootClock(LIBCSP_CAN_CLOCK_ROOT, &flexcanClkCfg);
    CLOCK_EnableClock(LIBCSP_CAN_CLOCK_GATE);

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate               = 1000000U;
     * flexcanConfig.bitRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    // Config Improved Timing Values
    memset(&flexcan_timing_config, 0, sizeof(flexcan_timing_config_t));

    if (FLEXCAN_CalculateImprovedTimingValues(LIBCSP_CAN_BASE, flexcanConfig.bitRate, LIBCSP_CAN_CLK_FREQ, &flexcan_timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &flexcan_timing_config, sizeof(flexcan_timing_config_t));
    }

    // Load the default flexcan config and improved timing configuration to init flex can
    FLEXCAN_Init(LIBCSP_CAN_BASE, &flexcanConfig, LIBCSP_CAN_CLK_FREQ);
}

i2c_io_t io_expander_i2c =
{
		.ui32I2cPort = 7
};
static void bsp_core_init_io_expander_i2c(void)
{
    lpi2c_master_config_t i2c_masterConfig;

    /* clang-format off */
    const clock_root_config_t lpi2cClkCfg =
    {
        .clockOff = false,
	    .mux = 0, // 24MHz oscillator source
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(IO_EXPAN_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(IO_EXPAN_CLOCK_GATE);

    /*
     * i2c_masterConfig.debugEnable = false;
     * i2c_masterConfig.ignoreAck = false;
     * i2c_masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2c_masterConfig.baudRate_Hz = 100000U;
     * i2c_masterConfig.busIdleTimeout_ns = 0;
     * i2c_masterConfig.pinLowTimeout_ns = 0;
     * i2c_masterConfig.sdaGlitchFilterWidth_ns = 0;
     * i2c_masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2c_masterConfig);

    /* Change the default baudrate configuration */
    i2c_masterConfig.baudRate_Hz = IO_EXPAN_BAUDRATE_HZ;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(IO_EXPAN_BASE, &i2c_masterConfig, IO_EXPAN_CLK_FREQ);

    // i2c_io_init(&io_expander_i2c);
    // i2c_io_init(&heater_i2c);
}

static void bsp_core_init_tec_cs_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t TEC_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(kCLOCK_Root_BusWakeup, &rgpioClkCfg);
    CLOCK_EnableClock(kCLOCK_Gpio3);

    // Option A: keep these pins Secure-only, leave others unchanged
    // GPIO3->PCNS &= ~((1u << 28) | (1u << 29) | (1u << 30) | (1u << 31));

    /* Init output LED GPIO. */
    RGPIO_PinInit(GPIO3, 29U, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 31U, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 28U, &TEC_CS_config);
    RGPIO_PinInit(GPIO3, 30U, &TEC_CS_config);
}

// do_t usb_en0_gpio =
// {
//     .port = 3,
//     .pin  = USB_PWR_GPIO_EN0_PIN,
//     .bStatus = false,
// };
// do_t usb_en1_gpio =
// {
//     .port = 3,
//     .pin  = USB_PWR_GPIO_EN1_PIN,
//     .bStatus = false,
// };
// __attribute__((unused)) static void bsp_core_init_usb_pwr_en_gpio(void)
// {
//     /* Define the init structure for the output LED pin*/
//     rgpio_pin_config_t usb_pwr_config =
//     {
//         kRGPIO_DigitalOutput,
//         0,
//     };

//     /* Board pin, clock, debug console init */
//     /* clang-format off */

//     const clock_root_config_t rgpioClkCfg =
//     {
//         .clockOff = false,
//         .mux = 0, // 24Mhz Mcore root buswake clock
//         .div = 1
//     };

//     CLOCK_SetRootClock(USB_PWR_GPIO_EN0_CLOCK_ROOT, &rgpioClkCfg);
//     CLOCK_EnableClock(USB_PWR_GPIO_EN0_CLOCK_GATE);

//     /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
//     USB_PWR_GPIO_EN0_PORT->PCNS = 0x0;

//     /* Init output LED GPIO. */
//     RGPIO_PinInit(USB_PWR_GPIO_EN0_PORT, USB_PWR_GPIO_EN0_PIN, &usb_pwr_config);
//     RGPIO_PinInit(USB_PWR_GPIO_EN0_PORT, USB_PWR_GPIO_EN1_PIN, &usb_pwr_config);
// }

i2c_io_t sensor_i2c =
{
		.ui32I2cPort = 4
};
static void bsp_core_init_sensor_i2c(void)
{
    lpi2c_master_config_t i2c_masterConfig;

    /* clang-format off */
    const clock_root_config_t lpi2cClkCfg =
    {
        .clockOff = false,
	    .mux = 0, // 24MHz oscillator source
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(I2C_SENSOR_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(I2C_SENSOR_CLOCK_GATE);

    /*
     * i2c_masterConfig.debugEnable = false;
     * i2c_masterConfig.ignoreAck = false;
     * i2c_masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2c_masterConfig.baudRate_Hz = 100000U;
     * i2c_masterConfig.busIdleTimeout_ns = 0;
     * i2c_masterConfig.pinLowTimeout_ns = 0;
     * i2c_masterConfig.sdaGlitchFilterWidth_ns = 0;
     * i2c_masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2c_masterConfig);

    /* Change the default baudrate configuration */
    i2c_masterConfig.baudRate_Hz = I2C_SENSOR_BAUDRATE_HZ;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(I2C_SENSOR_BASE, &i2c_masterConfig, I2C_SENSOR_CLK_FREQ);
}

do_t sensor_en0_gpio =
{
    .port = 4,
    .pin  = I2C_SENSOR_GPIO_EN0_PIN,
    .bStatus = false,
};
do_t sensor_en1_gpio =
{
    .port = 3,
    .pin  = I2C_SENSOR_GPIO_EN1_PIN,
    .bStatus = false,
};
static void bsp_core_init_sensor_en_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t sensor_cs_config =
    {
        kRGPIO_DigitalOutput,
        0,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(I2C_SENSOR_GPIO_EN0_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(I2C_SENSOR_GPIO_EN0_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    I2C_SENSOR_GPIO_EN0_PORT->PCNS = 0x0;

    RGPIO_PinInit(I2C_SENSOR_GPIO_EN0_PORT, I2C_SENSOR_GPIO_EN0_PIN, &sensor_cs_config);

    CLOCK_SetRootClock(I2C_SENSOR_GPIO_EN1_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(I2C_SENSOR_GPIO_EN1_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    // I2C_SENSOR_GPIO_EN1_PORT->PCNS &= ~((1u << I2C_SENSOR_GPIO_EN1_PIN));

    /* Init output LED GPIO. */
    RGPIO_PinInit(I2C_SENSOR_GPIO_EN1_PORT, I2C_SENSOR_GPIO_EN1_PIN, &sensor_cs_config);
}

i2c_io_t pump_i2c =
{
		.ui32I2cPort = 8
};
static void bsp_core_init_pump_i2c(void)
{
    lpi2c_master_config_t i2c_masterConfig;

    /* clang-format off */
    const clock_root_config_t lpi2cClkCfg =
    {
        .clockOff = false,
	    .mux = 0, // 24MHz oscillator source
	    .div = 1
    };
    /* clang-format on */

    CLOCK_SetRootClock(I2C_PUMP_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(I2C_PUMP_CLOCK_GATE);

    /*
     * i2c_masterConfig.debugEnable = false;
     * i2c_masterConfig.ignoreAck = false;
     * i2c_masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2c_masterConfig.baudRate_Hz = 100000U;
     * i2c_masterConfig.busIdleTimeout_ns = 0;
     * i2c_masterConfig.pinLowTimeout_ns = 0;
     * i2c_masterConfig.sdaGlitchFilterWidth_ns = 0;
     * i2c_masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2c_masterConfig);

    /* Change the default baudrate configuration */
    i2c_masterConfig.baudRate_Hz = I2C_PUMP_BAUDRATE_HZ;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(I2C_PUMP_BASE, &i2c_masterConfig, I2C_PUMP_CLK_FREQ);
}

do_t pump_en_gpio =
{
    .port = 4,
    .pin  = I2C_PUMP_GPIO_EN_PIN,
    .bStatus = false,
};
do_t pump_en2_gpio =
{
    .port = 4,
    .pin  = I2C_PUMP_GPIO_EN2_PIN,
    .bStatus = false,
};
static void bsp_core_init_pump_en_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t pump_en_config =
    {
        kRGPIO_DigitalOutput,
        0,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(I2C_PUMP_GPIO_EN_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(I2C_PUMP_GPIO_EN_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    I2C_PUMP_GPIO_EN_PORT->PCNS = 0x0;

    RGPIO_PinInit(I2C_PUMP_GPIO_EN_PORT, I2C_PUMP_GPIO_EN_PIN,  &pump_en_config);
    RGPIO_PinInit(I2C_PUMP_GPIO_EN_PORT, I2C_PUMP_GPIO_EN2_PIN, &pump_en_config);
}

SPI_Io_t onboard_adc_spi =
{
    .ui32SpiPort = 1,
};
static void bsp_core_init_onboard_adc_spi(void)
{
    uint32_t srcClock_Hz;
    lpspi_master_config_t masterConfig;

    const clock_root_config_t lpspiClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };

    CLOCK_SetRootClock(ONBOARD_ADC_SPI_CLOCK_ROOT, &lpspiClkCfg);
    CLOCK_EnableClock(ONBOARD_ADC_SPI_CLOCK_GATE);

    /* Get LPSPI module default Configuration. */
    /*
     * 
     * masterConfig->baudRate                       = 500000;
     * masterConfig->bitsPerFrame                   = 8;
     * masterConfig->cpol                           = kLPSPI_ClockPolarityActiveHigh;
     * masterConfig->cpha                           = kLPSPI_ClockPhaseFirstEdge;
     * masterConfig->direction                      = kLPSPI_MsbFirst;

     * masterConfig->pcsToSckDelayInNanoSec         = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->lastSckToPcsDelayInNanoSec     = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->betweenTransferDelayInNanoSec  = (1000000000U / masterConfig->baudRate) / 2U;

     * masterConfig->whichPcs                       = kLPSPI_Pcs0;
     * masterConfig->pcsActiveHighOrLow             = kLPSPI_PcsActiveLow;

     * masterConfig->pinCfg                         = kLPSPI_SdiInSdoOut;
     * masterConfig->dataOutConfig                  = kLpspiDataOutRetained;

     * masterConfig->enableInputDelay               = false;
     */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = ONBOARD_ADC_SPI_BAUDRATE;
    masterConfig.pcsToSckDelayInNanoSec         = 0;
    masterConfig.lastSckToPcsDelayInNanoSec     = 0;
    masterConfig.betweenTransferDelayInNanoSec  = 0;
    masterConfig.direction                      = kLPSPI_MsbFirst;
    masterConfig.cpol                           = kLPSPI_ClockPolarityActiveLow;
    masterConfig.cpha                           = kLPSPI_ClockPhaseSecondEdge;

    // fix cứng, có thể chỉnh lại
    // masterConfig.pinCfg                         = kLPSPI_SdoInSdiOut;
    masterConfig.whichPcs = kLPSPI_Pcs1;
    
    srcClock_Hz = ONBOARD_ADC_SPI_CLK_FREQ;
    LPSPI_MasterInit(ONBOARD_ADC_SPI_BASE, &masterConfig, srcClock_Hz);

    // Disable before config
    ONBOARD_ADC_SPI_BASE->CR &= ~LPSPI_CR_MEN_MASK;

    // Allow stalls (safe)
    ONBOARD_ADC_SPI_BASE->CFGR1 &= ~LPSPI_CFGR1_NOSTALL_MASK;

    // Force 8-bit frames; choose PCS1; hold PCS internally (no real pin toggling)
    ONBOARD_ADC_SPI_BASE->TCR = (ONBOARD_ADC_SPI_BASE->TCR & ~(LPSPI_TCR_FRAMESZ_MASK |
                            LPSPI_TCR_RXMSK_MASK   |
                            LPSPI_TCR_TXMSK_MASK   |
                            LPSPI_TCR_PCS_MASK     |
                            LPSPI_TCR_CONT_MASK    |
                            LPSPI_TCR_CONTC_MASK))
            |  LPSPI_TCR_FRAMESZ(7)         // 8-bit
            |  LPSPI_TCR_PCS(1)             // "PCS1" (not pin-muxed)
            |  LPSPI_TCR_RXMSK(0)
            |  LPSPI_TCR_TXMSK(0)
            |  LPSPI_TCR_CONT(1)            // keep PCS asserted internally
            |  LPSPI_TCR_CONTC(1);

    // Clean state
    ONBOARD_ADC_SPI_BASE->CR |=  (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);       // flush TX/RX FIFOs
    ONBOARD_ADC_SPI_BASE->SR  =   LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK; // clear sticky

    // Enable
    ONBOARD_ADC_SPI_BASE->CR |= LPSPI_CR_MEN_MASK;
}

do_t onboard_adc0_cs =
{
    .port = 4,
    .pin  = ONBOARD_ADC_GPIO_CS0_PIN,
};

do_t onboard_adc1_cs =
{
    .port = 4,
    .pin  = ONBOARD_ADC_GPIO_CS1_PIN,
};
static void bsp_core_init_onboard_adc_cs_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t onboard_ADC_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    rgpio_pin_config_t onboard_ADC_CV_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(ONBOARD_ADC_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(ONBOARD_ADC_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    ONBOARD_ADC_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_CS0_PIN, &onboard_ADC_CS_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_CS1_PIN, &onboard_ADC_CS_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_SYNC0_PIN, &onboard_ADC_CV_config);
    RGPIO_PinInit(ONBOARD_ADC_GPIO_CS_PORT, ONBOARD_ADC_GPIO_SYNC1_PIN, &onboard_ADC_CV_config);
}

do_t laser_dac_cs =
{
    .port = 4,
    .pin  = LASER_DAC_GPIO_CS_PIN,
};

do_t laser_dac_latch =
{
    .port = 4,
    .pin  = LASER_DAC_GPIO_LATCH_PIN,
};
void bsp_core_init_laser_dac_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t laser_DAC_CS_config =
    {
        kRGPIO_DigitalOutput,
        0,
    };

    rgpio_pin_config_t laser_DAC_latch_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(LASER_DAC_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(LASER_DAC_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    LASER_DAC_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(LASER_DAC_GPIO_CS_PORT, LASER_DAC_GPIO_CS_PIN, &laser_DAC_CS_config);
    RGPIO_PinInit(LASER_DAC_GPIO_CS_PORT, LASER_DAC_GPIO_LATCH_PIN, &laser_DAC_latch_config);
}

do_t laser_sw_int_cs =
{
    .port = 4,
    .pin  = LASER_SW_GPIO_INT_CS_PIN,
};

do_t laser_sw_ext_cs =
{
    .port = 4,
    .pin  = LASER_SW_GPIO_EXT_CS_PIN,
};
static void bsp_core_init_laser_switch_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t laser_SW_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };

    CLOCK_SetRootClock(LASER_SW_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(LASER_SW_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    LASER_SW_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(LASER_SW_GPIO_CS_PORT, LASER_SW_GPIO_INT_CS_PIN, &laser_SW_CS_config);
    RGPIO_PinInit(LASER_SW_GPIO_CS_PORT, LASER_SW_GPIO_EXT_CS_PIN, &laser_SW_CS_config);
}

SPI_Io_t photo_adc_spi =
{
    .ui32SpiPort = 5,
};
static void bsp_core_init_photo_adc_spi(void)
{
    uint32_t srcClock_Hz;
    lpspi_master_config_t masterConfig;

    const clock_root_config_t lpspiClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };

    CLOCK_SetRootClock(PHOTO_ADC_SPI_CLOCK_ROOT, &lpspiClkCfg);
    CLOCK_EnableClock(PHOTO_ADC_SPI_CLOCK_GATE);

    /* Get LPSPI module default Configuration. */
    /*
     * 
     * masterConfig->baudRate                       = 500000;
     * masterConfig->bitsPerFrame                   = 8;
     * masterConfig->cpol                           = kLPSPI_ClockPolarityActiveHigh;
     * masterConfig->cpha                           = kLPSPI_ClockPhaseFirstEdge;
     * masterConfig->direction                      = kLPSPI_MsbFirst;

     * masterConfig->pcsToSckDelayInNanoSec         = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->lastSckToPcsDelayInNanoSec     = (1000000000U / masterConfig->baudRate) / 2U;
     * masterConfig->betweenTransferDelayInNanoSec  = (1000000000U / masterConfig->baudRate) / 2U;

     * masterConfig->whichPcs                       = kLPSPI_Pcs0;
     * masterConfig->pcsActiveHighOrLow             = kLPSPI_PcsActiveLow;

     * masterConfig->pinCfg                         = kLPSPI_SdiInSdoOut;
     * masterConfig->dataOutConfig                  = kLpspiDataOutRetained;

     * masterConfig->enableInputDelay               = false;
     */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.pcsToSckDelayInNanoSec         = 0;
    masterConfig.lastSckToPcsDelayInNanoSec     = 0;
    masterConfig.betweenTransferDelayInNanoSec  = 0;
    masterConfig.direction                      = kLPSPI_MsbFirst;
    masterConfig.cpol                           = kLPSPI_ClockPolarityActiveLow;
    masterConfig.cpha                           = kLPSPI_ClockPhaseSecondEdge;
    masterConfig.baudRate = PHOTO_ADC_SPI_BAUDRATE;
    masterConfig.whichPcs = kLPSPI_Pcs1;
    
    srcClock_Hz = PHOTO_ADC_SPI_CLK_FREQ;
    LPSPI_MasterInit(PHOTO_ADC_SPI_BASE, &masterConfig, srcClock_Hz);

    // Disable before config
    PHOTO_ADC_SPI_BASE->CR &= ~LPSPI_CR_MEN_MASK;

    // Allow stalls (safe)
    PHOTO_ADC_SPI_BASE->CFGR1 &= ~LPSPI_CFGR1_NOSTALL_MASK;

    // Force 8-bit frames; choose PCS1; hold PCS internally (no real pin toggling)
    PHOTO_ADC_SPI_BASE->TCR = (PHOTO_ADC_SPI_BASE->TCR & ~(LPSPI_TCR_FRAMESZ_MASK |
                            LPSPI_TCR_RXMSK_MASK   |
                            LPSPI_TCR_TXMSK_MASK   |
                            LPSPI_TCR_PCS_MASK     |
                            LPSPI_TCR_CONT_MASK    |
                            LPSPI_TCR_CONTC_MASK))
            |  LPSPI_TCR_FRAMESZ(7)         // 8-bit
            |  LPSPI_TCR_PCS(1)             // "PCS1" (not pin-muxed)
            |  LPSPI_TCR_RXMSK(0)
            |  LPSPI_TCR_TXMSK(0)
            |  LPSPI_TCR_CONT(1)            // keep PCS asserted internally
            |  LPSPI_TCR_CONTC(1);

    // Clean state
    PHOTO_ADC_SPI_BASE->CR |=  (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);       // flush TX/RX FIFOs
    PHOTO_ADC_SPI_BASE->SR  =   LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK; // clear sticky

    // Enable
    PHOTO_ADC_SPI_BASE->CR |= LPSPI_CR_MEN_MASK;
}

do_t photo_adc_cs =
{
    .port = 4,
    .pin  = PHOTO_ADC_GPIO_CS_PIN,
};

do_t photo_adc_cv =
{
    .port = 4,
    .pin  = PHOTO_ADC_GPIO_CV_PIN,
};
static void bsp_core_init_photo_adc_cs_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t photo_ADC_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    rgpio_pin_config_t photo_ADC_CV_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 1, // 24Mhz Mcore root buswake clock
        .div = 4
    };

    CLOCK_SetRootClock(PHOTO_ADC_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(PHOTO_ADC_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    PHOTO_ADC_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(PHOTO_ADC_GPIO_CS_PORT, PHOTO_ADC_GPIO_CS_PIN, &photo_ADC_CS_config);
    RGPIO_PinInit(PHOTO_ADC_GPIO_CS_PORT, PHOTO_ADC_GPIO_CV_PIN, &photo_ADC_CV_config);
}

do_t photo_sw_cs =
{
    .port = 2,
    .pin  = PHOTO_SW_GPIO_CS_PIN,
};
static void bsp_core_init_photo_switch_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t photo_SW_CS_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 1, // 24Mhz Mcore root buswake clock
        .div = 4
    };

    CLOCK_SetRootClock(PHOTO_SW_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(PHOTO_SW_GPIO_CS_CLOCK_GATE);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    PHOTO_SW_GPIO_CS_PORT->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(PHOTO_SW_GPIO_CS_PORT, PHOTO_SW_GPIO_CS_PIN, &photo_SW_CS_config);
}

static void bsp_core_init_dsub_i2c_en_gpio(void)
{
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t DSUB_en_gpio_config =
    {
        kRGPIO_DigitalOutput,
        1,
    };

    /* Board pin, clock, debug console init */
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg =
    {
        .clockOff = false,
        .mux = 1, // 24Mhz Mcore root buswake clock
        .div = 4
    };

    CLOCK_SetRootClock(PHOTO_SW_GPIO_CS_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    GPIO4->PCNS = 0x0;

    /* Init output LED GPIO. */
    RGPIO_PinInit(GPIO4, 15U, &DSUB_en_gpio_config);
}

static void bsp_core_init_photo_adc_tim(void)
{
    const clock_root_config_t lptpmClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };

    CLOCK_SetRootClock(PHOTO_ADC_TIM_CLOCK_ROOT, &lptpmClkCfg);
    CLOCK_EnableClock(PHOTO_ADC_TIM_CLOCK_GATE);
}



#include "MIMX9352_cm33.h"   // Provides SAR_ADC_Type and base pointers (e.g., ADC1)

/* ===========================
 *  Minimal bitfield defines
 *  (verify against your RM)
 * =========================== */

/* MCR – Main Configuration */
// #define SAR_ADC_MCR_PWDN_MASK        (1u << 0)   /* 1 = Power down */
// #define SAR_ADC_MCR_ADCLKSE_MASK     (1u << 8)   /* 1 = ADCLK = Bus clock, 0 = Internal/aux */
// #define SAR_ADC_MCR_CALSTART_MASK    (1u << 14)  /* 1 = Start calibration */
// #define SAR_ADC_MCR_NSTART_MASK      (1u << 24)  /* 1 = Start normal conversion sequence */
// #define SAR_ADC_MCR_MODE_MASK        (1u << 29)  /* 0 = Normal (one-shot), 1 = Scan/loop */

/* MSR – Main Status (used for “status == idle/pd” if your RM exposes it) */
// #define SAR_ADC_MSR_ADCSTATUS_MASK   (0x7u << 0) /* Optional: status field (IDLE/PD/etc.) */
#define SAR_ADC_STATUS_IDLE          (0u)
#define SAR_ADC_STATUS_POWER_DOWN    (1u)

/* CALSTAT – Calibration Status */
#define SAR_ADC_CALSTAT_BUSY_MASK    (1u << 0)   /* 1 = Calibration in progress */
#define SAR_ADC_CALSTAT_FAIL_MASK    (1u << 1)   /* 1 = Calibration failed */

/* ISR/IMR – Global interrupt status/mask */
// #define SAR_ADC_ISR_ECH_MASK         (1u << 0)   /* End of chain */
// #define SAR_ADC_ISR_EOC_MASK         (1u << 1)   /* End of conversion */
#define SAR_ADC_ISR_ALL              (0xFFFFFFFFu)

/* CEOCFRx – Channel pending (write-1-to-clear on many NXP SARs) */
#define SAR_ADC_CEOCFR_ALL           (0xFFFFFFFFu)

/* WTIMR – Watchdog interrupt mask (0 = mask all) */
#define SAR_ADC_WTIMR_ALL_MASKED     (0u)

/* DMAE – DMA enable (0 = disabled) */
#define SAR_ADC_DMAE_DISABLE         (0u)

/* PSCR/PSR – Presampling (disable for clean defaults) */
#define SAR_ADC_PSCR_PSM_EN_MASK     (1u << 0)   /* 1 = presampling enable */
#define SAR_ADC_PSCR_DISABLE         (0u)

/* CTRx – Conversion timing
   NOTE: Exact bitfields are SoC-specific. The values below are conservative:
   - Moderate sample/acquisition window for external source settling
   - Default conversion time
   Adjust per your RM/AFE requirement.
*/
#define SAR_ADC_CTR0_SAFE_DEFAULT    (0x0000001Fu)  /* Example: ~medium acq time */
#define SAR_ADC_CTR1_SAFE_DEFAULT    (0x00000000u)

/* NCMR/JCMR – Channel masks: leave cleared at init */
#define SAR_ADC_NCMR_NONE            (0u)
#define SAR_ADC_JCMR_NONE            (0u)

/* USROFSGN – Neutral user gain/offset */
#define SAR_ADC_USROFSGN_NEUTRAL     (0x00000000u)

/* PDEDR – Power-down exit delay (ADC clock cycles). 32 is a safe default. */
#define SAR_ADC_PDEDR_DEFAULT        (32u)

/* Result formatting (PCDR width is 12-bit on i.MX93 SAR) */
#define SAR_ADC_PCDR_MASK_12B        (0x0FFFu)

/* ===========================
 *  Small local helpers
 * =========================== */
static inline void _tiny_delay(volatile uint32_t n)
{
    while (n--) __NOP();
}

static bool _wait_bits_cleared(const volatile uint32_t *reg, uint32_t mask, uint32_t iters)
{
    while (iters--)
    {
        if ( ((*reg) & mask) == 0u ) return true;
        __NOP();
    }
    return false;
}

static bool _wait_status_equals(const volatile uint32_t *reg, uint32_t mask, uint32_t expect, uint32_t iters)
{
    while (iters--)
    {
        if ( ((*reg) & mask) == expect ) return true;
        __NOP();
    }
    return false;
}

/* ============================================================
 *  bsp_core_init_laser_adc()
 *
 *  What this does:
 *    - Masks/clears all interrupts and pending flags
 *    - Ensures power-down, then power-up to IDLE
 *    - Disables presampling; sets safe timing (CTR0/1)
 *    - Neutral user offset/gain
 *    - Sets power-down exit delay (PDEDR)
 *    - Runs calibration and checks failure flag
 *    - Selects bus clock for runtime conversions
 *    - Leaves ADC in Normal (one-shot) mode, IDLE, ready to use
 *
 *  Pre-reqs (done by board code):
 *    - Enable ADC IPG clock root and gate
 *    - Configure VREF and analog supply rails (1.8V) stable
 *    - IOMUX set to desired analog channel(s)
 *
 *  Returns:
 *    0  on success
 *   -1 on bad args
 *   -2 on timeout waiting for calibration to end
 *   -3 on calibration failure
 * ============================================================ */
static int bsp_core_init_laser_adc(void)
{
    const clock_root_config_t lpadcClkCfg =
    {
        .clockOff = false,
	    .mux = 0,
	    .div = 1
    };

    CLOCK_SetRootClock(LASER_IMX_ADC_CLOCK_ROOT, &lpadcClkCfg);
    CLOCK_EnableClock(LASER_IMX_ADC_CLOCK_GATE);

    SAR_ADC_Type *base = LASER_IMX_ADC_BASE;

    if (!base) return -1;

    /* 0) Globally mask interrupts and clear any sticky status/pending flags */
    base->IMR    = 0u;                  /* Mask global EOC/ECH/etc. */
    base->CIMR0  = 0u;                  /* Mask per-channel interrupts (0..31) */
    base->CIMR1  = 0u;                  /* Mask per-channel interrupts (32..63) */
    base->WTIMR  = SAR_ADC_WTIMR_ALL_MASKED;
    base->ISR    = SAR_ADC_ISR_ALL;     /* W1C */
    base->CEOCFR0 = SAR_ADC_CEOCFR_ALL; /* W1C pending by channel (0..31) */
    base->CEOCFR1 = SAR_ADC_CEOCFR_ALL; /* W1C pending by channel (32..63) */
    base->DMAE   = SAR_ADC_DMAE_DISABLE;

    /* 1) Go to power-down first (clean start), then wait */
    base->MCR |= SAR_ADC_MCR_PWDN_MASK;
    _tiny_delay(10000); /* small guard; if MSR has status, wait for POWER_DOWN */
    (void)_wait_status_equals(&base->MSR, SAR_ADC_MSR_ADCSTATUS_MASK, (SAR_ADC_STATUS_POWER_DOWN << 0), 100000u);

    /* 2) Make sure calibration uses the default ADC clock (ADCLKSE = 0) */
    base->MCR &= ~SAR_ADC_MCR_ADCLKSE_MASK;

    /* 3) Program safe analog front-end defaults */
    base->PSCR   = SAR_ADC_PSCR_DISABLE;      /* presampling off */
    base->PSR0   = 0u;
    base->PSR1   = 0u;
    base->CTR0   = SAR_ADC_CTR0_SAFE_DEFAULT; /* moderate sample window */
    base->CTR1   = SAR_ADC_CTR1_SAFE_DEFAULT;
    base->USROFSGN = SAR_ADC_USROFSGN_NEUTRAL;/* no user gain/offset */
    base->PDEDR  = SAR_ADC_PDEDR_DEFAULT;     /* power-up settling guard */

    /* Clear channel masks (no channel selected yet) */
    base->NCMR0  = SAR_ADC_NCMR_NONE;
    base->NCMR1  = SAR_ADC_NCMR_NONE;
    base->JCMR0  = SAR_ADC_JCMR_NONE;
    base->JCMR1  = SAR_ADC_JCMR_NONE;

    /* 4) Exit power-down -> IDLE */
    base->MCR &= ~SAR_ADC_MCR_PWDN_MASK;
    _tiny_delay(10000);
    (void)_wait_status_equals(&base->MSR, SAR_ADC_MSR_ADCSTATUS_MASK, (SAR_ADC_STATUS_IDLE << 0), 500000u);

    /* 5) Start calibration and wait to complete */
    base->MCR |= SAR_ADC_MCR_CALSTART_MASK;

    /* Prefer CALSTAT if available; fall back to MSR-calbusy if needed */
    if (!_wait_bits_cleared(&base->CALSTAT, SAR_ADC_CALSTAT_BUSY_MASK, 2000000u))
    {
        /* ~2M poll iterations; adjust for your core clock if needed */
        return -2; /* calibration timeout */
    }
    if (base->CALSTAT & SAR_ADC_CALSTAT_FAIL_MASK)
    {
        return -3; /* calibration failed: check VREF/analog rails/noise */
    }

    /* 6) Select bus clock for better control (post-cal) */
    base->MCR |= SAR_ADC_MCR_ADCLKSE_MASK;

    /* 7) Ensure Normal (one-shot) mode (MODE = 0) */
    base->MCR &= ~SAR_ADC_MCR_MODE_MASK;

    // /* 8) Leave interrupts masked; user will poll on EOC/ECH or read PCDR */
    // /* Ready for: set NCMR bit -> set NSTART -> poll -> read PCDR[ch] */

    return 0;
}

/* ===========================
 *  Example usage (polling):
 * ===========================
 *
 * // After bsp_core_init_laser_adc(ADC1) succeeds:
 * static inline uint16_t adc_read_channel_blocking(SAR_ADC_Type *adc, uint8_t ch)
 * {
 *     // 1) Select one channel in normal sequence
 *     if (ch < 32) {
 *         adc->NCMR0 = (1u << ch);
 *         adc->NCMR1 = 0u;
 *     } else {
 *         adc->NCMR0 = 0u;
 *         adc->NCMR1 = (1u << (ch - 32));
 *     }
 *
 *     // 2) Clear status and start
 *     adc->ISR = SAR_ADC_ISR_ALL;          // W1C
 *     adc->MCR |= SAR_ADC_MCR_NSTART_MASK; // kick one-shot conversion
 *
 *     // 3) Poll EOC/ECH (either is fine for one channel)
 *     while ((adc->ISR & (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK)) == 0u) { __NOP(); }
 *
 *     // 4) Read result. External channels commonly map to PCDR[0..7],
 *     //    internal to ICDR[n]; verify your channel -> result register mapping.
 *     //    If your app uses a fixed channel set, map explicitly.
 *     uint16_t code = (uint16_t)(adc->PCDR[0] & SAR_ADC_PCDR_MASK_12B); // example: PCDR[0]
 *     adc->ISR = (SAR_ADC_ISR_EOC_MASK | SAR_ADC_ISR_ECH_MASK);         // W1C
 *     return code;
 * }
 */