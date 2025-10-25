/************************************************
 *  @file     : remote_call.c
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "remote_call.h"

#include "rpmsg/m33_rpmsg.h"
#include "fsl_debug_console.h"
#include <string.h>

/*============================================================*/
/*                      Defines                               */
/*============================================================*/

/*============================================================*/
/*                   Private Variables                        */
/*============================================================*/
static volatile uint32_t g_remote_addr = 0;

/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/

/*============================================================*/
/*              Private Function Prototypes                   */
/*============================================================*/

/*============================================================*/
/*                Function Implementations                    */
/*============================================================*/
void RemoteCall_UpdateRemoteAddr(uint32_t remote_addr)
{
    g_remote_addr = remote_addr;
}

Std_ReturnType RemoteCall_SendResponse(const char *resp)
{
    void *tx_buf;
    uint32_t tx_size;
    uint32_t resp_len = strlen(resp);

    if (g_remote_addr == 0)
    {
        PRINTF("[remote_call] No remote endpoint\r\n");
        return E_ERROR;
    }

    if (M33_RPMSG_AllocTxBuffer(&tx_buf, &tx_size) != E_OK)
    {
        return E_ERROR;
    }

    if (tx_size < 5 + resp_len) // header size = 5 bytes
    {
        PRINTF("[remote_call] Response too long\r\n");
        return E_ERROR;
    }

    uint8_t *payload = (uint8_t *)tx_buf;
    payload[0] = REMOTE_CALL_TARGET;               // target
    payload[1] = CMD_TYPE_NORMAL_RESP;             // type
    payload[2] = 0;                                // reserved
    payload[3] = (uint8_t)(resp_len & 0xFF);       // length LSB
    payload[4] = (uint8_t)((resp_len >> 8) & 0xFF);// length MSB

    memcpy(&payload[5], resp, resp_len);

    if (M33_RPMSG_SendNoCopy(g_remote_addr, tx_buf, 5 + resp_len) != E_OK)
    {
        PRINTF("[remote_call] Send failed\r\n");
        return E_ERROR;
    }

    return E_OK;
}

Std_ReturnType RemoteCall_SendCommand(const char *cmd)
{
    void *tx_buf;
    uint32_t tx_size;
    uint32_t cmd_len = strlen(cmd);

    if (g_remote_addr == 0)
    {
        PRINTF("[remote_call] No remote endpoint\r\n");
        return E_ERROR;
    }

    if (M33_RPMSG_AllocTxBuffer(&tx_buf, &tx_size) != E_OK)
    {
        return E_ERROR;
    }

    if (tx_size < 5 + cmd_len)
    {
        PRINTF("[remote_call] Command too long\r\n");
        return E_ERROR;
    }

    uint8_t *payload = (uint8_t *)tx_buf;
    payload[0] = REMOTE_CALL_TARGET;               // target
    payload[1] = CMD_TYPE_NORMAL_REQ;              // type
    payload[2] = 0;                                // reserved
    payload[3] = (uint8_t)(cmd_len & 0xFF);
    payload[4] = (uint8_t)((cmd_len >> 8) & 0xFF);

    memcpy(&payload[5], cmd, cmd_len);

    if (M33_RPMSG_SendNoCopy(g_remote_addr, tx_buf, 5 + cmd_len) != E_OK)
    {
        PRINTF("[remote_call] Send failed\r\n");
        return E_ERROR;
    }

    return E_OK;
}

Std_ReturnType RemoteCall_SendFileRequest(uint32_t size, const char *filename)
{
    void *tx_buf;
    uint32_t tx_size;

    if (g_remote_addr == 0)
    {
        PRINTF("[remote_call] No remote endpoint\r\n");
        return E_ERROR;
    }

    if (size == 0 || size > 0x8000000)
    {
        PRINTF("[remote_call] Invalid size\r\n");
        return E_ERROR;
    }

    if (M33_RPMSG_AllocTxBuffer(&tx_buf, &tx_size) != E_OK)
    {
        PRINTF("[remote_call] Failed to allocate TX buffer\r\n");
        return E_ERROR;
    }

    uint8_t *payload = (uint8_t *)tx_buf;
    memset(payload, 0, tx_size);

    // [0] target
    // [1] type
    // [2] flags
    // [3-4] reserved
    // [5-8] offset (4 bytes)
    // [9-12] size (4 bytes)
    // [13...] filename[240]

    if (tx_size < 13 + 240)
    {
        PRINTF("[remote_call] TX buffer too small\r\n");
        return E_ERROR;
    }

    payload[0] = REMOTE_CALL_TARGET;
    payload[1] = CMD_TYPE_FILE_REQ;
    payload[2] = 0;  // flags
    payload[3] = 0;  // reserved LSB
    payload[4] = 0;  // reserved MSB

    // offset = 0
    payload[5] = 0;
    payload[6] = 0;
    payload[7] = 0;
    payload[8] = 0;

    // size (little endian)
    payload[9]  = (uint8_t)(size & 0xFF);
    payload[10] = (uint8_t)((size >> 8) & 0xFF);
    payload[11] = (uint8_t)((size >> 16) & 0xFF);
    payload[12] = (uint8_t)((size >> 24) & 0xFF);

    strncpy((char *)&payload[13], filename, 239);
    payload[13 + 239] = '\0';

    if (M33_RPMSG_SendNoCopy(g_remote_addr, tx_buf, 13 + 240) != E_OK)
    {
        PRINTF("[remote_call] Failed to send file request\r\n");
        return E_ERROR;
    }

    return E_OK;
}
