#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H

/* 蜂鸣器相关接口宏定义 */
#define Buzzer_PIN    GPIO_Pin_7
#define Buzzer_PORT   GPIOG
#define Buzzer_CLK    RCC_AHB1Periph_GPIOG

void Buzzer_ON(void);
void Buzzer_OFF(void);

void Buzzer_Init(void);

#endif
