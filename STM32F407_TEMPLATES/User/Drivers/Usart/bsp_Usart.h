#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


//UART相关引脚宏定义
/*******************************************************/
#define USART_1					               		USART1
#define USART1_CLK                        RCC_APB2Periph_USART1
#define USART1_BAUDRATE                   115200  //串口波特率
						 
#define USART1_RX_GPIO_PORT               GPIOA
#define USART1_RX_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define USART1_RX_PIN                     GPIO_Pin_10
#define USART1_RX_AF                      GPIO_AF_USART1
#define USART1_RX_SOURCE                  GPIO_PinSource10
						 
#define USART1_TX_GPIO_PORT               GPIOA
#define USART1_TX_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define USART1_TX_PIN                     GPIO_Pin_9
#define USART1_TX_AF                      GPIO_AF_USART1
#define USART1_TX_SOURCE                  GPIO_PinSource9
						 
#define USART1_IRQHandler                 USART1_IRQHandler
#define USART1_IRQ                 				USART1_IRQn
/************************************************************/
#define USART_2					               		USART2
#define USART2_CLK                        RCC_APB1Periph_USART2
#define USART2_BAUDRATE                   115200  //串口波特率
						 
#define USART2_RX_GPIO_PORT               GPIOA
#define USART2_RX_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define USART2_RX_PIN                     GPIO_Pin_3
#define USART2_RX_AF                      GPIO_AF_USART2
#define USART2_RX_SOURCE                  GPIO_PinSource3
						 
#define USART2_TX_GPIO_PORT               GPIOA
#define USART2_TX_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define USART2_TX_PIN                     GPIO_Pin_2
#define USART2_TX_AF                      GPIO_AF_USART2
#define USART2_TX_SOURCE                  GPIO_PinSource2
						 
#define USART2_IRQHandler                 USART2_IRQHandler
#define USART2_IRQ                 				USART2_IRQn
/************************************************************/
#define USART_3					               		USART3
#define USART3_CLK                        RCC_APB1Periph_USART3
#define USART3_BAUDRATE                   115200  //串口波特率
						 
#define USART3_RX_GPIO_PORT               GPIOB
#define USART3_RX_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define USART3_RX_PIN                     GPIO_Pin_11
#define USART3_RX_AF                      GPIO_AF_USART3
#define USART3_RX_SOURCE                  GPIO_PinSource11
						 
#define USART3_TX_GPIO_PORT               GPIOB
#define USART3_TX_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define USART3_TX_PIN                     GPIO_Pin_10
#define USART3_TX_AF                      GPIO_AF_USART3
#define USART3_TX_SOURCE                  GPIO_PinSource10
						 
#define USART3_IRQHandler                 USART3_IRQHandler
#define USART3_IRQ                 				USART3_IRQn
/************************************************************/
/* USART1_DMA相关宏定义*/
#define USART1_DR_BASE               			(USART1_BASE+0x04)		
#define SENDBUFF_SIZE                     5				//发送的数据量
#define USART1_DMA_CLK               			RCC_AHB1Periph_DMA2	
#define USART1_DMA_CHANNEL           			DMA_Channel_4
#define USART1_DMA_STREAM            			DMA2_Stream7
/************************************************************/

void Usart_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void Usart_SendString(USART_TypeDef* USARTx, char *str);

extern uint8_t SendBuff[SENDBUFF_SIZE];

void USART1_DMA_Init(void);
void Usart_Init(void);
void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);

/* 重定向 */
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif /* __USART1_H */
