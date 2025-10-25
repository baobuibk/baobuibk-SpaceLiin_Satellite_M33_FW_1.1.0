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

#include "osDelay.h"

void osDelay(uint32_t mseconds){
    portTick ticks = mseconds/portTICK_RATE_MS;
    vTaskDelay(ticks);
}

portTick osDefineTime(uint32_t mseconds){
    return mseconds/portTICK_RATE_MS;
}

portTick osTaskGetTickCount(void){
	return xTaskGetTickCount();
}

void osTaskDelayUntil(portTick *lastTime, uint32_t mseconds){
    portTick ticks = osDefineTime(mseconds);
	vTaskDelayUntil(lastTime, ticks);
}

int osSetTimeUnix(int64_t time)
{
    return cpu_set_unixtime(time);
}

int64_t osGetTimeUnix(void)
{
    return cpu_get_unixtime();
}
