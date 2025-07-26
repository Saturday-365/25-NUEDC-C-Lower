#include "main.h"
#include "usart.h"
#include "ring_buffer.h"

// 状态码
#define JOHO_STATUS uint8_t
#define JOHO_STATUS_SUCCESS 0 // 设置/读取成功
#define JOHO_STATUS_FAIL 1 // 设置/读取失败
#define JOHO_STATUS_TIMEOUT 2 // 等待超时 
#define JOHO_STATUS_WRONG_RESPONSE_HEADER 3 // 响应头不对
#define JOHO_STATUS_UNKOWN_US_ID 4 // 未知的舵机号
#define JOHO_STATUS_SIZE_TOO_BIG 5 // 参数的size大于JOHO_PACK_RESPONSE_MAX_SIZE里面的限制
#define JOHO_STATUS_CHECKSUM_ERROR 6 // 校验和错误
#define JOHO_STATUS_ID_NOT_MATCH 7 // 请求的舵机ID跟反馈回来的舵机ID不匹配


//指令类型
#define CMDType_Ping 1
#define CMDType_Read 2
#define CMDType_Write 3

#define JOHO_US_NUM 254 //舵机ID最大值

// 串口通讯超时设置
#define JOHO_TIMEOUT_MS 100

// 在串口舵机的通信系统设计里, 使用的字节序为Little Endian(低字节序/小端格式)
// STM32系统的数值存储模式就是Little Endian
// 所以0xfeff 这个数值, 在实际发送的时候低位会在前面 0xff, 0xfe
#define JOHO_PACK_REQUEST_HEADER		0xffff
#define JOHO_PACK_RESPONSE_HEADER		0xf5ff

// 返回的响应数据包最长的长度
#define JOHO_PACK_RESPONSE_MAX_SIZE 50

// 帧头接收完成的标志位
#define JOHO_RECV_FLAG_HEADER 0x01
// 控制指令接收完成的标志位
#define JOHO_RECV_FLAG_US_ID 0x02
// 内容长度接收完成的标志位
#define JOHO_RECV_FLAG_SIZE 0x04

//舵机状态接收完成标志位
#define JOHO_RECV_FLAG_SSTAT 0x06

// 内容接收完成的标志位
#define JOHO_RECV_FLAG_CONTENT 0x08
// 校验和接收的标志位
#define JOHO_RECV_FLAG_CHECKSUM 0x10

// 请求数据帧的结构体
typedef struct{
    uint16_t header; // 字头
    uint8_t usId; // 舵机ID
    uint8_t size; //数据长度：参数值=【ID号+指令类型+参数】的字节总数，即=content+2。
		uint8_t sstat; //1.接收数据包时表示舵机状态   2.发送数据包时表示指令类型
    uint8_t content[JOHO_PACK_RESPONSE_MAX_SIZE]; // 包的内容
    uint8_t checksum; // 校验和

    // 数据帧的接收结束状态 flag标志位
    uint8_t status; 
}PackageTypeDef;

// 发送数据帧-HEX
void USL_Send_HEX(UART_HandleTypeDef *usart, uint8_t size, uint8_t *content);

//接收数据帧
JOHO_STATUS USL_RecvPackage(UART_HandleTypeDef *usart,PackageTypeDef *pkg);


void JOHO_PackageBuild_Send(UART_HandleTypeDef *usart, uint8_t usId, uint8_t size,uint8_t cmdType, uint8_t *content);



void JOHO_Package2RingBuffer(PackageTypeDef *pkg,  RingBufferTypeDef *ringBuf);
uint8_t JOHO_CalcChecksum(PackageTypeDef *pkg);


JOHO_STATUS US_Ping(UART_HandleTypeDef *usart, uint8_t servo_id);
void USL_SetServoAngle(UART_HandleTypeDef *usart, uint8_t servo_id, \
				float posi, uint16_t interval);

uint16_t USL_GETPositionVal(UART_HandleTypeDef *usart, uint8_t servo_id);

void SET_Torque(UART_HandleTypeDef *usart, uint8_t servo_id,uint8_t isopen);