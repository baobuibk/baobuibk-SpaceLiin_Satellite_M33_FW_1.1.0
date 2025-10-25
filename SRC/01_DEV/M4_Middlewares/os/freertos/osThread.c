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

#include "osThread.h"

/**
 * create a task in FreeRTOS
 */
int osCreateTask(void (*functionTask)(void *), char* name, unsigned short size, void * parameters, unsigned int priority, os_thread *thread)
{
#ifdef AVR32
    // FreeRTOS 7.0.0
    portBASE_TYPE created = xTaskCreate((*functionTask), (signed char*)name, size, parameters, priority, NULL);
#else
    // FreeRTOS > 8.0.0
    BaseType_t created = xTaskCreate((*functionTask), name, size, parameters, priority, NULL);
#endif
    return created == pdPASS ? 0 : 1;
}

void osTaskDelete(void *task_handle)
{
    vTaskDelete(task_handle);
}
