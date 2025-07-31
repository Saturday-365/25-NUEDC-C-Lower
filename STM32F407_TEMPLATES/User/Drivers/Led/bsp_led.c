/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  SuperLi
  * @version V1.0
  * @date    2015-05-24
  * @brief   LED应用函数接口
  ******************************************************************************
  * @attention
  * 实验平台:野火  STM32 F407 开发板  
  ******************************************************************************
  * @cfg	
  * 		
				R 红色灯
				LED1_PIN                  GPIO_Pin_6                 
				LED1_GPIO_PORT            GPIOF                      
				LED1_GPIO_CLK             RCC_AHB1Periph_GPIOF

				G 绿色灯
				LED2_PIN                  GPIO_Pin_7                 
				LED2_GPIO_PORT            GPIOF                      
				LED2_GPIO_CLK             RCC_AHB1Periph_GPIOF

				B 蓝色灯
				LED3_PIN                  GPIO_Pin_8                 
				LED3_GPIO_PORT            GPIOF                       
				LED3_GPIO_CLK             RCC_AHB1Periph_GPIOF
  ******************************************************************************
  */
#include "stm32f4xx.h"                  // Device header
#include "bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*默认打开红色灯*/
		LED_Red();
}
/**************************************************************************
函数简介    以下函数为改变RGB灯颜色
参数说明    无
返回参数    无
使用示例    LED_Off();	关闭RGB灯
备注信息   	无
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
