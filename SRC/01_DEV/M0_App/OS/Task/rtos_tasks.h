/************************************************
 *  @file     : rtos_task.h
 *  @author   : CAO HIEU
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/
#ifndef OS_TASK_RTOS_TASKS_H_
#define OS_TASK_RTOS_TASKS_H_
#include "Define/define.h"
#include "stdint.h"

void EXP_RootGrowUp(void);
uint32_t rpmsg_send(uint32_t msg_type, const char *msg);

#endif /* OS_TASK_RTOS_TASKS_H_ */
