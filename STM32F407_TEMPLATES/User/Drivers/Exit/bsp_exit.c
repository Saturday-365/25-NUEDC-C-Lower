/**
  ******************************************************************************
  * @file    bsp_exit.c
  * @author  fire
  * @version V1.0
  * @date    2025-05-23
  * @brief   外部中断函数接口
  ******************************************************************************
  * @attention
  * 实验平台:野火  STM32 F407 开发板  
  ******************************************************************************
  * @exitcfg		
  * 无
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_exit.h"
#include "bsp_key.h"
/**************************************************************************
函数简介    NVIC 配置函数
参数说明    无
返回参数    无
使用示例    NVIC_Configuration();
备注信息    NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组
						抢占优先级设为1
						子优先级设为1
**************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
//  /* 配置中断源：按键2 */
//  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
//	
//  NVIC_Init(&NVIC_InitStructure);
	
}
/**************************************************************************
函数简介    按键外部中断初始化函数
参数说明    无
返回参数    无
使用示例    EXTI_Key_Init();
备注信息    中断服务函数:stm32f4xx_it.c: KEY1_IRQHandler();
						抢占优先级设为1
						子优先级设为1			
**************************************************************************/
void EXTI_Key_Init(void)
{
	
  /* 使能 SYSCFG 时钟 ，使用GPIO外部中断时必须使能SYSCFG时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	/* 连接 EXTI 中断源 到key1引脚 */
  SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	
	NVIC_Configuration();
	/* 选择 EXTI 中断源 */
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
  /* 中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* 上升沿触发 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  /* 使能中断/事件线 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
  EXTI_Init(&EXTI_InitStructure);
	
//	/* 连接 EXTI 中断源 到key2引脚 */
//  SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);
//	/* 选择 EXTI 中断源 */
//  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
//  /* 中断模式 */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* 上升沿触发 */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//  /* 使能中断/事件线 */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	
//  EXTI_Init(&EXTI_InitStructure);
	
}

