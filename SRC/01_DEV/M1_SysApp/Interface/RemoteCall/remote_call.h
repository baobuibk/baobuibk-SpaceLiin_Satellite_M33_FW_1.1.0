/************************************************
 *  @file     : remote_call.h
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#ifndef remote_call_H
#define remote_call_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================*/
/*                        Includes                            */
/*============================================================*/
#include <stdint.h>
#include "Define/define.h"

/*============================================================*/
/*                         Defines                            */
/*============================================================*/
#define CMD_TYPE_NORMAL_REQ     0x23
#define CMD_TYPE_FILE_REQ       0x24
#define CMD_TYPE_NORMAL_RESP    0x2D
#define CMD_TYPE_FILE_RESP      0x3D

#define REMOTE_CALL_TARGET      0x55
#define REMOTE_CALL_MY_ADDR     0x33
/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/
struct cmd_header {
    uint8_t target;
    uint8_t type;
    uint8_t reserved;
    uint16_t length;
} __attribute__((packed));

struct file_transfer_msg {
    uint8_t target;
    uint8_t type;
    uint8_t flags;
    uint16_t reserved;
    uint32_t offset;
    uint32_t size;
    char filename[240];
} __attribute__((packed));

/*============================================================*/
/*                Function Prototypes                         */
/*============================================================*/
void RemoteCall_UpdateRemoteAddr(uint32_t remote_addr);

Std_ReturnType RemoteCall_SendCommand(const char *cmd);
Std_ReturnType RemoteCall_SendResponse(const char *resp);
Std_ReturnType RemoteCall_SendFileRequest(uint32_t size, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* remote_call_H */