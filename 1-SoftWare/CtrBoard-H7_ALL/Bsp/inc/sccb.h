#ifndef __SCCB_H__
#define __SCCB_H__

#include "main.h"

#define OV2640_DEVICE_ADDRESS 0x60 // OV2640地址
#define OV5640_DEVICE_ADDRESS 0X78 // OV5640地址

/*----------------------------------------- IIIC 引脚配置宏 -----------------------------------------------*/

#define SCCB_SCL_CLK_ENABLE 	__HAL_RCC_GPIOB_CLK_ENABLE() // SCL 引脚时钟
#define SCCB_SCL_PORT 				GPIOB								 // SCL 引脚端口
#define SCCB_SCL_PIN 					GPIO_PIN_8							 // SCL 引脚

#define SCCB_SDA_CLK_ENABLE 	__HAL_RCC_GPIOB_CLK_ENABLE() // SDA 引脚时钟
#define SCCB_SDA_PORT 				GPIOB								 // SDA 引脚端口
#define SCCB_SDA_PIN 					GPIO_PIN_9							 // SDA 引脚

/*------------------------------------------ IIC相关定义 -------------------------------------------------*/

#define ACK_OK 1  // 响应正常
#define ACK_ERR 0 // 响应错误
/*-------------------------------------------- IO口操作 ---------------------------------------------------*/

#define SCCB_SCL(x)                                                   \
	if (x)                                                            \
		HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); \
	else                                                              \
		HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_RESET)

#define SCCB_SDA(x)                                                   \
	if (x)                                                            \
		HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); \
	else                                                              \
		HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_RESET)

#define SCCB_READ_SDA HAL_GPIO_ReadPin(SCCB_SDA_PORT, SCCB_SDA_PIN)
/*--------------------------------------------- 函数声明 --------------------------------------------------*/

void sccb_init(void);					  // IIC引脚初始化
void sccb_delay(void);					  // IIC延时函数
void sccb_start(void);					  // 启动IIC通信
void sccb_stop(void);					  // IIC停止信号
void sccb_ack(void);					  // 发送响应信号
void sccb_nack(void);					  // 发送非应答信号
uint8_t sccb_wait_ack(void);			  // 等待应答信号
uint8_t sccb_send_byte(uint8_t data);	  // 写字节函数
uint8_t sccb_read_byte(uint8_t ack_mode); // 读字节函数

uint8_t sccb_write_reg(uint8_t addr, uint8_t value); // 对指定的寄存器(8位地址)写一字节数据，OV2640用到
uint8_t sccb_read_reg(uint8_t addr);				 // 对指定的寄存器(8位地址)读一字节数据，OV2640用到

uint8_t sccb_write_reg_16bit(uint16_t addr, uint8_t value);					// 对指定的寄存器(16位地址)写一字节数据，OV5640用到
uint8_t sccb_read_reg_16bit(uint16_t addr);									// 对指定的寄存器(16位地址)读一字节数据，OV5640用到
uint8_t sccb_write_buf_16bit(uint16_t addr, uint8_t *pData, uint32_t size); // 对指定的寄存器(16位地址)批量写数据，OV5640 写入自动对焦固件时用到

#endif //__SCCB_H__
