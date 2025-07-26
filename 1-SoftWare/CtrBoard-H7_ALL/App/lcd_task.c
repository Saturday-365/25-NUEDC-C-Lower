#include "lcd_task.h"
#include "cmsis_os.h"
#include "lcd.h"
#include "pic.h"

extern float vbus;
/* USER CODE BEGIN Header_LcdTask_Entry */
/**
* @brief Function implementing the LcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LcdTask_Entry */
void LcdTask_Entry(void const * argument)
{
    /* USER CODE BEGIN LcdTask_Entry */
    LCD_Init();//LCD初始化
    LCD_Fill(0,0,LCD_W, LCD_H,BLACK);
    
    /* Infinite loop */
    for(;;)
    {
        LCD_ShowString(120, 72,(uint8_t *)"dmBot", BRRED, BLACK, 24, 0);
        LCD_ShowChinese(84, 100, (uint8_t *)"达妙科技", WHITE, BLACK, 32, 0);
        LCD_DrawLine(10, 0, 10,  280,WHITE);
        LCD_DrawLine(270,0, 270, 280,WHITE);
//        LCD_ShowIntNum(50, 170, adc_val[0], 5, WHITE, BLACK, 32);
        LCD_ShowFloatNum(50, 170, vbus, 3, 2, WHITE, BLACK, 32);
        LCD_ShowPicture(180, 150, 40, 40, gImage_1);
        osDelay(10);
    }
    /* USER CODE END LcdTask_Entry */
}



