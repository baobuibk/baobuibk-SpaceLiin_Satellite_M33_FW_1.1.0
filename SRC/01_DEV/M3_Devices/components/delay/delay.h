#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_

#include "stdbool.h"
#include "stdint.h"

// void spi_enable_clock(uint32_t ui32SpiNum);
void delay_init(void);

uint32_t delay_cycles_now(void);

void delay_us(uint32_t time_us);

// wrap-safe: returns true if 'now' is before 'deadline'
bool delay_time_before(uint32_t now, uint32_t deadline);

// Wait until (reg & mask) != 0  OR timeout (us) expires. Returns true on timeout.
bool delay_wait_flag_set_timeout(volatile uint32_t *reg,
                                         uint32_t mask,
                                         uint32_t timeout_us);

// Wait until (reg & mask) == 0  OR timeout (us) expires. Returns true on timeout.
bool delay_wait_flag_clr_timeout(volatile uint32_t *reg,
                                         uint32_t mask,
                                         uint32_t timeout_us);

#endif /* DRIVERS_COMPONENTS_I2C_I2C_H_ */