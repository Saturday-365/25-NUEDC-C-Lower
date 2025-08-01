#include "HMI.h"
#include "SA_Usart.h"
#include "main.h" // Device header

Info_t Info = {
               .y_distance = 0,
               .x_length = 0,
               .square_num  = 0,

               .square_length_min = 0,
               .aim_square_num = 0,
               .tar_square_num = 0,

               .current_rlt = 0,
               .current_avg = 0,
               .current_max = 0,

               .power_rlt = 0,
               .power_avg = 0,
               .power_max = 0,
};
/**************************************************************************
函数简介    显示所有参数
参数说明    无
返回参数    无
使用示例    HMI_Send_EveryInfo();
备注信息
                                                D 目标物距离
-->		"Num_d.val"		-->		Info.Distance X
几何图形尺寸		-->		"Num_x.val"		-->
Info.Length I 供电电流				-->		"Num_I.val"
-->		Info.Elecurrent
**************************************************************************/
void HMI_Send_EveryInfo(void) {
  /* D 目标物距离 */
  HMI_send_number("y_distance.val", Info.y_distance);
  /* X 几何图形尺寸 */
  HMI_send_number("x_length.val", Info.x_length);
  /* N 正方形编号 */
  HMI_send_number("square_num.val", Info.square_num);
  /* I 实时电流 */  
  HMI_send_number("current_rlt.val", Info.current_rlt);
  /* I 平均电流 */  
  HMI_send_number("current_avg.val", Info.current_avg);
  /* P 实时功率 */  
  HMI_send_number("power_rlt.val", Info.power_rlt);
  /* PM 最大功率 */  
  HMI_send_number("power_max.val", Info.power_max);
  /* 电流显示波形 */ 
  HMI_VOFA("vofa", Info.current_rlt/10);
}

void HMI_send_string(char *name, char *showdata) {
  printf("%s=\"%s\"\xff\xff\xff", name, showdata);
}
void HMI_send_number(char *name, int num) {
  printf("%s=%d\xff\xff\xff", name, num);
}
void HMI_send_float(char *name, float num) {
  printf("%s=%d\xff\xff\xff", name, (int)(num));
}
void HMI_VOFA(char *name, float num) {
  printf("add %s.id,0,%d\xff\xff\xff", name, (int)(num));
}

void HMI_GOtoPage(char *name) { 
    printf("page %s\xff\xff\xff", name); 
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
