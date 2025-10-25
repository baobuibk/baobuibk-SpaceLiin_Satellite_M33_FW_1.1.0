/*
 * Copyright 2025 SUCHAI Flight Software v2 project and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCH_CPU_H
#define SCH_CPU_H

#include <stdint.h>

/**
 * Get system/rtc time in unix timestamp format
 * @return Unix timestamp
 */
int64_t cpu_get_unixtime(void);

/**
 * Set system/rtc time in unix timestamp
 * @param time Unix timestamp
 * @return 1 if Ok, 0 if errors.
 */
int cpu_set_unixtime(int64_t time);

/**
 * Debug CPU functioning, usually by blinking a LED.
 * Arg. can be used to select different kind o debug, or select
 * different LEDs.
 * @param arg Debug type
 * @return 1 if Ok, 0 if errors.
 */
int cpu_debug(int arg);

/**
 * Reset CPU (internal/external) watchdog timer.
 * Use arg to determine specific operations
 * @param arg WDT type:
 *  0: Internal or CPU WTD
 *  1: External WDT
 *  2: Other
 * @return 1 if Ok, 0 if errors.
 */
int cpu_reset_wdt(int arg);

/**
 * Reset (reboot) CPU.
 * Use arg to determine specific operations
 * @param arg Reset type:
 *  0: Soft reset
 *  1: Hard reset (power down/up)
 */
void cpu_reboot(int arg);

#endif //SCH_CPU_H