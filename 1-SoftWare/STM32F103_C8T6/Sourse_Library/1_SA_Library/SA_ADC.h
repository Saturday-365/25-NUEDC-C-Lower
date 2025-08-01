#ifndef __SA_ADC_H
#define __SA_ADC_H
#include "main.h"
extern volatile uint16_t ADC_buffer[1];

// 电流数据处理结构体
typedef struct {
    float current_value;  // 实时值
    float current_value_filt;  // 实时值
    float max_value;      // 最大值
    float avg_value;      // 平均值
    float Correct_parameters_k;// 修正值
    float Correct_parameters_b;// 修正值

    uint32_t count;       // 已采集次数
    uint32_t total_count; // 设定的总采集次数
    float sum;            // 累加和，用于计算平均值
} Data_I_TypeDef;

void ADC_I_Init(Data_I_TypeDef *DATA,uint32_t total_count);
float Get_I_Data(void);
uint8_t Collect_CurrentData(Data_I_TypeDef *data);
void ADC_I_DATA(Data_I_TypeDef *DATA,uint8_t flage);

#endif
