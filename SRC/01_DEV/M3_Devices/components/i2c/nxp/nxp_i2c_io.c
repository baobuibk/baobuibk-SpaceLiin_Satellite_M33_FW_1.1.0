/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stdbool.h"

#include "i2c_io.h"

#include "fsl_lpi2c.h"
#include "MIMX9352_cm33.h"

#include "fsl_device_registers.h" // brings LPI2C_Type and register bitfields for i.MX93

#include "delay.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define I2C_MAX_BUS_NUMBER 8

/*===================== LPI2C command helpers =====================*/
/* MTDR command field (CMD = bits [10:8], DATA = [7:0]) */
#ifndef LPI2C_MTDR_CMD
#define LPI2C_MTDR_CMD(x)  (((uint32_t)(x) & 0x7u) << 8)
#endif
#ifndef LPI2C_MTDR_DATA
#define LPI2C_MTDR_DATA(x) ((uint32_t)(x) & 0xFFu)
#endif

/* Command encodings (from LPI2C refman):
 * 0x0 = Transmit Data (DATA = byte)
 * 0x1 = Receive N bytes (DATA = N; HW will clock in N bytes into MRDR)
 * 0x2 = Generate STOP
 * 0x4 = Generate START + transmit address in DATA (7-bit addr in bits [7:1], R/W in bit0)
 */
#define CMD_TX_DATA   0x0u
#define CMD_RX_COUNT  0x1u
#define CMD_STOP      0x2u
#define CMD_START     0x4u

/* MSR bits (names as in device headers) */
#ifndef LPI2C_MSR_TDF_MASK
/* Fallbacks if your device header uses slightly different names; adjust if needed */
#define LPI2C_MSR_TDF_MASK   (1u<<0)  /* Transmit Data Flag: MTDR can accept a word */
#define LPI2C_MSR_RDF_MASK   (1u<<1)  /* Receive Data Flag: MRDR has a byte */
#define LPI2C_MSR_NDF_MASK   (1u<<2)  /* NACK Detect Flag */
#define LPI2C_MSR_SDF_MASK   (1u<<9)  /* STOP Detect Flag */
#endif

/* MFCR bits to reset FIFOs */
#ifndef LPI2C_MFCR_RTF_MASK
#define LPI2C_MFCR_RTF_MASK  (1u<<0)  /* Reset Transmit FIFO */
#define LPI2C_MFCR_RRF_MASK  (1u<<1)  /* Reset Receive FIFO  */
#endif

/* MCR MEN bit (module enable) */
#ifndef LPI2C_MCR_MEN_MASK
#define LPI2C_MCR_MEN_MASK (1u<<0)
#endif

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static LPI2C_Type* const i2c_periph[I2C_MAX_BUS_NUMBER + 1] =
{
    NULL,
    #if defined(LPI2C1)
        LPI2C1,
    #else
        NULL,
    #endif
    #if defined(LPI2C2)
        LPI2C2,
    #else
        NULL,
    #endif
    #if defined(LPI2C3)
        LPI2C3,
    #else
        NULL,
    #endif
    #if defined(LPI2C4)
        LPI2C4,
    #else
        NULL,
    #endif
    #if defined(LPI2C5)
        LPI2C5,
    #else
        NULL,
    #endif
    #if defined(LPI2C6)
        LPI2C6,
    #else
        NULL,
    #endif
    #if defined(LPI2C7)
        LPI2C7,
    #else
        NULL,
    #endif
    #if defined(LPI2C8)
        LPI2C8,
    #else
        NULL,
    #endif
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t i2c_io_send(struct i2c_io_t *me, uint8_t ui8SlaveAddr, const char *buf, int count)
{
    if (!me || !buf || count <= 0)
    {
        return 0;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    const uint32_t port = me->ui32I2cPort;

    if (port == 0 || port > I2C_MAX_BUS_NUMBER)
    {
        return 0;
    }

    LPI2C_Type* base = i2c_periph[port];
    if (!base)
    {
        return 0;
    }

    delay_init();

    /* Ensure module enabled (BSP should do this; keep it lean) */
    if ((base->MCR & LPI2C_MCR_MEN_MASK) == 0u)
    {
        return 0;
    }

    /* Clean FIFOs and sticky status (optional but recommended) */
    base->MFCR |= (LPI2C_MFCR_RTF_MASK | LPI2C_MFCR_RRF_MASK);

    // right after FIFO reset
    base->MSR = (LPI2C_MSR_NDF_MASK | LPI2C_MSR_SDF_MASK); // W1C if supported by your header 
    
    /* Clear flags by writing 1s to them if your header supports W1C on MSR.
       If not, the act of reading MSR/MRDR/MTDR progresses state anyway. */

    /* 1) START + address (write) */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
    {
        return 0;
    }

    base->MTDR = LPI2C_MTDR_CMD(CMD_START) | LPI2C_MTDR_DATA(((uint32_t)ui8SlaveAddr << 1) | 0u);

    /* Optionally bail early if NACK detected on address */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_NDF_MASK, 50u) == 0)
    {
        /* got NACK early */
        return 0;
    }

    /* no NACK flag → OK; do nothing */

    /* 2) Send payload bytes */
    for (int i = 0; i < count; i++)
    {
        if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
        {
            return 0;
        }

        base->MTDR = LPI2C_MTDR_CMD(CMD_TX_DATA) | LPI2C_MTDR_DATA((uint8_t)buf[i]);

        /* NACK check after each byte (optional but safer) */
        if (base->MSR & LPI2C_MSR_NDF_MASK)
        {
            return 0;
        }
    }

    /* 3) STOP */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
    {
        return 0;
    }

    base->MTDR = LPI2C_MTDR_CMD(CMD_STOP);

    /* Wait for STOP detected (optional fence) */
    (void)delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_SDF_MASK, 1000u);

    osSemaphoreGiven(&me->lock);

    return (uint32_t)count;
}

uint32_t i2c_io_recv(struct i2c_io_t *me, uint8_t ui8SlaveAddr, char *buf, int count)
{
    if (!me || !buf || count <= 0)
    {
        return 0;
    }

    int sem_ret = osSemaphoreTake(&me->lock, 1000);

    if (sem_ret != pdPASS)
    {
        return (uint32_t)sem_ret;
    }

    const uint32_t port = me->ui32I2cPort;
    if (port == 0 || port > I2C_MAX_BUS_NUMBER)
    {
        return 0;
    }

    LPI2C_Type* base = i2c_periph[port];
    if (!base)
    {
        return 0;
    }

    delay_init();

    if ((base->MCR & LPI2C_MCR_MEN_MASK) == 0u)
    {
        return 0;
    }

    /* Clean FIFOs */
    base->MFCR |= (LPI2C_MFCR_RTF_MASK | LPI2C_MFCR_RRF_MASK);

    /* 1) START + address (read) */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
    {
        return 0;
    }
    
    base->MTDR = LPI2C_MTDR_CMD(CMD_START) | LPI2C_MTDR_DATA(((uint32_t)ui8SlaveAddr << 1) | 1u);

    /* NACK on address? */
    if (base->MSR & LPI2C_MSR_NDF_MASK)
    {
        return 0;
    }

    /* 2) Tell controller to receive <count> bytes */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
    {
        return 0;
    }

    // base->MTDR = LPI2C_MTDR_CMD(CMD_RX_COUNT) | LPI2C_MTDR_DATA((uint32_t)count);

    int remaining = count;
    while (remaining > 0)
    {
        if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
        {
            return 0;
        }

        if (remaining > 256)
        {
            base->MTDR = LPI2C_MTDR_CMD(CMD_RX_COUNT) | LPI2C_MTDR_DATA(0xFFu); // 256 bytes
            remaining -= 256;
        }
        else
        {
            base->MTDR = LPI2C_MTDR_CMD(CMD_RX_COUNT) | LPI2C_MTDR_DATA((uint32_t)(remaining - 1));
            remaining = 0;
        }
    }

    /* 3) Queue STOP now (controller will issue it after RX completes) */
    if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_TDF_MASK, 1000u))
    {
        return 0;
    }

    base->MTDR = LPI2C_MTDR_CMD(CMD_STOP);

    /* 4) Read out bytes as they arrive */
    for (int i = 0; i < count; i++)
    {
        if (delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_RDF_MASK, 1000u))
        {
            return 0;
        }

        buf[i] = (char)(base->MRDR & 0xFFu);
    }

    /* Optional: wait for STOP detect */
    (void)delay_wait_flag_set_timeout(&base->MSR, LPI2C_MSR_SDF_MASK, 1000u);

    osSemaphoreGiven(&me->lock);

    return (uint32_t)count;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */