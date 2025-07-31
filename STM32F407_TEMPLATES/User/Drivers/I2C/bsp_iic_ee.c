/**
  ******************************************************************************
  * @file    bsp_iic_ee.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-28
  * @brief   硬件I2C应用函数接口
  ******************************************************************************
  * @attention
  * 实验平台:野火  STM32 F407 开发板  
  ******************************************************************************
  * @cfg	
	*			I2C1  SCL - PB8  SDA - PB9
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_iic_ee.h"
#include "bsp_led.h"
uint32_t EEPROM_Write_Byte(uint8_t WriteAddr, uint8_t WriteData)
{
	/* 发送起始位 */
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;
	
	/* 等待EV5事件 */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }
	/* 广播地址寻找EEPROM */
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDR, I2C_Direction_Transmitter);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;
	
	/* 等待EV6事件 */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }   
	/* 指定要写入的EPPROM地址 */
	I2C_SendData(EEPROM_I2C, WriteAddr);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;

	/* 等待EV8_2事件 */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
	/* 发送数据 */
	I2C_SendData(EEPROM_I2C, WriteData);
   
  I2CTimeout = I2CT_FLAG_TIMEOUT;

	/* 等待EV8_2事件 */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
	/* 发送结束位 */
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	
	return 1;
}
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  printf("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0;
}
/**************************************************************************
函数简介    EEPROM初始化
参数说明    无
返回参数    无
使用示例    EEPROM_Init();
备注信息    无
**************************************************************************/
void EEPROM_Init(void)
{
	/* 初始化串口用到的GPIO */
	RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
	
	GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT,EEPROM_I2C_SCL_SOURCE,EEPROM_I2C_SCL_AF);
	GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT,EEPROM_I2C_SDA_SOURCE,EEPROM_I2C_SDA_AF);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd		=	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Fast_Speed;

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	EEPROM_I2C_SCL_PIN;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	EEPROM_I2C_SDA_PIN;
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	
	/* 初始化IIC1 */
	RCC_APB1PeriphResetCmd(EEPROM_I2C_CLK, ENABLE);
	
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_Ack									=		I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress	=		I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed					=		I2C_Speed;
	I2C_InitStructure.I2C_DutyCycle						=		I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode								=		I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1					=		I2C_OWN_ADDRESS;
	
	I2C_Init(EEPROM_I2C, &I2C_InitStructure);
	
	/* 使能I2C */
	I2C_Cmd(EEPROM_I2C, ENABLE);
	
}
