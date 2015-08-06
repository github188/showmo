#ifndef __EXCHANGE_AL_ABILITY_SET_H__
#define __EXCHANGE_AL_ABILITY_SET_H__

#include "../Types/Types.h"
#include "CommExchange.h"
#include "MediaExchange.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
/// 系统能力级
/// 编码功能
enum EncodeFunctionTypes
{
	ENCODE_FUNCTION_TYPE_DOUBLE_STREAM,		///< 双码流功能
	ENCODE_FUNCTION_TYPE_COMBINE_STREAM,	///< 组合编码
	ENCODE_FUNCTION_TYPE_SNAP_STREAM,		///< 抓图编码
	ENCODE_FUNCTION_TYPE_WATER_MARK,		///< 水印功能
	ENCODE_FUNCTION_TYPE_NR,
};

/// 报警功能
enum AlarmFucntionTypes
{
	ALARM_FUNCTION_TYPE_MOTION_DETECT,	///< 动态检测
	ALARM_FUNCTION_TYPE_BLIND_DETECT,	///< 视屏遮挡
	ALARM_FUNCTION_TYPE_LOSS_DETECT,	///< 视屏丢失
	ALARM_FUNCTION_TYPE_LOCAL_ALARM,	///< 本地报警
	ALARM_FUNCTION_TYPE_NET_ALARM,		///< 网络报警
	ALARM_FUNCTION_TYPE_IP_CONFLICT,	///< IP地址冲突
	ALARM_FUNCTION_TYPE_NET_ABORT,		///< 网络异常
	ALARM_FUNCTION_TYPE_STORAGE_NOTEXIST,	///< 存储设备不存在
	ALARM_FUNCTION_TYPE_STORAGE_LOWSPACE,	///< 存储设备容量不足
	ALARM_FUNCTION_TYPE_STORAGE_FAILURE,	///< 存储设备访问失败
	ALARM_FUNCTION_TYPE_VIEDO_ANALYZE,      ///< 视频分析
	ALARM_FUNCTION_TYPE_NR
};

/// 网络服务功能
enum NetServerTypes
{
	NET_SERVER_TYPES_IPFILTER,		///< 白黑名单
	NET_SERVER_TYPES_DHCP,			///< DHCP功能
	NET_SERVER_TYPES_DDNS,			///< DDNS功能
	NET_SERVER_TYPES_EMAIL,			///< Email功能
	NET_SERVER_TYPES_MULTICAST,		///< 多播功能
	NET_SERVER_TYPES_NTP,			///< NTP功能
	NET_SERVER_TYPES_PPPOE,
	NET_SERVER_TYPES_DNS,
	NET_SERVER_TYPES_ARSP,			///< 主动注册服务
	NET_SERVER_TYPES_3G,            ///< 3G拨号功能
	NET_SERVER_TYPES_MOBILE,			///< 手机监控
	NET_SERVER_TYPES_UPNP,			///< UPNP
	NET_SERVER_TYPES_FTP,			///< FTP
	NET_SERVER_TYPES_WIFI,			///< WIFI
	NET_SERVER_TYPES_ALARM_CENTER,  ///< 告警中心
	NET_SERVER_TYPES_NETPLAT_MEGA,  ///< 互信互通
	NET_SERVER_TYPES_NETPLAT_XINWANG,  ///< 星望
	NET_SERVER_TYPES_NETPLAT_SHISOU,  ///< 视搜
	NET_SERVER_TYPES_NETPLAT_VVEYE,  ///< 威威眼
	NET_SERVER_TYPES_RTSP,           ///< RTSP
	NET_SERVER_TYPES_SHORT_MSG,      ///< 发送报警短信
	NET_SERVER_TYPES_MULTIMEDIA_MSG, ///< 发送带截图的彩信
	NET_SERVER_TYPES_DAS,			///<DAS
	NET_SERVER_TYPES_LOCALSDK_PLATFORM,///<网络平台信息设置
	NET_SERVER_TYPES_GOD_EYE,///<神眼接警中心系统
	NET_SERVER_TYPES_NAT,		///NAT穿透，MTU配置
	NET_SERVER_TYPES_VPN,		///VPN配置
	NET_SERVER_TYPES_MEDIASTREAM,		//流媒体配置
	NET_SERVER_TYPES_NR,
};

/// 预览功能
enum PreviewTypes
{
	PREVIEW_TYPES_TOUR,		///< 轮巡
	PREVIEW_TYPES_TALK,		///< GUI界面配置
	PREVIEW_TYPES_NR
};

///串口类型
enum CommTypes
{
	COMM_TYPES_RS485,			///<485串口
	COMM_TYPES_RS232,			///<232串口
	COMM_TYPES_NR
};

///输入法类型
enum InputMethod
{
	NO_SUPPORT_CHINESE,		//不支持中文输入
	NO_SUPPORT_NR
};

///标签显示
enum TipShow
{
	NO_BEEP_TIP_SHOW,
	NO_FTP_TIP_SHOW,
	NO_EMAIL_TIP_SHOW,
	NO_TIP_SHOW_NR
};

///车载功能
enum MobileDVR
{
	MOBILEDVR_STATUS_EXCHANGE,
	MOBILEDVR_DELAY_SET,
	MOBILEDVR_CARPLATE_SET,
	MOBILEDVR_GPS_TIMING,
	MOBILEDVR_NR
};

///其他功能
enum OtherFunction
{
	OTHER_DOWNLOADPAUSE,		//录像下载暂停功能
	OTHER_USB_SUPPORT_RECORD,	//USB支持录像功能
	OTHER_SD_SUPPORT_RECORD,	//SD支持录像功能
	OTHER_ONVIF_CLIENT_SUPPORT,	//是否支持ONVIF客户端
	OTHER_NR
};

/// 系统功能
struct SystemFunction
{
	bool vEncodeFunction[ENCODE_FUNCTION_TYPE_NR];	///< 编码功能EncodeFunctionTypes
	bool vAlarmFunction[ALARM_FUNCTION_TYPE_NR];	///< 报警功能AlarmFucntionTypes
	bool vNetServerFunction[NET_SERVER_TYPES_NR];	///< 网络服务功能NetServerTypes
	bool vPreviewFunction[PREVIEW_TYPES_NR];		///< 预览功能PreviewTypes
	bool vCommFunction[COMM_TYPES_NR];				///<串口类型CommTypes
	bool vInputMethodFunction[NO_SUPPORT_NR];		///<输入法类型InputMethod
	bool vTipShowFunction[NO_TIP_SHOW_NR];			//标签显示TipShow
	bool vMobileDVRFunction[MOBILEDVR_NR];			//车载功能MobileCar
	bool vOtherFunction[OTHER_NR];					//其他功能OtherFunction
};

//////////////////////////////////////////////////////////////////////////
/// 编码能力级

/// 编码信息
struct EncodeInfo
{
	bool bEnable;			///< 使能项
	int iStreamType;		///< 码流类型，capture_channel_t
	bool bHaveAudio;		///< 是否支持音频
	uint uiCompression;		///< capture_comp_t的掩码
	uint uiResolution;		///< capture_size_t的掩码
};

/// 编码能力
/// 说明： 
/// 1. vCombEncInfo 是组合编码的能力级只有3000系列的设备才有这个能力，5000之后的都没有使用了
/// 2. vEncodeInfo 是各个编码通道的能力信息，只描述上面与ImageSizePerChannel，ExImageSizePerChannel其实是重复的，但考虑到IE的兼容性这个没有拿掉
///    IE处理方式是当ImageSizePerChannel和ExImageSizePerChannel为0时使用vEncodeInfo里头的描述
/// 编码通道主码流支持的分辨率能力取决于2个方面: 1 - 当前通道支持的分辨率即ImageSizePerChannel 2 - 当前通道支持的最高编码能力即nMaxPowerPerChannel
/// 这个能力是主码流和辅码流的总和
/// 编码通道辅码流支持的分辨率能力取决于3个方面: 1 - 当前通道支持的分辨率即ExImageSizePerChannel 2 - 当前通道支持的最高编码能力即nMaxPowerPerChannel
/// 3 - 主码流设置的分辨率下辅码流支持的分辨率即ExImageSizePerChannel
/// iChannelMaxSetSync： 1 - 表示所有通道的分辨率都要一样
struct EncodeAbility
{
	int iMaxEncodePower;		///< 支持的总编码能力
	int iChannelMaxSetSync;		///< 每个通道分辨率是否需要同步 0-不同步, 1 -同步
	uint nMaxPowerPerChannel[N_SYS_CH];		///< 每个通道支持的最高编码能力
	uint ImageSizePerChannel[N_SYS_CH];		///< 每个通道支持的图像分辨率
	uint ExImageSizePerChannel[N_SYS_CH];		///< 每个通道支持的辅码流图像分辨率
	EncodeInfo vEncodeInfo[CAPTURE_CHN_NR];	///< 编码信息
	EncodeInfo vCombEncInfo[CAPTURE_CHN_NR];	///< 组合编码信息
	int	iMaxBps;				///< 支持的总码率大小Kbps
	uint ExImageSizePerChannelEx[N_SYS_CH][CAPTURE_IMAGE_SIZE_EXT_NR];	///< 指定主码流分辨率下每个通道的辅码流支持的图像分辨率
};

/// 区域遮挡能力集
struct BlindDetectFunction
{
	int iBlindConverNum;	///< 区域遮挡块数
};

/// 动检区域能力集
struct MotionDetectFunction
{
	int iGridRow;
	int iGridColumn;
};

/// 支持的DDNS类型
struct DDNSServiceFunction
{
	std::vector<std::string> vDDNSTypes;
};

/// 串口协议
struct CommFunction
{
	std::vector<std::string> vCommProtocols;
};

/// 云台协议
struct PTZProtocolFunction
{
	std::vector<std::string> vPTZProtocols;
};

/// 对讲音频能力集合
struct TalkAudioFormatFunction
{
	AudioInFormatConfigAll audioFormat;
};

/// 语言支持
struct MultiLangFunction
{
	std::vector<std::string> vMultiLanguage;
};

/// 视频制式支持
struct MultVstdFunction
{
	std::vector<std::string> vMultiVstd;
};

///网络优先级能力
struct NetOrderFunction
{
	bool bNetOrder;
};

///网络链接能力
struct NetConnectAbility
{
	int nCurTcpNum;
	int nMaxTcpNum;
	int nCurNatNum;
	int nMaxNatNum;
};
///车辆状态数
struct CarStatusNum
{
	int iCarStatusNum;
};

/// 支持的VGA分辨率列表
struct VGAResolutionAbility
{
	std::vector<std::string> vAbilityVGA;
};
#endif
