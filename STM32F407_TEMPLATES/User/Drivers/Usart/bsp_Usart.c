/**
  ******************************************************************************
  * @file    bsp_Usart.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-26
  * @brief   USART应用函数接口
  ******************************************************************************
  * @attention
  * 实验平台:野火  STM32 F407 开发板  
  ******************************************************************************
  * @cfg	
	*			USART1  TX - PA9  RX - PA10
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_Usart.h"


/**************************************************************************
函数简介    串口发送字符
参数说明    USART_TypeDef* USARTx	指定 USART 发送
参数说明    uint16_t Data					要发送的数据
返回参数    无
使用示例    Usart_SendByte(USART1,'a');
备注信息    无
**************************************************************************/
void Usart_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	USART_SendData(USARTx,Data);
	
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
}
/**************************************************************************
函数简介    串口发送字符串
参数说明    USART_TypeDef* USARTx	指定 USART 发送
参数说明    char *str							要发送的字符串
返回参数    无
使用示例    Usart_SendString(USART1,"abc");
备注信息    无
**************************************************************************/ 
void Usart_SendString(USART_TypeDef* USARTx, char *str)
{
	uint16_t k;
	do
	{
		USART_SendData(USARTx, * (str+k) );
		k++;
	}while(* (str+k) != '\0');
	
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
}

/**************************************************************************
函数简介    NVIC 配置函数
参数说明    无
返回参数    无
使用示例    NVIC_Configuration();
备注信息    NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组
						抢占优先级设为2
						子优先级设为1
						USART中断优先级均相同，为自然优先级
**************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置中断源：USART1 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 配置抢占优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* 配置中断源：USART2 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* 配置抢占优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* 配置中断源：USART3 */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  /* 配置抢占优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级：1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
}
/* 用于存放通过DMA使用USART1发送的数据 */
uint8_t SendBuff[SENDBUFF_SIZE];
/**************************************************************************
函数简介    USART1_DMA初始化
参数说明    无
返回参数    无
使用示例    USART1_DMA_Init();
备注信息    USART1 TX对应DMA2，通道4，数据流7
						USART1 相对于USART_BASE 偏移0x04
						由于禁用了FIFO FIFO阈值、存储器突发传输、外设突发传输配置均无效

						USART1 向 DMA发出TX请求:
						USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
**************************************************************************/
void USART1_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  /* 开启DMA时钟 */
	RCC_AHB1PeriphClockCmd(USART1_DMA_CLK, ENABLE);
  /* 复位初始化DMA数据流 */
	DMA_DeInit(USART1_DMA_STREAM);
  /* 确保DMA数据流复位完成 */
	while(DMA_GetCmdStatus(USART1_DMA_STREAM) != RESET);
	
  /* usart1 tx对应dma2，通道4，数据流7 */	
	DMA_InitStructure.DMA_Channel							=		USART1_DMA_CHANNEL;
  /* 内存地址(要传输的变量的指针) */
	DMA_InitStructure.DMA_Memory0BaseAddr			=		(u32)SendBuff;
  /* 设置DMA源：串口数据寄存器地址 */
	DMA_InitStructure.DMA_PeripheralBaseAddr	=		USART1_DR_BASE;
  /*方向：从内存到外设*/		
	DMA_InitStructure.DMA_DIR									=		DMA_DIR_MemoryToPeripheral;
  /* 传输大小DMA_BufferSize=SENDBUFF_SIZE */	
  DMA_InitStructure.DMA_BufferSize 					= 	SENDBUFF_SIZE;
  /* 外设地址不增 */	    
  DMA_InitStructure.DMA_PeripheralInc 			= 	DMA_PeripheralInc_Disable; 
  /* 内存地址自增 */
  DMA_InitStructure.DMA_MemoryInc 					= 	DMA_MemoryInc_Enable;	
  /* 外设数据单位 */	
	DMA_InitStructure.DMA_PeripheralDataSize	=		DMA_PeripheralDataSize_Byte;
	/* 内存数据单位 8bit */
	DMA_InitStructure.DMA_MemoryDataSize			=		DMA_MemoryDataSize_Byte;
	/* DMA模式：一次循环 */
	DMA_InitStructure.DMA_Mode								=		DMA_Mode_Normal;
//	DMA_InitStructure.DMA_Mode								=		DMA_Mode_Circular;
  /* 优先级：中 */	
	DMA_InitStructure.DMA_Priority						= 	DMA_Priority_Medium;
	
  /* 禁用FIFO */
	DMA_InitStructure.DMA_FIFOMode						=		DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold				=		DMA_FIFOThreshold_Full;
  /* 存储器突发传输 16个节拍 */
	DMA_InitStructure.DMA_MemoryBurst					= 	DMA_MemoryBurst_Single;
  /* 外设突发传输 1个节拍 */
	DMA_InitStructure.DMA_PeripheralBurst			=		DMA_PeripheralBurst_Single;  
	
  /* 配置DMA2的数据流7 */		   
	DMA_Init(USART1_DMA_STREAM, &DMA_InitStructure);
  /* 使能DMA */
	DMA_Cmd(USART1_DMA_STREAM, ENABLE);
	/* 等待DMA数据流有效*/
  while(DMA_GetCmdStatus(USART1_DMA_STREAM) != ENABLE);
	
}
/**************************************************************************
函数简介    USART初始化
参数说明    无
返回参数    无
使用示例    Usart_Init();
备注信息    使用USART_DMA时要关闭串口中断，具体原因未知，可能是接收中断和DMA
						竞争导致。
**************************************************************************/
void Usart_Init(void)
{
	/* USART1初始化 */
	USART1_Init();
	/* USART2初始化 */
	USART2_Init();
	/* USART3初始化 */
	USART3_Init();
	/* USART1_DMA初始化 */
//	USART1_DMA_Init();
	
//	printf("\r\n Usart Init Over \r\n");
}
/**************************************************************************
函数简介    USART1初始化
参数说明    无
返回参数    无
使用示例    USART1_Init();
备注信息    无
**************************************************************************/
void USART1_Init(void)
{
	/* 初始化串口用到的GPIO */
	RCC_AHB1PeriphClockCmd(USART1_RX_GPIO_CLK|USART1_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Fast_Speed;

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	USART1_RX_PIN;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	USART1_TX_PIN;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART1_RX_GPIO_PORT,USART1_RX_SOURCE,USART1_RX_AF);
	GPIO_PinAFConfig(USART1_TX_GPIO_PORT,USART1_TX_SOURCE,USART1_TX_AF);
	
	/* 初始化串口 */
	RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate						=	USART1_BAUDRATE;
	USART_InitStructure.USART_Mode								=	USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity							=	USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits						=	USART_StopBits_1;
	USART_InitStructure.USART_WordLength					=	USART_WordLength_8b;
	
	USART_Init(USART_1, &USART_InitStructure);
	
	/* 使能串口接收中断 */
	USART_ITConfig(USART_1, USART_IT_RXNE, ENABLE);
	
	/* 中断配置 */
	NVIC_Configuration();
	
	/* 使能串口 */
	USART_Cmd(USART_1, ENABLE);
	
}

/**************************************************************************
函数简介    USART2初始化
参数说明    无
返回参数    无
使用示例    USART2_Init();
备注信息    无
**************************************************************************/
void USART2_Init(void)
{
	/* 初始化串口用到的GPIO */
	RCC_AHB1PeriphClockCmd(USART2_RX_GPIO_CLK|USART2_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_OType		= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Fast_Speed;
		                               
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin			= USART2_RX_PIN;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin			= USART2_TX_PIN;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART2_RX_GPIO_PORT, USART2_RX_SOURCE, GPIO_AF_USART2);
	GPIO_PinAFConfig(USART2_TX_GPIO_PORT, USART2_TX_SOURCE, GPIO_AF_USART2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate						=	USART2_BAUDRATE;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode								=	USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity							=	USART_Parity_No;
	USART_InitStructure.USART_StopBits						=	USART_StopBits_1;
	USART_InitStructure.USART_WordLength					=	USART_WordLength_8b;
	
	USART_Init(USART_2, &USART_InitStructure);
	
	USART_ITConfig(USART_2, USART_IT_RXNE, ENABLE);
	
	NVIC_Configuration();
	
	USART_Cmd(USART_2, ENABLE);
}
/**************************************************************************
函数简介    USART3初始化
参数说明    无
返回参数    无
使用示例    USART3_Init();
备注信息    无
**************************************************************************/
void USART3_Init(void)
{
	/* 初始化串口用到的GPIO */
	RCC_AHB1PeriphClockCmd(USART3_RX_GPIO_CLK|USART3_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Fast_Speed;

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	USART3_RX_PIN;
	GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		=	USART3_TX_PIN;
	GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART3_RX_GPIO_PORT,USART3_RX_SOURCE,USART3_RX_AF);
	GPIO_PinAFConfig(USART3_TX_GPIO_PORT,USART3_TX_SOURCE,USART3_TX_AF);
	
	/* 初始化串口 */
	RCC_APB1PeriphClockCmd(USART3_CLK, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate						=	USART3_BAUDRATE;
	USART_InitStructure.USART_Mode								=	USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity							=	USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits						=	USART_StopBits_1;
	USART_InitStructure.USART_WordLength					=	USART_WordLength_8b;
	
	USART_Init(USART_3, &USART_InitStructure);
	
	/* 使能串口接收中断 */
	USART_ITConfig(USART_3, USART_IT_RXNE, ENABLE);
	
	/* 中断配置 */
	NVIC_Configuration();
	
	/* 使能串口 */
	USART_Cmd(USART_3, ENABLE);
	
}

/**************************************************************************
函数简介    重定向C库函数，重定向后可使用printf等函数
参数说明    无
返回参数    无
使用示例    无
备注信息    用户无需关心
**************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(USART_1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART_1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART_1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_1);
}
