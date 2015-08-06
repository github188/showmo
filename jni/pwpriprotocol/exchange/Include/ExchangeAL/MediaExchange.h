#ifndef __EXCHANGE_AL_MEDIA_EXCHANGE_H__
#define __EXCHANGE_AL_MEDIA_EXCHANGE_H__

#include "Exchange.h"
#include "CommExchange.h"
#include "../Types/Types.h"
#include "../Types/Defs.h"
#include "../Infra/Time.h"

/// 编码配置的类型
enum EncodeTypeByRecord
{
	ENCODE_TYPE_TIM = 0,
	ENCODE_TYPE_MTD = 1,
	ENCODE_TYPE_ALM = 2,
	ENCODE_TYPE_NUM = 3,
};

/// 捕获码流控制模式类型
enum CaptureBitrateControlType 
{
	CAPTURE_BITRATE_CBR,			///< 固定码流。
	CAPTURE_BITRATE_VBR,			///< 可变码流。
	CAPTURE_BITRATE_MBR,			///< 混合码流。
	CAPTURE_BITRATE_NR				///< 枚举的码流控制模式数目。
};

/// 捕获分辨率类型
enum CaptureImageSize 
{
	CAPTURE_IMAGE_SIZE_D1,			///< 704*576(PAL)	704*480(NTSC)
	CAPTURE_IMAGE_SIZE_HD1,			///< 704*288(PAL)	704*240(NTSC)
	CAPTURE_IMAGE_SIZE_BCIF,		///< 352*576(PAL)	352*480(NTSC)
	CAPTURE_IMAGE_SIZE_CIF,			///< 352*288(PAL)	352*240(NTSC)
	CAPTURE_IMAGE_SIZE_QCIF,		///< 176*144(PAL)	176*120(NTSC)
	CAPTURE_IMAGE_SIZE_VGA,			///< 640*480(PAL)	640*480(NTSC)
	CAPTURE_IMAGE_SIZE_QVGA,		///< 320*240(PAL)	320*240(NTSC)
	CAPTURE_IMAGE_SIZE_SVCD,		///< 480*480(PAL)	480*480(NTSC)
	CAPTURE_IMAGE_SIZE_QQVGA,		///< 160*128(PAL)	160*128(NTSC)
	CAPTURE_IMAGE_SIZE_NR = 9,      ///< 枚举的图形大小种类的数目。
	CAPTURE_IMAGE_SIZE_ND1 = 9,     ///< 240*192
	CAPTURE_IMAGE_SIZE_650TVL,      ///< 926*576
	CAPTURE_IMAGE_SIZE_720P,        ///< 1280*720
	CAPTURE_IMAGE_SIZE_1_3M,        ///< 1280*1024				// 12
	CAPTURE_IMAGE_SIZE_UXGA ,       ///< 1600*1200
	CAPTURE_IMAGE_SIZE_1080P,       ///< 1920*1080
	CAPTURE_IMAGE_SIZE_WUXGA,       ///< 1920*1200
	CAPTURE_IMAGE_SIZE_2_5M,        ///< 1872*1408
	CAPTURE_IMAGE_SIZE_3M,          ///< 2048*1536				// 17
	CAPTURE_IMAGE_SIZE_5M,          ///< 3744*1408
	CAPTURE_IMAGE_SIZE_5M_X,		///< 2048*1920
	CAPTURE_IMAGE_SIZE_EXT_NR,
};

/// 捕获压缩格式类型
enum CaptureCompressType 
{
	CAPTURE_COMPRESS_DIVX_MPEG4,	///< DIVX MPEG4。
	CAPTURE_COMPRESS_MS_MPEG4,		///< MS MPEG4。
	CAPTURE_COMPRESS_MPEG2,			///< MPEG2。
	CAPTURE_COMPRESS_MPEG1,			///< MPEG1。
	CAPTURE_COMPRESS_H263,			///< H.263
	CAPTURE_COMPRESS_MJPG,			///< MJPG
	CAPTURE_COMPRESS_FCC_MPEG4,		///< FCC MPEG4
	CAPTURE_COMPRESS_H264,			///< H.264
	CAPTURE_COMPRESS_NR				///< 枚举的压缩标准数目。
};

/// 捕获通道类型
enum CaptureChannelTypes 
{
	CAPTURE_CHN_MAIN = 0,		///< 主通道	－	主码流1 
	CAPTURE_CHN_2END = 1,		///< 辅通道	－	出辅码流2 
	CAPTURE_CHN_3IRD = 2,		///< 辅通道	－	出辅码流3 
	CAPTURE_CHN_4RTH = 3,		///< 辅通道	－	出辅码流4 
	CAPTURE_CHN_JPEG = 4,		///< 辅通道	－	出JPEG抓图 
	CAPTURE_CHN_NR,
};

/// 画质选择
enum CaptureImageQuality
{
	CAPTURE_IMG_QUALITY_WOREST = 1,		///< 最差
	CAPTURE_IMG_QUALITY_WORSE = 2,		///< 较差
	CAPTURE_IMG_QUALITY_GENERAL = 3,	///< 一般
	CAPTURE_IMG_QUALITY_GOOD = 4,		///< 好
	CAPTURE_IMG_QUALITY_BETTER = 5,		///< 较好
	CAPTURE_IMG_QUALITY_BEST = 6,		///< 很好
	CAPTURE_IMG_QUALITY_NR = 7,
};

/// 组合编码模式
enum CombineEncodeMode
{
	COMBINE_ENCODE_MULTIREPLAY = 1,		///< 多路回放
	COMBINE_ENCODE_NARROWBAND = 2,		///< 窄带传输
	COMBINE_ENCODE_NR = 3,
};

/// 视频格式
struct VIDEO_FORMAT
{
	int		iCompression;			//  压缩模式 	
	int		iResolution;			//  分辨率 参照枚举capture_size_t	
	int		iBitRateControl;		//  码流控制 参照枚举capture_brc_t
	int		iQuality;				//  码流的画质 档次1-6		
	int		nFPS;					//  帧率值，NTSC/PAL不区分,负数表示多秒一帧		
	int		nBitRate;				//  0-4096k,该列表主要由客户端保存，设备只接收实际的码流值而不是下标。
	int		iGOP;					//  描述两个I帧之间的间隔时间，2-12 
};

/// 音频格式
struct  AUDIO_FORMAT
{
	int		nBitRate;				//  码流kbps	
	int		nFrequency;				//  采样频率	
	int		nMaxVolume;				//  最大音量阈值
};

/// 媒体格式
struct MEDIA_FORMAT
{
	VIDEO_FORMAT vfFormat;			//  视频格式定义 			
	AUDIO_FORMAT afFormat;			//  音频格式定义 
	bool	bVideoEnable;			//  开启视频编码 
	bool	bAudioEnable;			//  开启音频编码 	
};

/// 编码设置
struct CONFIG_ENCODE
{
	MEDIA_FORMAT dstMainFmt[ENCODE_TYPE_NUM];			// 主码流格式 	
	MEDIA_FORMAT dstExtraFmt[MAX_EXTRA_STREAM_TYPE];	// 辅码流格式 
	MEDIA_FORMAT dstSnapFmt[ENCODE_TYPE_NUM];			// 抓图格式 
};

/// 全通道编码配置
struct EncodeConfigAll
{
	CONFIG_ENCODE vEncodeConfigAll[N_SYS_CH];
};

// 简化版本编码配置
/// 媒体格式
struct MEDIA_FORMAT_SIMPLIIFY
{
	VIDEO_FORMAT vfFormat;			//  视频格式定义 			
	bool	bVideoEnable;			//  开启视频编码 
	bool	bAudioEnable;			//  开启音频编码 	
};

/// 编码设置
struct CONFIG_ENCODE_SIMPLIIFY
{
	MEDIA_FORMAT dstMainFmt;		//  主码流格式 	
	MEDIA_FORMAT dstExtraFmt;		//  辅码流格式 
};

/// 全通道编码配置
struct EncodeConfigAll_SIMPLIIFY
{
	CONFIG_ENCODE_SIMPLIIFY vEncodeConfigAll[N_SYS_CH];
};

/// 组合编码通道配置
struct CombEncodeConfigAll
{
	CONFIG_ENCODE vEncodeConfigAll[N_SYS_COMB_CH];
};

/// 组合编码模式
struct CombEncodeParam
{
	int iEncodeMode;	 //见CombineEncodeMode
};

struct CombEncodeModeAll
{
	CombEncodeParam vEncodeParam[N_SYS_COMB_CH];
};

/// 视频物件结构
struct  VIDEO_WIDGET
{
	RGBA32 rgbaFrontground;		///< 物件的前景MakeRGB，和透明度	
	RGBA32 rgbaBackground;		///< 物件的后景MakeRGB，和透明度	
	Rect	rcRelativePos;		///< 物件边距与整长的比例*8191
	bool	bShowInPreview;		///< 预览叠加	
	bool	bShowInEncode;		///< 编码叠加
};

/// 视频物件设置
struct CONFIG_VIDEOWIDGET
{
	VIDEO_WIDGET	dstCovers[MAX_COVER_COUNT];
	VIDEO_WIDGET	ChannelTitle;
	VIDEO_WIDGET	TimeTitle;
	struct 
	{
		char strName[CHANNEL_NAME_MAX_LEN];
		int64 iSerialNo;	
	} ChannelName;					/// 通道名称
	int				iCoverNum;		/// 当前该通道有几个叠加的区域
};

/// 所有通道名称标题
struct ChannelNameConfigAll
{
	char channelTitle[N_SYS_CH][CHANNEL_NAME_MAX_LEN];
};

/// 视频物件
struct VideoWidgetConfigAll
{
	CONFIG_VIDEOWIDGET vVideoWidegetConfigAll[N_SYS_CH];
};


//OSD信息显示位置与状态配置
struct OSDWidgetConfig
{
	VIDEO_WIDGET alarmInfo; //报警提示信息
};

//所有通道的OSD信息显示位置与状态配置
struct OSDWidgetConfigAll
{
	OSDWidgetConfig vOSDWidgetConfigAll[N_SYS_CH];
};


#define N_COLOR_SECTION 2

//! 视频颜色结构
struct VIDEOCOLOR_PARAM
{
	int	nBrightness;		///< 亮度	0-100
	int	nContrast;			///< 对比度	0-100
	int	nSaturation;		///< 饱和度	0-100
	int	nHue;				///< 色度	0-100
	int	mGain;				///< 增益	0-100 第７位置1表示自动增益		
	int	mWhitebalance;		///< 自动白电平控制，bit7置位表示开启自动控制.0x0,0x1,0x2分别代表低,中,高等级
	int nAcutance;          ///< 锐度   0-15
}; 

///< 视频颜色设置
struct VIDEOCOLOR
{
	TimeSection			tsTimeSection;		/// 时间段
	VIDEOCOLOR_PARAM	dstColor;			/// 颜色定义
	int					iEnable;
};

struct CONFIG_VIDEOCOLOR
{
	VIDEOCOLOR dstVideoColor[N_COLOR_SECTION];
};

struct VideoColorConfigAll
{
	CONFIG_VIDEOCOLOR vVideoColorAll[N_SYS_CH];
};


/// 轮巡种类
enum TourTypes
{
	TOUR_TYPES_MONITOR,		///< 监视轮巡
	TOUR_TYPES_ALARM,		///< 抱紧轮巡
	TOUR_TYPES_MONTION,		///< 动检轮巡
	TOUR_TYPES_NR,
};

///<轮巡设置
struct TourConfig
{
	bool bEnable;	///< 进行轮巡使能	
	int iInterval;	///< 轮巡时间间隔	[5,120]
	int iType;		///< 轮巡类型
	uint iMask[N_SPLIT];	///< 轮巡掩码, 一维数组[画面分割种类]
	bool bReturn;     ///< 告警结束后是否恢复多原来的画面分割模式
};

/// 所有通道的轮巡设置
struct TourConfigAll
{
	TourConfig vTourConfigAll[TOUR_TYPES_NR];
};

///< TV调节设置
struct TVAdjustConfig
{
	Rect blackMargin;	///< 黑边调节区间
	Rect rctMargin;		///<TV缩小比率，各分量取值相同	[0,100]
	int iBrightness;	///< 亮度	[0,100]
	int iContrast;		///< 对比度[0,100]
	int iAntiDither;	///< 去抖动[0,100]
};

///< 回放设置
struct PlayBackConfig
{
	int iChannels;			///< 回放画面数	[0, 16]
	int iVolume[N_SYS_CH];	///< 回放音频,一维数组[通道]
	int iSearchMask;		///< 查询掩码
	bool bContinue;			///< 是否连续播放	
};

#define SPLITTYPES 6


/// 音频编码类型
enum AudioEncodeTypes
{
	AUDIO_ENCODE_NONE = 0,
	AUDIO_ENCODE_G729_8KBIT,
	AUDIO_ENCODE_G726_16KBIT,
	AUDIO_ENCODE_G726_24KBIT,
	AUDIO_ENCODE_G726_32KBIT,
	AUDIO_ENCODE_G726_40KBIT,
	AUDIO_ENCODE_PCM_8TO16BIT,
	AUDIO_ENCODE_PCM_ALAW,
	AUDIO_ENCODE_PCM_ULAW,
	AUDIO_ENCODE_ADPCM8K16BIT,
	AUDIO_ENCODE_ADPCM16K16BIT,
	AUDIO_ENCODE_G711_ALAW,
	AUDIO_ENCODE_MPEG2_LAYER1,
	AUDIO_ENCODE_AMR8K16BIT,
	AUDIO_ENCODE_G711_ULAW,
	AUDIO_ENCODE_IMA_ADPCM_8K16BIT,
	AUDIO_ENCODE_TYPES_NR,
};

/// 音频输入格式，语音对讲用
struct AudioInFormatConfig
{
	int iBitRate;	   ///< 码流大小，kbps为单位，比如192kbps，128kbps
	int iSampleRate;   ///< 采样率，Hz为单位，比如44100Hz
	int iSampleBit;    ///< 采样的位深
	int iEncodeType;   ///< 编码方式，参照AudioEncodeTypes定义
};

/// 语音对讲格式
struct AudioInFormatConfigAll
{
	AudioInFormatConfig vAudioInFormatConfig[AUDIO_ENCODE_TYPES_NR];
};

void AudioInFormatExchange(CConfigTable& table, AudioInFormatConfig& config, int state);

/// 录像类型
enum RecordTypes
{
	RECORD_TYPE_COMMON = 0x00,		///< 普通录像
	RECORD_TYPE_ALARM = 0x01,		///< 报警录像
	RECORD_TYPE_MOTION = 0x02,		///< 动检录像
	RECORD_TYPE_HAND = 0x03,		///< 手动录像
	RECORD_TYPE_SNAP = 0x04,		///< 抓图
	RECORD_TYPE_NR,
};

/// 录像模式种类
enum RecordModeTypes
{
	RECORD_MODE_CLOSED,		///< 关闭录像
	RECORD_MODE_MANUAL,		///< 手动录像
	RECORD_MODE_CONFIG,		///< 按配置录像
	RECORD_MODE_NR,		
};

///< 录像设置
struct RecordConfig
{
	int iPreRecord;			///< 预录时间，为零时表示关闭	
	bool bRedundancy;		///< 冗余开关
	int iPacketLength;		///< 录像打包长度（分钟）[1, 255]
	int iRecordMode;		///< 录像模式，见RecordModeTypes
	WorkSheet wcWorkSheet;				///< 录像时间段	
	uint typeMask[N_WEEKS][N_TSECT];	///< 录像类型掩码，见enum RecordTypes
};

struct RecordConfigAll
{
	RecordConfig vRecordConfigAll[N_SYS_CH+N_DECORDR_CH];
};

/// 录像模式种类
enum SnapModeTypes
{
	SNAP_MODE_CLOSED,		///< 关闭抓图
	SNAP_MODE_MANUAL,		///< 禁止抓图
	SNAP_MODE_CONFIG,		///< 按配置抓图
	SNAP_MODE_NR,		
};

///< 图片设置
struct SnapshotConfig
{
	int iPreSnap;			///< 预抓图片数
	bool bRedundancy;		///< 冗余开关
	int iSnapMode;		///< 录像模式，见RecordModeTypes
	WorkSheet wcWorkSheet;				///< 录像时间段	
	uint typeMask[N_WEEKS][N_TSECT];	///< 录像类型掩码，见enum RecordTypes
};

struct SnapshotConfigAll
{
	SnapshotConfig vSnapshotConfigAll[N_SYS_CH];
};

/// DVR通道类型
enum ChanTypes
{
	CHAN_TYPE_SIMULATE,		///< 模拟通道
	CHAN_TYPE_DIGIT,		///< 数字通道
	CHAN_TYPE_NR,		
};

/// 视频通道配置
struct VideoChannel
{
	int iChnType;		///< 通道类型
	int iChn;			///< 通道号
};

/// 数字水印配置
struct WaterMarkConfig
{
	bool	bEnable;		//  开启水印
	std::string sKey;
	std::string sUserData;
};

struct WaterMarkConfigAll
{
	WaterMarkConfig vWaterMarkConfigAll[N_SYS_CH];
};

// 编码最大分辨率
struct EncoderPower
{
	int iMaxResolution[N_SYS_CH];	// 最大的编码分辨率
};

//音频设置
/// 音频输入模式种类
enum AudioModeTypes
{
	AUDIO_MODE_SINGLE,		///<单通道模式
	AUDIO_MODE_DOUBLE,		///< 双通道模式
	AUDIO_MODE_NR,		
};

struct AudioVolume
{
	int AudioMode;  //通道模式,如果当通道两个音频同时设置
	int iLInVolume; //左声道
	int iRInVolume; //预览右音频音量
	int reserved; //保留
};

struct AudioVolumeAll
{
	AudioVolume Volume[N_SYS_CH]; //预览音频音量
};


enum SpotTypes
{
	SpotAsMain, //spot最为主输出
	SpotAsAux,	//sopt作为辅输出
	SpotTypeNr
};
///<spot配置
struct SpotConfig
{
	int iType;		///<显示输出类型SpotTypes
	bool bTourEnable;	///< 进行轮巡使能	
	int iInterval;	///< 轮巡时间间隔	[5,120]
	uint iMask[N_SPLIT];	///< 轮巡掩码, 一维数组[画面分割种类]
};

struct SpotConfigAll
{
	SpotConfig vSpotAllCfg[MAX_SPOT_NUMBER];
};

struct VideoChannelSeq
{
	bool bFullScreen; //是否全屏
	bool bChannelState[N_SYS_CH + N_DECORDR_CH];//通道的开关状态
	int vTopChSeq[N_SYS_CH + N_DECORDR_CH]; //桌面上显示的通道顺序
	int vSplitChSeq[N_SYS_CH + N_DECORDR_CH]; //视频分割中通道的显示顺序
	int vDecChSeq[N_PLY_CH + N_DECORDR_CH]; //通道对应解码器的映射顺序
	int vDecChResSeq[N_PLY_CH + N_DECORDR_CH]; //切换单画面前保存的解码器顺序
};

//车辆信息配置
struct CarInfoDisplay
{
	VIDEO_WIDGET  stCarInfoWidget;
	char          pReserve[32];
};

//GPS信息显示配置
struct GPSDisplay
{
	VIDEO_WIDGET  stGPSWidget;
	char          pReserve[32];
};

//车载相关显示配置
struct CarDisplay
{
	CarInfoDisplay stCarInfoDisplay;
	GPSDisplay     stGPSDisplay;
};

/*enum Profile_t
{
	BASELINE = 1,
	MAINPROFILE = 2,
	HIGHPROFILE = 3
};*/


///编码器静态参数
struct EncodeStaticParam
{
	char reserved[2];
	int profile;
	int level;
	int reserved1[4];
};

struct EncodeStaticParamAll
{
	EncodeStaticParam	vEncodeStaticParamAll[N_SYS_CH];
};

#endif
