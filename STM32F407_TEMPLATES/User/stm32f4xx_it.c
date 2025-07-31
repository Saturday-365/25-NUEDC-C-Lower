/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "bsp_exit.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_Usart.h"
#include "bsp_Adc.h"
#include <stdio.h>
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
/**************************************************************************
函数简介    USART1接收中断服务函数
参数说明    无
返回参数    无
使用示例    USART1_IRQHandler();
备注信息    
**************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART_1,USART_IT_RXNE)!=RESET)
	{		
		
	}	
}
/**************************************************************************
函数简介    USART2接收中断服务函数
参数说明    无
返回参数    无
使用示例    USART2_IRQHandler();
备注信息    
**************************************************************************/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART_2,USART_IT_RXNE)!=RESET)
	{		
		
	}	
}
/**************************************************************************
函数简介    USART3接收中断服务函数
参数说明    无
返回参数    无
使用示例    USART3_IRQHandler();
备注信息    
**************************************************************************/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART_3,USART_IT_RXNE)!=RESET)
	{		
		
	}	
}
extern __IO uint16_t ADC_ConvertedValue;

/**************************************************************************
函数简介    ADC采集中断服务函数
参数说明    无
返回参数    无
使用示例    ADC_IRQHandler();
备注信息    
**************************************************************************/
void ADC_IRQHandler(void)
{
	if(ADC_GetITStatus(RHEOSTAT_ADC,ADC_IT_EOC)==SET)
	{
  // 读取ADC的转换值
		ADC_ConvertedValue = ADC_GetConversionValue(RHEOSTAT_ADC);

	}
	ADC_ClearITPendingBit(RHEOSTAT_ADC,ADC_IT_EOC);

}	
/**************************************************************************
函数简介    系统定时器中断服务函数
参数说明    无
返回参数    无
使用示例    SysTick_Handler();
备注信息    * SystemFrequency / 1000    	1ms中断一次
						* SystemFrequency / 100000	 	10us中断一次
						* SystemFrequency / 1000000 	1us中断一次
						要更改中断周期到 bsp_systick.h SysTick_Init(); 修改形参数值
**************************************************************************/
void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/**************************************************************************
函数简介    按键1外部中断服务函数
参数说明    无
返回参数    无
使用示例    KEY1_IRQHandler();
备注信息    无							
**************************************************************************/
void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{	
		LED_Darkblue();
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}
/**************************************************************************
函数简介    按键2外部中断服务函数
参数说明    无
返回参数    无
使用示例    KEY1_IRQHandler();
备注信息    无							
**************************************************************************/
//void KEY2_IRQHandler(void)
//{
//  //确保是否产生了EXTI Line中断
//	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
//	{
//		LED_Green();
//    //清除中断标志位
//		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
//	}  
//}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
