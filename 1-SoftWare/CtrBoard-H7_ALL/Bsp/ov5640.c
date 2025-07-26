#include "ov5640.h"
#include "ov5640_cfg.h"
#include "usart.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "dcmi.h"

volatile uint8_t OV5640_FrameState = 0; // DCMI状态标志，当数据帧传输完成时，会被 HAL_DCMI_FrameEventCallback() 中断回调函数置 1
volatile uint8_t OV5640_FPS;			// 帧率
uint8_t dcmi_frame_cnt = 0;
/**
******************************************************************************************************************************
* @brief:      	ov5640_delay
* @param:      	ms - 延时时间
* @details:    	简单延时函数，不是很精确
* @note:       	为了移植的简便性,此处采用软件延时，实际项目中可以替换成RTOS的延时或者HAL库的延时
******************************************************************************************************************************
**/
void ov5640_delay(uint32_t ms)
{
    HAL_Delay(ms);
//	osDelay(ms); // 可使用HAL库的延时
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_init
* @details:    	初始SCCB、DCMI、DMA以及配置OV5640
* @retval:     	OV5640_Success - 初始化成功
* @retval:     	OV5640_Error - 初始化失败
******************************************************************************************************************************
**/
int8_t ov5640_init(void)
{
	uint16_t id; // 定义变量存储器件ID

	sccb_init(); // SCCB引脚初始化

	ov5640_reset();		  // 执行软件复位
	id = ov5640_read_id(); // 读取器件ID

	if (id == 0x5640) // 进行匹配
	{
		printf("OV5640 OK,ID:0x%X\r\n", id); // 匹配通过

		ov5640_para_set();								   // 配置各项参数
		ov5640_set_framesize(OV5640_Width, OV5640_Height); //	设置OV5640输出的图像大小
														   //		ov5640_dcmi_crop( Display_Width, Display_Height, OV5640_Width, OV5640_Height );	// 将输出图像裁剪成适应屏幕的大小，JPG模式不需要裁剪

		return OV5640_Success; // 返回成功标志
	}
	else
	{
		printf("OV5640 ERROR!!!!!  ID:%X\r\n", id); // 读取ID错误
		return OV5640_Error;						// 返回错误标志
	}
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_send_continuous
* @param:      	buf - DMA将要传输的地址，即用于存储摄像头数据的存储区地址
* @param:      	buf_size - 传输的数据大小，32位宽
* @retval:     	void
* @details:    	启动DMA传输，连续模式
* @note:       	1. 开启连续模式之后，会一直进行传输，除非挂起或者停止DCMI
*               2. OV5640使用RGB565模式时，1个像素点需要2个字节来存储
*               3. 因为DMA配置传输数据为32位宽，计算 buf_size 时，需要除以4，例如：
*                   要获取 240*240分辨率 的图像，需要传输 240*240*2 = 115200 字节的数据，
*                   则 buf_size = 115200 / 4 = 28800 。
******************************************************************************************************************************
**/
void ov5640_send_continuous(uint32_t buf, uint32_t buf_size)
{
	hdma_dcmi.Init.Mode = DMA_CIRCULAR; // 循环模式

	HAL_DMA_Init(&hdma_dcmi); // 配置DMA

	// 使能DCMI采集数据,连续采集模式
	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)buf, buf_size);
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_send_snapshot
* @param:      	buf - DMA将要传输的地址，即用于存储摄像头数据的存储区地址
* @param:      	buf_size - 传输的数据大小，32位宽
* @retval:     	void
* @details:    	启动DMA传输，快照模式，传输一帧图像后停止
* @note:       	1. 快照模式，只传输一帧的数据
*               2. OV5640使用RGB565模式时，1个像素点需要2个字节来存储
*               3. 因为DMA配置传输数据为32位宽，计算 buf_size 时，需要除以4，例如：
*                   要获取 240*240分辨率 的图像，需要传输 240*240*2 = 115200 字节的数据，
*                   则 buf_size = 115200 / 4 = 28800 。
*               4. 使用该模式传输完成之后，DCMI会被挂起，再次启用传输之前，需要调用 ov5640_dcmi_resume() 恢复DCMI
******************************************************************************************************************************
**/
void ov5640_send_snapshot(uint32_t buf, uint32_t buf_size)
{
	hdma_dcmi.Init.Mode = DMA_NORMAL; // 正常模式

	HAL_DMA_Init(&hdma_dcmi); // 配置DMA

	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)buf, buf_size);
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_suspend
* @param:      	void
* @retval:     	void
* @details:    	挂起DCMI，停止捕获数据
* @note:       	1. 开启连续模式之后，再调用该函数，会停止捕获DCMI的数据
*               2. 可以调用 ov5640_dcmi_resume() 恢复DCMI
*               3. 需要注意的，挂起DCMI期间，DMA是没有停止工作的
******************************************************************************************************************************
**/
void ov5640_dcmi_suspend(void)
{
	HAL_DCMI_Suspend(&hdcmi); // 挂起DCMI
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_resume
* @param:      	void
* @retval:     	void
* @details:    	恢复DCMI，开始捕获数据
* @note:       	1. 当DCMI被挂起时，可以调用该函数恢复
*               2. 使用 ov5640_send_snapshot() 快照模式，传输完成之后，DCMI也会被挂起，再次启用传输之前，
*                  需要调用本函数恢复DCMI捕获
******************************************************************************************************************************
**/
void ov5640_dcmi_resume(void)
{
	(&hdcmi)->State = HAL_DCMI_STATE_BUSY;	   // 变更DCMI标志
	(&hdcmi)->Instance->CR |= DCMI_CR_CAPTURE; // 开启DCMI捕获
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_stop
* @param:      	void
* @retval:     	void
* @details:    	禁止DCMI的DMA请求，停止DCMI捕获，禁止DCMI外设
******************************************************************************************************************************
**/void ov5640_dcmi_stop(void)
{
	HAL_DCMI_Stop(&hdcmi);
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_dcmi_crop
* @param:       dx - 显示器的宽度
* @param:       dy - 显示器的高度
* @param:       sx - 摄像头传感器输出图像的宽度
* @param:       sy - 摄像头传感器输出图像的高度
* @retval:     	OV5640_Success表示成功
* @retval:     	OV5640_Error表示失败
* @details:    	使用DCMI的裁剪功能，将传感器输出的图像裁剪成适应屏幕的大小
* @note:       	1. 因为摄像头输出的画面比例不一定匹配显示器，所以需要裁剪
*               2. 摄像头的输出画面比例由 ov5640_para_set()配置参数决定，最终画面大小由 ov5640_set_framesize()决定
*               3. DCMI的水平有效像素也必须要能被4整除！
*               4. 函数会计算水平和垂直偏移，尽量让画面居中裁剪
******************************************************************************************************************************
**/
int8_t ov5640_dcmi_crop(uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy)
{
	uint16_t x_off, y_off; // 水平和垂直偏移，垂直代表的是行数，水平代表的是像素时钟数（PCLK周期数）
	uint16_t dcmi_capcnt;				   // 水平有效像素，代表的是像素时钟数（PCLK周期数）
	uint16_t dcmi_vline;				   // 垂直有效行数

	if ((dx >= sx) || (dy >= sy))
	{
		//		printf("实际显示的尺寸大于或等于摄像头输出的尺寸，退出DCMI裁剪\r\n");
		return OV5640_Error; // 如果实际显示的尺寸大于或等于摄像头输出的尺寸，则退出当前函数，不进行裁剪
	}
	// 在设置为RGB565格式时，水平偏移，必须是奇数，否则画面色彩不正确，
	// 因为一个有效像素是2个字节，需要2个PCLK周期，所以必须从奇数位开始，不然数据会错乱，
	// 需要注意的是，寄存器值是从0开始算起的	！
	x_off = sx - dx; // 实际计算过程为（sx - LCD_XSize）/2*2

	// 计算垂直偏移，尽量让画面居中裁剪，该值代表的是行数，
	y_off = (sy - dy) / 2 - 1; // 寄存器值是从0开始算起的，所以要-1

	// 因为一个有效像素是2个字节，需要2个PCLK周期，所以要乘2
	// 最终得到的寄存器值，必须要能被4整除！
	dcmi_capcnt = dx * 2 - 1; // 寄存器值是从0开始算起的，所以要-1

	dcmi_vline = dy - 1; // 垂直有效行数

	//	printf("%d  %d  %d  %d\r\n",x_off,y_off,dcmi_capcnt,dcmi_vline);

	HAL_DCMI_ConfigCrop(&hdcmi, x_off, y_off, dcmi_capcnt, dcmi_vline); // 设置裁剪窗口
	HAL_DCMI_EnableCrop(&hdcmi);														// 使能裁剪

	return OV5640_Success;
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_reset
* @param:      	void
* @retval:     	void
* @details:    	执行软件复位
* @note:       	期间有多个延时操作
******************************************************************************************************************************
**/
void ov5640_reset(void)
{
	OV5640_RST(0);

	ov5640_delay(100); // 等待模块上电稳定，最少5ms，然后拉低PWDN

	OV5640_PWDN_OFF; // PWDN 引脚输出低电平，不开启掉电模式，摄像头正常工作，此时摄像头模块的白色LED会点亮

	ov5640_delay(100);
	OV5640_RST(1); /* 结束复位 */
				   // 复位完成之后，要>=20ms方可执行SCCB配置
	ov5640_delay(100);

	sccb_write_reg_16bit(0x3103, 0x11); // 根据手册的建议，复位之前，直接将时钟输入引脚的时钟作为主时钟
	sccb_write_reg_16bit(0x3008, 0x82); // 执行一次软复位
	ov5640_delay(5);					// 延时5ms
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_read_id
* @param:      	void
* @retval:     	uint16_t - OV5640的器件ID
* @details:    	读取 OV5640 的器件ID
******************************************************************************************************************************
**/
uint16_t ov5640_read_id(void)
{
	uint8_t id_h, id_l; // ID变量

	id_h = sccb_read_reg_16bit(OV5640_ChipID_H); // 读取ID高字节
	id_l = sccb_read_reg_16bit(OV5640_ChipID_L); // 读取ID低字节

	return (id_h << 8) | id_l; // 返回完整的器件ID
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_para_set
* @param:      	void
* @retval:     	void
* @details:    	配置 OV5640 各个寄存器参数
******************************************************************************************************************************
**/
void ov5640_para_set(void)
{
	uint32_t i; // 计数变量

	//	uint8_t	read_reg; // 读取配置，用于调试

	for (i = 0; i < (sizeof(OV5640_INIT_Config) / 4); i++)
	{
		sccb_write_reg_16bit(OV5640_INIT_Config[i][0], OV5640_INIT_Config[i][1]); // 写入配置
		ov5640_delay(1);
		//		read_reg = sccb_read_reg_16bit(OV5640_INIT_Config[i][0]);	// 读取配置，用于调试

		//		if(OV5640_INIT_Config[i][1] != read_reg )	// 配置不成功
		//		{
		//			printf("出错位置：%d\r\n",i);	// 打印出错位置
		//			printf("0x%x-0x%x-0x%x\r\n",OV5640_INIT_Config[i][0],OV5640_INIT_Config[i][1],read_reg);
		//		}
	}
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_pixformat
* @param:      	pixformat - 像素格式，可选择 Pixformat_RGB565、Pixformat_GRAY、Pixformat_JPEG
* @retval:     	void
* @details:    	设置输出的像素格式
******************************************************************************************************************************
**/
void ov5640_set_pixformat(uint8_t pixformat)
{
	uint8_t reg; // 寄存器的值

	if (pixformat == Pixformat_JPEG)
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x30);	   //	设置数据接口输出的格式
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // 设置ISP的格式

		sccb_write_reg_16bit(OV5640_JPEG_MODE_SELECT, 0x02); // JPEG 模式2

		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_CTRL00, 0xA0); // JPEG 固定行数

		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_HSIZE_H, OV5640_Width >> 8);		 // JPEG输出水平尺寸,高字节
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_HSIZE_L, (uint8_t)OV5640_Width);	 // JPEG输出水平尺寸,低字节
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_VSIZE_H, OV5640_Height >> 8);	 // JPEG输出垂直尺寸,低字节
		sccb_write_reg_16bit(OV5640_JPEG_VFIFO_VSIZE_L, (uint8_t)OV5640_Height); // JPEG输出垂直尺寸,低字节
	}
	else if (pixformat == Pixformat_GRAY)
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x10);	   //	设置数据接口输出的格式
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // 设置ISP的格式
	}
	else // RGB565
	{
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL, 0x60);	   // 此处设置为RGB565格式，序列为 G[2:0]B[4:0], R[4:0]G[5:3]
		sccb_write_reg_16bit(OV5640_FORMAT_CONTROL_MUX, 0x01); // 设置ISP的格式
	}

	reg = sccb_read_reg_16bit(0x3821); // 读取寄存器值，Bit[5]用于是否使能JPEG模式
	sccb_write_reg_16bit(0x3821, (reg & 0xDF) | ((pixformat == Pixformat_JPEG) ? 0x20 : 0x00));

	reg = sccb_read_reg_16bit(0x3002); // 读取寄存器值，Bit[7]、Bit[4]和Bit[2]使能 VFIFO、JFIFO、JPG
	sccb_write_reg_16bit(0x3002, (reg & 0xE3) | ((pixformat == Pixformat_JPEG) ? 0x00 : 0x1C));

	reg = sccb_read_reg_16bit(0x3006); // 读取寄存器值，Bit[5]和Bit[3] 用于是否使能JPG时钟
	sccb_write_reg_16bit(0x3006, (reg & 0xD7) | ((pixformat == Pixformat_JPEG) ? 0x28 : 0x00));
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_jpeg_quantization_scale
* @param:      	scale - 压缩等级，取值 0x01~0x3F
* @retval:     	void
* @details:    	数值越大，压缩就越厉害，得到的图片占用空间就越小，但相应的画质会变差，客户可自行调节
******************************************************************************************************************************
**/
void ov5640_jpeg_quantization_scale(uint8_t scale)
{
	sccb_write_reg_16bit(0x4407, scale); // JPEG 压缩等级
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_framesize
* @param:      	width - 实际输出图像的长度
* @param:      	height - 实际输出图像的宽度
* @retval:     	int8_t - OV5640_Success: 成功, OV5640_Error: 失败
* @details:    	设置实际输出的图像大小（缩放后）
******************************************************************************************************************************
**/
int8_t ov5640_set_framesize(uint16_t width, uint16_t height)
{
	// OV5640的很多操作，都要加上这种对应 group 的配置
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // 开始 group 3 的配置

	sccb_write_reg_16bit(OV5640_TIMING_DVPHO_H, width >> 8); // DVPHO，设置输出水平尺寸
	sccb_write_reg_16bit(OV5640_TIMING_DVPHO_L, width & 0xff);
	sccb_write_reg_16bit(OV5640_TIMING_DVPVO_H, height >> 8); // DVPVO，设置输出垂直尺寸
	sccb_write_reg_16bit(OV5640_TIMING_DVPVO_L, height & 0xff);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // 结束配置
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // 启用设置

	return OV5640_Success;
}

/**
******************************************************************************************************************************
* @brief:      	ov5640_set_horizontal_mirror
* @param:      	state - 置1时，图像会水平镜像，置0时恢复正常
* @retval:     	int8_t - OV5640_Success: 成功, OV5640_Error: 失败
* @details:    	用于设置输出的图像是否进行水平镜像
******************************************************************************************************************************
**/
int8_t ov5640_set_horizontal_mirror(int8_t state)
{
	uint8_t reg; // 寄存器的值

	reg = sccb_read_reg_16bit(OV5640_TIMING_Mirror); // 读取寄存器值

	// Bit[2:1]用于设置是否水平镜像
	if (state == OV5640_Enable) // 如果使能镜像
	{
		reg |= 0X06;
	}
	else // 取消镜像
	{
		reg &= 0xF9;
	}
	return sccb_write_reg_16bit(OV5640_TIMING_Mirror, reg); // 写入寄存器
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_vertical_flip
* @param:      	state - 置1时，图像会垂直翻转，置0时恢复正常
* @retval:     	int8_t - OV5640_Success: 成功, OV5640_Error: 失败
* @details:    	用于设置输出的图像是否进行垂直翻转
******************************************************************************************************************************
**/
int8_t ov5640_set_vertical_flip(int8_t state)
{
	uint8_t reg; // 寄存器的值

	reg = sccb_read_reg_16bit(OV5640_TIMING_Flip); // 读取寄存器值

	// Bit[2:1]用于设置是否垂直翻转
	if (state == OV5640_Enable)
	{
		reg |= 0X06;
	}
	else // 取消翻转
	{
		reg &= 0xF9;
	}
	return sccb_write_reg_16bit(OV5640_TIMING_Flip, reg); // 写入寄存器
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_brightness
* @param:      	brightness - 亮度，可设置为9个等级：4，3，2，1，0，-1，-2，-3，-4，数字越大亮度越高
* @retval:     	void
* @details:    	设置图像亮度，直接使用OV5640手册给出的代码
******************************************************************************************************************************
**/
void ov5640_set_brightness(int8_t brightness)
{
	brightness = brightness + 4;
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // 开始 group 3 的配置

	sccb_write_reg_16bit(0x5587, OV5640_brightness_Config[brightness][0]);
	sccb_write_reg_16bit(0x5588, OV5640_brightness_Config[brightness][1]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // 结束配置
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // 启用设置
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_contrast
* @param:      	Contrast - 对比度，可设置为7个等级：3，2，1，0，-1，-2，-3
* @retval:     	void
* @details:    	设置图像对比度，直接使用OV5640手册给出的代码
******************************************************************************************************************************
**/
void ov5640_set_contrast(int8_t Contrast)
{
	Contrast = Contrast + 3;
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // 开始 group 3 的配置

	sccb_write_reg_16bit(0x5586, OV5640_Contrast_Config[Contrast][0]);
	sccb_write_reg_16bit(0x5585, OV5640_Contrast_Config[Contrast][1]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // 结束配置
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // 启用设置
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_set_effect
* @param:      	mode - 特效模式，可选择参数 OV5640_Effect_Normal、OV5640_Effect_Negative、
*                          OV5640_Effect_BW、OV5640_Effect_Solarize
* @retval:     	void
* @details:    	用于设置OV5640的特效，正常、负片、黑白、正负片叠加模式
*               这里仅列举了4个模式，更多特效模式可以参考手册进行配置
******************************************************************************************************************************
**/
void ov5640_set_effect(uint8_t mode)
{
	sccb_write_reg_16bit(OV5640_GroupAccess, 0X03); // 开始 group 3 的配置

	sccb_write_reg_16bit(0x5580, OV5640_Effect_Config[mode][0]);
	sccb_write_reg_16bit(0x5583, OV5640_Effect_Config[mode][1]);
	sccb_write_reg_16bit(0x5584, OV5640_Effect_Config[mode][2]);
	sccb_write_reg_16bit(0x5003, OV5640_Effect_Config[mode][3]);

	sccb_write_reg_16bit(OV5640_GroupAccess, 0X13); // 结束配置
	sccb_write_reg_16bit(OV5640_GroupAccess, 0Xa3); // 启用设置
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_download_firmware
* @param:      	void
* @retval:     	int8_t - OV5640_Success if firmware download successful, OV5640_Error otherwise
* @details:    	将自动对焦固件写入OV5640
*               因为OV5640片内没有flash，不能保存固件，因此每次上电都要写入一次
******************************************************************************************************************************
**/
int8_t ov5640_af_download_firmware(void)
{
	uint8_t af_reg = 0;			   // 对焦状态
	uint16_t i = 0;					   // 计数变量
	uint16_t OV5640_MCU_Addr = 0x8000; // OV5640 MCU 存储器的起始地址为 0x8000，大小为4KB

	sccb_write_reg_16bit(0x3000, 0x20); // Bit[5]，复位MCU，写入固件之前，需要执行此操作
										// 开始写入固件，批量写入，提高写入速度
	sccb_write_buf_16bit(OV5640_MCU_Addr, (uint8_t *)OV5640_AF_Firmware, sizeof(OV5640_AF_Firmware));
	sccb_write_reg_16bit(0x3000, 0x00); // Bit[5]，写入完毕，写0使能MCU

	// 写入固件之后，会有个初始化的过程，因此尝试读取100次状态，根据状态进行判断
	for (i = 0; i < 100; i++)
	{
		af_reg = sccb_read_reg_16bit(OV5640_AF_FW_STATUS); // 读取状态寄存器
		if (af_reg == 0x7E)
		{
			printf("AF firmware initiating>>>\r\n");
		}
		if (af_reg == 0x70) // 释放马达，镜头回到初始（对焦为无穷远处）位置，意味着固件写入成功
		{
			printf("AF firmware write OK！\r\n");
			return OV5640_Success;
		}
	}
	// 尝试100次读取之后，还是没有读到0x70状态，说明固件没写入成功
	printf("自动对焦固件写入失败！！！error！！\r\n");
	return OV5640_Error;
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_query_status
* @param:      	void
* @retval:     	int8_t - OV5640_AF_End if autofocus is completed, OV5640_AF_Focusing if autofocus is in progress
* @details:    	对焦状态查询
*               1. 对焦过程大概会持续500多ms
*               2. 对焦没完成时，采集到的的图像不在焦点，会非常模糊
******************************************************************************************************************************
**/
int8_t ov5640_af_query_status(void)
{
	uint8_t af_reg = 0; // 对焦状态

	af_reg = sccb_read_reg_16bit(OV5640_AF_FW_STATUS); // 读取状态寄存器
	printf("af_reg:0x%x\r\n", af_reg);

	// 单次对焦模式	下，返回 0x10，持续对焦模式下，返回0x20
	if ((af_reg == 0x10) || (af_reg == 0x20))
		return OV5640_AF_End; // 返回 对焦结束 标志
	else
		return OV5640_AF_Focusing; // 返回 正在对焦 标志
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_constant
* @param:      	void
* @retval:     	void
* @details:    	持续触发对焦，当OV5640检测到当前画面不在焦点时，会一直触发对焦，无需用户干预
*               1.可以调用 ov5640_af_query_status() 函数查询对焦状态
*               2.可以调用 ov5640_af_release() 退出持续对焦模式
*               3.对焦过程大概会持续500多ms
*               4.有时环境光线太暗，OV5640会反复的进行对焦，用户可根据实际情况切换到单次对焦模式
******************************************************************************************************************************
**/
void ov5640_af_trigger_constant(void)
{
	sccb_write_reg_16bit(0x3022, 0x04); //	持续对焦
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_single
* @param:      	void
* @retval:     	void
* @details:    	触发一次自动对焦
*               对焦过程大概会持续500多ms，用户可以调用 ov5640_af_query_status() 函数查询对焦状态
******************************************************************************************************************************
**/
void ov5640_af_trigger_single(void)
{
	sccb_write_reg_16bit(OV5640_AF_CMD_MAIN, 0x03); // 触发一次自动对焦
}
/**
******************************************************************************************************************************
* @brief:      	ov5640_af_trigger_single
* @param:      	void
* @retval:     	void
* @details:    	触发一次自动对焦
*               对焦过程大概会持续500多ms，用户可以调用 ov5640_af_query_status() 函数查询对焦状态
******************************************************************************************************************************
**/
void ov5640_af_release(void)
{
	sccb_write_reg_16bit(OV5640_AF_CMD_MAIN, 0x08); // 对焦释放指令
}
/**
******************************************************************************************************************************
* @brief:      	HAL_DCMI_FrameEventCallback
* @param[in]  	hdcmi: 指向 DCMI_HandleTypeDef 结构体的指针
* @retval:     	void
* @details:    	帧回调函数，每传输一帧数据，会进入该中断服务函数
******************************************************************************************************************************
**/
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	static uint32_t dcmi_tick = 0;		 // 用于保存当前的时间计数值

	if (HAL_GetTick() - dcmi_tick >= 1000) // 每隔 1s 计算一次帧率
	{
		dcmi_tick = HAL_GetTick(); // 重新获取当前时间计数值

		OV5640_FPS = dcmi_frame_cnt; // 获得fps

		dcmi_frame_cnt = 0; // 计数清0
	}
	dcmi_frame_cnt++; // 每进入一次中断（每次传输完一帧数据），计数值+1

	OV5640_FrameState = 1; // 传输完成标志位置1
}
/**
******************************************************************************************************************************
* @brief:      	HAL_DCMI_ErrorCallback
* @param[in]  	hdcmi: 指向 DCMI_HandleTypeDef 结构体的指针
* @retval:     	void
* @details:    	错误回调函数
******************************************************************************************************************************
**/
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
	// if( HAL_DCMI_GetError(hdcmi) == HAL_DCMI_ERROR_OVR)
	// {
	//    printf("FIFO溢出错误！！！\r\n");
	// }
	//   printf("error:0x%x!!!!\r\n",HAL_DCMI_GetError(hdcmi));
}

