/**
  ******************************************************************************
  * @file    HMI.c
  * @author  SuperLi
  * @version V1.0
  * @date    2025-07-30
  * @brief   HMIӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  ******************************************************************************
  */
	
#include "main.h"                  // Device header
#include "SA_Usart.h"
#include "HMI.h"
/*
*/

///* ��λΪmm */
//struct Info_e
//{
//	int Distance;
//	int Length;
//	int Elecurrent;
//	
//	int Length_Min;
//	int P_current;
//	int P_Max;
//		
//}Info;
/* ��λ:mm */
Info_t Info = {
    .Distance 	= 10,
    .Length 		= 50,
    .Elecurrent = 30,
    .Length_Min = 0,
    .P_current 	= 0,
    .P_Max 			= 0,
    .Elecurrent_Avg=0
};
/**************************************************************************
�������    ��ʾ���в���
����˵��    ��
���ز���    ��
ʹ��ʾ��    HMI_Send_EveryInfo();
��ע��Ϣ    
						D Ŀ�������			-->		"Num_d.val"		-->		Info.Distance
						X ����ͼ�γߴ�		-->		"Num_x.val"		-->		Info.Length
						I �������				-->		"Num_I.val"		-->		Info.Elecurrent
**************************************************************************/
void HMI_Send_EveryInfo(void)
{
	/* D Ŀ������� */
	HMI_send_number("Num_d.val",Info.Distance);
	/* X ����ͼ�γߴ� */
	HMI_send_number("Num_x.val",Info.Length);
	/* I ������� */
	HMI_send_number("Pre_I.val",Info.Elecurrent);
    
	HMI_send_number("Avg_I.val",Info.Elecurrent_Avg);
    
 	HMI_send_number("Pwr.val",Info.Elecurrent*5);
   HMI_VOFA("vofa",Info.Elecurrent);


}

void HMI_send_string(char* name, char* showdata)
{
	printf("%s=\"%s\"\xff\xff\xff", name, showdata);
}
void HMI_send_number(char* name, int num)
{
	printf("%s=%d\xff\xff\xff", name, num);
}
void HMI_send_float(char* name, float num)
{
	printf("%s=%d\xff\xff\xff", name, (int)(num));
}
void HMI_VOFA(char* name, float num)
{
    printf("add %s.id,0,%d\xff\xff\xff",name, (int)(num));
}

void HMI_GOtoPage(char* name)
{
    printf("page %s\xff\xff\xff",name);

}

/*-----------------------------------------------------------------------
 if (USART_RX_BUF[0] == '1') // 0x31
            {
                HMI_send_string("t0.txt", "test_num");
                // printf("t0.txt=\"%d\"\xff\xff\xff", test_num);
            }
            if (USART_RX_BUF[0] == '2') // 0x32
            {
                test_float = test_float + 0.3;
                HMI_send_float("x0.val", test_float);
                // printf("t0.txt=\"%d\"\xff\xff\xff", test_num);
            }
            if (USART_RX_BUF[0] == '3') // 0x33
            {
                test_num++;
                HMI_send_number("n0.val", test_num);
                // printf("t0.txt=\"%d\"\xff\xff\xff", test_num);
            }
-----------------------------------------------------------------------*/
