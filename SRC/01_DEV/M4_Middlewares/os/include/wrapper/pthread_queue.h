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

/*
Inspired by c-pthread-queue by Matthew Dickinson
http://code.google.com/p/c-pthread-queue/
*/

#ifndef _PTHREAD_QUEUE_H_
#define _PTHREAD_QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>

typedef struct os_thread_queue_s {
	void * buffer;
	int size;
	int item_size;
	int items;
	int in;
	int out;
	pthread_mutex_t mutex;
	pthread_cond_t cond_full;
	pthread_cond_t cond_empty;
} os_pthread_queue_t;

#define PTHREAD_QUEUE_ERROR 0
#define PTHREAD_QUEUE_EMPTY 0
#define PTHREAD_QUEUE_FULL 0
#define PTHREAD_QUEUE_OK 1

os_pthread_queue_t * os_pthread_queue_create(int length, size_t item_size);
int os_pthread_queue_send(os_pthread_queue_t *queue, void *value, uint32_t timeout);
int os_pthread_queue_receive(os_pthread_queue_t *queue, void *buf, uint32_t timeout);

#endif 

