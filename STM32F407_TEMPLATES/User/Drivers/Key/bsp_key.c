/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-22
  * @brief   ����Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  ******************************************************************************
  *@cfg	
	*			KEY_UP 			->	PD11
	*			KEY_YES 		->	PD12
	*			KEY_LEFT 		->	PD13
	*			KEY_DOWN 		->	PD14
	*			KEY_RIGHT 	->	PD15
	*			
	*			
  *				
				
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include "bsp_systick.h"
#include "bsp_key.h"
#include "delay.h"
/**************************************************************************
�������    ������ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    Key_Init();
��ע��Ϣ    ��
**************************************************************************/
void Key_Init(void)
{
    RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       // ��������
    GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN | 
																	KEY_YES_PIN | 
																	KEY_LEFT_PIN | 
																	KEY_DOWN_PIN | 
																	KEY_RIGHT_PIN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}
/**************************************************************************
�������    ��ȡ�������ؼ�ֵ
����˵��    ��
���ز���    uint8_t KeyNum;
ʹ��ʾ��    int keynum = Key_GetNum();
��ע��Ϣ    �жϰ����Ƿ��µ�������Ҫ����ʵ�ʵ�·����
						��
						�������½ӵ͵�ƽ��GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0
						�������½Ӹߵ�ƽ��GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 1
**************************************************************************/
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    /* �������£��͵�ƽ�� */
    if (GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN) == 0)  
    {
        Delay_ms(20);                                   
        while (GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN) == 0); 
        Delay_ms(20);                                   
        KeyNum = KEY_UP;                                     
    }
    
    if (GPIO_ReadInputDataBit(KEY_YES_PORT, KEY_YES_PIN) == 0)  
    {
        Delay_ms(20);                                   
        while (GPIO_ReadInputDataBit(KEY_YES_PORT, KEY_YES_PIN) == 0); 
        Delay_ms(20);                                   
        KeyNum = KEY_YES;                                     
    }
		
		if (GPIO_ReadInputDataBit(KEY_LEFT_PORT, KEY_LEFT_PIN) == 0)  
    {
        Delay_ms(20);                                   
        while (GPIO_ReadInputDataBit(KEY_LEFT_PORT, KEY_LEFT_PIN) == 0); 
        Delay_ms(20);                                   
        KeyNum = KEY_LEFT;                                    
    }
		
		if (GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0)  
    {
        Delay_ms(20);                                   
        while (GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0); 
        Delay_ms(20);                                   
        KeyNum = 4;                                     
    }
		
		if (GPIO_ReadInputDataBit(KEY_RIGHT_PORT, KEY_RIGHT_PIN) == 0)  
    {
        Delay_ms(20);                                   
        while (GPIO_ReadInputDataBit(KEY_RIGHT_PORT, KEY_RIGHT_PIN) == 0); 
        Delay_ms(20);                                   
        KeyNum = 5;                                     
    }
    
    return KeyNum;  
}
void KEY_TEST(void)
{
		uint8_t keynum = Key_GetNum();
		if( keynum != 0 )
		{
			if(keynum == KEY_LEFT)
			printf("\r\n KEY_LEFT \r\n");
			if(keynum == KEY_RIGHT)
			printf("\r\n KEY_RIGHT \r\n");
			if(keynum == KEY_UP)
			printf("\r\n KEY_UP \r\n");
			if(keynum == KEY_DOWN)
			printf("\r\n KEY_DOWN \r\n");
			if(keynum == KEY_YES)
			printf("\r\n KEY_YES \r\n");
		}
}
