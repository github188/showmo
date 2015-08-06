#ifndef __EXCHANGE_AL_EXCHANAGE_KIND_H__
#define __EXCHANGE_AL_EXCHANAGE_KIND_H__

#include "../Types/Defs.h"
#include <vector>
#include <string>

/// 配置操作相关错误号
enum ConfigErrorNo
{
	CONFIG_OPT_RESTART = ERROR_BEGIN_CONFIG + 1,		///< 需要重启应用程序
	CONFIG_OPT_REBOOT = ERROR_BEGIN_CONFIG + 2,			///< 需要重启系统
	CONFIG_OPT_FILE_ERROR = ERROR_BEGIN_CONFIG + 3,		///< 写文件出错
	CONFIG_OPT_CAPS_ERROR = ERROR_BEGIN_CONFIG + 4,		///< 特性不支持
	CONFIG_OPT_VALIT_ERROR = ERROR_BEGIN_CONFIG + 5,	///< 验证失败
	CONFIG_OPT_NOT_EXSIST = ERROR_BEGIN_CONFIG + 8,		///< 配置不存在
	CONFIG_OPT_PARSE_FAILED = ERROR_BEGIN_CONFIG + 6,	///< 配置解析出错
};

/// 默认配置种类
enum DefaultConfigKinds
{
	DEFAULT_CFG_GENERAL,			// 普通配置
	DEFAULT_CFG_ENCODE,				// 编码配置
	DEFAULT_CFG_RECORD,				// 录像配置
	DEFAULT_CFG_NET_SERVICE,		// 网络服务
	DEFAULT_CFG_NET_COMMON,			// 通用网络
	DEFAULT_CFG_ALARM,				// 报警
	DEFAULT_CFG_PTZCOMM,			// 云台，串口
	DEFAULT_CFG_USERMANAGER,		// 用户管理
	DEFAULT_CFG_PREVIEW,			// 预览配置
	DEFAULT_CFG_CAMERA_PARAM,		// 网络摄像头配置
	DEFAULT_CFG_END
};

/// 根据默认配置种类DefaultConfigKinds取得相应的名称
const char *getDefaultKindName(int iDefaultKind);

/// 根据配置名得到枚举常量
int getDefaultKindIndex(const char *iDefaultName);

/// 配置种类
enum ConfigKinds
{
	CFG_ENCODE,			///< 编码配置
	CFG_WIDEOWIDGET,	///< 视频物件
	CFG_VIDEOCOLOR,		///< 视频颜色
	CFG_RECORD,			///< 录像配置
	CFG_MOTIONDETECT,	///< 动检配置
	CFG_BLINDDETECT,	///< 遮挡检测
	CFG_LOSSDETECT,		///< 视频丢失
	CFG_LOCALALAEM,		///< 本地报警
	CFG_NETALAEM,		///< 网络报警
	CFG_NETIPCONFLICT,	///< IP地址冲突
	CFG_NETABORT,		///< 网络异常
	CFG_STORAGENOTEXIST,	///< 存储设备不存在
	CFG_STORAGELOWSPACE,	///< 存储设备容量不足
	CFG_STORAGEFAILURE,		///< 存储设备访问失败
	CFG_PTZALARM,			///< 云台报警
	CFG_NETCOMMON,			///< 通用网络配置
	CFG_NETIPFILTER,		///< 白黑名单
	CFG_NETDHCP,		///< DHCP
	CFG_NETDDNS,		///< DDNS
	CFG_NETEMAIL,		///< EMail
	CFG_NETMULTICAST,	///< 多播
	CFG_NETNTP,			///< 时间同步服务
	CFG_NETPPPOE,		///< PPPOE
	CFG_NETDNS,			///< DNS
	CFG_COMM,			///< 串口配置
	CFG_PTZ,			///< 云台配置
	CFG_PTZPRESET,		///< 云台预置点配置
	CFG_PTZTOUR,		///<
	CFG_TOUR,			///< 轮巡配置
	CFG_GUISET,			///< GUISet配置
	CFG_TVADJUST,		///< TV调节
	CFG_AUDIOINFORMAT,	///< 对讲配置
	CFG_PLAY,			///< 回放
	CFG_GENERAL,		///< 普通配置
	CFG_LOCATION,		///< 本地化配置
	CFG_AUTOMAINTAIN,	///< 自动维护配置
	CFG_CHANNELTITLE,	///< !! 通道名称配置，特殊
	CFG_SNAPSHOT,		///< 图片存储配置
	CFG_COMBINEENCODE,  ///< 组合编码
	CFG_COMBINEENCODEMODE,  ///< 组合编码模式
	CFG_NETFTP,			///< FTP配置
	CFG_NETARSP,		///< ARSP配置
	CFG_NETPLAT_MEGANET, ///< HXHT平台接入
	CFG_NETPLAT_MEGAMOTION, ///< HXHT平台保存的动检参数
	CFG_ABILITY_SUPPORTLANGUAGE,	///< 支持的语言
	CFG_NETDECORDER,	///< 网络解码器配置
	CFG_3GNET,			///< 3G网络配置
	CFG_MOBILE,         ///< 手机监控配置
	CFG_ABILITY_SUPPORTVSTD,	///< 支持的视频制式
	CFG_VIDEOCHANNEL,	///< 支持的视频通道配置
	CFG_CARPLATE,		///< 车牌号配置
	CFG_NETUPNP,           ///< UPNP配置
	CFG_ABILITY_DEVDESC,	///< 设备描述配置
	CFG_STORAGE_POSITION,
	CFG_WIFI,          ///< WIFI
	CFG_SYSTEM_STATE,  ///< 记录系统当前状态
	CFG_VIDEOOUT,
	CFG_ABILITY_VGARESOLUTION,	///< 支持的VGA分辨率列表
	CFG_ABILITY_THEMELIST,		///< 支持的GUI主题列表
	CFG_ENCODE_SIMPLIFY,		///< 最新简化版本编码配置
	CFG_RS485,                  ///< 485设备配置
	CFG_NETALARMSERVER,         ///< 报警中心
	CFG_ALARMOUT,               ///< 报警输出
	CFG_NETPLAT_XINWANG,        ///< 新望平台
	CFG_NETPLAT_SHISOU,        ///< 视搜平台
	CFG_NETPLAT_VVEYE,        ///< VVEYE平台
	CFG_MEDIA_WATERMARK,	///< 数字水印配置
	CFG_NETDECORDER_V2,		///< 网络解码器配置V2版本
	CFG_NETPLAT_WELLSUN,    ///< 惠尔森平台
	CFG_ABILITY_ENOCDE,		///< 编码能力集
	CFG_NET_RTSP,		    ///< RTSP配置
	CFG_ABILITY_SERIALNO,		///< 设置序列号
	CFG_NETDECORDER_V3,		//数字通道配置
	CFG_NETDECORDER_CHNMODE,  //通道模式配置
	CFG_NETDECORDER_CHNSTATUS,  //通道状态
	CFG_PRODUCE_TEST,		///<生产测试配置
	CFG_NET_POS,			//POS机配置
	CFG_CAMERA_PARAM,         //网络摄像头参数配置
	CFG_OSD_WIDGET,           //OSD信息位置与状态配置
	CFG_NET_DAS,
	CFG_NETPLAT_NANRUI,       //楠瑞平台接入
	CFG_VOLUME,					//音量调节
	CFG_SPOT,                 //SPOT输出
	CFG_PHONE_SHORT_MSG,      //手机短信
	CFG_PHONE_MULTIMEDIA_MSG, //手机彩信
	CFG_CAR_INPUT_EXCHANGE,   //外部信息输入与车辆状态的对应关系
	CFG_CAR_DELAY_TIME,       //车载系统延时配置
	CFG_NET_ORDER,            //网络优先级
	CFG_LOCALSDK_NET_PLATFORM, //网络平台信息设置
	CFG_GPS_TIMING,           //GPS校时相关配置
	CFG_CAR_DISPLAY,          //车载显示配置
	CFG_VIDEO_ANALYZE,        //视频分析(智能DVR)
	CFG_GODEYE_ALARM,		//神眼接警中心系统
	CFG_NAT_STATUS_INFO,	//NAT状态信息
	CFG_ENCODE_STATICPARAM,	//编码器静态参数
	CFG_LOSS_SHOW_STR,		//视频丢失显示字符串
	CFG_DIGMANAGER_SHOW,	//通道管理显示配置
	CFG_VIDEOOUT_PRIORITY,   //显示HDMI VGA优先级别配置
	CFG_NAT,					//NAT功能使能与MTU值配置
	CFG_ANALYZE_LINK,			//智能分析联动配置
	CFG_GUIDE,					//向导配置
	CFG_VIDEOSEQUE,			//CMS风格预览画面配置
	CFG_AUTOLOGIN,				//自动登录配置
	CFG_VPN,
	CFG_RESUME_PTZ_STATE,	// 恢复云台状态相关配置
	CFG_CAPTURE_PANORAMIC,	// 全景参数设置
	CFG_CAMERA_DM8127,		// DM8127平台对应的摄像头参数配置
	CFG_CHAIN,				// 联动
	CFG_NET_MEDIA_STREAM,		//网络流媒体配置
	CFG_LEDLIGHT,			//led灯配置
	CFG_WIFISTATUS,			//Wifi状态
	CFG_AP_SWAP_WIFI,		//AP切换到WIFI
	CFG_NR,
};

/// 根据配置种类ConfigKinds取得相应名称
const char *getConfigName(int iConfigKind);

/// 根据配置名称得到配置种类常量
int getConfigIndex(const char *sConfigKind);

enum AbilityKinds
{
	ABILITY_SYSTEM_FUNCTION,	///< 系统功能
	ABILITY_ENCODE,				///< 编码功能
	ABILITY_BLIND,				///< 遮挡检测功能
	ABILITY_MOTION,				///< 动态检测功能
	ABILITY_DDNS_TYPES,			///< DDNS服务类型
	ABILITY_COMM_PROTOCOLS,		///< 串口协议
	ABILITY_PTZ_PROTOCOLS,		///< 云台协议
	ABILITY_TALK_ATTRIBUTE,		///< 对讲属性
	ABILITY_MULTI_LANG,			///< 允许设置的语言
	ABILITY_LANG_LIST,			///< 实际支持的语言集
	ABILITY_MULTI_VSTD,			///< 允许设置的视频制式
	ABILITY_VSTD_LIST,			///< 实际支持的视频制式
	ABILITY_UART_PROTOCOLS,		///<458协议
	ABILITY_CAMERA,             ///< 摄像头相关能力集
	ABILITY_NETORDER,           //网络优先级设置能力
	ABILITY_INTELLIGENT,	//智能分析能力集
	ABILITY_NET_CONNECT,	//网络链接能力
	ABILITY_CARSTATUSNUM,	//车载车辆状态数
	ABILITY_VGARESOLUTION,// 支持的VGA分辨率能力集
	ABILITY_KIND_NR,
};

/// 根据能力集获取相应的字符串
const char *getAbilityName(int iAbilityKind);

int getAbilityIndex(const char *sAbilityKind);

/// 系统信息种类
enum DeviceInfoKinds
{
	DEVICE_INFO_SYSTEM,		///< 系统信息
	DEVICE_INFO_STORAGE,	///< 存储设备信息
	DEVICE_INFO_WORKSTATE,	///< 工作状态信息
	DEVICE_INFO_WIFI_AP,    ///< WIFI AP信息
	DEVICE_OEM_INFO,
	DEVICE_INFO_NR,
};

const char *getDeviceInfoName(int iDeviceInfoKind);

int getDeviceInfoIndex(const char *sDeviceInfoKind);

/// 基本系统操作
enum SystemOperations
{
	OPERATION_MACHINE,			///< 关机，重启操作
	OPERATION_DEFAULT_CONFIG,	///< 默认配置操作
	OPERATION_PTZ,				///< 云台控制
	OPERATION_MONITOR,			///< 监视控制
	OPERATION_PLAYBACK,			///< 回放控制
	OPERATION_TALK,				///< 语音对讲控制
	OPERATION_DISK_MANAGER,		///< 磁盘管理
	OPERATION_LOG_MANAGER,		///< 日志管理
	OPERATION_SYSTEM_UPGRADE,	///< 系统升级
	OPERATION_FILE_QUERY,		///< 文件查询
	OPERATION_LOG_QUERY,		///< 日志查询
	OPERATION_TIME_SETTING,		///< 设置系统时间
	OPERATION_NET_KEYBOARD,		///< 网络键盘
	OPERATION_NET_ALARM,		///< 网络告警
	OPERATION_SNAP, 		    ///< 网络手动抓图
	OPERATION_TRANS, 			///< 透明串口
	OPERATION_UPDATA,			///< 上传数据
	OPERATION_TIME_SETTING_NORTC,///<对于没有rtc的设备配置时间
	OPERATION_CPCDATA,		///<人数统计查询
	OPERATION_NR,
};

const char *getOperationName(int iOperationKind);

int getOperationIndex(const char *sOperationKind);

typedef enum app_event_code
{
	appEventInit = 0,
	appEventAlarmLocal = 1,
	appEventAlarmNet,
	appEventAlarmManual,
	appEventVideoMotion,
	appEventVideoLoss,
	appEventVideoBlind,
	appEventVideoTitle,
	appEventVideoSplit,
	appEventVideoTour,
	appEventStorageNotExist,
	appEventStorageFailure,
	appEventStorageLowSpace,
	appEventNetAbort,
	appEventComm,
	appEventStorageReadErr,
	appEventStorageWriteErr,
	appEventIPConflict,
	appEventAlarmEmergency,
	appEventDecConnect,
	appEventUpgrade,
	appEventBackup,
	appEventShutdown,
	appEventReboot,
	appEventNewFile,
	appEventVideoAnalyze,
	appEventAll
}appEventCode;

const char *getEventName(int iOperationKind);

int getEventIndex(const char *sOperationKind);

// 日志类型
enum LogType
{
	logTypeAll,
	logTypeSystem,	/// 系统日志
	logTypeConfig,	///
	logTypeStorage,
	logTypeAlarm,
	logTypeRecord,
	logTypeAccount,
	logTypeAccess,
	logTypeNr,
};

/// 得到日志类型名称
const char *getLogTypeName(int iLogType);

enum LogItemType
{
	logItemReboot,
	logItemShutDown,
	logItemClearLog,
	logItemModifyTime,
	logItemZeroBitrate,
	logItemUpgrade,
	logItemException,
	logItemUpdate,
	logItemSetTime,
	logItemSaveConfig,
	logItemSetDriverType,
	logItemClearDriver,
	logItemStorageDeviceError,
	logItemDiskChanged,
	logItemEventStart,
	logItemEventStop,
	logItemRecord,
	logItemLogIn,
	logItemLogOut,
	logItemAddUser,
	logItemDeleteUser,
	logItemModifyUser,
	logItemModifyPassword,
	logItemAddGroup,
	logItemDeleteGroup,
	logItemModifyGroup,
	logItemAccountRestore,
	logItemFileAccessError,
	logItemFileSearch,
	logItemFileAccess,
	logItemRecoverTime,
	logItemNr,
};

const char *getLogData(int iLogItemKind);
int getLogType(const char *sLog);


//系统调试相关
enum SystemDebug
{
	DEBUG_CAMERA,
	DEBUG_SHELL,
	DEBUG_CAMERA_SAVE_CMD,
	DEBUG_NR
};

const char *getDebugName(int iDebugKind);
int getDebugKind(const char *pDebugName);

#endif

