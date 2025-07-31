/**
  ******************************************************************************
  * @file    bsp_Usart.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-05-26
  * @brief   USARTӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  ******************************************************************************
  * @cfg	
	*			USART1  TX - PA9  RX - PA10
  ******************************************************************************
  */
	
#include "stm32f4xx.h"                  // Device header
#include "bsp_Usart.h"


/**************************************************************************
�������    ���ڷ����ַ�
����˵��    USART_TypeDef* USARTx	ָ�� USART ����
����˵��    uint16_t Data					Ҫ���͵�����
���ز���    ��
ʹ��ʾ��    Usart_SendByte(USART1,'a');
��ע��Ϣ    ��
**************************************************************************/
void Usart_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
	USART_SendData(USARTx,Data);
	
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
}
/**************************************************************************
�������    ���ڷ����ַ���
����˵��    USART_TypeDef* USARTx	ָ�� USART ����
����˵��    char *str							Ҫ���͵��ַ���
���ز���    ��
ʹ��ʾ��    Usart_SendString(USART1,"abc");
��ע��Ϣ    ��
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
�������    NVIC ���ú���
����˵��    ��
���ز���    ��
ʹ��ʾ��    NVIC_Configuration();
��ע��Ϣ    NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����
						��ռ���ȼ���Ϊ2
						�����ȼ���Ϊ1
						USART�ж����ȼ�����ͬ��Ϊ��Ȼ���ȼ�
**************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* �����ж�Դ��USART1 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* �����ж�Դ��USART2 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* �����ж�Դ��USART3 */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
}
/* ���ڴ��ͨ��DMAʹ��USART1���͵����� */
uint8_t SendBuff[SENDBUFF_SIZE];
/**************************************************************************
�������    USART1_DMA��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    USART1_DMA_Init();
��ע��Ϣ    USART1 TX��ӦDMA2��ͨ��4��������7
						USART1 �����USART_BASE ƫ��0x04
						���ڽ�����FIFO FIFO��ֵ���洢��ͻ�����䡢����ͻ���������þ���Ч

						USART1 �� DMA����TX����:
						USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
**************************************************************************/
void USART1_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  /* ����DMAʱ�� */
	RCC_AHB1PeriphClockCmd(USART1_DMA_CLK, ENABLE);
  /* ��λ��ʼ��DMA������ */
	DMA_DeInit(USART1_DMA_STREAM);
  /* ȷ��DMA��������λ��� */
	while(DMA_GetCmdStatus(USART1_DMA_STREAM) != RESET);
	
  /* usart1 tx��Ӧdma2��ͨ��4��������7 */	
	DMA_InitStructure.DMA_Channel							=		USART1_DMA_CHANNEL;
  /* �ڴ��ַ(Ҫ����ı�����ָ��) */
	DMA_InitStructure.DMA_Memory0BaseAddr			=		(u32)SendBuff;
  /* ����DMAԴ���������ݼĴ�����ַ */
	DMA_InitStructure.DMA_PeripheralBaseAddr	=		USART1_DR_BASE;
  /*���򣺴��ڴ浽����*/		
	DMA_InitStructure.DMA_DIR									=		DMA_DIR_MemoryToPeripheral;
  /* �����СDMA_BufferSize=SENDBUFF_SIZE */	
  DMA_InitStructure.DMA_BufferSize 					= 	SENDBUFF_SIZE;
  /* �����ַ���� */	    
  DMA_InitStructure.DMA_PeripheralInc 			= 	DMA_PeripheralInc_Disable; 
  /* �ڴ��ַ���� */
  DMA_InitStructure.DMA_MemoryInc 					= 	DMA_MemoryInc_Enable;	
  /* �������ݵ�λ */	
	DMA_InitStructure.DMA_PeripheralDataSize	=		DMA_PeripheralDataSize_Byte;
	/* �ڴ����ݵ�λ 8bit */
	DMA_InitStructure.DMA_MemoryDataSize			=		DMA_MemoryDataSize_Byte;
	/* DMAģʽ��һ��ѭ�� */
	DMA_InitStructure.DMA_Mode								=		DMA_Mode_Normal;
//	DMA_InitStructure.DMA_Mode								=		DMA_Mode_Circular;
  /* ���ȼ����� */	
	DMA_InitStructure.DMA_Priority						= 	DMA_Priority_Medium;
	
  /* ����FIFO */
	DMA_InitStructure.DMA_FIFOMode						=		DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold				=		DMA_FIFOThreshold_Full;
  /* �洢��ͻ������ 16������ */
	DMA_InitStructure.DMA_MemoryBurst					= 	DMA_MemoryBurst_Single;
  /* ����ͻ������ 1������ */
	DMA_InitStructure.DMA_PeripheralBurst			=		DMA_PeripheralBurst_Single;  
	
  /* ����DMA2��������7 */		   
	DMA_Init(USART1_DMA_STREAM, &DMA_InitStructure);
  /* ʹ��DMA */
	DMA_Cmd(USART1_DMA_STREAM, ENABLE);
	/* �ȴ�DMA��������Ч*/
  while(DMA_GetCmdStatus(USART1_DMA_STREAM) != ENABLE);
	
}
/**************************************************************************
�������    USART��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    Usart_Init();
��ע��Ϣ    ʹ��USART_DMAʱҪ�رմ����жϣ�����ԭ��δ֪�������ǽ����жϺ�DMA
						�������¡�
**************************************************************************/
void Usart_Init(void)
{
	/* USART1��ʼ�� */
	USART1_Init();
	/* USART2��ʼ�� */
	USART2_Init();
	/* USART3��ʼ�� */
	USART3_Init();
	/* USART1_DMA��ʼ�� */
//	USART1_DMA_Init();
	
//	printf("\r\n Usart Init Over \r\n");
}
/**************************************************************************
�������    USART1��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    USART1_Init();
��ע��Ϣ    ��
**************************************************************************/
void USART1_Init(void)
{
	/* ��ʼ�������õ���GPIO */
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
	
	/* ��ʼ������ */
	RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate						=	USART1_BAUDRATE;
	USART_InitStructure.USART_Mode								=	USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity							=	USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits						=	USART_StopBits_1;
	USART_InitStructure.USART_WordLength					=	USART_WordLength_8b;
	
	USART_Init(USART_1, &USART_InitStructure);
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART_1, USART_IT_RXNE, ENABLE);
	
	/* �ж����� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��� */
	USART_Cmd(USART_1, ENABLE);
	
}

/**************************************************************************
�������    USART2��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    USART2_Init();
��ע��Ϣ    ��
**************************************************************************/
void USART2_Init(void)
{
	/* ��ʼ�������õ���GPIO */
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
�������    USART3��ʼ��
����˵��    ��
���ز���    ��
ʹ��ʾ��    USART3_Init();
��ע��Ϣ    ��
**************************************************************************/
void USART3_Init(void)
{
	/* ��ʼ�������õ���GPIO */
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
	
	/* ��ʼ������ */
	RCC_APB1PeriphClockCmd(USART3_CLK, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate						=	USART3_BAUDRATE;
	USART_InitStructure.USART_Mode								=	USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity							=	USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits						=	USART_StopBits_1;
	USART_InitStructure.USART_WordLength					=	USART_WordLength_8b;
	
	USART_Init(USART_3, &USART_InitStructure);
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART_3, USART_IT_RXNE, ENABLE);
	
	/* �ж����� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��� */
	USART_Cmd(USART_3, ENABLE);
	
}

/**************************************************************************
�������    �ض���C�⺯�����ض�����ʹ��printf�Ⱥ���
����˵��    ��
���ز���    ��
ʹ��ʾ��    ��
��ע��Ϣ    �û��������
**************************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(USART_1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART_1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART_1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_1);
}
