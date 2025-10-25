/************************************************
 *  @file     : m33_rpmsg.c
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "m33_rpmsg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "remoteproc/rsc_table.h"

#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"

/*============================================================*/
/*                      Defines                               */
/*============================================================*/
#ifndef RPMSG_LITE_LINK_ID
#define RPMSG_LITE_LINK_ID            (RL_PLATFORM_IMX93_M33_A55_USER_LINK_ID)
#endif
#ifndef RPMSG_LITE_SHMEM_BASE
#define RPMSG_LITE_SHMEM_BASE         (VDEV1_VRING_BASE)
#endif
#ifndef RPMSG_LITE_NS_ANNOUNCE_STRING
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-virtual-tty-channel"
#endif

#define RPMSG_LITE_MASTER_IS_LINUX

/*============================================================*/
/*                   Private Variables                        */
/*============================================================*/
static struct rpmsg_lite_instance *volatile m33_rpmsg = NULL;
static struct rpmsg_lite_endpoint *volatile m33_ept = NULL;
static volatile rpmsg_queue_handle m33_queue = NULL;
static uint32_t m33_local_ept_addr = LOCAL_EPT_ADDR;

static const char *m33_ns_name = RPMSG_LITE_NS_ANNOUNCE_STRING;
/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/

/*============================================================*/
/*              Private Function Prototypes                   */
/*============================================================*/
// static int32_t rpmsg_queue_rx_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
// {
//     /* callback invoked by rpmsg-lite when endpoint receives message.
//        We simply push to the queue to be received by rpmsg_queue_recv_nocopy. */
//     rpmsg_queue_callback(m33_rpmsg, (rpmsg_queue_handle)priv);
//     /* Note: the actual push to queue is handled internally by rpmsg-lite/queue when using rpmsg_queue_rx_cb. */
//     (void)payload;
//     (void)payload_len;
//     (void)src;
// }

/*============================================================*/
/*                Function Implementations                    */
/*============================================================*/
Std_ReturnType M33_RPMSG_Init(void)
{
    if (m33_rpmsg)
        return E_OK; 

    m33_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    if (m33_rpmsg == NULL)
    {
        return E_ERROR;
    }
    return E_OK;
}

void M33_RPMSG_WaitForLinkUp(void)
{
    if (m33_rpmsg == NULL)
        return;
    rpmsg_lite_wait_for_link_up(m33_rpmsg, RL_BLOCK);
}

Std_ReturnType M33_RPMSG_CreateEndpoint(uint32_t local_ept_addr)
{
    if (m33_rpmsg == NULL)
        return E_ERROR;

    if (m33_ept)
        return E_OK; /* already created */

    m33_queue = rpmsg_queue_create(m33_rpmsg);
    if (m33_queue == NULL)
        return E_ERROR;

    /* create endpoint with queue callback (the callback enqueues message into m33_queue) */
    m33_local_ept_addr = local_ept_addr ? local_ept_addr : LOCAL_EPT_ADDR;
    m33_ept = rpmsg_lite_create_ept(m33_rpmsg, m33_local_ept_addr, rpmsg_queue_rx_cb, m33_queue);
    if (m33_ept == NULL)
    {
        rpmsg_queue_destroy(m33_rpmsg, m33_queue);
        m33_queue = NULL;
        return E_ERROR;
    }

    (void)rpmsg_ns_announce(m33_rpmsg, m33_ept, m33_ns_name, RL_NS_CREATE);

    return E_OK;
}

Std_ReturnType M33_RPMSG_AnnounceNameservice(const char *name)
{
    if (name == NULL)
        return E_ERROR;
    m33_ns_name = name;
    if (m33_rpmsg && m33_ept)
    {
        (void)rpmsg_ns_announce(m33_rpmsg, m33_ept, m33_ns_name, RL_NS_CREATE);
    }
    return E_OK;
}

Std_ReturnType M33_RPMSG_AllocTxBuffer(void **tx_buf, uint32_t *size)
{
    if (m33_rpmsg == NULL || tx_buf == NULL || size == NULL)
        return E_ERROR;

    *tx_buf = rpmsg_lite_alloc_tx_buffer(m33_rpmsg, size, RL_BLOCK);
    if (*tx_buf == NULL)
        return E_ERROR;
    return E_OK;
}

Std_ReturnType M33_RPMSG_SendNoCopy(uint32_t remote_addr, void *tx_buf, uint32_t len)
{
    if (m33_rpmsg == NULL || m33_ept == NULL || tx_buf == NULL)
        return E_ERROR;

    int32_t res = rpmsg_lite_send_nocopy(m33_rpmsg, m33_ept, remote_addr, tx_buf, len);
    return (res == 0) ? E_OK : E_ERROR;
}

Std_ReturnType M33_RPMSG_SendCopy(uint32_t remote_addr, const void *buf, uint32_t len)
{
    if (m33_rpmsg == NULL || m33_ept == NULL || buf == NULL || len == 0)
        return E_ERROR;

    /* allocate tx buffer, copy, send */
    void *tx_buf = NULL;
    uint32_t tx_size = 0;
    tx_buf = rpmsg_lite_alloc_tx_buffer(m33_rpmsg, &tx_size, RL_BLOCK);
    if (tx_buf == NULL)
        return E_ERROR;
    if (tx_size < len)
    {
        /* rpmsg-lite doesn't provide a direct free for tx buffer allocated (send_nocopy will take it),
           but since we got it and can't use it, we'll attempt to send 0 or just return error.
           Safer behaviour: copy up to tx_size. */
        memcpy(tx_buf, buf, tx_size);
        int32_t res = rpmsg_lite_send_nocopy(m33_rpmsg, m33_ept, remote_addr, tx_buf, tx_size);
        return (res == 0) ? E_OK : E_ERROR;
    }
    memcpy(tx_buf, buf, len);
    int32_t res = rpmsg_lite_send_nocopy(m33_rpmsg, m33_ept, remote_addr, tx_buf, len);
    return (res == 0) ? E_OK : E_ERROR;
}

Std_ReturnType M33_RPMSG_ReceiveNoCopy(uint32_t *remote_addr, void **rx_buf, uint32_t *len)
{
    if (m33_rpmsg == NULL || m33_queue == NULL || rx_buf == NULL || len == NULL)
        return E_ERROR;

    /* Blocking receive (maps to RL_BLOCK) */
    int32_t result = rpmsg_queue_recv_nocopy(m33_rpmsg, m33_queue, remote_addr, (char **)rx_buf, len, RL_BLOCK);
    return (result == 0) ? E_OK : E_ERROR;
}

Std_ReturnType M33_RPMSG_ReleaseRxBuffer(void *rx_buf)
{
    if (m33_rpmsg == NULL || rx_buf == NULL)
        return E_ERROR;

    int32_t result = rpmsg_queue_nocopy_free(m33_rpmsg, rx_buf);
    return (result == 0) ? E_OK : E_ERROR;
}

int32_t M33_RPMSG_ReceiveCopy(void *user_buf, uint32_t max_len, uint32_t timeout_ms, uint32_t *out_len)
{
    /* NOTE: current implementation ignores timeout_ms and always blocks (RL_BLOCK).
       If needed, map timeout_ms to rpmsg-lite non-blocking or timed wait (not available in base API).
    */
    if (user_buf == NULL || max_len == 0)
        return -1;

    uint32_t remote;
    void *rx_buf = NULL;
    uint32_t len = 0;
    int rv = M33_RPMSG_ReceiveNoCopy(&remote, &rx_buf, &len);
    if (rv != E_OK)
        return -1;

    uint32_t copy_len = (len < max_len) ? len : max_len;
    memcpy(user_buf, rx_buf, copy_len);
    (void)M33_RPMSG_ReleaseRxBuffer(rx_buf);
    if (out_len)
        *out_len = copy_len;
    return (int32_t)copy_len;
}

Std_ReturnType M33_RPMSG_DestroyEndpoint(void)
{
    if (m33_ept)
    {
        rpmsg_lite_destroy_ept(m33_rpmsg, m33_ept);
        m33_ept = NULL;
    }
    if (m33_queue)
    {
        rpmsg_queue_destroy(m33_rpmsg, m33_queue);
        m33_queue = NULL;
    }
    return E_OK;
}

void M33_RPMSG_Deinit(void)
{
    (void)M33_RPMSG_DestroyEndpoint();

    if (m33_rpmsg)
    {
        rpmsg_lite_deinit(m33_rpmsg);
        m33_rpmsg = NULL;
    }
}