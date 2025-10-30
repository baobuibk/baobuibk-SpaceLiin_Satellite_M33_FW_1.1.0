/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "spi_io.h"

#include "fsl_lpspi.h"
#include "MIMX9352_cm33.h"

#include "delay.h"
#include "fsl_debug_console.h"

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

    // LPSPI_Type *base = spi_periph[me->ui32SpiPort];
    LPSPI_Type *base = LPSPI1;

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

uint32_t spi_io_read_sync(SPI_Io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    LPSPI_Type *base = spi_periph[me->ui32SpiPort];;  // tùy bạn map từ me->ui32SpiPort

    if (!base || !pui8RxBuff || (ui32Length == 0u))
        return ERROR_INVALID_PARAM;

    delay_init();

    /* 1) Đảm bảo module không bận */
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
        return ERROR_TIMEOUT;

    /* 2) Chuẩn bị FIFO/flags sạch sẽ */
    /* Theo thói quen an toàn:
       - Tắt MEN để flush (một số bản RM yêu cầu MEN=0 trước khi set RTF/RRF)
       - Flush TX/RX FIFO
       - Clear các cờ kết thúc cũ (TCF) */
    uint32_t cr_saved = base->CR;
    base->CR &= ~LPSPI_CR_MEN_MASK;                 // MEN = 0
    base->CR |= (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK); // flush FIFO
    base->SR  = LPSPI_SR_TCF_MASK;                  // clear TCF
    base->CR  = cr_saved | LPSPI_CR_MEN_MASK;       // bật lại MEN

    /* 3) Prime 1 command vào Command FIFO:
          LPSPI chỉ clock-out khi có cả TCR (command) và TDR (data).
          Ghi lại chính TCR hiện tại để đẩy một command vào FIFO. */
    uint32_t tcr_val = base->TCR;
    /* Bảo đảm FRAMESZ >= 7 (8-bit). Nếu FRAMESZ đang =0 (1-bit) sẽ khó nhìn thấy sóng/data */
    if (((tcr_val & LPSPI_TCR_FRAMESZ_MASK) >> LPSPI_TCR_FRAMESZ_SHIFT) < 7u) {
        tcr_val = (tcr_val & ~LPSPI_TCR_FRAMESZ_MASK) | LPSPI_TCR_FRAMESZ(7u);
    }
    base->TCR = tcr_val; // đẩy command vào FIFO

    for (uint32_t i = 0; i < ui32Length; i++)
    {
        /* 4) Chờ TX FIFO có chỗ trống */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        /* 5) Ghi dummy để clock-in 1 byte từ slave */
        base->TDR = 0xFFU;   // dummy 0xFF an toàn, tùy thiết bị có thể dùng 0x00

        /* 6) Chờ RX có dữ liệu */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        /* 7) Đọc về 1 byte */
        pui8RxBuff[i] = (uint8_t)base->RDR;
    }

    /* 8) Kết thúc: đảm bảo khung đã xong và module không bận */
    if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TCF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    base->SR = LPSPI_SR_TCF_MASK; // clear TCF

    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    return ERROR_OK;
}

uint32_t spi_io_write_sync(SPI_Io_t *me, uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    LPSPI_Type *base = spi_periph[me->ui32SpiPort];

    if (!base || !pui8TxBuff || (ui32Length == 0u))
        return ERROR_INVALID_PARAM;

    delay_init();

    /* 1) Đảm bảo module không bận trước khi bắt đầu */
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
        return ERROR_TIMEOUT;

    /* 2) Chuẩn bị FIFO/flags sạch sẽ (MEN=0 -> flush -> clear TCF -> MEN=1) */
    uint32_t cr_saved = base->CR;
    base->CR &= ~LPSPI_CR_MEN_MASK;                           // MEN = 0
    base->CR |= (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);      // flush TX/RX FIFO
    base->SR  = LPSPI_SR_TCF_MASK;                            // clear TCF
    base->CR  = cr_saved | LPSPI_CR_MEN_MASK;                 // bật lại MEN

    /* 3) Prime 1 command vào Command FIFO: ghi lại TCR hiện tại
          & đảm bảo FRAMESZ >= 7 (tức 8-bit) */
    uint32_t tcr_val = base->TCR;
    if (((tcr_val & LPSPI_TCR_FRAMESZ_MASK) >> LPSPI_TCR_FRAMESZ_SHIFT) < 7u) {
        tcr_val = (tcr_val & ~LPSPI_TCR_FRAMESZ_MASK) | LPSPI_TCR_FRAMESZ(7u);
    }
    base->TCR = tcr_val; // đẩy command vào FIFO

    /* 4) Ghi từng byte: chờ TDF -> ghi TDR; chờ RDF -> đọc bỏ RDR để giải phóng RX FIFO */
    for (uint32_t i = 0; i < ui32Length; i++)
    {
        /* TX FIFO có chỗ trống? */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        /* Ghi dữ liệu ra TDR */
        base->TDR = (uint32_t)pui8TxBuff[i];

        /* Với mỗi byte phát đi sẽ có 1 byte “rác” clock-in về RX -> đọc bỏ để tránh đầy FIFO */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        (void)base->RDR;  // discard
    }

    /* 5) Kết thúc phiên: đợi khung hoàn tất (TCF=1), clear TCF; đợi MBF=0 */
    if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TCF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    base->SR = LPSPI_SR_TCF_MASK; // clear TCF

    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    return ERROR_OK;
}
uint32_t spi_io_transfer_sync(SPI_Io_t *me, uint8_t *pui8TxBuff, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    LPSPI_Type *base = spi_periph[me->ui32SpiPort];

    if (!base || !pui8TxBuff || !pui8RxBuff || (ui32Length == 0u))
        return ERROR_INVALID_PARAM;

    delay_init();

    /* 1) Đảm bảo module không bận trước khi bắt đầu */
    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, 1000u))
        return ERROR_TIMEOUT;

    /* 2) MEN=0 -> flush FIFO -> clear TCF -> MEN=1 */
    uint32_t cr_saved = base->CR;
    base->CR &= ~LPSPI_CR_MEN_MASK;                         // MEN = 0
    base->CR |= (LPSPI_CR_RTF_MASK | LPSPI_CR_RRF_MASK);    // flush TX/RX FIFO
    base->SR  = LPSPI_SR_TCF_MASK;                          // clear TCF
    base->CR  = cr_saved | LPSPI_CR_MEN_MASK;               // bật lại MEN

    /* 3) Prime 1 command vào Command FIFO & đảm bảo FRAMESZ >= 7 (8-bit) */
    uint32_t tcr_val = base->TCR;
    if (((tcr_val & LPSPI_TCR_FRAMESZ_MASK) >> LPSPI_TCR_FRAMESZ_SHIFT) < 7u) {
        tcr_val = (tcr_val & ~LPSPI_TCR_FRAMESZ_MASK) | LPSPI_TCR_FRAMESZ(7u);
    }
    base->TCR = tcr_val;   // đẩy command vào FIFO

    /* 4) Full-duplex: ghi từng byte và đọc lại byte tương ứng */
    for (uint32_t i = 0; i < ui32Length; i++)
    {
        /* TX FIFO có chỗ trống? */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        /* Ghi 1 byte ra TDR */
        base->TDR = (uint32_t)pui8TxBuff[i];

        /* Chờ RX có dữ liệu về */
        if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_RDF_MASK, LPSPI_BYTE_TIMEOUT_US))
            return ERROR_TIMEOUT;

        /* Đọc 1 byte vào buffer */
        pui8RxBuff[i] = (uint8_t)base->RDR;
    }

    /* 5) Kết thúc phiên: đợi TCF=1 rồi clear; đảm bảo MBF=0 */
    if (delay_wait_flag_set_timeout(&base->SR, LPSPI_SR_TCF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    base->SR = LPSPI_SR_TCF_MASK; // clear TCF

    if (delay_wait_flag_clr_timeout(&base->SR, LPSPI_SR_MBF_MASK, LPSPI_BYTE_TIMEOUT_US))
        return ERROR_TIMEOUT;

    return ERROR_OK;
}
