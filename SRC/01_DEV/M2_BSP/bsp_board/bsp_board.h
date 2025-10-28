#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

/*********************DEBUG DB9**********************/
#define DEBUG_DB9_LPUART_CLOCK_ROOT     kCLOCK_Root_Lpuart2
#define DEBUG_DB9_LPUART_CLOCK_GATE     kCLOCK_Lpuart2
#define DEBUG_DB9_LPUART_CLK_FREQ       CLOCK_GetIpFreq(DEBUG_DB9_LPUART_CLOCK_ROOT)

#define DEBUG_DB9_LPUART_BASE           LPUART2
#define DEBUG_DB9_LPUART_IRQn           LPUART2_IRQn

#define DEBUG_DB9_LPUART_BAUDRATE       115200

/*********************LIBCSP CAN**********************/
#define LIBCSP_CAN_CLOCK_ROOT           kCLOCK_Root_Can2
#define LIBCSP_CAN_CLOCK_GATE           kCLOCK_Can2
#define LIBCSP_CAN_CLK_FREQ             CLOCK_GetIpFreq(LIBCSP_CAN_CLOCK_ROOT)

#define LIBCSP_CAN_BASE                 CAN2
#define LIBCSP_CAN_IRQn                 CAN2_IRQn

#define LIBCSP_CAN_BITRATE_BPS          (500000u)

/*********************TEC SPI**********************/
#define TEC_SPI_CLOCK_ROOT              kCLOCK_Root_Lpspi6
#define TEC_SPI_CLOCK_GATE              kCLOCK_Lpspi6
#define TEC_SPI_CLK_FREQ                CLOCK_GetIpFreq(TEC_SPI_CLOCK_ROOT)

#define TEC_SPI_BASE                    LPSPI6
#define TEC_SPI_IRQn                    LPSPI6_IRQn

#define TEC_SPI_BAUDRATE                (25000000)

#define TEC_SPI_GPIO_CS_CLOCK_ROOT      kCLOCK_Root_BusWakeup
#define TEC_SPI_GPIO_CS_CLOCK_GATE      kCLOCK_Gpio2
#define TEC_SPI_GPIO_CS_CLK_FREQ        CLOCK_GetIpFreq(TEC_SPI_GPIO_CS_CLOCK_ROOT)

#define TEC_SPI_GPIO_PORT               GPIO2
#define TEC_SPI_GPIO_SPI_CS_PIN         0U

/*********************I/O EXPANDER I2C**********************/
#define IO_EXPAN_CLOCK_ROOT              kCLOCK_Root_Lpi2c7
#define IO_EXPAN_CLOCK_GATE              kCLOCK_Lpi2c7
#define IO_EXPAN_CLK_FREQ                CLOCK_GetIpFreq(IO_EXPAN_CLOCK_ROOT)

#define IO_EXPAN_BASE                    LPI2C7
#define IO_EXPAN_IRQn                    LPI2C7_IRQn

#define IO_EXPAN_BAUDRATE_HZ             (100000U)

/*********************ONBOARD ADC SPI**********************/
#define ONBOARD_ADC_SPI_CLOCK_ROOT        kCLOCK_Root_Lpspi1
#define ONBOARD_ADC_SPI_CLOCK_GATE        kCLOCK_Lpspi1
#define ONBOARD_ADC_SPI_CLK_FREQ          CLOCK_GetIpFreq(ONBOARD_ADC_SPI_CLOCK_ROOT)

#define ONBOARD_ADC_SPI_BASE              LPSPI1
#define ONBOARD_ADC_SPI_IRQn              LPSPI1_IRQn

#define ONBOARD_ADC_SPI_BAUDRATE          (500000)

#define ONBOARD_ADC_GPIO_CS_CLOCK_ROOT    kCLOCK_Root_BusWakeup
#define ONBOARD_ADC_GPIO_CS_CLOCK_GATE    kCLOCK_Gpio4
#define ONBOARD_ADC_GPIO_CS_CLK_FREQ      CLOCK_GetIpFreq(ONBOARD_ADC_GPIO_CS_CLOCK_ROOT)

#define ONBOARD_ADC_GPIO_CS_PORT          GPIO4
#define ONBOARD_ADC_GPIO_CS0_PIN          16U
#define ONBOARD_ADC_GPIO_CS1_PIN          20U
#define ONBOARD_ADC_GPIO_SYNC0_PIN        17U
#define ONBOARD_ADC_GPIO_SYNC1_PIN        21U

/*********************LASER DAC**********************/
#define LASER_DAC_SPI_CLOCK_ROOT        kCLOCK_Root_Lpspi1
#define LASER_DAC_SPI_CLOCK_GATE        kCLOCK_Lpspi1
#define LASER_DAC_SPI_CLK_FREQ          CLOCK_GetIpFreq(LASER_DAC_SPI_CLOCK_ROOT)

#define LASER_DAC_SPI_BASE              LPSPI1
#define LASER_DAC_SPI_IRQn              LPSPI1_IRQn

#define LASER_DAC_SPI_BAUDRATE          (500000)

#define LASER_DAC_GPIO_CS_CLOCK_ROOT    kCLOCK_Root_BusWakeup
#define LASER_DAC_GPIO_CS_CLOCK_GATE    kCLOCK_Gpio4
#define LASER_DAC_GPIO_CS_CLK_FREQ      CLOCK_GetIpFreq(LASER_DAC_GPIO_CS_CLOCK_ROOT)

#define LASER_DAC_GPIO_CS_PORT          GPIO4
#define LASER_DAC_GPIO_CS_PIN           27U
#define LASER_DAC_GPIO_LATCH_PIN        24U

/*********************LASER SW**********************/
#define LASER_SW_SPI_CLOCK_ROOT         kCLOCK_Root_Lpspi1
#define LASER_SW_SPI_CLOCK_GATE         kCLOCK_Lpspi1
#define LASER_SW_SPI_CLK_FREQ           CLOCK_GetIpFreq(LASER_SW_SPI_CLOCK_ROOT)

#define LASER_SW_SPI_BASE               LPSPI1
#define LASER_SW_SPI_IRQn               LPSPI1_IRQn

#define LASER_SW_SPI_BAUDRATE           (500000)

#define LASER_SW_GPIO_CS_CLOCK_ROOT     kCLOCK_Root_BusWakeup
#define LASER_SW_GPIO_CS_CLOCK_GATE     kCLOCK_Gpio4
#define LASER_SW_GPIO_CS_CLK_FREQ       CLOCK_GetIpFreq(LASER_SW_GPIO_CS_CLOCK_ROOT)

#define LASER_SW_GPIO_CS_PORT           GPIO4
#define LASER_SW_GPIO_INT_CS_PIN        25U
#define LASER_SW_GPIO_EXT_CS_PIN        26U

/*********************PHOTO ADC SPI**********************/
#define PHOTO_ADC_SPI_CLOCK_ROOT        kCLOCK_Root_Lpspi5
#define PHOTO_ADC_SPI_CLOCK_GATE        kCLOCK_Lpspi5
#define PHOTO_ADC_SPI_CLK_FREQ          CLOCK_GetIpFreq(PHOTO_ADC_SPI_CLOCK_ROOT)

#define PHOTO_ADC_SPI_BASE              LPSPI5
#define PHOTO_ADC_SPI_IRQn              LPSPI5_IRQn

#define PHOTO_ADC_SPI_BAUDRATE          (25000000)

#define PHOTO_ADC_GPIO_CS_CLOCK_ROOT    kCLOCK_Root_BusWakeup
#define PHOTO_ADC_GPIO_CS_CLOCK_GATE    kCLOCK_Gpio4
#define PHOTO_ADC_GPIO_CS_CLK_FREQ      CLOCK_GetIpFreq(PHOTO_ADC_GPIO_CS_CLOCK_ROOT)

#define PHOTO_ADC_GPIO_CS_PORT          GPIO4
#define PHOTO_ADC_GPIO_CS_PIN           19U

#define PHOTO_ADC_GPIO_CV_PORT          GPIO4
#define PHOTO_ADC_GPIO_CV_PIN           18U

/*********************PHOTO SW**********************/
#define PHOTO_SW_SPI_CLOCK_ROOT         kCLOCK_Root_Lpspi5
#define PHOTO_SW_SPI_CLOCK_GATE         kCLOCK_Lpspi5
#define PHOTO_SW_SPI_CLK_FREQ           CLOCK_GetIpFreq(PHOTO_SW_SPI_CLOCK_ROOT)

#define PHOTO_SW_SPI_BASE               LPSPI5
#define PHOTO_SW_SPI_IRQn               LPSPI5_IRQn

#define PHOTO_SW_SPI_BAUDRATE           (25000000)

#define PHOTO_SW_GPIO_CS_CLOCK_ROOT     kCLOCK_Root_BusWakeup
#define PHOTO_SW_GPIO_CS_CLOCK_GATE     kCLOCK_Gpio2
#define PHOTO_SW_GPIO_CS_CLK_FREQ       CLOCK_GetIpFreq(PHOTO_SW_GPIO_CS_CLOCK_ROOT)

#define PHOTO_SW_GPIO_CS_PORT           GPIO2
#define PHOTO_SW_GPIO_CS_PIN            7U

/*********************PHOTO ADC TIM**********************/
#define PHOTO_ADC_TIM_CLOCK_ROOT        kCLOCK_Root_Tpm2
#define PHOTO_ADC_TIM_CLOCK_GATE        kCLOCK_Tpm2
#define PHOTO_ADC_TIM_CLK_FREQ          CLOCK_GetIpFreq(PHOTO_ADC_TIM_CLOCK_ROOT)

#define PHOTO_ADC_TIM_BASE              TPM2
#define PHOTO_ADC_TIM_IRQn              TPM2_IRQn

#define PHOTO_ADC_TIM_PERIOD_US         (2U)
/* Calculate the clock division based on the PWM frequency to be obtained */
#define PHOTO_ADC_TIM_PRESCALER         TPM_CalculateCounterClkDiv(PHOTO_ADC_TIM_BASE, 1000000U / PHOTO_ADC_TIM_PERIOD_US, PHOTO_ADC_TIM_CLK_FREQ);

/*********************LASER IMX ADC**********************/
#define LASER_IMX_ADC_CLOCK_ROOT        kCLOCK_Root_Adc
#define LASER_IMX_ADC_CLOCK_GATE        kCLOCK_Adc1
#define LASER_IMX_ADC_CLK_FREQ          CLOCK_GetIpFreq(LASER_IMX_ADC_CLOCK_GATE)

#define LASER_IMX_ADC_BASE              ADC1

/*********************LASER ADC TIM**********************/
#define LASER_ADC_TIM_CLOCK_ROOT        kCLOCK_Root_Tpm3
#define LASER_ADC_TIM_CLOCK_GATE        kCLOCK_Tpm3
#define LASER_ADC_TIM_CLK_FREQ          CLOCK_GetIpFreq(LASER_ADC_TIM_CLOCK_ROOT)

#define LASER_ADC_TIM_BASE              TPM3
#define LASER_ADC_TIM_IRQn              TPM3_IRQn

#define LASER_ADC_TIM_PERIOD_US         (2U)
/* Calculate the clock division based on the PWM frequency to be obtained */
#define LASER_ADC_TIM_PRESCALER         TPM_CalculateCounterClkDiv(LASER_ADC_TIM_BASE, 1000000U / LASER_ADC_TIM_PERIOD_US, LASER_ADC_TIM_CLK_FREQ);

/*********************I/O EXPANDER I2C**********************/
#define I2C_SENSOR_CLOCK_ROOT            kCLOCK_Root_Lpi2c4
#define I2C_SENSOR_CLOCK_GATE            kCLOCK_Lpi2c4
#define I2C_SENSOR_CLK_FREQ              CLOCK_GetIpFreq(I2C_SENSOR_CLOCK_ROOT)

#define I2C_SENSOR_BASE                  LPI2C4
#define I2C_SENSOR_IRQn                  LPI2C4_IRQn

#define I2C_SENSOR_BAUDRATE_HZ           (100000U)

#define I2C_SENSOR_GPIO_EN0_CLOCK_ROOT   kCLOCK_Root_BusWakeup
#define I2C_SENSOR_GPIO_EN0_CLOCK_GATE   kCLOCK_Gpio4
#define I2C_SENSOR_GPIO_EN0_CLK_FREQ     CLOCK_GetIpFreq(I2C_SENSOR_GPIO_EN0_CLOCK_ROOT)

#define I2C_SENSOR_GPIO_EN0_PORT         GPIO4
#define I2C_SENSOR_GPIO_EN0_PIN          14U

#define I2C_SENSOR_GPIO_EN1_CLOCK_ROOT   kCLOCK_Root_BusWakeup
#define I2C_SENSOR_GPIO_EN1_CLOCK_GATE   kCLOCK_Gpio3
#define I2C_SENSOR_GPIO_EN1_CLK_FREQ     CLOCK_GetIpFreq(I2C_SENSOR_GPIO_EN1_CLOCK_ROOT)

#define I2C_SENSOR_GPIO_EN1_PORT         GPIO3
#define I2C_SENSOR_GPIO_EN1_PIN          25U

/*********************PUMP I2C**********************/
#define I2C_PUMP_CLOCK_ROOT            kCLOCK_Root_Lpi2c8
#define I2C_PUMP_CLOCK_GATE            kCLOCK_Lpi2c8
#define I2C_PUMP_CLK_FREQ              CLOCK_GetIpFreq(I2C_PUMP_CLOCK_ROOT)

#define I2C_PUMP_BASE                  LPI2C8
#define I2C_PUMP_IRQn                  LPI2C8_IRQn

#define I2C_PUMP_BAUDRATE_HZ           (100000U)

#define I2C_PUMP_GPIO_EN_CLOCK_ROOT   kCLOCK_Root_BusWakeup
#define I2C_PUMP_GPIO_EN_CLOCK_GATE   kCLOCK_Gpio4
#define I2C_PUMP_GPIO_EN_CLK_FREQ     CLOCK_GetIpFreq(I2C_PUMP_GPIO_EN_CLOCK_ROOT)

#define I2C_PUMP_GPIO_EN_PORT         GPIO4
#define I2C_PUMP_GPIO_EN_PIN          22U
#define I2C_PUMP_GPIO_EN2_PIN         23U

/*********************USP PWR EN GPIO**********************/
#define USB_PWR_GPIO_EN0_CLOCK_ROOT   kCLOCK_Root_BusWakeup
#define USB_PWR_GPIO_EN0_CLOCK_GATE   kCLOCK_Gpio3
#define USB_PWR_GPIO_EN0_CLK_FREQ     CLOCK_GetIpFreq(USB_PWR_GPIO_EN0_CLOCK_ROOT)

#define USB_PWR_GPIO_EN0_PORT         GPIO3
#define USB_PWR_GPIO_EN0_PIN          23U

#define USB_PWR_GPIO_EN1_CLOCK_ROOT   kCLOCK_Root_BusWakeup
#define USB_PWR_GPIO_EN1_CLOCK_GATE   kCLOCK_Gpio3
#define USB_PWR_GPIO_EN1_CLK_FREQ     CLOCK_GetIpFreq(USB_PWR_GPIO_EN1_CLOCK_ROOT)

#define USB_PWR_GPIO_EN1_PORT         GPIO3
#define USB_PWR_GPIO_EN1_PIN          24U

#define RPMSG_SMEM_BASE  ((uint8_t *)0xA4220000U) 
#define RPMSG_SMEM_SIZE  (0x08000000U)   // 128 MB

#define RAM_TEST_BASE  ((uint8_t *)0xA4220000U)
#define RAM_TEST_END   ((uint8_t *)(0xA4220000U + 10U + 0x08000000U))



#endif /* _BSP_BOARD_H_ */
