/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "delay.h"

#include "MIMX9352_cm33.h"
#include "core_cm33.h"      // DWT cycle counter (timeout)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define CPU_HZ SystemCoreClock  // 200 MHz CM33 (adjust if you change clocks)


#define CYCLES_FROM_US(us) ((uint32_t)(((uint64_t)(us) * (uint64_t)CPU_HZ) / 1000000u))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static bool dwt_init = false;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void delay_init(void)
{
    if (dwt_init == true)
    {
        return;
    }
    
    // Enable trace (needed for DWT)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Some MCUs lock DWT; unlock if LAR present
    #ifdef DWT_LAR
    DWT->LAR = 0xC5ACCE55;
    #endif
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

    dwt_init = true;
}

uint32_t delay_cycles_now(void)
{
    return DWT->CYCCNT;
}

// wrap-safe: returns true if 'now' is before 'deadline'
bool delay_time_before(uint32_t now, uint32_t deadline)
{
    return (int32_t)(now - deadline) < 0;
}

void delay_us(uint32_t time_us)
{
    const uint32_t deadline = delay_cycles_now() + CYCLES_FROM_US(time_us);

    while (delay_time_before(delay_cycles_now(), deadline))
    {
        __NOP();
    }
}

// Wait until (reg & mask) != 0  OR timeout (us) expires. Returns true on timeout.
bool delay_wait_flag_set_timeout(volatile uint32_t *reg,
                                         uint32_t mask,
                                         uint32_t timeout_us)
{
    const uint32_t deadline = delay_cycles_now() + CYCLES_FROM_US(timeout_us);

    while (delay_time_before(delay_cycles_now(), deadline))
    {
        if ((*reg & mask) != 0u)
        {
            return false;
        }

        __NOP();
    }

    if ((*reg & mask) == 0u)
    {
        return true;
    }
    
    return false;
}

// Wait until (reg & mask) == 0  OR timeout (us) expires. Returns true on timeout.
bool delay_wait_flag_clr_timeout(volatile uint32_t *reg,
                                         uint32_t mask,
                                         uint32_t timeout_us)
{
    const uint32_t deadline = delay_cycles_now() + CYCLES_FROM_US(timeout_us);

    while (delay_time_before(delay_cycles_now(), deadline))
    {
        if ((*reg & mask) == 0u)
        {
            return false;
        }

        __NOP();
    }

    if ((*reg & mask) != 0u)
    {
        return true;
    }

    return false;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */