/************************************************
 *  @file     : rtos.c
 *  @author   : CAO HIEU
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/
#include "rtos.h"
#include "OS/Task/rtos_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
/*--------------------Start RTOS--------------*/
void EXP_RTOS_Start(void)
{
	EXP_RootGrowUp();
}

 /*--------------------RTOS Task List--------------*/

/* Hook prototypes */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{

}

void vApplicationMallocFailedHook(void)
{

}

/* Idle Hook */
void vApplicationIdleHook(void)
{

}

/* Tick Hook */
void vApplicationTickHook(void)
{

}

static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
