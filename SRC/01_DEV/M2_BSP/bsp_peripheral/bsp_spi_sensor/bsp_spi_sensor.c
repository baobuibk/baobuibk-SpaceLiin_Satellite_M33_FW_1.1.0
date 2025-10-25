/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Board Support includes. */
#include "bsp_board.h"
#include "bsp_spi_sensor.h"

/* Component includes. */
#include "sp_spi.h"
#include "sp_gpio.h"

/* Freescale inlcudes. */
#include "fsl_tpm.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define RB_TX_BUFFER_SIZE  2048
#define RB_RX_BUFFER_SIZE  64

#define PHOTO_ADC_TPM_HANDLER TPM2_IRQHandler

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct _spi_device_t_
{
	sp_spi_t  	*p_spi;

	sp_gpio_t 	*cs_port;
	uint16_t 	 cs_pin;
} spi_device_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
sp_spi_t  	 photo_adc_spi_handle;
sp_gpio_t 	 photo_adc_gpio_handle;

spi_device_t photo_adc_spi_device;
lpspi_master_handle_t photo_adc_fsl_spi_handle;

ring_buffer_t       photo_adc_spi_TX_ring_buffer;
sp_spi_irq_frame_t  photo_adc_spi_TX_buffer[RB_TX_BUFFER_SIZE];

ring_buffer_t       photo_adc_spi_RX_ring_buffer;
uint8_t             photo_adc_spi_RX_buffer[RB_RX_BUFFER_SIZE];

uint8_t     cs_state = 0;

/* =========================
 *  TRẠNG THÁI TỐI THIỂU
 * ========================= */
static volatile bool s_spi6_tx_busy = false;   // [PORTABLE] true: đang gửi 1 byte

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData);
static void SPI6_TxIrq_Init_Regs_SingleByte(void);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
lpspi_transfer_t fsl_TX_frame;
uint8_t data = 0xEE;

void bsp_spi_sensor_init()
{
    ring_buffer_init(&photo_adc_spi_TX_ring_buffer, (uint8_t*)photo_adc_spi_TX_buffer, (uint32_t)sizeof(photo_adc_spi_TX_buffer), RB_TX_BUFFER_SIZE, (uint32_t)sizeof(sp_spi_irq_frame_t));
    ring_buffer_init(&photo_adc_spi_RX_ring_buffer, (uint8_t*)photo_adc_spi_RX_buffer, (uint32_t)sizeof(photo_adc_spi_RX_buffer), RB_RX_BUFFER_SIZE, (uint32_t)sizeof(uint8_t));

    sp_spi_init_t photo_adc_spi_init =
    {
        .p_spi_base     = TEC_SPI_BASE,
        .irqn           = TEC_SPI_IRQn,

        .p_master_handle = &photo_adc_fsl_spi_handle,

        .p_TX_buffer    = &photo_adc_spi_TX_ring_buffer,
        .p_RX_buffer    = &photo_adc_spi_RX_ring_buffer,

        .pcs            = kLPSPI_Pcs1,
        .pcsContinuous  = false, 
    };

    SP_SPI_master_irq_init(&photo_adc_spi_handle, &photo_adc_spi_init);
    SPI6_TxIrq_Init_Regs_SingleByte();
    // SP_SPI_master_blocking_init(&photo_adc_spi_handle, &photo_adc_spi_init);

	photo_adc_gpio_handle.p_base = PHOTO_ADC_GPIO_CS_PORT; //Bao added

    photo_adc_spi_device.p_spi   = &photo_adc_spi_handle;
    photo_adc_spi_device.cs_port = &photo_adc_gpio_handle;
    photo_adc_spi_device.cs_pin  = PHOTO_ADC_GPIO_CS_PIN;

    // LPSPI_MasterTransferCreateHandle(photo_adc_spi_device.p_spi->p_spi_base, &photo_adc_fsl_spi_handle, LPSPI_MasterUserCallback, NULL);

    TPM_EnableInterrupts(PHOTO_ADC_TIM_BASE, kTPM_TimeOverflowInterruptEnable);

    EnableIRQ(PHOTO_ADC_TIM_IRQn);

    SP_GPIO_PinWrite(photo_adc_spi_device.cs_port, photo_adc_spi_device.cs_pin, 0);

    fsl_TX_frame.txData      = &data;
    fsl_TX_frame.rxData      = NULL;
    fsl_TX_frame.dataSize    = 1;
    fsl_TX_frame.configFlags = ((kLPSPI_Pcs1 & 3u) << LPSPI_MASTER_PCS_SHIFT) | (0);
}

int SPI6_SendByte_Regs_IT(uint16_t data)
{
    LPSPI_Type *base = LPSPI6;                                   // [CMSIS]
    int rc = 0;                                                  // [PORTABLE]

    // uint32_t p = _crit_enter();                                  // [CMSIS]
    if (s_spi6_tx_busy)                                          // [PORTABLE]
    {
        rc = -1;  // bận
        // _crit_exit(p);                                           // [CMSIS]
        return rc;
    }

    /* Đảm bảo TX FIFO đang trống (theo mô hình "1 ô FIFO")                 // [CMSIS] */
    if ((base->SR & LPSPI_SR_TDF_MASK) == 0u)
    {
        rc = -1;  // hiếm khi xảy ra nếu ta quản busy đúng; phòng hờ         // [PORTABLE]
        // _crit_exit(p);                                                       // [CMSIS]
        return rc;
    }

    /* Clear WCF trước khi bắt đầu để tránh bắt nhầm cờ cũ                  // [CMSIS] */
    base->SR = LPSPI_SR_WCF_MASK;

    /* Đánh dấu bận và nạp đúng 1 byte vào TDR                              // [PORTABLE]+[CMSIS] */
    s_spi6_tx_busy = true;                                     // [PORTABLE]
    base->TDR = (uint32_t)data;                                // [CMSIS]

    /* Bật ngắt WCF (word complete) — không bật TDIE để khỏi phải “đổ FIFO”  // [CMSIS] */
    base->IER |= LPSPI_IER_WCIE_MASK;

    // _crit_exit(p);                                                // [CMSIS]
    return rc;                                                    // [PORTABLE]
}

void LPSPI6_DriverIRQHandler(void);
void LPSPI6_DriverIRQHandler(void)
{
    LPSPI_Type *base = LPSPI6;                                      // [CMSIS]
    volatile uint32_t sr = base->SR;                                         // [CMSIS]

    /* Clear lỗi nếu có (TX/ RX/ Data Match)                               // [CMSIS] */
    volatile uint32_t err = sr & (LPSPI_SR_TEF_MASK | LPSPI_SR_REF_MASK | LPSPI_SR_DMF_MASK);
    if (err)
    {
        base->SR = err;  // W1C                                           // [CMSIS]
    }

    /* Xử lý “xong 1 word”                                                 // [CMSIS] */
    if (sr & LPSPI_SR_WCF_MASK)
    {
        base->SR  = LPSPI_SR_WCF_MASK;     // W1C                          // [CMSIS]
        // base->IER &= ~LPSPI_IER_WCIE_MASK; // tắt ngắt                     // [CMSIS]
        s_spi6_tx_busy = false;            // rảnh                          // [PORTABLE]
    }

    /* (Tuỳ chọn) Clear thêm TCF/FCF nếu bạn có bật CONT/CONTC             // [CMSIS] */
    volatile uint32_t done = sr & (LPSPI_SR_TCF_MASK | LPSPI_SR_FCF_MASK);
    if (done)
    {
        base->SR = done;  // W1C                                           // [CMSIS]
    }
}

void PHOTO_ADC_TPM_HANDLER(void)
{
    /* Clear interrupt flag.*/
    // TPM_ClearStatusFlags(PHOTO_ADC_TIM_BASE, kTPM_TimeOverflowFlag);

    PHOTO_ADC_TIM_BASE->STATUS = kTPM_TimeOverflowFlag;

    //	GPIOD->BSRR = GPIO_BSRR_BR_10; 	// CV LOW
	PHOTO_ADC_GPIO_CS_PORT->PCOR = 1UL << PHOTO_ADC_GPIO_CV_PIN;

    //	GPIOD->BSRR = GPIO_BSRR_BS_9;  	// CS HIGH
	PHOTO_ADC_GPIO_CS_PORT->PSOR = 1UL << PHOTO_ADC_GPIO_CS_PIN;

    //	GPIOD->BSRR = GPIO_BSRR_BS_10; 	// CV HIGH
	PHOTO_ADC_GPIO_CV_PORT->PSOR = 1UL << PHOTO_ADC_GPIO_CV_PIN;

    //	GPIOD->BSRR = GPIO_BSRR_BR_9; 	// CS LOW
	PHOTO_ADC_GPIO_CS_PORT->PCOR = 1UL << PHOTO_ADC_GPIO_CS_PIN;

    //	SPI2->DR = 0xAAAA;				// Send DUMY to SPI
	// PHOTO_SPI->DR = 0xAAAA;
    
    /* Clear WCF trước khi bắt đầu để tránh bắt nhầm cờ cũ // [CMSIS] */
    LPSPI6->SR = LPSPI_SR_WCF_MASK;

    /* Đánh dấu bận và nạp đúng 1 byte vào TDR // [PORTABLE]+[CMSIS] */
    s_spi6_tx_busy = true;                     // [PORTABLE]
    LPSPI6->TDR = (uint32_t)0xAAAA;              // [CMSIS]

    // PHOTO_ADC_GPIO_PORT->PCOR = 1UL << PHOTO_ADC_GPIO_SPI_CS_PIN;

    // __DSB();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void SPI6_TxIrq_Init_Regs_SingleByte(void)
{
    LPSPI_Type *base = LPSPI6;                                   // [CMSIS]

    /* (khuyến nghị) Flush FIFO trước nếu nghi ngờ trạng thái cũ            // [CMSIS] */
    base->CR &= ~LPSPI_CR_MEN_MASK;          // MEN=0
    base->CR |=  LPSPI_CR_RRF_MASK | LPSPI_CR_RTF_MASK;  // reset RX/TX FIFO
    base->CR |=  LPSPI_CR_MEN_MASK;          // MEN=1

    /* 8-bit/frame + mask RX                                                // [CMSIS] */
    base->TCR = (base->TCR & ~LPSPI_TCR_FRAMESZ_MASK)
              |  LPSPI_TCR_FRAMESZ(16u - 1u)
              |  LPSPI_TCR_RXMSK_MASK;

    /* TXWATER=0, RXWATER=0                                                 // [CMSIS] */
    base->FCR = LPSPI_FCR_TXWATER(0u) | LPSPI_FCR_RXWATER(0u);

    /* Clear cờ W1C nếu còn treo                                            // [CMSIS] */
    base->SR = LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK
             | LPSPI_SR_DMF_MASK | LPSPI_SR_REF_MASK | LPSPI_SR_TEF_MASK;

    /* Tắt toàn bộ enable ngắt ban đầu                                      // [CMSIS] */
    base->IER = 0;
    LPSPI6->IER |= LPSPI_IER_WCIE_MASK;

    /* NVIC cho LPSPI6                                                      // [CMSIS] */
    NVIC_ClearPendingIRQ(LPSPI6_IRQn);
    NVIC_SetPriority(LPSPI6_IRQn, 6);   // chỉnh theo hệ thống/FreeRTOS của bạn
    NVIC_EnableIRQ(LPSPI6_IRQn);

    /* Trạng thái app                                                       // [PORTABLE] */
    // uint32_t p = _crit_enter();
    s_spi6_tx_busy = false;
    // _crit_exit(p);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
