/************************************************
 *  @file     : m33_rpmsg.h
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#ifndef m33_rpmsg_H
#define m33_rpmsg_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================*/
/*                        Includes                            */
/*============================================================*/
#include <stdint.h>
#include <stddef.h>

#include "Define/define.h"
/*============================================================*/
/*                         Defines                            */
/*============================================================*/
#ifndef M33_RPMSG_DEFAULT_TIMEOUT_MS
#define M33_RPMSG_DEFAULT_TIMEOUT_MS  (0) /* 0 means block (library maps to RL_BLOCK) */
#endif

#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30U)
#endif
/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/

/*============================================================*/
/*                Function Prototypes                         */
/*============================================================*/
Std_ReturnType M33_RPMSG_Init(void);
void           M33_RPMSG_WaitForLinkUp(void);
Std_ReturnType M33_RPMSG_CreateEndpoint(uint32_t local_ept_addr);
Std_ReturnType M33_RPMSG_AnnounceNameservice(const char *name);
Std_ReturnType M33_RPMSG_SendCopy(uint32_t remote_addr, const void *buf, uint32_t len);
Std_ReturnType M33_RPMSG_SendNoCopy(uint32_t remote_addr, void *tx_buf, uint32_t len); /* tx_buf must be allocated by rpmsg_lite_alloc_tx_buffer */
Std_ReturnType M33_RPMSG_AllocTxBuffer(void **tx_buf, uint32_t *size); /* get tx buffer (nocopy) */
Std_ReturnType M33_RPMSG_ReceiveNoCopy(uint32_t *remote_addr, void **rx_buf, uint32_t *len); /* blocking */
Std_ReturnType M33_RPMSG_ReleaseRxBuffer(void *rx_buf);
Std_ReturnType M33_RPMSG_DestroyEndpoint(void);
void           M33_RPMSG_Deinit(void);

/* Helper: blocking receive into user buffer (copies data). Returns length in out_len (or negative on error) */
int32_t M33_RPMSG_ReceiveCopy(void *user_buf, uint32_t max_len, uint32_t timeout_ms, uint32_t *out_len);


#ifdef __cplusplus
}
#endif

#endif /* m33_rpmsg_H */