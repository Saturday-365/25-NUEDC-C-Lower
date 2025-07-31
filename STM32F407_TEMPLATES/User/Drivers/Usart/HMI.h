#ifndef __HMI_H
#define	__HMI_H

#include "stm32f4xx.h"
#include <stdio.h>
/* �ṹ���������� */
typedef struct {
    int Distance;     // Ŀ������루mm��
    int Length;       // ����ͼ�γߴ磨mm��
    int Elecurrent;   // ���������mA��
    int Length_Min;   // ��С�����α߳���mm��
    int P_current;    // ʵʱ���ģ�mW��
    int P_Max;        // ��󹦺ģ�mW��
} Info_t;

/* ����ȫ�ֱ�����extern��ʾ�����������ļ��ж��壩 */
extern Info_t Info;

void HMI_Send_EveryInfo(void);
void HMI_send_string(char* name, char* showdata);
void HMI_send_number(char* name, int num);
void HMI_send_float(char* name, float num);
#endif /* __USART1_H */
