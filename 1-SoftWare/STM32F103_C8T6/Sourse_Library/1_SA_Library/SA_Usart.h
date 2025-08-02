#ifndef __SA_USART_H
#define __SA_USART_H
#include "main.h"

 #define LENGTH 10     //接受缓冲区大小
 
float Get_Data(uint8_t *pData);
void HMI_Uart_DMA_RX_Init(void);
uint8_t Report_stage(void);
//void Vofa_PID_Adjust(void);
void JustFloat_4(float data1,float data2,float data3,float data4);
void JustFloat_8(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8);
void JustFloat_5(float data1,float data2,float data3,float data4,float data5);
void JustFloat_16_rs232(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
                        float data9,float data10,float data11,float data12,float data13,float data14,float data15,float data16);
void JustFloat_16(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
                  float data9,float data10,float data11,float data12,float data13,float data14,float data15,float data16);
void JustFloat_10_rs232(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10);
void JustFloat_10(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10);
void Serial_SendString(char *String);
void Serial_Printf(char *format, ...);

#endif
