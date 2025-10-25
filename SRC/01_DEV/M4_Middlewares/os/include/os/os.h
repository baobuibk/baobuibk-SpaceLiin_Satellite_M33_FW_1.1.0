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

#ifndef _OS_H
#define _OS_H

#include "config.h"

#ifdef SCH_OS_FREERTOS
    #include "FreeRTOSConfig.h"
    #include "FreeRTOS.h"
    #include "task.h"
 	#include "queue.h"
 	#include "semphr.h"
    #include "timers.h"
    #include "portable.h"
#else
    #define portMAX_DELAY (uint32_t) 0xffffffff
    #define pdPASS				     1
    #define configMINIMAL_STACK_SIZE 1
    #define portBASE_TYPE	short
    #define ClrWdt(){};
#endif


#endif //_OS_H
