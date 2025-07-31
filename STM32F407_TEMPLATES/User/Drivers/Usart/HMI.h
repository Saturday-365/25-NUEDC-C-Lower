#ifndef __HMI_H
#define	__HMI_H

#include "stm32f4xx.h"
#include <stdio.h>
/* 结构体类型声明 */
typedef struct {
    int Distance;     // 目标物距离（mm）
    int Length;       // 几何图形尺寸（mm）
    int Elecurrent;   // 供电电流（mA）
    int Length_Min;   // 最小正方形边长（mm）
    int P_current;    // 实时功耗（mW）
    int P_Max;        // 最大功耗（mW）
} Info_t;

/* 声明全局变量（extern表示变量在其他文件中定义） */
extern Info_t Info;

void HMI_Send_EveryInfo(void);
void HMI_send_string(char* name, char* showdata);
void HMI_send_number(char* name, int num);
void HMI_send_float(char* name, float num);
#endif /* __USART1_H */
