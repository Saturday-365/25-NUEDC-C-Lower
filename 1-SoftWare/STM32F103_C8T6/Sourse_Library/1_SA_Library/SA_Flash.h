#ifndef __SA_FLASH_H__
#define __SA_FLASH_H__

#include "main.h"
#include <stdint.h>
#include <string.h>
extern uint16_t Store_Data[];
extern uint8_t flahs_flage;

#define       STM32_FLASH_SIZE 64   //所选STM32的FLASH容量大小(单位为K)；本产品选用的型号为：STM32F103RCT6；FALSH大小为256K

     #if      STM32_FLASH_SIZE < 256
		 #define  STM32_SECTOR_SIZE 1024  //一页为1K
		 #else
		 #define  STM32_SECTOR_SIZE 2048   //一页为2K
		 #endif
		 
#define STM32_FLASH_BASE 0x08000000 //STM32 FLASH起始地址
//0x0800FC00
#define STORE_START_ADDRESS		FLASH_SAVE_ADDR		//存储的起始地址
#define STORE_COUNT				64				//存储数据的个数

//写入的FLASH地址，这里为从倒数第一个扇区地址(0x807f800)开始写
//STM32_FLASH_BASE + STM32_SECTOR_SIZE*255 = 0x08000000 + (2048*255) = 0x0807f800
#define FLASH_SAVE_ADDR  STM32_FLASH_BASE+STM32_SECTOR_SIZE*(STM32_FLASH_SIZE-1)
		 

void FLASH_WriteData(uint32_t FLASH_Addr, uint16_t *FLASH_Data, uint16_t Size);

uint32_t FLASH_ReadWord(uint32_t Address);
uint16_t FLASH_ReadHalfWord(uint32_t Address);
uint8_t FLASH_ReadByte(uint32_t Address);

void FLASH_ReadData(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
void FLASH_ErasePage(uint32_t ErasePageBaseAddr,uint32_t ErasePageNbPageCount);
void Store_Init(void);
void Store_Save(void);

// 直接获取float的前16位(内存视角)
uint16_t float_to_uint16_mem_high(float f) ;

// 直接获取float的后16位(内存视角)
uint16_t float_to_uint16_mem_low(float f) ;

// 将两个uint16_t组合成一个float
float uint16s_to_float(uint16_t high, uint16_t low) ;

//// 联合体用于float和两个uint16_t之间的转换
//typedef union {
//    float f;                  // 浮点数
//    uint16_t u16[2];          // 拆分为两个16位无符号整数
//} FloatToUint16Union;

///**
// * 将float转换为两个uint16_t值
// * @param f 要转换的浮点数
// * @param high 存储高16位结果的指针
// * @param low 存储低16位结果的指针
// */
//void float_to_uint16(float f, uint16_t *high, uint16_t *low);

///**
// * 将两个uint16_t值转换为float
// * @param high 高16位值
// * @param low 低16位值
// * @return 转换后的浮点数
// */
//float uint16_to_float(uint16_t high, uint16_t low);

#endif


///*将Write_Data数据的内容写进FLASH*/
//	FLASH_WriteData(FLASH_SAVE_ADDR,(uint16_t *)&Write_Data,10);
//	
//	/*从STM32 FLASH中读出写入的数据，将数据储存在Read_Data中*/
//	FLASH_ReadData(FLASH_SAVE_ADDR,(uint16_t *)&Read_Data,10);
//————————————————

//版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
//                        
//原文链接：https://blog.csdn.net/weixin_43362027/article/details/132746977
