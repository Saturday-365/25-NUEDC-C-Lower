#ifndef __BSP_KEY_H
#define __BSP_KEY_H

/* ������ؽӿں궨�� */
#define KEY_UP_PIN    GPIO_Pin_11
#define KEY_UP_PORT   GPIOD
#define KEY_UP_CLK    RCC_AHB1Periph_GPIOD

#define KEY_YES_PIN    GPIO_Pin_12
#define KEY_YES_PORT   GPIOD
#define KEY_YES_CLK    RCC_AHB1Periph_GPIOD

#define KEY_LEFT_PIN    GPIO_Pin_13
#define KEY_LEFT_PORT   GPIOD
#define KEY_LEFT_CLK    RCC_AHB1Periph_GPIOD

#define KEY_DOWN_PIN    GPIO_Pin_14
#define KEY_DOWN_PORT   GPIOD
#define KEY_DOWN_CLK    RCC_AHB1Periph_GPIOD

#define KEY_RIGHT_PIN    GPIO_Pin_15
#define KEY_RIGHT_PORT   GPIOD
#define KEY_RIGHT_CLK    RCC_AHB1Periph_GPIOD

#define KEY_PORT		GPIOD
#define KEY_CLK			RCC_AHB1Periph_GPIOD

/* ��������궨�� */
#define KEY_LEFT		3
#define KEY_RIGHT		5
#define KEY_UP			1
#define KEY_DOWN		4
#define KEY_YES			2



/* ��ȡ�������ؼ�ֵ */
uint8_t Key_GetNum(void);
void KEY_TEST(void);

/* ������ʼ�� */
void Key_Init(void);

#endif
