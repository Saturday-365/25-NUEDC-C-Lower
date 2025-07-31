/**
 ******************************************************************************
 * @file    main.c
 * @author  Licr
 * @version V1.0.8
 * @date    2025-5-24
 * @brief   基于[STM32F407ZGT6]的最小系统工程，包含基础外设驱动框架，支持通过模块化扩展
 *          ADC/DAC、定时器、PWM、通信接口等电赛常用外设。采用分层架构设计，
 *          外设驱动与业务逻辑解耦，便于后续功能迭代。
 ******************************************************************************
 * @attention
 * 1. 外设添加建议遵循 硬件接口层 -> 驱动层 -> 应用层 的分层模式
 * 2. 新外设模块请在Drivers目录下创建独立文件夹（如/Drivers/ADC），模块内必须包含：
 *    - [模块名].c（功能实现）
 *    - [模块名].h（API接口）
 * 3. 新函数必须在其定义前一行添加功能注释，格式：
 ***************************************************************************
 *函数简介    **************************************************************
 *参数说明    **************************************************************
 *返回参数    **************************************************************
 *使用示例    **************************************************************
 *备注信息    **************************************************************
 ***************************************************************************
 * 4. 硬件相关参数必须通过宏定义在_cfg.h中集中管理，例如：
 *    #define MOTOR_PWM_TIM         TIM3
 *    #define MOTOR_PWM_CHANNEL     TIM_Channel_2
 *    #define MOTOR_PWM_FREQ        10000
 * 5. 使用 Git 进行版本迭代，每次上传新版本时必须添加修改的内容
 *
 * 实验平台:野火  STM32 F407 开发板
 * @config
		 USART1:
			TX -> PA9
			RX -> PA10
		 USART2:
			TX -> PA2
			RX -> PA3
		 USART3:
			TX -> PB10
			RX -> PB11
 ******************************************************************************
 */
// 测试
#include "stm32f4xx.h"

#include <stdio.h>
#include "delay.h"
#include "bsp_exit.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_Buzzer.h"
#include "bsp_Usart.h"
#include "bsp_iic_ee.h"
#include "bsp_spi_Flash.h"
#include "bsp_Adc.h"

#include "HMI.h"

// ADC转换的电压值通过MDA方式传到SRAM
__IO uint16_t ADC_ConvertedValue = 0;
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_Vol; 
/**************************************************************************
函数简介    初始化所有外设
参数说明    无
返回参数    无
使用示例    Everything_Init();
备注信息    某些外设初始化需注意顺序
						发现Usart初始化要放在较后的位置，否则会出现printf不完全的情况，
						不知是什么原因。
**************************************************************************/
void Everything_Init(void)
{
	/* 系统定时器初始化 */
	SysTick_Init();
	/* LED 初始化 */
	LED_Init();
	/* 按键初始化 */
	Key_Init();
	/* 蜂鸣器初始化 */
	Buzzer_Init();
	/* 外部中断初始化 */
//	EXTI_Key_Init();
	/* USART初始化 */
	Usart_Init();
	/* ADC初始化 */
	Rheostat_Init();
	/* EPPROM初始化 */
//	EEPROM_Init();
	/* W25Q128初始化 */
//	SPI_FLASH_Init();
	
	printf("\r\n EverythingInit has already completed \r\n");
}
/**************************************************************************
函数简介    参数预设
参数说明    无
返回参数    无
使用示例    ValueSet();
备注信息    用于统一管理需要预设的参数
**************************************************************************/
void ValueSet(void)
{
}
int main(void)
{
	
	/* 所有外设初始化 */
	Everything_Init();
	/* 参数预设 */
	ValueSet();
	/* 发送数据 */
//	HMI_Send_EveryInfo();
	/* test */
//	HMI_send_string("t0.txt", "test_num");
//	HMI_send_float("Num_d.val", 5);
//	HMI_send_number("Num_I.val", 20);
	while (1)
	{
		ADC_Vol =(float) ADC_ConvertedValue/4096*(float)3.3*40*(float)0.01; // 读取转换的AD值

		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",ADC_Vol);     
		
		Delay_ms(500);
		/* test */
//		LED_Yellow();
//		Delay_ms(500);
//		LED_Red();
//		Delay_ms(500);
//		Usart_SendByte(USART3,'test');
	
	}
}

/*********************************************END OF FILE**********************/
