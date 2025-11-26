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


#include "osSemphr.h"

int osSemaphoreCreateMutex(osSemaphore* mutex){
	*mutex = xSemaphoreCreateMutex();
	if (*mutex) {
		return OS_SEMAPHORE_OK;
	} else {
		return OS_SEMAPHORE_ERROR;
	}
}


int osSemaphoreCreateBinnary(osSemaphore* semaphore){
	*semaphore = xSemaphoreCreateBinary();
	if (*semaphore) {
		return OS_SEMAPHORE_OK;
	} else {
		return OS_SEMAPHORE_ERROR;
	}
}

int osSemaphoreTake(osSemaphore *mutex, uint32_t timeout){

	// if (xSemaphoreTake(*mutex, timeout) == pdPASS) {
	 	return OS_SEMAPHORE_OK;
	// } else {
	// 	return OS_SEMAPHORE_ERROR;
	// }
}

int osSemaphoreGiven(osSemaphore *mutex){
	// if (xSemaphoreGive(*mutex) == pdPASS) {
	 	return OS_SEMAPHORE_OK;
	// } else {
	// 	return OS_SEMAPHORE_ERROR;
	// }
}
