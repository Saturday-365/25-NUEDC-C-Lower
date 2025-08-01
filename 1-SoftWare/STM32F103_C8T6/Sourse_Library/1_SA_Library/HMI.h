#ifndef __HMI_H
#define __HMI_H

#include "main.h"
#include <stdio.h>
/* 结构体类型声明 */
typedef struct {
  int y_distance;              // 目标物距离（mm）
  int x_length;                // 几何图形尺寸（边长或者直径）（mm）
  int square_num;
  int square_length_min;       // 最小正方形边长（mm）
  int aim_square_num;          // 目标正方形编号
  int tar_square_num;          // 最大正方形编号

  int current_rlt;        // 供电实时电流（mA）
  int current_avg;             // 供电平均电流（mA）
  int current_max;             // 最大电流（mW）

  int power_rlt;          // 实时功耗（mW）
  int power_avg;               // 平均功耗（mW）
  int power_max;               // 最大功耗（mW）
  int parameters_k;
  int parameters_b;
} Info_t;

#define HMI_KEY_LIST { HMI_BTN1,HMI_BTN2,HMI_UP,HMI_DOWN,HMI_UP2,HMI_DOWN2}
#define HMI_key_num 6
typedef enum {
  HMI_BTN1,
  HMI_BTN2,
  HMI_UP,
  HMI_DOWN,  
  HMI_UP2,
  HMI_DOWN2,
} HMI_key_index_enum; // 按键索引 对应上方定义的按键引脚个数 默认定义四个按键

typedef enum {
    HMI_KEY_PRESS=1,      // 短按保持（新状态）
    HMI_KEY_RELEASE=0,
} HMI_key_state_enum;

/* 声明全局变量（extern表示变量在其他文件中定义） */
extern Info_t Info;

void HMI_Send_EveryInfo(void);
void HMI_send_string(char *name, char *showdata);
void HMI_send_number(char *name, int num);
void HMI_send_float(char *name, float num);
void HMI_VOFA(char *name, float num);
void HMI_key_scanner(float data);
HMI_key_state_enum HMI_key_get_state(HMI_key_index_enum key_n);
void HMI_key_clear_state(HMI_key_index_enum key_n);

#endif /* __USART1_H */
