/* MCUXpresso autogen includes. */
#include "pin_mux.h"
#include "board.h"

/* User includes. */
#include "bsp.h"
#include "bsp_core.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Board Support main Init
 */
static void EXP_InitPins(void);

void BSP_Init(void)
{
    /* Init board hardware. */
    EXP_InitPins();


    /* Init core peripheral. */
    bsp_core_init();
}
static void EXP_InitPins(void)
{                                
    /*!< Function assigned for the core: undefined[cm33] */
    /* LPUART2 Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_RXD__LPUART2_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_TXD__LPUART2_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_RXD__LPUART2_RX, 
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_TXD__LPUART2_TX, 
                        IOMUXC_PAD_DSE(15U));

    /* FLEXCAN2 Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO25__CAN2_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO27__CAN2_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO25__CAN2_TX, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO27__CAN2_RX, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* IO Expander & Heater Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);

    /* ONBOARD ADC Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_TXC__LPSPI1_SIN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_RXD0__LPSPI1_SOUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_TXD0__LPSPI1_SCK, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_TXC__LPSPI1_SIN, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_RXD0__LPSPI1_SOUT, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_TXD0__LPSPI1_SCK, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TD3__GPIO4_IO16, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TX_CTL__GPIO4_IO20, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TD2__GPIO4_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TXC__GPIO4_IO21, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TD3__GPIO4_IO16, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TX_CTL__GPIO4_IO20, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TD2__GPIO4_IO17, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TXC__GPIO4_IO21, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PU_MASK);

    /* LASER DAC Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RD3__GPIO4_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RD0__GPIO4_IO24, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RD3__GPIO4_IO27, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RD0__GPIO4_IO24, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* LASER SWITCH Init Pin */
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RD1__GPIO4_IO25, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RD2__GPIO4_IO26, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RD1__GPIO4_IO25, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RD2__GPIO4_IO26, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* PHOTO SPI Init Pin */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO19__LPSPI5_SIN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO20__LPSPI5_SOUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO21__LPSPI5_SCK, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO19__LPSPI5_SIN, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO20__LPSPI5_SOUT, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO21__LPSPI5_SCK, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO07__GPIO2_IO07, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TD0__GPIO4_IO19, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_TD1__GPIO4_IO18, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO07__GPIO2_IO07, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TD0__GPIO4_IO19, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_TD1__GPIO4_IO18, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* TEC Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TMS_SWDIO__GPIO3_IO29, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDO_TRACESWO__GPIO3_IO31, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDI__GPIO3_IO28, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TCLK_SWCLK__GPIO3_IO30, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TMS_SWDIO__GPIO3_IO29, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDO_TRACESWO__GPIO3_IO31, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDI__GPIO3_IO28, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TCLK_SWCLK__GPIO3_IO30, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* I2C SENSOR Init Pin. */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO03__LPI2C4_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO02__LPI2C4_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO03__LPI2C4_SCL, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO02__LPI2C4_SDA, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_SD3_DATA3__GPIO3_IO25, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_MDC__GPIO4_IO14, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_SD3_DATA3__GPIO3_IO25, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_MDC__GPIO4_IO14, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* I2C Highdriver Pump */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO11__LPI2C8_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO10__LPI2C8_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO11__LPI2C8_SCL, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO10__LPI2C8_SDA, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RX_CTL__GPIO4_IO22, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET2_RXC__GPIO4_IO23, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RX_CTL__GPIO4_IO22, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET2_RXC__GPIO4_IO23, 
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    /* USB PWR En GPIO */
    // IOMUXC_SetPinMux(IOMUXC_PAD_SD3_DATA1__GPIO3_IO23, 0U);
    // IOMUXC_SetPinMux(IOMUXC_PAD_SD3_DATA2__GPIO3_IO24, 0U);

    // IOMUXC_SetPinConfig(IOMUXC_PAD_SD3_DATA1__GPIO3_IO23, 
    //                     IOMUXC_PAD_DSE(15U) |
    //                     IOMUXC_PAD_FSEL1(2U) |
    //                     IOMUXC_PAD_PD_MASK);

    // IOMUXC_SetPinConfig(IOMUXC_PAD_SD3_DATA2__GPIO3_IO24, 
    //                     IOMUXC_PAD_DSE(15U) |
    //                     IOMUXC_PAD_FSEL1(2U) |
    //                     IOMUXC_PAD_PD_MASK);
}