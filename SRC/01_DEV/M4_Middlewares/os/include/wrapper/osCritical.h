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

#ifndef SUCHAI_OS_CRITICAL_H
#define SUCHAI_OS_CRITICAL_H

#include <FreeRTOS.h>
#include <task.h>

/**
 * Enters a critical section, disabling interrupts to protect shared resources.
 * This macro disables task switching and interrupts.
 */
#define os_enterCritical() \
    do { \
        taskENTER_CRITICAL(); \
    } while(0)

/**
 * Exits a critical section, re-enabling interrupts previously disabled.
 * This macro must be called after os_enterCritical() to restore normal operation.
 */
#define os_exitCritical() \
    do { \
        taskEXIT_CRITICAL(); \
    } while(0)

/**
 * Enters a critical section from an ISR context.
 * This version is safe to call from interrupt service routines.
 * Note: Requires passing pxHigherPriorityTaskWoken for yield handling.
 */
#define os_enterCriticalFromISR(pxHigherPriorityTaskWoken) \
    taskENTER_CRITICAL_FROM_ISR(pxHigherPriorityTaskWoken)

/**
 * Exits a critical section from an ISR context.
 * This must pair with os_enterCriticalFromISR().
 * Note: Requires passing pxHigherPriorityTaskWoken for yield handling.
 */
#define os_exitCriticalFromISR(pxHigherPriorityTaskWoken) \
    taskEXIT_CRITICAL_FROM_ISR(pxHigherPriorityTaskWoken)

#endif /* SUCHAI_OS_CRITICAL_H */
