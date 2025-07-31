/**
  ******************************************************************************
  * @file    bsp_Buzzer.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-24
  * @brief   ������Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
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
�������    ��������ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    Buzzer_Init();
��ע��Ϣ    ��
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
	/* Ĭ�Ϲر� */
	GPIO_ResetBits(Buzzer_PORT,Buzzer_PIN);
	
}

/**************************************************************************
�������    ����������
����˵��    ��
���ز���    ��
ʹ��ʾ��    Buzzer_ON();
��ע��Ϣ    ��
**************************************************************************/
void Buzzer_ON(void)
{
	GPIO_SetBits(Buzzer_PORT,Buzzer_PIN);
}

/**************************************************************************
�������    �رշ�����
����˵��    ��
���ز���    ��
ʹ��ʾ��    Buzzer_OFF();
��ע��Ϣ    ��
**************************************************************************/
void Buzzer_OFF(void)
{
	GPIO_ResetBits(Buzzer_PORT,Buzzer_PIN);
}

//void Buzzer_Beep()


