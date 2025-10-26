/************************************************
 *  @file     : boot_manager.c
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "boot_manager.h"
#include "OS/Kernel/rtos.h"
#include "DateTime/date_time.h"
#include "CRC/crc16_xmodem.h"
#include "remoteproc/m33_remoteproc.h"
#include "rpmsg/m33_rpmsg.h"
#include "bsp_core.h"
/*============================================================*/
/*                      Defines                               */
/*============================================================*/

/*============================================================*/
/*                   Private Variables                        */
/*============================================================*/

/*============================================================*/
/*                   Public Variables                         */
/*============================================================*/

/*============================================================*/
/*              Private Function Prototypes                   */
/*============================================================*/

/*============================================================*/
/*                Function Implementations                    */
/*============================================================*/

Std_ReturnType BootManager_SystemInit(void){
    M33_RPROC_Init();
    Utils_SoftTime_Init();
    CRC16_XMODEM_Init();
    M33_RPMSG_Init();

    return E_OK;
}

void BootManager_SystemStart(void){

	EXP_RTOS_Start();
}
