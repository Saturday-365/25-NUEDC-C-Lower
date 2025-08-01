//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 测试硬件：单片机STM32F407ZGT6,正点原子Explorer
// STM32F4开发板,主频168MHZ，晶振12MHZ QDtech-TFT液晶驱动 for STM32 IO模拟
// xiao冯@ShenZhen QDtech co.,LTD
// 公司网站:www.qdtft.com
// 淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
// 我司提供技术支持，任何技术问题欢迎随时交流学习
// 固话(传真) :+86 0755-23594567
// 手机:15989313508（冯工）
// 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// 技术支持QQ:3002773612  3002778157
// 技术交流QQ群:324828016
// 创建日期:2018/08/22
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接          3.3V         //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI总线
//     LCD模块                STM32单片机
//       SDA         接          PB5          //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机
//       BLK         接          PB13
//液晶屏背光控制信号，如果不需要控制，接3.3V
//       SCL         接          PB3          //液晶屏SPI总线时钟信号
//       DC          接          PB14         //液晶屏数据/命令控制信号
//       RES         接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
**************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 **************************************************************************************************/
#include "lcd.h"
#include "main.h"
#include "spi.h"
#include "stdlib.h"

// 管理LCD重要参数
// 默认为竖屏
_lcd_dev lcddev;

// 画笔颜色,背景颜色
uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
uint16_t DeviceCode;

/*****************************************************************************
 * @name       :void LCD_WR_REG(uint8_t data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
 ******************************************************************************/
void LCD_WR_REG(uint8_t data) {
  LCD_CS_CLR;
  LCD_RS_CLR;
  HAL_SPI_Transmit(&hspi1, &data, 1, 0xffff);
  LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(uint8_t data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
 ******************************************************************************/
void LCD_WR_DATA(uint8_t data) {
  LCD_CS_CLR;
  LCD_RS_SET;
  HAL_SPI_Transmit(&hspi1, &data, 1, 0xffff);
  LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
 * @date       :2018-08-09
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue) {
  LCD_WR_REG(LCD_Reg);
  LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
 ******************************************************************************/
void LCD_WriteRAM_Prepare(void) { LCD_WR_REG(lcddev.wramcmd); }

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(uint16_t Data)
 * @date       :2018-08-09
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
 ******************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data) {
  uint8_t senddata1 = Data >> 8;
  uint8_t senddata2 = Data;
  LCD_CS_CLR;
  LCD_RS_SET;
  HAL_SPI_Transmit(&hspi1, &senddata1, 1, 0xffff);
  HAL_SPI_Transmit(&hspi1, &senddata2, 1, 0xffff);
  LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(uint16_t x,uint16_t y)
 * @date       :2018-08-09
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y) {
  LCD_SetCursor(x, y); // 设置光标位置
  Lcd_WriteData_16Bit(POINT_COLOR);
}

/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
 ******************************************************************************/
void LCD_Clear(uint16_t Color) {
  unsigned int i, m;
  LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);
  LCD_CS_CLR;
  LCD_RS_SET;
  for (i = 0; i < lcddev.height; i++) {
    for (m = 0; m < lcddev.width; m++) {
      uint8_t Colordata1 = Color >> 8;
      uint8_t Colordata2 = Color;
      HAL_SPI_Transmit(&hspi1, &Colordata1, 1, 0xffff);
      HAL_SPI_Transmit(&hspi1, &Colordata2, 1, 0xffff);
    }
  }
  LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
 ******************************************************************************/

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
 ******************************************************************************/
void LCD_RESET(void) {
  LCD_RST_CLR;
  HAL_Delay(20);
  LCD_RST_SET;
  HAL_Delay(20);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
 ******************************************************************************/
void LCD_Init(void) {
  //	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_2);
  LCD_RESET(); // LCD 复位
  //************* ST7789初始化**********//
  LCD_WR_REG(0x36);
  LCD_WR_DATA(0x00);

  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x05);

  LCD_WR_REG(0xB2);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x33);
  LCD_WR_DATA(0x33);

  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0x35);

  LCD_WR_REG(0xBB);
  LCD_WR_DATA(0x19);

  LCD_WR_REG(0xC0);
  LCD_WR_DATA(0x2C);

  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0x01);

  LCD_WR_REG(0xC3);
  LCD_WR_DATA(0x12);

  LCD_WR_REG(0xC4);
  LCD_WR_DATA(0x20);

  LCD_WR_REG(0xC6);
  LCD_WR_DATA(0x0F);

  LCD_WR_REG(0xD0);
  LCD_WR_DATA(0xA4);
  LCD_WR_DATA(0xA1);

  LCD_WR_REG(0xE0);
  LCD_WR_DATA(0xD0);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x0D);
  LCD_WR_DATA(0x11);
  LCD_WR_DATA(0x13);
  LCD_WR_DATA(0x2B);
  LCD_WR_DATA(0x3F);
  LCD_WR_DATA(0x54);
  LCD_WR_DATA(0x4C);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x0D);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x1F);
  LCD_WR_DATA(0x23);

  LCD_WR_REG(0xE1);
  LCD_WR_DATA(0xD0);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x11);
  LCD_WR_DATA(0x13);
  LCD_WR_DATA(0x2C);
  LCD_WR_DATA(0x3F);
  LCD_WR_DATA(0x44);
  LCD_WR_DATA(0x51);
  LCD_WR_DATA(0x2F);
  LCD_WR_DATA(0x1F);
  LCD_WR_DATA(0x1F);
  LCD_WR_DATA(0x20);
  LCD_WR_DATA(0x23);

  LCD_WR_REG(0x21);

  LCD_WR_REG(0x11);
  // Delay (120);

  LCD_WR_REG(0x29);
  LCD_direction(USE_HORIZONTAL); // 设置LCD显示方向
  //	LCD_LED=1;//点亮背光
  LCD_Clear(WHITE); // 清全屏白色
}

/*****************************************************************************
 * @name       :void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t
xEnd,uint16_t yEnd)
 * @date       :2018-08-09
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
                                                                yStar:the
bebinning y coordinate of the LCD display window xEnd:the endning x coordinate
of the LCD display window yEnd:the endning y coordinate of the LCD display
window
 * @retvalue   :None
******************************************************************************/
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd,
                    uint16_t yEnd) {
  LCD_WR_REG(lcddev.setxcmd);
  LCD_WR_DATA(xStar >> 8);
  LCD_WR_DATA(xStar);
  LCD_WR_DATA(xEnd >> 8);
  LCD_WR_DATA(xEnd);

  LCD_WR_REG(lcddev.setycmd);
  LCD_WR_DATA(yStar >> 8);
  LCD_WR_DATA(yStar);
  LCD_WR_DATA(yEnd >> 8);
  LCD_WR_DATA(yEnd);

  LCD_WriteRAM_Prepare(); // 开始写入GRAM
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @date       :2018-08-09
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
                                                                Ypos:the  y
coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos) {
  LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/*****************************************************************************
 * @name       :void LCD_direction(uint8_t direction)
 * @date       :2018-08-09
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
                                                                                                        2-180 degree
                                                                                                        3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_direction(uint8_t direction) {
  lcddev.setxcmd = 0x2A;
  lcddev.setycmd = 0x2B;
  lcddev.wramcmd = 0x2C;
  switch (direction) {
  case 0:
    lcddev.width = LCD_W;
    lcddev.height = LCD_H;
    LCD_WriteReg(0x36, 0); // BGR==1,MY==0,MX==0,MV==0
    break;
  case 1:
    lcddev.width = LCD_H;
    lcddev.height = LCD_W;
    LCD_WriteReg(0x36, (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
    break;
  case 2:
    lcddev.width = LCD_W;
    lcddev.height = LCD_H;
    ;
    LCD_WriteReg(0x36, (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
    break;
  case 3:
    lcddev.width = LCD_H;
    lcddev.height = LCD_W;
    LCD_WriteReg(0x36, (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
    break;
  default:
    break;
  }
}
