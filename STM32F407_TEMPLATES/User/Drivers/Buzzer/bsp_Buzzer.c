/**
  ******************************************************************************
  * @file    bsp_Buzzer.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-24
  * @brief   蜂鸣器应用函数接口
  ******************************************************************************
  * @attention
  * 实验平台:野火  STM32 F407 开发板  
  ******************************************************************************
  * @cfg	
	*			PG7	<-- 
  * 		Buzzer_PIN    GPIO_Pin_7
				Buzzer_PORT   GPIOG
				Buzzer_CLK    RCC_AHB1Periph_GPIOG
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_Buzzer.h"
/**************************************************************************
函数简介    蜂鸣器初始化
参数说明    无
返回参数    无
使用示例    Buzzer_Init();
备注信息    无
**************************************************************************/
void Buzzer_Init(void)
{
	RCC_AHB1PeriphClockCmd(Buzzer_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin		=	Buzzer_PIN;
	GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Low_Speed;

	GPIO_Init(Buzzer_PORT, &GPIO_InitStructure);
	/* 默认关闭 */
	GPIO_ResetBits(Buzzer_PORT,Buzzer_PIN);
	
}

/**************************************************************************
函数简介    开启蜂鸣器
参数说明    无
返回参数    无
使用示例    Buzzer_ON();
备注信息    无
**************************************************************************/
void Buzzer_ON(void)
{
	GPIO_SetBits(Buzzer_PORT,Buzzer_PIN);
}

/**************************************************************************
函数简介    关闭蜂鸣器
参数说明    无
返回参数    无
使用示例    Buzzer_OFF();
备注信息    无
**************************************************************************/
void Buzzer_OFF(void)
{
	GPIO_ResetBits(Buzzer_PORT,Buzzer_PIN);
}

//void Buzzer_Beep()


