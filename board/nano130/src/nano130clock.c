/****************************************************************************
 * @file     nano130clock.c
 * @version  V1.04
 * @Date     2020/07/15-22:24:31
 * @brief    NuMicro generated code file
 *
 * Copyright (C) 2013-2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

/********************
MCU:NANO130KE3BN(LQFP128)
Base Clocks:
LIRC:10kHz
HIRC:12MHz
LXT:32.7680kHz
HXT:12MHz
PLL:96MHz
HCLK:32MHz
PCLK:32MHz
Enabled-Module Frequencies:
GPIO=Bus Clock(HCLK):32MHz
ISP=Bus Clock(HCLK):32MHz/Engine Clock:12MHz
SRAM=Bus Clock(HCLK):32MHz
SYSTICK=Bus Clock(HCLK):32MHz/Engine Clock:4MHz
TICK=Bus Clock(HCLK):32MHz
UART0=Bus Clock(PCLK):32MHz/Engine Clock:12MHz
UART1=Bus Clock(PCLK):32MHz/Engine Clock:12MHz
WDT=Bus Clock(PCLK):32MHz/Engine Clock:10kHz
WWDT=Bus Clock(PCLK):32MHz/Engine Clock:15.6250kHz
********************/

#include "Nano100Series.h"

/*----------------------------------------------------------------------------
  Define HXT clock.
  Please locate and modify the real one in your project.
  Otherwise, the project may fail to build.
 *----------------------------------------------------------------------------*/
#define __HXT         (12000000UL)  /*!< High Speed External Crystal Clock Frequency */

/*
 * @brief This function updates clock registers to fulfil the configuration
 * @param None
 * @return None
 */
void SYS_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    //CLK->PWRCTL = (CLK->PWRCTL & ~(0x0000000Ful)) | 0x0000031Ful;
    //CLK->PLLCTL = (CLK->PLLCTL & ~(0x0003133Ful)) | 0x00000100ul;
    //CLK->CLKDIV0 = (CLK->CLKDIV0 & ~(0xF0FFFFFFul)) | 0x00000002ul;
    //CLK->CLKDIV1 = (CLK->CLKDIV1 & ~(0x000000FFul)) | 0x00000000ul;
    //CLK->CLKSEL0 = (CLK->CLKSEL0 & ~(0x00000007ul)) | 0x00000002ul;
    //CLK->CLKSEL1 = (CLK->CLKSEL1 & ~(0x000077FFul)) | 0x0007FFFFul;
    //CLK->CLKSEL2 = (CLK->CLKSEL2 & ~(0x007F77FCul)) | 0x000FFFFFul;
    //CLK->AHBCLK = (CLK->AHBCLK & ~(0x0000003Ful)) | 0x00000035ul;
    //CLK->APBCLK = (CLK->APBCLK & ~(0xFEF373FFul)) | 0x00030001ul;
    //CLK->FRQDIV = (CLK->FRQDIV & ~(0x0000001Ful)) | 0x00000000ul;
    //SysTick->CTRL = (SysTick->CTRL & ~(0x00000005ul)) | 0x00000001ul;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable clock source */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRC_EN_Msk|CLK_PWRCTL_HIRC_EN_Msk|CLK_PWRCTL_LXT_EN_Msk|CLK_PWRCTL_HXT_EN_Msk);

    /* Waiting for clock source ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_LIRC_STB_Msk|CLK_CLKSTATUS_HIRC_STB_Msk|CLK_CLKSTATUS_LXT_STB_Msk|CLK_CLKSTATUS_HXT_STB_Msk);

    /* Disable PLL first to avoid unstable when setting PLL */
    CLK_DisablePLL();

    /* Set PLL frequency */
    CLK->PLLCTL = (CLK->PLLCTL & ~(0x0003133Ful)) | 0x00000100ul;

    /* Waiting for PLL ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk);

    /* If the defines do not exist in your project, please refer to the related clk.h in the Header folder appended to the tool package. */
    /* Set HCLK clock */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_HCLK_CLK_DIVIDER(2));

    /* Enable IP clock */
    CLK_EnableModuleClock(GPIO_MODULE);
    CLK_EnableModuleClock(ISP_MODULE);
    CLK_EnableModuleClock(SRAM_MODULE);
    CLK_EnableModuleClock(TICK_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(UART1_MODULE);
    CLK_EnableModuleClock(WDT_MODULE);
    CLK_EnableSysTick(CLK_CLKSEL0_STCLKSEL_HCLK_DIV8, 0);

    /* Set IP clock */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_UART_CLK_DIVIDER(1));
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_UART_CLK_DIVIDER(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

    return;
}

/*** (C) COPYRIGHT 2013-2020 Nuvoton Technology Corp. ***/
