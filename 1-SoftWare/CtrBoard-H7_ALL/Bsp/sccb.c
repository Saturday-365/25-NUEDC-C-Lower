#include "sccb.h"

/**
************************************************************************
* @brief:      	sccb_init
* @param:       void
* @retval:     	void
* @details:    	初始化IIC的GPIO口，推挽输出
* @note:       	由于IIC通信速度不高，这里的IO口速度配置为2M即可
************************************************************************
**/
void sccb_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	SCCB_SCL_CLK_ENABLE; // 初始化IO口时钟
	SCCB_SDA_CLK_ENABLE;

	GPIO_InitStruct.Pin = SCCB_SCL_PIN;			 // SCL引脚
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;	 // 开漏输出
	GPIO_InitStruct.Pull = GPIO_NOPULL;			 // 不带上下拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 速度等级
	HAL_GPIO_Init(SCCB_SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SCCB_SDA_PIN; // SDA引脚
	HAL_GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
	GPIO_InitStruct.Pull = GPIO_PULLUP;			// 上拉

	HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); // SCL输出高电平
	HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); // SDA输出高电平
}
/**
************************************************************************
* @brief:      	sccb_delay
* @param:       a - 延时时间
* @retval:     	void
* @details:    	简单延时函数
* @note:       	为了移植的简便性且对延时精度要求不高，所以不需要使用定时器做延时
************************************************************************
**/
void sccb_delay(void)
{
	uint32_t a = 8;
	volatile uint16_t i;
	while (a--)
		for (i = 0; i < 3; i++);
}
/**
************************************************************************
* @brief:      	sccb_start
* @param:       void
* @retval:     	void
* @details:    	IIC起始信号
* @note:       	在SCL处于高电平期间，SDA由高到低跳变为起始信号
************************************************************************
**/
void sccb_start(void)
{
	SCCB_SDA(1);
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();
	SCCB_SCL(0);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_stop
* @param:       无
* @retval:     	无
* @details:    	IIC停止信号
* @note:       	在SCL处于高电平期间，SDA由低到高跳变为起始信号
************************************************************************
**/
void sccb_stop(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();

	SCCB_SCL(1);
	sccb_delay();
	SCCB_SDA(1);
	sccb_delay();
}

/**
************************************************************************
* @brief:      	sccb_ack
* @param:       void
* @retval:     	void
* @details:    	IIC应答信号
* @note:       	在SCL为高电平期间，SDA引脚输出为低电平，产生应答信号
************************************************************************
**/
void sccb_ack(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(0);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SCL(0); // SCL输出低时，SDA应立即拉高，释放总线
	SCCB_SDA(1);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_nack
* @param:       void
* @retval:     	void
* @details:    	IIC非应答信号
* @note:       	在SCL为高电平期间，若SDA引脚为高电平，产生非应答信号
************************************************************************
**/
void sccb_nack(void)
{
	SCCB_SCL(0);
	sccb_delay();
	SCCB_SDA(1);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();
	SCCB_SCL(0);
	sccb_delay();
}
/**
************************************************************************
* @brief:      	sccb_wait_ack
* @param:       void
* @retval:     	void
* @details:    	等待接收设备发出应答信号
* @note:       	在SCL为高电平期间，若检测到SDA引脚为低电平，则接收设备响应正常
************************************************************************
**/
uint8_t sccb_wait_ack(void)
{
	SCCB_SDA(1);
	sccb_delay();
	SCCB_SCL(1);
	sccb_delay();

	if (SCCB_READ_SDA != 0) // 判断设备是否有做出响应
	{
		SCCB_SCL(0);
		sccb_delay();
		return ACK_ERR; // 无应答
	}
	else
	{
		SCCB_SCL(0);
		sccb_delay();
		return ACK_OK; // 应答正常
	}
}
/**
************************************************************************
* @brief:      	sccb_send_byte
* @param:      	data - 要写入的8位数据
* @retval:     	ACK_OK  - 设备响应正常
*              	ACK_ERR - 设备响应错误
* @details:    	写一字节数据
* @note:       	高位在前
************************************************************************
**/
uint8_t sccb_send_byte(uint8_t data)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		SCCB_SDA(data & 0x80);

		sccb_delay();
		SCCB_SCL(1);
		sccb_delay();
		SCCB_SCL(0);
		if (i == 7)
		{
			SCCB_SDA(1);
		}
		data <<= 1;
	}

	return sccb_wait_ack(); // 等待设备响应
}
/**
************************************************************************
* @brief:      	sccb_read_byte
* @param:      	ack_mode - 响应模式，输入1则发出应答信号，输入0发出非应答信号
* @retval:     	ACK_OK  - 设备响应正常
*              	ACK_ERR - 设备响应错误
* @details:    	读一字节数据
* @note:       	1.高位在前
*              	2.应在主机接收最后一字节数据时发送非应答信号
************************************************************************
**/
uint8_t sccb_read_byte(uint8_t ack_mode)
{
	uint8_t data = 0;
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
	{
		data <<= 1;

		SCCB_SCL(1);
		sccb_delay();
		data |= (SCCB_READ_SDA & 0x01);
		SCCB_SCL(0);
		sccb_delay();
	}

	if (ack_mode == 1) //	应答信号
		sccb_ack();
	else
		sccb_nack(); // 非应答信号

	return data;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_handle
* @param:      	addr - 要进行操作的寄存器(8位地址)
* @retval:     	SUCCESS - 操作成功，ERROR	  - 操作失败
* @details:    	对指定的寄存器(8位地址)执行写操作，OV2640用到
******************************************************************************************************************************
**/
uint8_t sccb_write_handle(uint8_t addr)
{
	uint8_t status; // 状态标志位

//	sccb_start();										 // 启动IIC通信
	if (sccb_send_byte(OV2640_DEVICE_ADDRESS) == ACK_OK) // 写数据指令
	{
		if (sccb_send_byte((uint8_t)(addr)) != ACK_OK)
		{
			status = ERROR; // 操作失败
		}
	}
	status = SUCCESS; // 操作成功
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_reg
* @param:      	addr - 要写入的寄存器(8位地址)，
* @param:      	value - 要写入的数据
* @retval:     	SUCCESS - 操作成功， ERROR	  - 操作失败
* @details:    	对指定的寄存器(8位地址)写一字节数据，OV2640用到
******************************************************************************************************************************
**/
uint8_t sccb_write_reg(uint8_t addr, uint8_t value)
{
	uint8_t status;

	sccb_start(); // 启动IIC通讯

	if (sccb_write_handle(addr) == SUCCESS) // 写入要操作的寄存器
	{
		if (sccb_send_byte(value) != ACK_OK) // 写数据
		{
			status = ERROR;
		}
	}
	sccb_stop(); // 停止通讯

	status = SUCCESS; // 写入成功
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_read_reg
* @param:      	addr - 要读取的寄存器(8位地址)
* @retval:     	读到的数据
* @details:    	对指定的寄存器(8位地址)读取一字节数据，OV2640用到
******************************************************************************************************************************
**/
uint8_t sccb_read_reg(uint8_t addr)
{
	uint8_t value = 0;

	sccb_start(); // 启动IIC通信

	if (sccb_write_handle(addr) == SUCCESS) // 写入要操作的寄存器
	{
		sccb_stop();  // 停止IIC通信
		sccb_start(); // 重新启动IIC通讯

		if (sccb_send_byte(OV2640_DEVICE_ADDRESS | 0X01) == ACK_OK) // 发送读命令
		{
			value = sccb_read_byte(0); // 读到最后一个数据时发送 非应答信号
		}
		sccb_stop(); // 停止IIC通信
	}

	return value;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_handle_16Bit
* @param:      	addr - 要进行操作的寄存器(16位地址)
* @retval:     	SUCCESS - 操作成功
* @retval:			ERROR - 操作失败
* @details:    	对指定的寄存器(16位地址)执行写操作，OV5640用到
******************************************************************************************************************************
**/
uint8_t sccb_write_handle_16Bit(uint16_t addr)
{
	uint8_t status; // 状态标志位

//	sccb_start();										 // 启动IIC通信
	if (sccb_send_byte(OV5640_DEVICE_ADDRESS) == ACK_OK) // 写数据指令
	{
		if (sccb_send_byte((uint8_t)(addr >> 8)) == ACK_OK) // 写入16位地址
		{
			if (sccb_send_byte((uint8_t)(addr)) != ACK_OK)
			{
				status = ERROR; // 操作失败
			}
		}
	}
	status = SUCCESS; // 操作成功
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_reg_16bit
* @param:      	addr - 要写入的寄存器(16位地址)  
* @param:      	value - 要写入的数据
* @retval:     	SUCCESS - 操作成功
* @retval:     	ERROR	  - 操作失败
* @details:    	对指定的寄存器(16位地址)写一字节数据，OV5640用到
******************************************************************************************************************************
**/
uint8_t sccb_write_reg_16bit(uint16_t addr, uint8_t value)
{
	uint8_t status;

	sccb_start(); // 启动IIC通讯

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
	{
		if (sccb_send_byte(value) != ACK_OK) // 写数据
		{
			status = ERROR;
		}
	}
	sccb_stop(); // 停止通讯

	status = SUCCESS; // 写入成功
	return status;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_read_reg_16bit
* @param:      	addr - 要读取的寄存器(16位地址)
* @retval:     	读到的数据
* @details:    	对指定的寄存器(16位地址)读取一字节数据，OV5640用到
******************************************************************************************************************************
**/
uint8_t sccb_read_reg_16bit(uint16_t addr)
{
	uint8_t value = 0;

	sccb_start(); // 启动IIC通信

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
	{
		sccb_stop();  // 停止IIC通信
		sccb_start(); // 重新启动IIC通讯

		if (sccb_send_byte(OV5640_DEVICE_ADDRESS | 0X01) == ACK_OK) // 发送读命令
		{
			value = sccb_read_byte(0); // 读到最后一个数据时发送 非应答信号
		}
		sccb_stop(); // 停止IIC通信
	}

	return value;
}
/**
******************************************************************************************************************************
* @brief:      	sccb_write_buf_16bit
* @param:      	addr - 要写入的寄存器(16位地址)  
* @param:      	*pData - 数据区  
* @param:      	size - 要传输数据的大小
* @retval:     	SUCCESS - 操作成功
* @retval:     	ERROR	  - 操作失败
* @details:    	对指定的寄存器(16位地址)批量写数据，OV5640 写入自动对焦固件时用到
******************************************************************************************************************************
**/
uint8_t sccb_write_buf_16bit(uint16_t addr, uint8_t *pData, uint32_t size)
{
	uint8_t status;
	uint32_t i;

	sccb_start(); // 启动IIC通讯

	if (sccb_write_handle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
	{
		for (i = 0; i < size; i++)
		{
			sccb_send_byte(*pData); // 写数据
			pData++;
		}
	}
	sccb_stop(); // 停止通讯

	status = SUCCESS; // 写入成功
	return status;
}
