

#ifndef __PAL_VIDEO_H__
#define __PAL_VIDEO_H__

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup VideoAPI API Video
/// 视频设置API。主要是视频输入参数，输出参数的设置，还有一些辅助的设置功能。
///	\n 调用流程图:
/// \code
///    =======================================================
///                |                            
///          VideoGetCaps            
///                |                            
///           VideoCreate                      
///      +---------|--------------+----------------------+     
///      |         |(In)          |(Out)                 |(Others)
///      |   VideoSetColor   VideoSwitch         VideoWriteRegister         
///      |   VideoSetCover VideoSetBkColor        VideoReadRegister
///      |   VideoSetVstd   VideoSetTVMargin        VideoMatrix
///      |         |        VideoSetTVColor      VideoSwitchTVMonitor
///      |         |                           VideoSwitchVGAMonitor
///      |         |      VideoSetOutputAdapter          |
///      |         |      VideoSetOutputAdapter          |
///      |         |              |                      |
///      +---------|--------------+----------------------+          
///            VideoDestory   
///                |                            
///    =======================================================
/// \endcode
/// @{

/// 视频制式类型
enum video_standard_t {
	VIDEO_STANDARD_PAL = 0,	///< PAL制式。
	VIDEO_STANDARD_NTSC,	///< NTSC制式。
	VIDEO_STANDARD_SECAM	///< SECAM制式。
};

/// 视频颜色格式
typedef struct VIDEO_COLOR{
	unsigned char	Brightness;		///< 亮度，取值0-100。
	unsigned char	Contrast;		///< 对比度，取值0-100。
	unsigned char 	Saturation;		///< 饱和度，取值0-100。
	unsigned char 	Hue;			///< 色调，取值0-100。
	unsigned char 	Gain;			///< 增益，取值0-100。bit7置位表示自动增益，其他位被忽略。
	unsigned char	WhiteBalance;	///< 自动白电平控制，bit7置位表示开启自动控制.0x0,0x1,0x2分别代表低,中,高等级
	unsigned short	Acutance;       ///< 锐度，取值0-15, 第8位表示水平锐度，高8为表示垂直锐度。
}VIDEO_COLOR;

/// VGA视频分辨率
typedef struct VGA_RESOLUTION {
	int	Width;						///< 水平分辨率。
	int	Height;						///< 垂直分辨率。
} VGA_RESOLUTION;


/// 视频覆盖操作类型
enum video_cover_t {
	VIDEO_COVER_WINDOW_CLEAR = 0,	///< 清除窗口覆盖区域。
	VIDEO_COVER_WINDOW_DRAW,		///< 绘制窗口覆盖区域。
	VIDEO_COVER_FULLSCREEN_DRAW,	///< 绘制全屏覆盖区域。
	VIDEO_COVER_FULLSCREEN_CLEAR	///< 清楚全屏覆盖区域。
};

/// 视频覆盖参数
typedef struct VIDEO_COVER_PARAM
{
	/// 覆盖的矩形区域，采用相对坐标体系。
	VIDEO_RECT	rect;

	/// 被覆盖区域显示的颜色。16进制表示为0x00BBGGRR，最高字节保留。
	unsigned int		color;

	/// 区域操作的类型， 取video_cover_t类型的值。窗口和全屏操作应该是独立的， 
	/// 即窗口内的操作不影响全屏，全屏内的操作也不影响窗口。 清除区域时，其他
	/// 参数被忽略。
	int			enable;
}VIDEO_COVER_PARAM;

/// 视频切换类型
enum video_switch_t{
	VIDEO_SWITCH_MONITOR = 0,	///< 切换到监视输出。
	VIDEO_SWITCH_PLAY,			///< 切换到回放输出。
	VIDEO_SWITCH_DECODE,		///< 切换到数字解码输出。
	VIDEO_SWITCH_MIX,		    ///< 切换到模拟和数字解码输出。
};

/// 视频输出适配器类型
enum video_output_t{
	VIDEO_OUTPUT_AUTO = 0,	///< 自动检测设备。
	VIDEO_OUTPUT_VGA,		///< 切换到VGA输出。
	VIDEO_OUTPUT_TV,		///< 切换到TV输出。
	VIDEO_OUTPUT_VGA_LCD	///< 切换到LCD VGA输出。
};

/// 视屏设备特性结构
typedef struct VIDEO_CAPS
{
	/// 是否支持TV输出颜色设置。置1表示支持该枚举值对应的特性，置0表示不支持。
	unsigned char TVColor;
} VIDEO_CAPS;				


/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VideoCreate(void);


/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VideoDestory(void);


/// 设置视频的颜色参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向颜色结构VIDEO _ COLOR的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetColor(int channel, VIDEO_COLOR * pColor);

/// 图像支持的特性列表掩码
/// \note 作为图像功能列表属性mask 1:可设置 0:不可设置
enum pic_attr
{
	PIC_COLOR, ///< 亮度,对比度,色度,增益,饱和度
	PIC_BACKLIGHT,///< 背光补偿
	PIC_EXPOSAL, 	///< 曝光选择
	PIC_AUTOC2BW, ///< 自动彩黑转换
	PIC_MIRROR,		//镜像
	PIC_FLIP,		//翻转
	PIC_MASKNUM,  ///< 属性选项数
};
 
/// 视频控制结构
typedef struct VIDEO_CONTROL
{
	unsigned char  Exposure;		///< 曝光模式 1-6:手动曝光等级; 0:自动曝光
	unsigned char  Backlight;		///< 背光补偿 1:打开补偿; 0:停止补偿
	unsigned char  AutoColor2BW;	///< 自动彩黑转换 1:打开转换; 0:停止转换
	unsigned char  Mirror;			//< 镜像  1 开， 0关
}VIDEO_CONTROL;
 
/// 设置视频的控制参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向视频控制结构VIDEO_CONTROL的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetControl(int channel, VIDEO_CONTROL * pColor);


/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] index 遮挡区域号
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetCover(int channel, int index, VIDEO_COVER_PARAM *pParam);


/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,unsigned int dwStandard);	


/// 写一个字节到视频设备寄存器。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \param [in] Data 要写入的值。
/// \return 无
void VideoWriteRegister(unsigned char Chip, unsigned char Page, unsigned char Register, unsigned char Data);


/// 从视频设备寄存器读一个字节。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \return读到的值。
unsigned char VideoReadRegister(unsigned char Chip, unsigned char Page, unsigned char Register);


/// 切换视频输出的类型。
/// \param [in] dwType 输出的类型，取下表中的值。
/// \retval 0  切换成功。
/// \retval <0  切换失败。
int VideoSwitch(unsigned int dwType);
int VideoSwitchEx(unsigned int dwType, int channel);


/// 设置TV输出的边界。视频显示在某些监视器上时， 4个边上会有损失。这个函数的目的
/// 就是设置4个边界压缩的比例，使视频落在监视器的可视区域内。调节范围0-100是一个
/// 示意值，实际上对应缩小的尺寸需要在底层做转换和限制。
/// 
/// \param [in] left 左边界比率，取值0-100， 默认值为0。
/// \param [in] top 上边界比率，取值0-100， 默认值为0。
/// \param [in] right 右边界比率，取值0-100， 默认值为0。
/// \param [in] bottom 下边界比率，取值0-100， 默认值为0。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVMargin(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom);


/// 设置TV输出的颜色。
/// 
/// \param [in] brightness 亮度，取值0-100， 默认值为50。
/// \param [in] contrast 对比度，取值0-100， 默认值为50。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVColor(unsigned char brightness, unsigned char contrast);


/// 设置视频输出的适配器类型。
/// 
/// \param [in] type 适配器类型，取video_output_t类型的值。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetOutputAdapter(unsigned char type);


/// 获取视频输出的适配器类型。
/// 
/// \param [out] ptype 适配器类型，取video_output_t类型的值。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetOutputAdapter(unsigned char *ptype);


/// 连接视频矩阵的输出通道和输入通道。
/// 
/// \param [in]  channelIn 视频输入通道
/// \param [in]  channelOut 视频输出通道
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoMatrix(unsigned char channelIn,unsigned char channelOut);


/// 开关TV监视器
/// 
/// \param [in] open TRUE-开TV监视器，FALSE-关TV监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchTVMonitor(int open);


/// 开关VGA监视器
/// 
/// \param [in] open TRUE-开VGA监视器，FALSE-关VGA监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchVGAMonitor(int open);

/// 得到视频支持的特性。
/// 
/// \param [out] pCaps 指向视频特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps);


/// 设置没有视频的监视画面的背景颜色.
/// 
/// \param [in] color 要设置的颜色，16进制表示为0x00BBGGRR，最高字节保留。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetBkColor(unsigned int color);


typedef struct tagWHITE_LEVEL_CAPS //Added by wangjun2-20080805
{
	int	 enable;		// 为1表示使能，为0时表示禁用
}WHITE_LEVEL_CAPS;

int WhiteLevelGetCaps(WHITE_LEVEL_CAPS * pCaps);


/// 设置VGA视频分辨率
/// \param [in] resolution 要设置的视频分辨率
/// \retval 0 成功
/// \retval < 0 设置失败
/// \note 目前支持设置如下的分辨率：800*600、1024*768、1280*1024、1366*768、1400*900
int VideoSetResolution(const VGA_RESOLUTION *resolution); 

enum Video_OUT_PRIORITY
{
	Video_OUT_PRIORITY_HDMI = 0,
	Video_OUT_PRIORITY_VGA = 1,
};

/// \priority 用于海思新平台上区分优先使用VGA  还是 HDMI,HDMI=0,VGA=1
int VideoSetResolutionV2(const VGA_RESOLUTION *resolution,int priority);

/// 获取当前VGA视频分辨率
/// \param [out] resolution 要设置的视频分辨率
/// \retval 0 成功
/// \retval < 0 获取失败
/// \note 目前支持设置如下的分辨率：800*600、1024*768、1280*1024、1366*768、1400*900
int VideoGetResolution(VGA_RESOLUTION *resolution); 

/*
*Peaking Filter contr
*/
typedef enum 
{
	M_PFC_Bypass = 0,
	M_2dB,
	M_3P5dB,
	M_6dB
}pic_ypfc_mode_t;

/*
*Low Pass Filter control 	 
*/
typedef enum 
{
	M_LPFC_Bypass = 0,
	M_4P2MHZ,
	M_5P6MHZ,
	M_7P2MHZ,
}pic_ylpfc_mode_t;

/*
*Selects the filter to separate the color 
*and brightness information out of the input signal(CVBS)
*/
typedef enum 
{
	M_APT_SPEC = 0,		//Adaptive Comb + special condition
	M_APT,				//Adaptive Comb
	M_2L_C12,			//2line comb (1,2 line)
	M_2L_C23,			//2line comb (2,3 line)
	M_3L_C123,		//3line comb (1,2,3 line)
	M_NOTCH,			//notch filter
	M_APT_NOTCH		//Adaptive comb + notch (auto select)
}pic_comb_mode_t;

/*
*FSC_LOCK_MODE
*/
typedef enum 
{
	M_CONTROL_ERR_0 = 0,
	M_CONTROL_ERR_1,
	M_CONTROL_ERR_2,
	M_CONTROL_ERR_3,
}pic_flerr_mode_t;

typedef enum 
{
	M_VALTER_SIG_GEN_0 = 0,
	M_VALTER_SIG_GEN_1,
	M_VALTER_SIG_GEN_2,
	M_VALTER_SIG_GEN_3
}pic_flgen_mode_t;




/*
*Locking speed control & Tracking Filter selection
*/
typedef enum 
{
	M_LSC_TFC_0 = 0,		//Bypass
	M_LSC_1,						//1/2 Cut
	M_LSC_2,						//1/4 Cut
	M_LSC_3,						//1/8 Cut
	M_LSC_4,						//1/16 Cut
	M_LSC_5,						//1/2 NO Cut
	M_LSC_6,						//1/4 NO Cut
	M_LSC_7,						//1/8 NO Cut
	M_LSC_8,						//1/16 NO Cut
	M_TFC_FASTEST,			//Fastest on Tracking Filter
	M_TFC_FAST,					//Fast on Tracking Filter
	M_TFC_NORMAL,				//Normal on Tracking Filter
	M_TFC_SLOW,					//Slow on Tracking Filter
	M_TFC_SLOWEST				//Slowest on Tracking Filter
}pic_fl_spd_mode_t;

/*
*H_V_SHARPNESS
*/
typedef enum 
{
	M_SHARPNESS_0X = 0,
	M_SHARPNESS_0P5X = 4,
	M_SHARPNESS_1X = 8,
	M_SHARPNESS_2X = 15
}pic_sharpness_mode_t;

/*
* IF Filter SEL
*/
typedef enum 
{
	M_IF_FILTER_BYPASS = 0,
	M_IF_FILTER_0,
	M_IF_FILTER_1,
	M_IF_FILTER_2
}pic_ciffl_mode_t;

/*
* CLP Filter SEL
*/
typedef enum 
{
	M_CLP_FILTER_BYPASS = 0,
	M_CLP_FILTER_0,
	M_CLP_FILTER_1,
	M_CLP_FILTER_2
}pic_clpfl_mode_t;

/*
* CKILL SEL
*/
typedef enum 
{
	M_CKC_BURST_FSC_0 = 0,
	M_CKC_BURST_FSC_1,
	M_CKC_BURST_FSC_2,
	M_CKC_BURST_FSC_3,
	M_CKC_ALWAYS_COLOR_ON,
	M_CKC_ALWAYS_COLOR_OFF,
}pic_ckill_mode_t;

typedef enum 
{
	M_WPD_COUNT_0 =0,
	M_WPD_COUNT_1,
	M_WPD_COUNT_2,
	M_WPD_COUNT_3,
	M_WPD_COUNT_4,
	M_WPD_COUNT_5
}pic_wpdctrl_stavalue_t;

typedef enum 
{
	M_WPD_SAMLEVEL_0 = 0,
	M_WPD_SAMLEVEL_1,
	M_WPD_SAMLEVEL_2,
	M_WPD_SAMLEVEL_3,
	M_WPD_SAMLEVEL_4,
	M_WPD_SAMLEVEL_5,
	M_WPD_SAMLEVEL_6,
	M_WPD_SAMLEVEL_7
}pic_wpdctrl_level_t;

enum pic_wpdmax_level_t
{
	M_WPD_MAX_LEVEL0 = 0,
	M_WPD_MAX_LEVEL1,
	M_WPD_MAX_LEVEL2,
};

typedef struct white_peak_control_t
{
	int swt;
	unsigned char wpd_maxlevel;
	unsigned char wpd_spd;
	pic_wpdctrl_stavalue_t wpd_count;
	pic_wpdctrl_level_t wpd_samlevel;
}white_peak_control;


/// 视频图像扩展参数格式
typedef struct VIDEO_EXTPICPARAM{
	unsigned char				Cti;			///< 色度瞬态改善滤波器,取0/1值。								
	pic_ypfc_mode_t		YPeakFilter;	///< 输入通道亮度尖峰滤波，取值0-3。
	pic_ylpfc_mode_t	YLowPassFilter;	///< 输入通道亮度低通滤波，取值0-3。
	pic_comb_mode_t 	CombMode;		///< 输入CVBS信号色度和亮度分离时滤波选择，取值0-6。
	pic_flerr_mode_t 	FsclockerrMode;	///< 色度锁存错误模式选择，取值0-3。
	pic_flgen_mode_t	FsclockgenMode;	///< 色度轮询信号产生模式选择，取值0-3。
	pic_fl_spd_mode_t 	FsclockSpd;		///< 色度锁存速度选择，取值0-14。
	pic_ciffl_mode_t	CIFFilter;		///< 色度中波滤波，取值0-3。
	pic_clpfl_mode_t	CLowPassFilter;	///< 色度低通滤波，取值0-3。
	unsigned char	ColorKill;					///< 消色控制，bit3置位表示开启自动控制.bit[0]-[2]消色模式选择。
	unsigned char	retention[32];
}VIDEO_EXTPICPARAM;

int VideoSetExPicParam(int channel, VIDEO_EXTPICPARAM * pExtpicparam);

/// 视频图像格式
typedef struct VIDEO_PICTURE
{
	unsigned char Exfunc; ///< bit0: LTI,亮度度线性过渡的改进
	///< bit1: CTI,色度线性过渡的改进
	///< bit2: NEC3DCOMB,Select 3D Comb function on/off on NEC video decoder
	///< bit3: CROSSCOLOR,CCS(Cross Color Suppression) control
	///< bit4: Black & White level Extension/Suppression control
	unsigned char Edgeenhance; ///< Select direction of edge preserving deinterlace
	unsigned char Brightness; ///< 亮度，取值0-255。
	unsigned char Contrast; ///< 对比度，取值0-255。
	unsigned char  Saturation; ///< 饱和度，取值0-255。
	unsigned char  Hue; ///< 色调，取值0-255。
	unsigned char  Sharpness; ///< 锐度，取值0-127。
	unsigned char  Reserve[16]; 
}VIDEO_PICTURE;

/// 视频图像逐行处理格式
typedef struct VIDEO_DEINTERLACE{
	unsigned char Deintmode; ///< bit7:Select deinterlace mode (default : Adapt) 
	unsigned char Edgeangle; ///< bit0-3:Select direction of edge preserving deinterlace (default : All)
	unsigned char Motion; ///< bit0:For slow motion demo (default : On)
	///< bit1:Fast motion detection Control (default : Very Fast)
	///< bit2:Motion boundary preserving control (default : On)
	unsigned char Film; ///< bit0-3:Film mode detection Control (default : All)
	///< bit4:  Film bad edit detection and correction function. (default : On)
	unsigned char Nrset; ///< bit0-3:Select 3D Noise Reduction (default : Typ)
	///< bit4:  For noise robust 1 demo (default : On)
	///< bit5:  For noise robust 2 demo (default : On)
	unsigned char NrUltra; ///< UltraNR Level
	unsigned char Reserve[16]; 
}VIDEO_DEINTERLACE;

typedef struct VIDEO_TILOUTPUT
{
	unsigned char	hdelay;	    ///< 【0x00-0x7F】   水平方向前肩延时。
	unsigned char	vdelay;		///< 【0x00-0x2F】	 垂直方向前肩延时。
	unsigned char	hblkend;	///< 【0x00-0x5F】	 水平方向后肩延时。
	unsigned char	vblkend;	///< 【0x00-0x3F】	 垂直方向后肩延时。
}VIDEO_TILOUTPUT;

/// 设置前端图像输出时序。
/// 
/// \param [in] channel 通道号。
/// \param [in] pTiloutput 指向图像输出时序结构VIDEO_TILOUTPUT的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTilOutput(int channel, VIDEO_TILOUTPUT *pTiloutput);


enum ad_type
{
	AD_NVP1108 = 0,
	AD_NVP1104B,
	AD_NVP1114A,
	AD_CX25824,
	AD_CX25828,
	AD_TW2868,
	AD_TW2866,	//TW2867,TW2865
	AD_BUFF_TYPE
};

enum ad_color
{
	COLOR_BRIGHTNESS = 0,
	COLOR_CONTRAST,
	COLOR_SATURATION,
	COLOR_HUE,
	COLOR_GAIN,
	COLOR_SHARPNESS,        //锐度
	COLOR_WHITEBALANCE,     //白平衡
	COLOR_BUFF_TYPE
};

enum ad_tioutput
{
	TIOUTPUT_H = 0,
	TIOUTPUT_V,
	TIOUTPUT_BUFF_TYPE
};

enum ad_alarm
{
	AD_VIDEOMOTIONDETECL= 0,	//移动侦测
	AD_VIDEOLOSSDETECL,			//视频丢失
	AD_VIDEOBINDDETECL,			//视频遮挡
	AD_DETECT_TYPE
};

/// 具体AD支持的功能
typedef struct AD_CAPTION {
	int	ad_type;						///< AD类型。
	int	audio_volume_en;				///< 音量调节支持：支持1，不支持0
	int	audio_volume_cap;				///< 音量调节能力：具体的值。
	int colorcaps;						///< 图像调节支持类型：每一位表示调节的功能
	int tioutputcaps;					///< 黑边调节支持类型：
	int tioutput_hvalue;				///< 黑边调节水平方向值范围
	int tioutput_vvalue;				///< 黑边调节垂直方向值范围
	int ad_alarm_caps;					///< 前端报警支持类型：每一位表示支持的功能
} AD_CAPTION;

int VideoGetADecodeCaption(AD_CAPTION *adcaption); 

/// @} end of group


#ifdef __cplusplus
}
#endif

#endif 
