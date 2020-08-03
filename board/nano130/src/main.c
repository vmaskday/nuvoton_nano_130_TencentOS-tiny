/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 2 $
 * $Date: 14/05/14 6:35p $
 * @brief    MPU9250 project for Nano100 series MCU
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*/


//#include "main.h"
#include "Nano100Series.h"
#include <stdio.h>
#include <tos_k.h>
#include "cmsis_os.h"
#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    //CLK->PWRCTL |= CLK_PWRCTL_HXT_EN_Msk;
    CLK->PWRCTL |= CLK_PWRCTL_HIRC_EN_Msk;

    /* Waiting for 12MHz clock ready */
    //while((CLK->CLKSTATUS & CLK_CLKSTATUS_HXT_STB_Msk) == 0){};
    while ((CLK->CLKSTATUS & CLK_CLKSTATUS_HIRC_STB_Msk) == 0) {};

    /* Switch PLL clock source to XTAL */
    //CLK->PLLCTL &= CLK_PLLCTL_PLL_SRC_HXT;
    CLK->PLLCTL &= CLK_PLLCTL_PLL_SRC_HIRC;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    CLK->PLLCTL &= CLK_PLLCTL_PD_Msk;

    /* Set PLL frequency 84 MHz */
    CLK->PLLCTL |= 0x1000;

    /* Waiting for clock ready */
    while ((CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk) == 0) {};

    /* Set HCLK_N = 1 */
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLK_N_Msk) | 0x01;

    /* Switch HCLK clock source to PLL */
    CLK->CLKSEL0 = CLK_CLKSEL0_HCLK_S_PLL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

}
#else
void SYS_Init(void);
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Init UART1                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void UART_Init(void)
{
    /* Set PC.10 and PC.11 multi-function pins for UART1_RXD, UART1_TXD */
    //SYS->PC_H_MFP |= (SYS_PC_H_MFP_PC11_MFP_UART1_TX | SYS_PC_H_MFP_PC10_MFP_UART1_RX);
	  SYS->PB_L_MFP = SYS_PB_L_MFP_PB5_MFP_UART1_TX | SYS_PB_L_MFP_PB4_MFP_UART1_RX | SYS_PB_L_MFP_PB1_MFP_UART0_TX | SYS_PB_L_MFP_PB0_MFP_UART0_RX;
    UART_Open(UART0,115200);
	  UART_Open(UART1,115200);

}

void AUTO_GPIO_INIT()
{
    SYS->PA_H_MFP = 0x00000000;
    SYS->PA_L_MFP = 0x00000000;
	  SYS->PB_H_MFP = 0x00000000;
	  //SYS->PB_L_MFP = 0x00000000;
    //SYS->PB_H_MFP = SYS_PB_H_MFP_PB12_MFP_CKO;
    SYS->PB_L_MFP = SYS_PB_L_MFP_PB5_MFP_UART1_TX | SYS_PB_L_MFP_PB4_MFP_UART1_RX | SYS_PB_L_MFP_PB1_MFP_UART0_TX | SYS_PB_L_MFP_PB0_MFP_UART0_RX;
    SYS->PC_H_MFP = 0x00000000;
    SYS->PC_L_MFP = 0x00000000;
    SYS->PD_H_MFP = 0x00000000;
    SYS->PD_L_MFP = 0x00000000;
    SYS->PE_H_MFP = 0x00000000;
    SYS->PE_L_MFP = 0x00000000;
    SYS->PF_L_MFP = SYS_PF_L_MFP_PF3_MFP_HXT_IN | SYS_PF_L_MFP_PF2_MFP_HXT_OUT | SYS_PF_L_MFP_PF1_MFP_ICE_CLK | SYS_PF_L_MFP_PF0_MFP_ICE_DAT;
}

#define TASK1_STK_SIZE		256
void task1_fun(void *pdata);
osThreadDef(task1_fun, osPriorityNormal, 1, TASK1_STK_SIZE);
   
   //task2
#define TASK2_STK_SIZE		256
void task2_task(void *pdata);
osThreadDef(task2_task, osPriorityNormal, 1, TASK2_STK_SIZE);
//task1 function
void task1_fun(void *Parameter)
{
    while(1)
    {
        printf("\n\nCPU @ %dHz\n", SysGet_HCLKFreq());
        //PB14 =~ PB14;
        osDelay(1000);
    }
}

void task2_task(void *Parameter)
{
    //k_err_t err;
    while(1)
    {
        printf("Task2: hello world, hello Nuvoton \r\n");
        osDelay(1000);
    }
}

int main()
{
	  //k_err_t err;
    /* Initial system */
	  AUTO_GPIO_INIT();
    SYS_Init();

    /* Initial UART to 115200-8n1 for print message */
    UART_Init();
	
    osKernelInitialize(); //TOS Tiny kernel initialize
    osThreadCreate(osThread(task1_fun), NULL);// Create task1
    osThreadCreate(osThread(task2_task), NULL);// Create task2
    osKernelStart();//Start TOS Tiny
}


//systick interrupt Handler function
void SysTick_Handler(void)
{
    if (tos_knl_is_running())
    {
        tos_knl_irq_enter();
        tos_tick_handler();
        tos_knl_irq_leave();
    }
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
