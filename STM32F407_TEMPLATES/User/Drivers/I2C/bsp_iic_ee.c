/**
  ******************************************************************************
  * @file    bsp_iic_ee.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-28
  * @brief   Ӳ��I2CӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
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
	/* ������ʼλ */
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;
	
	/* �ȴ�EV5�¼� */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }
	/* �㲥��ַѰ��EEPROM */
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDR, I2C_Direction_Transmitter);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;
	
	/* �ȴ�EV6�¼� */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }   
	/* ָ��Ҫд���EPPROM��ַ */
	I2C_SendData(EEPROM_I2C, WriteAddr);
	
  I2CTimeout = I2CT_FLAG_TIMEOUT;

	/* �ȴ�EV8_2�¼� */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
	/* �������� */
	I2C_SendData(EEPROM_I2C, WriteData);
   
  I2CTimeout = I2CT_FLAG_TIMEOUT;

	/* �ȴ�EV8_2�¼� */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
	/* ���ͽ���λ */
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	
	return 1;
}
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾIIC��ȡʧ��.
  */
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  printf("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}
/**************************************************************************
�������    EEPROM��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    EEPROM_Init();
��ע��Ϣ    ��
**************************************************************************/
void EEPROM_Init(void)
{
	/* ��ʼ�������õ���GPIO */
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
	
	
	/* ��ʼ��IIC1 */
	RCC_APB1PeriphResetCmd(EEPROM_I2C_CLK, ENABLE);
	
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_Ack									=		I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress	=		I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed					=		I2C_Speed;
	I2C_InitStructure.I2C_DutyCycle						=		I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode								=		I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1					=		I2C_OWN_ADDRESS;
	
	I2C_Init(EEPROM_I2C, &I2C_InitStructure);
	
	/* ʹ��I2C */
	I2C_Cmd(EEPROM_I2C, ENABLE);
	
}
