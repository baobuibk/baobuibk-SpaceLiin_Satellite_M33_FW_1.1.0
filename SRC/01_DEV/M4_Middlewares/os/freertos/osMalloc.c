/*
 * osMalloc.c
 *
 *  Created on: Oct 4, 2025
 *      Author: Admin
 */


#include "osMalloc.h"

#include <stdint.h>



void * os_malloc(size_t size) {
	return pvPortMalloc(size);
}

void os_free(void *ptr) {
	vPortFree(ptr);
}
