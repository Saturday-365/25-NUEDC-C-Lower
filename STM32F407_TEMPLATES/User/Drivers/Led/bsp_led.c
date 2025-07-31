/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  SuperLi
  * @version V1.0
  * @date    2015-05-24
  * @brief   LEDӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  ******************************************************************************
  * @cfg	
  * 		
				R ��ɫ��
				LED1_PIN                  GPIO_Pin_6                 
				LED1_GPIO_PORT            GPIOF                      
				LED1_GPIO_CLK             RCC_AHB1Periph_GPIOF

				G ��ɫ��
				LED2_PIN                  GPIO_Pin_7                 
				LED2_GPIO_PORT            GPIOF                      
				LED2_GPIO_CLK             RCC_AHB1Periph_GPIOF

				B ��ɫ��
				LED3_PIN                  GPIO_Pin_8                 
				LED3_GPIO_PORT            GPIOF                       
				LED3_GPIO_CLK             RCC_AHB1Periph_GPIOF
  ******************************************************************************
  */
#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_Init(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*Ĭ�ϴ򿪺�ɫ��*/
		LED_Red();
}
/**************************************************************************
�������    ���º���Ϊ�ı�RGB����ɫ
����˵��    ��
���ز���    ��
ʹ��ʾ��    LED_Off();	�ر�RGB��
��ע��Ϣ   	��
**************************************************************************/
void LED_OFF(void)
{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Darkblue(void)
{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Green(void)
{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Blue(void)
{
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Red(void)
{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Purple(void)
{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_Yellow(void)
{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}
void LED_White(void)
{
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN);
}

/*********************************************END OF FILE**********************/
