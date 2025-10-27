/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"
#include "Boot/boot_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void IMX_LPTMP1_Init(void);
static void IMX_MU_Init(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Initialize all configured peripherals */
 //   IMX_LPTMP1_Init();
    IMX_MU_Init();

    /* USER CODE BEGIN */
    PRINTF("\r\n\r\n== Pay-Exp Core M33 App Start ==\r\n");
    BootManager_SystemInit();
    BootManager_SystemStart();
    
    while (1)
    {
        ;
    }
}

/*******************************************************************************
 * Peripheral Init Function
 ******************************************************************************/

/**
  * @brief LPTMP1 Initialization Function
  * @param None
  * @retval None
  */
#define LPTMR_SOURCE_CLOCK 32000U
#define LPTMR_USEC_COUNT 1000000U

static void IMX_LPTMP1_Init(void)
{
    lptmr_config_t lptmrConfig;
    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */

    /* Ungate the LPTMR clock*/
    CLOCK_EnableClock(kCLOCK_Lptmr1);
    /* Get default LPTMR configuration */
    LPTMR_GetDefaultConfig(&lptmrConfig);

    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR1, &lptmrConfig);

    /*
     * Set timer period.
     * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
     */
    LPTMR_SetTimerPeriod(LPTMR1, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));
 
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQWithPriority(LPTMR1_IRQn, 10);

    /* Start counting */
    LPTMR_StartTimer(LPTMR1);
}

static void IMX_MU_Init(void)
{
    /* Enable the MU clock */
    CLOCK_EnableClock(kCLOCK_Mu_A);
    CLOCK_EnableClock(kCLOCK_Mu_B);
}