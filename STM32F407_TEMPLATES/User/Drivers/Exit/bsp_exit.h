#ifndef __BSP_EXIT_H
#define __BSP_EXIT_H

/* 外部中断接口宏定义 */
#define KEY1_INT_GPIO_PORT                GPIOD
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_11

#define KEY1_INT_EXTI_CLK                 RCC_APB2Periph_SYSCFG
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource11
#define KEY1_INT_EXTI_LINE                EXTI_Line11
#define KEY1_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define KEY1_IRQHandler                   EXTI15_10_IRQHandler

//#define KEY2_INT_GPIO_PORT                GPIOC
//#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOC
//#define KEY2_INT_GPIO_PIN                 GPIO_Pin_13
//					 
//#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC
//#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource13
//#define KEY2_INT_EXTI_LINE                EXTI_Line13
//#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn

//#define KEY2_IRQHandler                   EXTI15_10_IRQHandler




void EXTI_Key_Init(void);


#endif
