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

#ifndef _OS_THREAD_H_
#define _OS_THREAD_H_

#include "config.h"


    #include "FreeRTOSConfig.h"
    #include "FreeRTOS.h"
    #include "task.h"
    typedef portBASE_TYPE os_thread;


/**
 * Create a new task.
 * In GNU/Linux a new thread is created and started inmediately. In FreeRTOS
 * a new Task is created but will start after a call to osScheduler().
 *
 * @param functionTask Pointer to the target function task
 * @param name Task name. In GNU/Linux max 16 chars.
 * @param size Task stack size.
 * For GNU/Linux @see http://man7.org/linux/man-pages/man3/pthread_attr_setstacksize.3.html.
 * For FreeRTOS @see https://www.freertos.org/a00125.htm)
 * @param parameters Pointer to stack parameters
 * @param priority Task priority.
 * For GNU/Linux @see http://man7.org/linux/man-pages/man3/pthread_getschedparam.3.html
 * For FreeRTOS @see https://www.freertos.org/a00125.html
 * @param thread Pointer to store the task handler
 *
 * @return Returns 0 on success, error code if the task can not be created.
 */
int osCreateTask(void (*functionTask)(void *), char* name, unsigned short size, void * parameters, unsigned int priority, os_thread* thread);

/**
 * Delete a task. Only in FreeRTOS, not implemented for GNU/Linux
 * @param task_handle Pinter to a task handler
 */
void osTaskDelete(void *task_handle);

#endif // _OS_THREAD_H_
