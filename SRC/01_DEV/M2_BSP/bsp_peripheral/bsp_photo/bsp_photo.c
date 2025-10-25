/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "bsp_photo.h"

#include "bsp.h"

#include "delay.h"

#include "fsl_tpm.h"
#include "fsl_lpspi.h"

#include "MIMX9352_cm33.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint16_t photo_data[24][128] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_PD_position(int x);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t photo_spi_set_count = 0;
uint32_t photo_spi_count	 = 0;
uint32_t current_channel	 = 0;
uint8_t  is_spi_counter_finish = 0;

adg1414_dev_t photo_sw_dev =
{
	.spi = &photo_adc_spi,
	.cs  = { .port = 2, .pin = 7, .bStatus = true },
	.num_of_sw = 3
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void bsp_photo_init(void)
{
	adg1414_chain_init(&photo_sw_dev, &photo_adc_spi, &photo_sw_dev.cs, photo_sw_dev.num_of_sw);
}

void bsp_photo_int_sw_on(uint8_t channel)
{
	uint8_t real_channel = map_PD_position(channel);
	spi_io_set_mode(photo_sw_dev.spi, 1);
	adg1414_chain_sw_on(&photo_sw_dev, real_channel);
}

void bsp_photo_int_sw_off(uint8_t channel)
{
	uint8_t real_channel = map_PD_position(channel);
	spi_io_set_mode(photo_sw_dev.spi, 1);
	adg1414_chain_sw_off(&photo_sw_dev, real_channel);
}

void bsp_photo_int_all_sw_off(void)
{
	spi_io_set_mode(photo_sw_dev.spi, 1);
	adg1414_chain_all_sw_off(&photo_sw_dev);
}

void bsp_photo_setup_timmer(uint32_t sampling_rate_khz)
{
	tpm_config_t exp_tpm_config;

	TPM_GetDefaultConfig(&exp_tpm_config);

	uint32_t exp_period_us =  1000 / sampling_rate_khz;

	/* TPM clock divide by TPM_PRESCALER */
    exp_tpm_config.prescale = TPM_CalculateCounterClkDiv(PHOTO_ADC_TIM_BASE, (sampling_rate_khz * 1000), PHOTO_ADC_TIM_CLK_FREQ);

    /* Initialize TPM module */
    TPM_Init(PHOTO_ADC_TIM_BASE, &exp_tpm_config);

	/* Set timer period */
    TPM_SetTimerPeriod(PHOTO_ADC_TIM_BASE, USEC_TO_COUNT(exp_period_us, PHOTO_ADC_TIM_CLK_FREQ / (1U << exp_tpm_config.prescale)));

    TPM_EnableInterrupts(PHOTO_ADC_TIM_BASE, kTPM_TimeOverflowInterruptEnable);

	NVIC_ClearPendingIRQ(PHOTO_ADC_TIM_IRQn);
    NVIC_SetPriority   	(PHOTO_ADC_TIM_IRQn, 3);
    NVIC_EnableIRQ     	(PHOTO_ADC_TIM_IRQn);
}

void bsp_photo_start_timer(void)
{
    TPM_Type *base = PHOTO_ADC_TIM_BASE;

    // Optional: reset counter to 0 before starting
    base->CNT = 0U;

    // Start TPM: select system clock (CMOD = 1). Preserve prescaler and other bits.
    uint32_t sc = base->SC;
    sc &= ~TPM_SC_CMOD_MASK;        		// clear CMOD
    sc |= TPM_SC_CMOD(kTPM_SystemClock);    // 1 = TPM counter increments on TPM input clock
    base->SC = sc;
}

void bsp_photo_stop_timer(void)
{
    TPM_Type *base = PHOTO_ADC_TIM_BASE;

    // Stop TPM: disable clock to counter (CMOD = 0). Preserve other bits.
    base->SC &= ~TPM_SC_CMOD_MASK;
}

#ifndef PHOTO_ADC_SPI_IRQ_PRIO
#define PHOTO_ADC_SPI_IRQ_PRIO 4
#endif

uint32_t bsp_photo_spi_irq_init(void)
{
    LPSPI_Type *base = PHOTO_ADC_SPI_BASE;

    if (base == NULL)
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

    /* --- Force 16-bit frames (FRAMESZ = bits-1 = 15) --- */
    uint32_t tcr = base->TCR;
    tcr &= ~LPSPI_TCR_FRAMESZ_MASK;
    tcr |=  LPSPI_TCR_FRAMESZ(15);
    base->TCR = tcr;

    /* --- FIFO watermarks ---
       RX IRQ asserts when RX count > RXWATER.
       To interrupt when there is at least 1 frame, use RXWATER=0. */
    uint32_t fcr = base->FCR;
    fcr &= ~(LPSPI_FCR_RXWATER_MASK | LPSPI_FCR_TXWATER_MASK);
    fcr |=  LPSPI_FCR_RXWATER(0)   /* ≥1 frame triggers RDIE */
         |  LPSPI_FCR_TXWATER(0);  /* default; TDIE is disabled anyway */
    base->FCR = fcr;

    /* No DMA in IRQ mode */
    base->DER = 0u;

    /* Clean FIFOs & sticky flags */
    base->CR |= (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);
    base->SR  =  (LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK);

    /* Enable only Receive Data interrupt */
	uint32_t ier = base->IER;
	ier &= ~(LPSPI_IER_RDIE_MASK | LPSPI_IER_TDIE_MASK);
	ier |= LPSPI_IER_RDIE_MASK;
    base->IER = ier;

    /* Re-enable module */
    base->CR |= LPSPI_CR_MEN_MASK;

    /* NVIC: enable ISR */
    NVIC_ClearPendingIRQ(PHOTO_ADC_SPI_IRQn);
    NVIC_SetPriority   	(PHOTO_ADC_SPI_IRQn, PHOTO_ADC_SPI_IRQ_PRIO);
    NVIC_EnableIRQ     	(PHOTO_ADC_SPI_IRQn);

    return ERROR_OK;
}

uint32_t bsp_photo_spi_irq_deinit(void)
{
    LPSPI_Type *base = PHOTO_ADC_SPI_BASE;

    if (base == NULL)
	{
		return ERROR_INVALID_PARAM;
	}

    /* Stop further IRQs first */
    NVIC_DisableIRQ(PHOTO_ADC_SPI_IRQn);
    NVIC_ClearPendingIRQ(PHOTO_ADC_SPI_IRQn);

	delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        return ERROR_TIMEOUT;
    }

	/* Disable SPI before modifying configuration */
    base->CR &= ~LPSPI_CR_MEN_MASK;

    /* Disable all LPSPI interrupts */
    base->IER = 0u;

    /* Restore polling defaults: 8-bit frames (FRAMESZ=7) */
    uint32_t tcr = base->TCR;
    tcr &= ~LPSPI_TCR_FRAMESZ_MASK;
    tcr |=  LPSPI_TCR_FRAMESZ(7);
    base->TCR = tcr;

    /* (Optional) leave watermarks at 0; they won't matter with IER=0.
       Housekeeping: flush FIFOs & clear flags */
    base->CR |= (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);
    base->SR  =  (LPSPI_SR_WCF_MASK | LPSPI_SR_FCF_MASK | LPSPI_SR_TCF_MASK);

    /* Back on for polling use */
    base->CR |= LPSPI_CR_MEN_MASK;

    return ERROR_OK;
}

static uint32_t CV_PIN = 1UL << PHOTO_ADC_GPIO_CV_PIN;
static uint32_t CS_PIN = 1UL << PHOTO_ADC_GPIO_CS_PIN;

void TPM2_IRQHandler(void);
void TPM2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
	PHOTO_ADC_TIM_BASE->STATUS = kTPM_TimeOverflowFlag;

	//	GPIOD->BSRR = GPIO_BSRR_BR_10; 	// CV LOW
	PHOTO_ADC_GPIO_CV_PORT->PCOR = CV_PIN;

	//	GPIOD->BSRR = GPIO_BSRR_BS_9;  	// CS HIGH
	PHOTO_ADC_GPIO_CS_PORT->PSOR = CS_PIN;

	//	GPIOD->BSRR = GPIO_BSRR_BS_10; 	// CV HIGH
	PHOTO_ADC_GPIO_CV_PORT->PSOR = CV_PIN;

	//	GPIOD->BSRR = GPIO_BSRR_BR_9; 	// CS LOW
	PHOTO_ADC_GPIO_CS_PORT->PCOR = CS_PIN;

	//	LPSPI5->TDR = 0xAAAA;
	PHOTO_ADC_SPI_BASE->TDR = 0xAAAA;
}

#define PHOTO_MAX_CHANNEL 24
void LPSPI5_IRQHandler(void);
void LPSPI5_IRQHandler(void)
{
	LPSPI_Type *base = PHOTO_ADC_SPI_BASE;
    uint32_t sr = base->SR;

    if (sr & LPSPI_SR_RDF_MASK)
	{
        uint16_t rx = (uint16_t)(base->RDR & 0xFFFFu); // 16-bit frame
        // Push rx into buffer

		photo_data[current_channel][photo_spi_count] = rx;
		photo_spi_count += 1;

		if (photo_spi_count < photo_spi_set_count)
		{
			return;
		}
		// else if (photo_spi_count >= photo_spi_set_count)
		// reset the spi count
		photo_spi_count = 0;
		is_spi_counter_finish = 1;

		TPM_Type *base = PHOTO_ADC_TIM_BASE;

		// Stop TPM: disable clock to counter (CMOD = 0). Preserve other bits.
		base->SC &= ~TPM_SC_CMOD_MASK;

		// Reset counter to 0
    	base->CNT = 0U;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int8_t map_PD_position(int x)
{
    static const uint8_t map[] =
	{
        0,
        24, 23, 22, 21, 17, 18, 19, 20,
        16, 15, 14, 13, 9, 10, 11, 12,
        8, 7, 6, 5, 1, 2, 3, 4
    };

    if (x < 1 || x > 24)
	{
		return -1;
	}

    return map[x];
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */