/************************************************
 *  @file     : rtos.h
 *  @author   : CAO HIEU
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/
#ifndef OS_KERNEL_RTOS_H_
#define OS_KERNEL_RTOS_H_

#include "stdint.h"
enum {
    RPMSG_MSG_UPDATE_PARAM = 0,
    RPMSG_MSG_CAPTURE = 1,
    RPMSG_MSG_RESPONSE_OK = 2}; 

#define RPMSG_WAIT     1000
void EXP_RTOS_Start(void);

 uint32_t rpmsg_send(uint32_t msg_type, const char *msg); // messsage service

#endif /* OS_KERNEL_RTOS_H_ */
