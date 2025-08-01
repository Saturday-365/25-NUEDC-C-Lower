#include "main.h"
#include "SA_ADC.h"
#include "SA_karman.h"
#include "adc.h"
#include "dma.h"
volatile uint16_t ADC_buffer[1]={0};
#define WINDOW_SIZE 100

static float window_buffer[WINDOW_SIZE] = {0};  // 滑动窗口缓冲区
uint8_t window_index = 0;     // 当前缓冲区索引
static uint8_t sample_count = 0;     // 已采样总数（用于初始化阶段）
static float window_sum = 0.0f;      // 窗口内数据总和

void ADC_I_Init(Data_I_TypeDef *DATA,uint32_t total_count){
    HAL_ADCEx_Calibration_Start(&hadc1);//ADC自动校准
    HAL_Delay(2000);//延时200ms
    DATA->max_value=0;
    DATA->avg_value=0;
    DATA->current_value=0;
    DATA->Correct_parameters=0;
    DATA->total_count=total_count;
    DATA->count=0;
    DATA->sum=0;
    DATA->current_value_filt=0;
}    


float Get_I_Data(void){
   HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_buffer,1);
   float ADC_Vol =(float) ADC_buffer[0]/(4096/(float)3.3818*40*(float)0.01); 
    ADC_Vol=0.8498*ADC_Vol - (28.0369/1000);
    if (ADC_Vol<=0) ADC_Vol=0;
    return ADC_Vol;
}
extern Kalman_Typedef Curreny_Kalman;

uint8_t Collect_CurrentData(Data_I_TypeDef *data) {
    
    // 获取当前电流值（调用现有函数）
    data->current_value=Get_I_Data();
    data->current_value_filt = KalmanFilter(&Curreny_Kalman,data->current_value);
    data->current_value_filt = data->current_value_filt*data->Correct_parameters;

    // 更新最大值
    if (sample_count == 0 ||data->current_value_filt > data->max_value) {
        data->max_value = data->current_value_filt;
    }
    
    //// 获取滑动窗口平均值
    if (sample_count < WINDOW_SIZE) {
        // 初始化阶段：直接累加
        window_sum += data->current_value_filt;
        window_buffer[window_index++] = data->current_value_filt;
    } else {
        // 窗口已满：先减去即将移除的值，再加新值
        window_sum -= window_buffer[window_index];
        window_sum += data->current_value_filt;
        window_buffer[window_index] = data->current_value_filt;
        window_index = (window_index + 1) % WINDOW_SIZE;  // 循环索引
    }
    if (sample_count<=WINDOW_SIZE)
    {
        sample_count++;  // 总采样数递增
        
    }
    else sample_count=WINDOW_SIZE;
    // 初始化阶段（未填满窗口）用实际采样数计算
    uint8_t actual_count = (sample_count <= WINDOW_SIZE) ? sample_count : WINDOW_SIZE;
    data->avg_value = window_sum / (actual_count+1);

    return 1; // 采集成功
}


