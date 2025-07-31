/**
  ******************************************************************************
  * @file    bsp_exit.c
  * @author  fire
  * @version V1.0
  * @date    2025-05-23
  * @brief   �ⲿ�жϺ����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  ******************************************************************************
  * @exitcfg		
  * ��
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_exit.h"
#include "bsp_key.h"
/**************************************************************************
�������    NVIC ���ú���
����˵��    ��
���ز���    ��
ʹ��ʾ��    NVIC_Configuration();
��ע��Ϣ    NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����
						��ռ���ȼ���Ϊ1
						�����ȼ���Ϊ1
**************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
//  /* �����ж�Դ������2 */
//  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
//	
//  NVIC_Init(&NVIC_InitStructure);
	
}
/**************************************************************************
�������    �����ⲿ�жϳ�ʼ������
����˵��    ��
���ز���    ��
ʹ��ʾ��    EXTI_Key_Init();
��ע��Ϣ    �жϷ�����:stm32f4xx_it.c: KEY1_IRQHandler();
						��ռ���ȼ���Ϊ1
						�����ȼ���Ϊ1			
**************************************************************************/
void EXTI_Key_Init(void)
{
	
  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	/* ���� EXTI �ж�Դ ��key1���� */
  SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	
	NVIC_Configuration();
	/* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
  /* �ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* �����ش��� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  /* ʹ���ж�/�¼��� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
  EXTI_Init(&EXTI_InitStructure);
	
//	/* ���� EXTI �ж�Դ ��key2���� */
//  SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);
//	/* ѡ�� EXTI �ж�Դ */
//  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
//  /* �ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* �����ش��� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//  /* ʹ���ж�/�¼��� */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	
//  EXTI_Init(&EXTI_InitStructure);
	
}

