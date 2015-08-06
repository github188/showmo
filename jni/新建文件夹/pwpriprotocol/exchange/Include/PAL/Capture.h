
#ifndef __PAL_CAPTURE_H__
#define __PAL_CAPTURE_H__

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup CaptureAPI API Capture
/// 多路多码流音视频编码接口，依赖于Memory接口。
///	\n 调用流程图:
/// \code
///    ============================================
///                   	|    
///             CaptureGetChannelsCap   
///                   	|    
///             CaptureGetChannels   
///                   	|    
///             CaptureSetChannels   
///                   	|    
///             CaptureSysInit   
///                   	|    
///             CaptureCreate   
///                     |
///             CaptureJPEGCreate
///       +-----------|------------------+         
///       |     CaptureSetVstd     	CaptureGetChannels
///		  |			  |           	CaptureGetDspInfo,
///       |   	CaptureSetFormat	CaptureGetCaps,
///    	  |	CaptureSetPacketPolicy	CaptureGetExtCaps      
///       |           |           	CaptureSetTime   	 
///       |     CaptureStart     	CaptureSetTitle   
///       | +---------|          	CaptureSetCover   
///       | |  CaptureGetBuffer  	CaptureSetVolume  
///       | | CaptureForceIFrame 	CaptureSynchronize
///       | |  CaptureGetVolume          |         
///       | +---------|                  |         
///       |      CaptureStop             |         
///       +-----------|------------------+         
///           CaptureDestroy                       
///                   |                            
///    ============================================


/// 捕获通道类型
enum capture_channel_t {
	CHL_MAIN_T = 0,		///< 主通道	－	主码流1 
	CHL_2END_T = 1,		///< 辅通道	－	出辅码流2 
	CHL_3IRD_T = 2,		///< 辅通道	－	出辅码流3 
	CHL_4RTH_T = 3,		///< 辅通道	－	出辅码流4 
	CHL_JPEG_T = 4,		///< 辅通道	－	出JPEG抓图 
	CHL_FUNCTION_NUM
};

/// 捕获开始类型
enum capture_start_t {
	CAPTURE_START_VIDEO = 1,	///< 视频编码开。
	CAPTURE_START_AUDIO = 2		///< 音频编码开。
};

/// 捕获压缩格式类型
enum capture_comp_t {
	CAPTURE_COMP_DIVX_MPEG4,	///< DIVX MPEG4。
	CAPTURE_COMP_MS_MPEG4,		///< MS MPEG4。
	CAPTURE_COMP_MPEG2,			///< MPEG2。
	CAPTURE_COMP_MPEG1,			///< MPEG1。
	CAPTURE_COMP_H263,			///< H.263
	CAPTURE_COMP_MJPG,			///< MJPG
	CAPTURE_COMP_FCC_MPEG4,		///< FCC MPEG4
	CAPTURE_COMP_H264,			///< H.264
	CAPTURE_COMP_NR				///< 枚举的压缩标准数目。
};

/// 捕获码流控制模式类型
enum capture_brc_t {
	CAPTURE_BRC_CBR,			///< 固定码流。
	CAPTURE_BRC_VBR,			///< 可变码流。
	CAPTURE_BRC_MBR,			///< 混合码流。
	CAPTURE_BRC_NR				///< 枚举的码流控制模式数目。
};

/// 捕获分辨率类型
/// \note更新下面这个结构时，要同步更新Record.cpp里验证编码能力
enum capture_size_t {
	CAPTURE_SIZE_D1,		///< 720*576(PAL)	720*480(NTSC)
	CAPTURE_SIZE_HD1,		///< 352*576(PAL)	352*480(NTSC)
	CAPTURE_SIZE_BCIF,		///< 720*288(PAL)	720*240(NTSC)
	CAPTURE_SIZE_CIF,		///< 352*288(PAL)	352*240(NTSC)
	CAPTURE_SIZE_QCIF,		///< 176*144(PAL)	176*120(NTSC)
	CAPTURE_SIZE_VGA,		///< 640*480(PAL)	640*480(NTSC)
	CAPTURE_SIZE_QVGA,		///< 320*240(PAL)	320*240(NTSC)
	CAPTURE_SIZE_SVCD,		///< 480*480(PAL)	480*480(NTSC)
	CAPTURE_SIZE_QQVGA,		///< 160*128(PAL)	160*128(NTSC)
	CAPTURE_SIZE_NR = 9,	///< 等临时处理等底层一起修改
	CAPTURE_SIZE_ND1 = 9,   ///< 240*192
	CAPTURE_SIZE_650TVL,    ///< 928*576
	CAPTURE_SIZE_720P,      ///< 1280*720
	CAPTURE_SIZE_1_3M,      ///< 1280*960
	CAPTURE_SIZE_UXGA ,     ///< 1600*1200
	CAPTURE_SIZE_1080P,     ///< 1920*1080
	CAPTURE_SIZE_WUXGA,     ///< 1920*1200
	CAPTURE_SIZE_2_5M,      ///< 1872*1408
	CAPTURE_SIZE_3M,        ///< 2048*1536
	CAPTURE_SIZE_5M,        ///< 3744*1408
	CAPTURE_SIZE_5M_X,		///< 2048*1920
	CAPTURE_SIZE_EXT_NR,    ///< 枚举的图形大小种类的数目。
};

/// 捕获设备特性结构
typedef struct CAPTURE_CAPS
{
	/// 支持的压缩标准的掩码，位序号对应枚举类型capture_comp_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	unsigned int Compression;

	/// 支持的码流控制模式的掩码，位序号对应枚举类型capture_brc_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	unsigned int BitRateControl;

	/// 支持的图像大小类型的掩码，位序号对应枚举类型capture_size_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	/// 在V2版本接口中已经不在使用
	unsigned int ImageSize;

	/// 支持的媒体类型的掩码，与枚举类型capture_start_t的每一个值相与，得到对应
	/// 的特性。置1表示支持该枚举值对应的特性，置0表示不支持。
	unsigned int Media;

	/// 区域覆盖的支持的块数。
	int CoverBlocks;

	/// 保留。
	int Reserved;

	/// 支持最多的标题叠加的个数。
	int	TitleCount;

	/// 是否支持区域覆盖 0-不支持，1-支持实时的区域覆盖， 即设置区域时
	/// 速度较快，可以实时调用， 2-支持非实时的区域覆盖，即设置区域时速度较慢，
	/// 只能在用户确定区域后调用。  
	int	RegionCover;
} CAPTURE_CAPS;

/// 捕获数据格式结构
typedef struct CAPTURE_FORMAT
{
	unsigned char	Compression;		///< 压缩标准，取capture_comp_t类型的值。 
	unsigned char	BitRateControl; 	///< 码流控制模式，取capture_brc_t类型的值。 
	unsigned char	ImageSize; 			///< 图像大小，取capture_size_t类型的值。 
	unsigned char	ImageQuality;		///< 图像质量，取值1-6，值越大，图像质量越好,只有在可变码流下才起作用。
	unsigned char	FramesPerSecond;	///< 每秒钟的帧数。一般PAL制1-25帧/秒，NTSC制1-30帧/秒。 
	unsigned char	AVOption;			///< 音视频选项，最低位表示视频，其次位表示音频 
	unsigned short	BitRate;			///< 参考码流值，Kbps为单位
	unsigned char	GOP; 				///< 帧间隔的数值，建议值49～99
	unsigned char	reserved[3]; 		///< 保留字节
} CAPTURE_FORMAT;


/// 捕获标题叠加参数结构
typedef struct CAPTURE_TITLE_PARAM
{
	/// 标题序号，最多支持的标题数从CaptureGetCaps可以取得。序号0对应的是
	/// 时间标题。时间标题点阵由底层内部产生，width height raster参数被忽略。
	int		index;

	/// 是否显示。0-不显示，其他参数忽略，1-显示。
	int		enable;

	/// 标题左上角x坐标，取8的整数倍，采用相对坐标体系(CIF)。
	unsigned short	x;		

	/// 标题左上角y坐标，采用相对坐标体系(CIF)。
	unsigned short	y;
	
	/// 标题宽度，取8的整数倍。					
	unsigned short	width;		

	/// 标题高度。
	unsigned short	height;		

	/// 前景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// text color, rgba 8:8:8:8	
	unsigned int	fg_color;
	
	/// 背景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// background color, rgba 8:8:8:8	
	unsigned int	bg_color;

	/// 标题单色点阵数据。0表示该点无效，1表示该点有效。每个字节包含8个象素的
	/// 数据，最左边的象素在最高位，紧接的下一个字节对应右边8个象素，直到一行
	/// 结束，接下来是下一行象素的点阵数据。
	unsigned char	*raster;
}CAPTURE_TITLE_PARAM;


/// 捕获标题叠加参数结构 V2版
typedef struct CAPTURE_TITLE_PARAM_V2
{
	//0对应时间标题，1对应通道标题，2和3是其它标题
	int		index;

	/// 是否显示。0-不显示，其他参数忽略，1-显示。
	int		enable;

	/// 标题左上角x坐标，取8的整数倍，采用相对坐标体系(CIF)。
	unsigned short	x;		

	/// 标题左上角y坐标，采用相对坐标体系(CIF)。
	unsigned short	y;

	/// 标题宽度，即像素点的个数，取8的整数倍。
	unsigned short	width;		

	/// 标题高度，即像素点的个数，取8的整数倍。
	unsigned short	height;	

	/// 前景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// text color, rgba 8:8:8:8	
	unsigned int	fg_color;

	/// 背景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// background color, rgba 8:8:8:8	
	unsigned int	bg_color;

	//颜色格式，如枚举值GraphicsColorFormat描述的
	unsigned int    color_format;

	/// 标题像素颜色值，如果color_format的值为graphicsColorRGB555，
	/// 则需要两个字节来表示一个像素的颜色，所以pixel保存的数据至少
	/// 应该是 (width * height * 2)字节。
	/// 如果pixel是NULL，并且index为0，则表示由底层库自己产生并设置时间标题。
	unsigned char	*pixel;

}CAPTURE_TITLE_PARAM_V2;

/// 捕获编码能力结构
typedef struct CAPTURE_DSPINFO 
{
	unsigned int	nMaxEncodePower;	///< DSP 支持的最高编码能力。
	unsigned short	nMaxSupportChannel;	///< DSP 支持最多输入视频通道数。
	unsigned short	bChannelMaxSetSync;	///< DSP 每通道的最大编码设置是否同步 0-不同步, 1 -同步。
}CAPTURE_DSPINFO, *PCAPTURE_DSPINFO;

typedef struct tagCAPTURE_DSPINFO_EXT 
{
	unsigned int	nMaxEncodePower;	/*-	DSP 支持的最高编码能力 	-*/
	unsigned short	nMaxSupportChannel;	/*-	DSP 支持最多输入视频通道数 	-*/
	unsigned short	bChannelMaxSetSync; /*-	DSP 每通道的最大编码设置是否同步 0-不同步, 1 -同步 	-*/
	unsigned int	nMaxDecodePower; /*-	DSP 编码能力中可用于解码的能力 	-*/
	unsigned int nMaxPowerPerChannel[32];	///< 每个通道支持的最高编码能力
	unsigned int ImageSizePerChannel[32];	///< 每个通道支持的图像分辨率
	unsigned int	reserved[21]; /*-	保留字段 	-*/
}CAPTURE_DSPINFO_EXT, *PCAPTURE_DSPINFO_EXT;

/// 捕获辅助码流支持特性结构
typedef struct CAPTURE_EXT_STREAM
{
	unsigned int ExtraStream;						///< 用channel_t的位来表示支持的功能。
	unsigned int CaptureSizeMask[CAPTURE_SIZE_NR];	///< 每一个值表示对应分辨率支持的辅助码流。（目前只能用于辅码流）
}CAPTURE_EXT_STREAM, *pCAPTURE_EXT_STREAM;	

/// 编码能力
/// 说明： 
/// 编码通道主码流支持的分辨率能力取决于2个方面: 1 - 当前通道支持的分辨率即uiImageSizePerChannel 2 - 当前通道支持的最高编码能力即uiMaxPowerPerChannel
/// 这个能力是主码流和辅码流的总和
/// 编码通道辅码流支持的分辨率能力取决于3个方面: 1 - 当前通道支持的分辨率即uiExImageSizePerChannel 2 - 当前通道支持的最高编码能力即uiMaxPowerPerChannel
/// 3 - 主码流设置的分辨率下辅码流支持的分辨率即uiExImageSizePerChannelEx
/// iChannelMaxSetSync： 1 - 表示所有通道的分辨率都要一样
typedef struct CAPTURE_CHANNEL_ABILITY
{
	unsigned int uiMaxPower;  ///< 每个通道支持的最高编码能力
	unsigned int uiImageSize;  ///< 每个通道支持的图像分辨率
	unsigned int uiExImageSize;  ///< 每个通道支持的辅码流图像分辨率
	unsigned int uiExImageSizeEx[CAPTURE_SIZE_EXT_NR]; ///< 指定主码流分辨率下每个通道的辅码流支持的图像分辨率
	unsigned int uiResverd[8];
}CAPTURE_CHANNEL_ABILITY;

typedef struct CAPTURE_DSPINFO_V2
{
	CAPTURE_CAPS Caps;          ///< 编码相关的辅助信息
	unsigned int uiMaxEncodePower;      ///< 支持的总编码能力
	unsigned int uiMaxBps;              ///< 支持的总码率大小Kbps
	int  iChannelMaxSetSync;    ///< 每个通道分辨率是否需要同步 0-不同步, 1 -同步
	int	 nMaxSupportChannel;	    ///< DSP 支持最多输入视频通道数
	unsigned int nMaxDecodePower;    ///< DSP 编码能力中可用于解码的能力
	unsigned int uiStream;           ///< 支持的码流类型
	CAPTURE_CHANNEL_ABILITY DspAbility[32];
	unsigned int uiResverd[16];
}CAPTURE_DSPINFO_V2;


/// 数字水印结构体，共128个字节
#define WM_KEY_LEN  8	/// 密钥字符的最大个数 
#define WM_CHAR_LEN 16	/// 水印字符个数
typedef struct CAPTURE_WM_ATTR_S
{
	unsigned int enable;  				//水印使能  1表示开开启，0表示关闭
	unsigned char au8Key[WM_KEY_LEN];	/// 数字水印密钥字符串 
	unsigned char au8User[WM_CHAR_LEN]; /// 数字水印用户字符 
	unsigned char resv[128 - WM_KEY_LEN - WM_CHAR_LEN - 4];
} CAPTURE_WM_ATTR_S;

struct VIDEO_COVER_PARAM;

/// 捕获通道信息
typedef struct CAPTURE_CHNINFO 
{
	unsigned int	nOrgChannel;	///< 从加密信息获取到的原始数据
	unsigned int	nCustomChannel;	///< 应用程序定制视频通道数。
	unsigned int    nRes[8];
}CAPTURE_CHNINFO, *PCAPTURE_CHNINFO;

//抓拍能力
typedef struct tagCAPTURE_SNAP_CAPS
{
	unsigned int nMaxSupportChannel;
	unsigned int ImageSizePerChannel[32];
	unsigned int reerved[6];
}CAPTURE_SNAP_CAPS;


typedef struct tagCHANNEL_CAPS
{
	int nTotalChannel;       /// 编解码总通道数
	int nHostChannel;        /// 主片支持通道数
	int	nPerSlaveChannel;    /// 每片从片的通道数
	int	nSlaveChipNum   ;    /// 从片的个数
	int	nSlaveChipAlign;     /// 每片从片的对齐通道数
	int	nPlayChannel;        /// 回放的通道数
	int	reserved[4];
} CHANNEL_CAPS;

/// 获得DSP的编码能力
///
int CaptureGetDspInfo(CAPTURE_DSPINFO *pDspInfo); 		 //已经废弃掉
int CaptureGetDspInfoExt(CAPTURE_DSPINFO_EXT *pDspInfo); //已经废弃掉

/// 得到捕获的通道数目。不包括语音捕获的通道，语音捕获的通道是最大捕获通道号加1。
/// 
/// \return捕获的通道数目。
int CaptureGetChannels(void);

/// 设置捕获的通道数目。不包括语音捕获的通道，语音捕获的通道是最大捕获通道号加1。
/// 
/// \return捕获的通道数目。
int CaptureSetChannels(int nCapture);

/// 得到捕获支持的特性。
/// 
/// \param [out] pCaps 指向捕获特性结构CAPTURE_CAPS的指针。
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureGetCaps(CAPTURE_CAPS * pCaps); //已经废弃掉

//! 获得捕获通道能力级
/// 
/// \param [out] pCaps 指向捕获能力级结构CHANNEL_CAPS的指针。
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureGetChannelsCap(CHANNEL_CAPS * pCaps);

/// 得到捕获双码流支持的特性。参数和CaptureGetCaps一样，但得到得是双码流支持得特性，
/// 而不是普通编码特性。 
/// 
/// \param [out] pCaps 指向捕获特性结构CAPTURE_CAPS的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int CaptureGetExtCaps(CAPTURE_EXT_STREAM *pCaps);

/// 得到捕获编码相关信息，改版本统一了一下3个接口
/// int CaptureGetCaps(CAPTURE_CAPS * pCaps);
/// int CaptureGetDspInfo(CAPTURE_DSPINFO *pDspInfo);
/// int CaptureGetDspInfoExt(CAPTURE_DSPINFO_EXT *pDspInfo);
/// 以上接口将逐步废除
/// \param [out] pDspInfo 指向捕获编码相关信息结构CAPTURE_DSPINFO_V2的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int CaptureGetDspInfoV2(CAPTURE_DSPINFO_V2 *pDspInfo);

/// 得到捕获设备系统初始化
/// 
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureSysInit();

/// 创建捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 创建失败
/// \retval 0 创建成功
int CaptureCreate(int channel);


/// 销毁捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 销毁失败
/// \retval 0 销毁成功
int CaptureDestroy(int channel);


/// 应用程序定时调用， 通知捕获模块检测自身状态， 如果有错误， 重启模块。 
/// 
void CaptureSynchronize(void);


/// 设置视频制式。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval 0  设置失败
int CaptureSetVstd(int channel,unsigned int dwStandard);	


/// 设置捕获时间和格式。应用程序需要定时调用此函数与捕获的时间进行同步。
/// 
/// \param [in] channel 通道号。
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \param [in] datefmt 日期格式，取date_fmt枚举值。
/// \param [in] datespc 日期分割符，取date_spec枚举值。
/// \param [in] timefmt 时间格式，取time_fmt枚举值。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int CaptureSetTime(int channel, const SYSTEM_TIME * pTime, int datefmt,int datespc,int timefmt);


/// 设置标题叠加。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构CAPTURE_TITLE_PARAM的指针。
/// \retval 0   设置成功。
/// \retval 非0 设置失败。
int CaptureSetTitle(int channel, CAPTURE_TITLE_PARAM *pParam);


/// 设置标题叠加，V2版本，增加了一个参数，结构体也有点变化
/// 
/// \param [in] channel 通道号。
/// \param [in] streamType 码流类型，如枚举值capture_channel_t描述的.
/// \param [in] pParam 指向标题叠加结构CAPTURE_TITLE_PARAM_V2的指针。
/// \retval 0   设置成功。
/// \retval 非0 设置失败。
int CaptureSetTitleV2(int channel, int streamType, CAPTURE_TITLE_PARAM_V2 *pParam);

/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int CaptureSetCover(int channel,int index, struct VIDEO_COVER_PARAM *pParam);



/// 设置捕获的音量。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval 0  设置失败
int CaptureSetVolume(int channel, unsigned int dwLVolume, unsigned int dwRVolume);


/// 强制编码器产生I帧。 
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int CaptureForceIFrame(int  channel, unsigned int dwType);

/// 设置捕获数据压缩的格式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] pFormat 指向捕获格式结构CAPTURE_FORMAT的指针。
/// \retval 0  设置成功
/// \retval 0  设置失败
int CaptureSetFormat(int channel, unsigned int dwType, CAPTURE_FORMAT * pFormat);


/// 开始编码。在视频制式、捕获选项、图像质量、帧率、码流控制方式、分辨率、编码
/// 模式等参数被改变时，应用程序会停止捕获，然后重新开始捕获。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType 参数表示启动捕获的选项，是capture_channel_t类型的值的组合。
/// \retval 0  开始成功
/// \retval 0  开始失败
int CaptureStart(int  channel, unsigned int dwType);


/// 停止编码，当前操作立即返回。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  dwType
/// \retval 0  停止成功
/// \retval 0  停止失败
int CaptureStop(int  channel, unsigned int dwType);


/// 读取采样数据，读为阻塞超时模式。内存块由设备申请和填充，设备得到适当的数据
/// 后返回。超时时立即返回；若内存块中已经填充数据，则设置实际的数据长度并返回；
/// 若内存块中没有数据，返回错误；将超时时间设置的较小，可以提高数据的实时性。
/// 
/// \param [in] pchannel 通道号，-1表示读所有通道的数据，读到的通道号作为函数
///        返回值返回。
/// \param [in] pdwType
/// \param [out] phMemory 内存块句柄指针，用来保存取到的内存块句柄。内存块的实际数据长
///        度表示捕获到的数据长度。
/// \return>=0 通道号（在输入通道号为-1时）
/// \retval 0  读成功
/// \retval 0  读失败
int CaptureGetBuffer(int *pchannel, unsigned int *pdwType, void* *phMemory);


/// 对编码通道的输出进行排列组合。可以由多个通道的视频输出组合成一个输出画面。
/// 
/// \param [in] channel 编码通道号
/// \param [in] dwMode 组合中的通道数目，取splitt类型的值。
/// \param [in] dwIndex 当前通道数目下组合方式的序号。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int CaptureCombine(int channel, unsigned int dwMode, unsigned int dwIndex);

/// 创建JPEG编码通道
/// 
/// \param [in] channel 编码通道号
/// \retval 0  设置成功。
/// \retval <0  设置失败。
/// \该接口需要在CaptureSetVstd前调用
int CaptureJPEGCreate(int channel);


/// 设置编码数字水印属性
/// \param [in] channel 要设置的通道
/// \param [in] pstWM 要设置水印信息
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int CaptureSetWmAttr(int channel, const CAPTURE_WM_ATTR_S *pstWM);


/// 设置编码信息帧信息
/// \param [in] channel 要设置的通道
/// \param [in] pInfo 要设置的信息
/// \param [in] nLen  信息长度
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int CaptureSetInfoFrame(int channel, const unsigned char *pInfo, int nLen);

///编码器静态参数
typedef struct ENCODE_STATICPARAM
{
	char reserved[2];
	int  profile;    
	int level;
	int reserved1[4];
} ENCODE_STATICPARAM;

/// 设置编码器静态参数。
/// \param [in] channel 通道号。
/// \param [in] dwType
/// >= 0 设置成功, =1 设备需要重启
///   <0 设置失败
int CaptureSetEncodeStaticParam(int channel, unsigned int dwType, ENCODE_STATICPARAM * encode_static_param);

//触发实时抓图
int CaptureTrigerSnap(int channel, unsigned int dwType);

//抓拍能力.放回0表示支持
int CaptureGetSnapCaps(CAPTURE_SNAP_CAPS *pCaps);

#ifdef __cplusplus
}
#endif

#endif

