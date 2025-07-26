#include "main.h"
#include "usart.h"
#include "ring_buffer.h"

// ״̬��
#define JOHO_STATUS uint8_t
#define JOHO_STATUS_SUCCESS 0 // ����/��ȡ�ɹ�
#define JOHO_STATUS_FAIL 1 // ����/��ȡʧ��
#define JOHO_STATUS_TIMEOUT 2 // �ȴ���ʱ 
#define JOHO_STATUS_WRONG_RESPONSE_HEADER 3 // ��Ӧͷ����
#define JOHO_STATUS_UNKOWN_US_ID 4 // δ֪�Ķ����
#define JOHO_STATUS_SIZE_TOO_BIG 5 // ������size����JOHO_PACK_RESPONSE_MAX_SIZE���������
#define JOHO_STATUS_CHECKSUM_ERROR 6 // У��ʹ���
#define JOHO_STATUS_ID_NOT_MATCH 7 // ����Ķ��ID�����������Ķ��ID��ƥ��


//ָ������
#define CMDType_Ping 1
#define CMDType_Read 2
#define CMDType_Write 3

#define JOHO_US_NUM 254 //���ID���ֵ

// ����ͨѶ��ʱ����
#define JOHO_TIMEOUT_MS 100

// �ڴ��ڶ����ͨ��ϵͳ�����, ʹ�õ��ֽ���ΪLittle Endian(���ֽ���/С�˸�ʽ)
// STM32ϵͳ����ֵ�洢ģʽ����Little Endian
// ����0xfeff �����ֵ, ��ʵ�ʷ��͵�ʱ���λ����ǰ�� 0xff, 0xfe
#define JOHO_PACK_REQUEST_HEADER		0xffff
#define JOHO_PACK_RESPONSE_HEADER		0xf5ff

// ���ص���Ӧ���ݰ���ĳ���
#define JOHO_PACK_RESPONSE_MAX_SIZE 50

// ֡ͷ������ɵı�־λ
#define JOHO_RECV_FLAG_HEADER 0x01
// ����ָ�������ɵı�־λ
#define JOHO_RECV_FLAG_US_ID 0x02
// ���ݳ��Ƚ�����ɵı�־λ
#define JOHO_RECV_FLAG_SIZE 0x04

//���״̬������ɱ�־λ
#define JOHO_RECV_FLAG_SSTAT 0x06

// ���ݽ�����ɵı�־λ
#define JOHO_RECV_FLAG_CONTENT 0x08
// У��ͽ��յı�־λ
#define JOHO_RECV_FLAG_CHECKSUM 0x10

// ��������֡�Ľṹ��
typedef struct{
    uint16_t header; // ��ͷ
    uint8_t usId; // ���ID
    uint8_t size; //���ݳ��ȣ�����ֵ=��ID��+ָ������+���������ֽ���������=content+2��
		uint8_t sstat; //1.�������ݰ�ʱ��ʾ���״̬   2.�������ݰ�ʱ��ʾָ������
    uint8_t content[JOHO_PACK_RESPONSE_MAX_SIZE]; // ��������
    uint8_t checksum; // У���

    // ����֡�Ľ��ս���״̬ flag��־λ
    uint8_t status; 
}PackageTypeDef;

// ��������֡-HEX
void USL_Send_HEX(UART_HandleTypeDef *usart, uint8_t size, uint8_t *content);

//��������֡
JOHO_STATUS USL_RecvPackage(UART_HandleTypeDef *usart,PackageTypeDef *pkg);


void JOHO_PackageBuild_Send(UART_HandleTypeDef *usart, uint8_t usId, uint8_t size,uint8_t cmdType, uint8_t *content);



void JOHO_Package2RingBuffer(PackageTypeDef *pkg,  RingBufferTypeDef *ringBuf);
uint8_t JOHO_CalcChecksum(PackageTypeDef *pkg);


JOHO_STATUS US_Ping(UART_HandleTypeDef *usart, uint8_t servo_id);
void USL_SetServoAngle(UART_HandleTypeDef *usart, uint8_t servo_id, \
				float posi, uint16_t interval);

uint16_t USL_GETPositionVal(UART_HandleTypeDef *usart, uint8_t servo_id);

void SET_Torque(UART_HandleTypeDef *usart, uint8_t servo_id,uint8_t isopen);