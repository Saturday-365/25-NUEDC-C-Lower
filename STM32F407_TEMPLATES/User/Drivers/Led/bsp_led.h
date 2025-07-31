#ifndef __BSP_LED_H
#define	__BSP_LED_H

#include "stm32f4xx.h"

//引脚定义
/*******************************************************/
//R 红色灯
#define LED1_PIN                  GPIO_Pin_6                 
#define LED1_GPIO_PORT            GPIOF                      
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOF

//G 绿色灯
#define LED2_PIN                  GPIO_Pin_7                 
#define LED2_GPIO_PORT            GPIOF                      
#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOF

//B 蓝色灯
#define LED3_PIN                  GPIO_Pin_8                 
#define LED3_GPIO_PORT            GPIOF                       
#define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOF
/************************************************************/

void LED_OFF(void);
void LED_Darkblue(void);
void LED_Green(void);
void LED_Blue(void);
void LED_Red(void);
void LED_Purple(void);
void LED_Yellow(void);
void LED_White(void);

void LED_Init(void);


#endif /* __BSP_LED_H */
