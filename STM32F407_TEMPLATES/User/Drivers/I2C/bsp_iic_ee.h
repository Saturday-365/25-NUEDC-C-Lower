#ifndef __BSP_IIC_EE_H
#define	__BSP_IIC_EE_H

#include "stm32f4xx.h"
#include <stdio.h>

#define I2C_OWN_ADDRESS      								0X66 
#define EEPROM_I2C_ADDR											0xA0

/* STM32 I2C 快速模式 */
#define I2C_Speed              							400000

//EEPROM相关引脚宏定义
#define EEPROM_I2C                          I2C1
#define EEPROM_I2C_CLK                      RCC_APB1Periph_I2C1
#define EEPROM_I2C_CLK_INIT									RCC_APB1PeriphClockCmd

#define EEPROM_I2C_SCL_PIN                  GPIO_Pin_8                 
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_SOURCE               GPIO_PinSource8
#define EEPROM_I2C_SCL_AF                   GPIO_AF_I2C1

#define EEPROM_I2C_SDA_PIN                  GPIO_Pin_9                  
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_SOURCE               GPIO_PinSource9
#define EEPROM_I2C_SDA_AF                   GPIO_AF_I2C1

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
static __IO uint32_t  I2CTimeout; 

uint32_t EEPROM_Write_Byte(uint8_t WriteAddr, uint8_t WriteData);
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
void EEPROM_Init(void);

#endif /* __bsp_iic_ee_H */

