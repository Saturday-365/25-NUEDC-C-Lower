#include "HMI.h"
#include "SA_Usart.h"
#include "usart.h"
#include "main.h" // Device header
#include "string.h"
;static HMI_key_index_enum       HMI_key_index[HMI_key_num] = HMI_KEY_LIST;
static HMI_key_state_enum       HMI_key_state[HMI_key_num];                              // 按键状态

Info_t Info = {
               .y_distance = 0,
               .x_length = 0,
               .square_num  = 0,
                .square_area=0,

               .square_length_min = 0,
               .aim_square_num = 10,
               .tar_square_num = 0,

               .current_rlt = 0,
               .current_avg = 0,
               .current_max = 0,

               .power_rlt = 0,
               .power_avg = 0,
               .power_max = 0,
    
    .parameters_k= 0,
    .parameters_b= 0,
    .ina240=0,
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
  HMI_send_number("y_distance.val", (int)Info.y_distance);
  /* X 几何图形尺寸 */
  HMI_send_number("x_length.val", Info.x_length);
  /* N 正方形编号 */
  HMI_send_number("square_num.val", Info.square_num);
  /* I 实时电流 */  
  HMI_send_number("current_rlt.val", Info.current_rlt);
  /* I 平均电流 */  
  HMI_send_number("current_avg.val", Info.current_rlt);
  /* P 实时功率 */  
  HMI_send_number("power_rlt.val", Info.power_rlt);
  /* PM 最大功率 */  
  HMI_send_number("power_max.val", Info.power_max);
    
  HMI_send_number("ina240.val", Info.ina240);

  /* 电流显示波形 */
  HMI_VOFA("vofa", Info.current_rlt/10);
  HMI_send_number("square_area.val", Info.square_area/100);
    
  HMI_send_number("num_k.val", Info.parameters_k);
  HMI_send_number("num_b.val", Info.parameters_b);
  HMI_send_number("aim_squre_num.val",Info.aim_square_num);  

}

void clear_info(void){
       Info.y_distance = 0;
       Info.x_length = 0;
//       Info.aim_square_num = 10;
       Info.square_area=0;
}

uint8_t num[11]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
//char num16="10";
uint8_t usart3_flage=0;
//void send_aim_num(int data) {
//    if (usart3_flage==1) Serial_Printf("%d",data);
//}
void send_aim_num(int data) {
    if (usart3_flage==1) 
    {   
        if(data==0) HAL_UART_Transmit(&huart3,&num[0],1,2);
        else if(data==1) HAL_UART_Transmit(&huart3,&num[1],1,2);
        else if(data==2) HAL_UART_Transmit(&huart3,&num[2],1,2);
        else if(data==3) HAL_UART_Transmit(&huart3,&num[3],1,2);
        else if(data==4) HAL_UART_Transmit(&huart3,&num[4],1,2);
        else if(data==5) HAL_UART_Transmit(&huart3,&num[5],1,2);
        else if(data==6) HAL_UART_Transmit(&huart3,&num[6],1,2);
        else if(data==7) HAL_UART_Transmit(&huart3,&num[7],1,2);
        else if(data==8) HAL_UART_Transmit(&huart3,&num[8],1,2);
        else if(data==9) HAL_UART_Transmit(&huart3,&num[9],1,2);
        else if(data==10) HAL_UART_Transmit(&huart3,&num[10],1,2);
    }
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
void HMI_key_scanner(int HMI_key)
{
    if(HMI_key==0) HMI_key_state[0] =HMI_KEY_PRESS;  
    else if(HMI_key==1) HMI_key_state[1] =HMI_KEY_PRESS;        
    else if(HMI_key==2) HMI_key_state[2] =HMI_KEY_PRESS;        
    else if(HMI_key==3) HMI_key_state[3] =HMI_KEY_PRESS;        
    else if(HMI_key==4) HMI_key_state[4] =HMI_KEY_PRESS;  
    else if(HMI_key==5) HMI_key_state[5] =HMI_KEY_PRESS;
    else if(HMI_key==6) HMI_key_state[6] =HMI_KEY_PRESS;

}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取按键状态
// 参数说明     key_n           按键索引
// 返回参数     key_state_enum  按键状态
// 使用示例     key_get_state(KEY_1);
//-------------------------------------------------------------------------------------------------------------------
HMI_key_state_enum HMI_key_get_state(HMI_key_index_enum key_n)
{
    return HMI_key_state[key_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除对应按键状态
// 参数说明     key_n           按键索引
// 返回参数     void            无
// 使用示例     key_clear_state(KEY_1);
//-------------------------------------------------------------------------------------------------------------------
void HMI_key_clear_state(HMI_key_index_enum key_n)
{
        HMI_key_state[key_n] = HMI_KEY_RELEASE;
    
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
