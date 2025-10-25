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

#include "osQueue.h"

osQueue osQueueCreate(int length, size_t item_size) {
	return xQueueCreate(length, item_size);
}

int osQueueSend(osQueue queue, void * value, uint32_t timeout) {
	return xQueueSend(queue, value, timeout);
}

int osQueueReceive(osQueue queue, void * buf, uint32_t timeout){
    return xQueueReceive(queue, buf, timeout);
}
