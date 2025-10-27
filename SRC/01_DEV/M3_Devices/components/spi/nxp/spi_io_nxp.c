/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "spi_io.h"

#include "fsl_lpspi.h"
#include "MIMX9352_cm33.h"

#include "delay.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SPI_MAX_BUS_NUMBER 8

#define LPSPI_BYTE_TIMEOUT_US 100u

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static LPSPI_Type* const spi_periph[SPI_MAX_BUS_NUMBER + 1] =
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

// static uint32_t const spi_clock_en[SPI_MAX_BUS_NUMBER + 1] =
// {
//     0,
//     RCC_APB2ENR_SPI1EN,
//     RCC_APB1ENR_SPI2EN,
//     RCC_APB1ENR_SPI3EN,
//     RCC_APB2ENR_SPI4EN,
//     RCC_APB2ENR_SPI5EN,
//     RCC_APB2ENR_SPI6EN
// };

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t spi_io_set_mode(SPI_Io_t *me, uint8_t spi_mode)
{
    if (me == NULL || me->ui32SpiPort == 0 || me->ui32SpiPort > SPI_MAX_BUS_NUMBER)
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
        osSemaphoreGiven(&me->lock);
        return ERROR_INVALID_PARAM;
    }

    delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        osSemaphoreGiven(&me->lock);
        return ERROR_TIMEOUT;
    }

    /* Disable SPI before modifying configuration */
    base->CR &= ~LPSPI_CR_MEN_MASK;

    // Map SPI mode to CPOL/CPHA (mode: 0..3)
    uint32_t cpol = (spi_mode >> 1) & 0x1u; // mode 2/3 => CPOL=1
    uint32_t cpha = (spi_mode >> 0) & 0x1u; // mode 1/3 => CPHA=1

    // Update TCR: CPOL/CPHA bits (preserve the rest)
    uint32_t temp_tcr = base->TCR;

    /* Clear CPOL and CPHA bits first */
    temp_tcr &= ~(LPSPI_TCR_CPOL_MASK | LPSPI_TCR_CPHA_MASK);

    // Implement cpol, cpha accordingly
    temp_tcr |=  (LPSPI_TCR_CPOL(cpol) | LPSPI_TCR_CPHA(cpha));
    base->TCR = temp_tcr;

    /* Enable SPI again */
    base->CR |= LPSPI_CR_MEN_MASK;

    osSemaphoreGiven(&me->lock);

    return ERROR_OK;
}

/******************************************************************************/
/*************************** Synchronous Functions ****************************/
/******************************************************************************/
uint32_t spi_io_read_sync(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    LPSPI_Type*base = spi_periph[me->ui32SpiPort];

    if (!base || !pui8RxBuff || ui32Length == 0U)
    {
        return ERROR_INVALID_PARAM;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        osSemaphoreGiven(&me->lock);
        return ERROR_TIMEOUT;
    }

    for (uint32_t i = 0; i < ui32Length; i++)
    {
        // Wait for TX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }
        
        // dummy
        base->TDR = 0xAA;

        // Wait for RX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }

        // read 1 byte
        pui8RxBuff[i] = (uint8_t)base->RDR;
    }

    // Finish: wait complete & not busy (optional but safe)
    // while (!(base->SR & LPSPI_SR_TCF_MASK))
    // {
    //     __NOP();
    // }

    // while  ( base->SR & LPSPI_SR_MBF_MASK )
    // {
    //     __NOP();
    // }

    // Optional: clear TCF once for neatness
    base->SR = LPSPI_SR_TCF_MASK;

    osSemaphoreGiven(&me->lock);

    return ERROR_OK;
}

uint32_t spi_io_write_sync(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    LPSPI_Type *base = spi_periph[me->ui32SpiPort];

    if (!base || !pui8TxBuff || ui32Length == 0U)
    {
        return ERROR_INVALID_PARAM;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        osSemaphoreGiven(&me->lock);
        return ERROR_TIMEOUT;
    }

    for (uint32_t i = 0; i < ui32Length; i++)
    {
        // Wait for TX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }

        // Write one byte
        base->TDR = (uint32_t)pui8TxBuff[i];

        // Wait for RX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }

        // discard
        (void)base->RDR;  
    }

    // Optionally ensure transfer complete & not busy
    // while (!(base->SR & LPSPI_SR_TCF_MASK)) { __NOP(); }
    // while (base->SR & LPSPI_SR_MBF_MASK)    { __NOP(); }

    // Optional: clear sticky TCF
    base->SR = LPSPI_SR_TCF_MASK;

    osSemaphoreGiven(&me->lock);

    return ERROR_OK;
}

uint32_t spi_io_transfer_sync(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    LPSPI_Type *base = spi_periph[me->ui32SpiPort];

    if (!base || !pui8TxBuff || !pui8RxBuff || ui32Length == 0U)
    {
        return ERROR_INVALID_PARAM;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    delay_init();

    // Ensure module not busy (equiv. to STM32 BSY=0)
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
    {
        osSemaphoreGiven(&me->lock);
        return ERROR_TIMEOUT;
    }

    for (uint32_t i = 0; i < ui32Length; i++)
    {
        // Wait for TX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }

        // Write one byte
        base->TDR = (uint32_t)pui8TxBuff[i];

        // Wait for RX to ready
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
        {
            osSemaphoreGiven(&me->lock);
            return ERROR_TIMEOUT;
        }

        // Read one byte
        pui8RxBuff[i] = (uint8_t)base->RDR;
    }

    // Optionally ensure transfer complete & not busy
    // while (!(base->SR & LPSPI_SR_TCF_MASK)) { __NOP(); }
    // while (base->SR & LPSPI_SR_MBF_MASK)    { __NOP(); }

    // Optional: clear sticky TCF
    base->SR = LPSPI_SR_TCF_MASK;

    osSemaphoreGiven(&me->lock);

    return ERROR_OK;
}


/******************************************************************************/
/**************************** Asynchronous Functions **************************/
/******************************************************************************/

// Stub implementations for async (not supported)
uint32_t spi_io_read_async(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_transfer_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_and_read_async(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32TxLength, uint8_t *pui8RxBuff, uint32_t ui32RxLength)
{
    (void)me; (void)pui8TxBuff; (void)ui32TxLength; (void)pui8RxBuff; (void)ui32RxLength;
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************************/
/******************************** DMA Functions *******************************/
// Stub implementations for DMA (not supported)
uint32_t spi_io_read_dma(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_transfer_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    (void)me; (void)pui8TxBuff; (void)pui8RxBuff; (void)ui32Length;
    return ERROR_NOT_SUPPORTED;
}

uint32_t spi_io_write_and_read_dma(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32TxLength, uint8_t *pui8RxBuff, uint32_t ui32RxLength)
{
    (void)me; (void)pui8TxBuff; (void)ui32TxLength; (void)pui8RxBuff; (void)ui32RxLength;
    return ERROR_NOT_SUPPORTED;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */