/*
 * osMalloc.h
 *
 *  Created on: Oct 4, 2025
 *      Author: Admin
 */

#ifndef _SUCHAI_OSMALLOC_H_
#define _SUCHAI_OSMALLOC_H_
/* FreeRTOS includes */
#include <FreeRTOS.h>

void * os_malloc(size_t size);
void os_free(void *ptr) ;

#endif /* MIDDLEWARE_SUCHAI_OS_INCLUDE_SUCHAI_OSMALLOC_H_ */
