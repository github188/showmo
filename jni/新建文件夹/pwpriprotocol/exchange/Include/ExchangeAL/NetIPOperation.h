#ifndef __EXCHANGE_AL_NETIP_OPERATION_H__
#define __EXCHANGE_AL_NETIP_OPERATION_H__

#include "ExchangeKind.h"
#include "MediaExchange.h"
#include "../Types/Defs.h"
#include "../Infra/Time.h"
#include <string>
#include <vector>

/// 网络操作错误号
enum NetOperationErrorNo
{
	NET_OPERATION_COMMAND_INVALID = ERROR_BEGIN_NETOPERATION + 1,			///< 命令不合法
	NET_OPERATION_TALK_ALAREADY_START = ERROR_BEGIN_NETOPERATION + 2,		///< 对讲已经开启
	NET_OPERATION_TALK_NOT_START = ERROR_BEGIN_NETOPERATION + 3,			///< 对讲未开启
	NET_OPERATION_UPGRADE_ALAREADY_START = ERROR_BEGIN_NETOPERATION + 10,   ///< 已经开始升级
	NET_OPERATION_UPGRADE_NOT_START = ERROR_BEGIN_NETOPERATION + 11,		///< 未开始升级
	NET_OPERATION_UPGRADE_DATA_ERROR = ERROR_BEGIN_NETOPERATION + 12,		///< 升级数据错误
	NET_OPERATION_UPGRADE_FAILED = ERROR_BEGIN_NETOPERATION + 13,			///< 升级失败
	NET_OPERATION_UPGRADE_SUCCESS = ERROR_BEGIN_NETOPERATION + 14,			///< 升级成功
	NET_OPERATION_SETDEFAULT_FAILED = ERROR_BEGIN_NETOPERATION + 20,		///< 还原默认失败
	NET_OPERATION_SETDEFAULT_REBOOT = ERROR_BEGIN_NETOPERATION + 21,		///< 需要重启设备
	NET_OPERATION_SETDEFAULT_VALIDATEERROR = ERROR_BEGIN_NETOPERATION + 22,	///< 默认配置非法
};

/// 监视动作
enum MonitorAction
{
	MONITOR_ACTION_START,
	MONITOR_ACTION_STOP,
	MONITOR_ACTION_CLAIM,	/// 标识连接信息,主连接通过这些信息来认领媒体连接
	MONITOR_ACTION_PAUSE,   /// 暂停
	MONITOR_ACTION_CONTINUE,   /// 恢复
	MONITOR_ACTION_REQUEST,
	MONITOR_ACTION_NR
};

/// 传输模式
enum MonitorTansMode
{
	MONITOR_TRANSMODE_TCP,		///< TCP传输
	MONITOR_TRANSMODE_UDP,		///< UDP传输
	MONITOR_TRANSMODE_MCAST,	///< 多播
	MONITOR_TRANSMODE_RTP,		///< RTP传输
	MONITOR_TRANSMODE_NR
};

/// 组合编码模式
enum CombinType
{
	COMBIN_NONE,
	COMBIN_1,
	COMBIN_2,
	COMBIN_3,
	COMBIN_4,
	COMBIN_5,
	COMBIN_6,
	COMBIN_7,
	COMBIN_8,
	COMBIN_9,
	COMBIN_10,
	COMBIN_11,
	COMBIN_12,
	COMBIN_13,
	COMBIN_14,
	COMBIN_15,
	COMBIN_16,
	COMBIN_1_4,
	COMBIN_5_8,
	COMBIN_9_12,
	COMBIN_13_16,
	COMBIN_1_8,
	COMBIN_9_16,
	COMBIN_1_9,
	COMBIN_8_16,
	COMBIN_1_16,
	CONNECT_ALL = 0xFF
};

/// 监视控制
struct MonitorControl
{
	int iAction;		///< 监视动作MonitorAction
	int iChannel;		///< 监视通道
	int iStreamType;	///< 监视码流类型
	int iTransMode;		///< 监视传输模式
	int iCombinType;	///< 组合编码模式
};

/// 回放动作
enum PlayBackAction
{
	PLAY_BACK_ACTION_START,		/// 开始回放 
	PLAY_BACK_ACTION_STOP,		/// 停止回放,不关闭连接
	PLAY_BACK_ACTION_PAUSE,		/// 暂停回放
	PLAY_BACK_ACTION_CONTINUE,	/// 继续回放
	PLAY_BACK_ACTION_LOCATE,	/// 回放定位
	PLAY_BACK_ACTION_EOF,		/// 回放文件结束,由PU通知CU
	PLAY_BACK_ACTION_CLOSE,		/// 关闭回放, 会断开连接
	PLAY_BACK_ACTION_CLAIM,		/// 标识连接信息,主连接通过这些信息来认领媒体连接
	PLAY_BACK_ACTION_DOWNLOADSTART,	/// 录像下载开始
	PLAY_BACK_ACTION_DOWNLOADSTOP,	/// 录像下载结束
	PLAY_BACK_ACTION_FAST,	    /// 加速回放
	PLAY_BACK_ACTION_SLOW,	    /// 加速回放
	PLAY_BACK_ACTION_REQUEST,	///回放请求，主动注册用
	PLAY_BACK_ACTION_DOWNLOAD_REQUEST,///下载请求，主动注册用
	PLAY_BACK_ACTION_DOWNLOAD_PAUSE,	///下载暂停
	PLAY_BACK_ACTION_DOWNLOAD_CONTINUE,///下载继续
	PLAY_BACK_ACTION_NR
};

/// 回放模式
enum PlaybackMode
{
	PLAYBACK_BY_NAME,		///< 按文件名回放
	PLAYBACK_BY_TIME,		///< 按时间回放
	PLAYBACK_MODE_NR
};

/// 回放控制
struct PlayBackControl
{
	int iAction;	///< 回放动作
	int iTransMode;	///< 传输模式
	std::string sFileName;
	SystemTime stStartTime;	///< 开始时间
	SystemTime stEndTime;		///< 结束时间
	int iPlayMode;	///< 回放模式，见PlaybackMode
	int iValue;	    ///< 部分回放动作有参数带入
};

/// 设置模式状态
enum PtzControlSetPatternStatusTypes
{
	PTZ_PATTERN_STATUS_START,		///< 设置模式开始
	PTZ_PATTERN_STATUS_STOP,		///< 设置模式结束
	PTZ_PATTERN_STATUS_NR,
};

/// 模式状态
enum PtzControlPatternTypes
{
	PTZ_PATTERN_RUN,		///< 运行模式
	PTZ_PATTERN_STOP,		///< 停止模式
	PTZ_PATTERN_CLEAR,		///< 清除模式
	PTZ_PATTERN_NR,
};

/// 菜单操作
enum PtzMenuOperator
{
	PTZ_MENU_OPT_ENTER,		///< 进入菜单
	PTZ_MENU_OPT_LEAVE,		///< 退出菜单
	PTZ_MENU_OPT_OK,		///< 确认
	PTZ_MENU_OPT_CANCEL,	///< 取消
	PTZ_MENU_OPT_UP,		///< 上
	PTZ_MENU_OPT_DOWN,		///< 下
	PTZ_MENU_OPT_LEFT,		///< 左
	PTZ_MENU_OPT_RIGHT,		///< 右
	PTZ_MENU_OPT_NR,
};

/// 云台操作控制命令
enum PtzOperationCommand
{
	PTZ_OPERATION_DIRECTION_UP,		///< 方向
	PTZ_OPERATION_DIRECTION_DOWN,
	PTZ_OPERATION_DIRECTION_LEFT,
	PTZ_OPERATION_DIRECTION_RIGHT,
	PTZ_OPERATION_DIRECTION_LEFTUP,
	PTZ_OPERATION_DIRECTION_LEFTDOWN,
	PTZ_OPERATION_DIRECTION_RIGHTUP,
	PTZ_OPERATION_DIRECTION_RIGHTDOWN,
	PTZ_OPERATION_ZOOMWIDE,			///< 变倍 
	PTZ_OPERATION_ZOOMTILE,
	PTZ_OPERATION_FOCUSFAR,			///< 聚焦 
	PTZ_OPERATION_FOCUSNEAR,
	PTZ_OPERATION_IRISLARGE,		///< 光圈 
	PTZ_OPERATION_IRISSMALL,
	PTZ_OPERATION_ALARM,			///< 报警功能 
	PTZ_OPERATION_LIGHTON,			///< 灯光开
	PTZ_OPERATION_LIGHTOFF,			///< 灯光关
	PTZ_OPERATION_SETPRESET,		///< 设置预置点 
	PTZ_OPERATION_CLEARPRESET,		///< 清除预置点 
	PTZ_OPERATION_GOTOPRESET,		///< 转至预置点 
	PTZ_OPERATION_AUTOPANON,		///< 水平开始 
	PTZ_OPERATION_AUTOPANOFF,		///< 水平结束 
	PTZ_OPERATION_SETLIMITLEFT,		///< 设置左边界 
	PTZ_OPERATION_SETLIMITRIGHT,	///< 设置右边界
	PTZ_OPERATION_AUTOSCANON,		///< 自动扫描开始
	PTZ_OPERATION_AUTOSCANOFF,		///< 自动扫描停止 
	PTZ_OPERATION_ADDTOUR,			///< 增加巡航点 
	PTZ_OPERATION_DELETETOUR,		///< 删除巡航点 
	PTZ_OPERATION_STARTTOUR,		///< 开始巡航 
	PTZ_OPERATION_STOPTOUR,			///< 结束巡航 
	PTZ_OPERATION_CLEARTOUR,		///< 删除巡航 
	PTZ_OPERATION_POSITION,			///< 快速定位 
	PTZ_OPERATION_AUX,				///< 辅助开关 
	PTZ_OPERATION_MENU,				///< 球机菜单操作
	PTZ_OPERATION_FLIP,				///< 镜头翻转
	PTZ_OPERATION_RESET,			///< 云台复位
	PTZ_OPERATION_OPT_NUM			///< 操作的个数 
};

/// 云台辅助功能
enum PtzAuxStatus
{
	PTZ_AUX_ON,		///< 开
	PTZ_AUX_OFF,	///< 关
	PTZ_AUX_NR,		
};

struct PTZControl
{
	int iCommand;				///< 云台操作命令

	/// 云台操作参数
	struct OperationParam
	{	
		int iChannel;	///< 云台控制的通道，通道从0开始
		int iStep;		///< 步长
		int iPreset;	///< 预置点
		int iTour;		///< 路线编号
		int iPattern;	///< 模式,开始模式，停止模式，清除模式
		int iMenuOpts;		///< 菜单操作，进入，退出，确认，取消，上，下，左，右
		struct 
		{
			int iNumber;
			int iStatus;	/// 功能状态，见PtzAuxStatus
		} AUX;				///< 辅助功能
	} parameter;
};

/// 机器操作动作
enum MachineAction
{
	MACHINE_ACTION_REBOOT,		///< 重启机器
	MACHINE_ACTION_SHUTDOWN,	///< 关闭机器
	MACHINE_ACTION_NR,
};

/// 重启，关闭操作
struct MachineControl
{
	int iAction;	///< 机器控制动作，见MachineAction
};

/// 默认配置操作
struct DefaultConfigControl
{
	bool vDefaultConfig[DEFAULT_CFG_END];
};

/// 语音对讲控制
enum TalkControlTypes
{
	TALK_CONTROL_TYPES_START,	///< 开始对讲
	TALK_CONTROL_TYPES_STOP,	///< 停止对讲
	TALK_CONTROL_TYPES_CLAIM,		/// 标识连接信息,主连接通过这些信息来认领媒体连接
	TALK_CONTROL_TYPES_REQUEST,
	TALK_CONTROL_TYPES_NR,
};

/// 语音对讲
struct TalkControl
{
	int iAction;
	AudioInFormatConfig afAudioFormat;
};

/// 存储设备控制类型
enum StorageDeviceControlTypes
{
	STORAGE_DEVICE_CONTROL_SETTYPE,		///< 设置类型
	STORAGE_DEVICE_CONTROL_RECOVER,		///< 恢复错误
	STORAGE_DEVICE_CONTROL_PARTITIONS,	///< 分区操作
	STORAGE_DEVICE_CONTROL_CLEAR,		///< 清除操作
	STORAGE_DEVICE_CONTROL_NR,
};

/// 清除磁盘数据类型
enum StorageDeviceClearTypes
{
	STORAGE_DEVICE_CLEAR_DATA,			///< 清除录像数据
	STORAGE_DEVICE_CLEAR_PARTITIONS,	///< 清除分区
	STORAGE_DEVICE_CLEAR_NR,
};

/// 存储设备控制
struct StorageDeviceControl
{
	int iAction;	///< 见enum StorageDeviceControlTypes
	int iSerialNo;	///< 磁盘序列号
	int iPartNo;    ///< 分区号
	int iType;		///< enum StorageDeviceClearTypes或者DRIVER_TYPE
	int iPartSize[2/*MAX_DRIVER_PER_DISK*/];	///< 各个分区的大小
};

/// 日志管理种类
enum LogControlActionTypes
{
	LOG_CONTROL_REMOVEALL,		/// 删除所有种类的日志
	LOG_CONTROL_NR,
};

/// 日志管理
struct LogControl
{
	int iAction;
};

/// 升级动作类型
enum UpgradeActionTypes
{
	UPGRADE_ACTION_TYPES_START,		///< 开始升级
	UPGRADE_ACTION_TYPES_ABORT,		///< 中止升级
	UPGRADE_ACTION_TYPES_NR
};

/// 升级类型
enum UpgradeTypes
{
	UPGRADE_TYPES_SYSTEM,	///< 升级系统
	UPGRADE_TYPES_NR,
};

/// 系统升级
struct UpgradeControl
{
	int iAction;
	int iType;
};

/// 升级信息获取
struct UpgradeInfo
{
	std::string strSerial;
	std::string strHardware;
	std::string strVendor;
	uint uiLogoArea[2];
};

/// 文件查询条件
struct SearchCondition
{
	int iChannel;			///< 录像通道号掩码
	char sType[24];			///< 查询文件类型
	char sEvent[32];			///< 查询事件类型
	uint uiDriverTypeMask;		///< 查询驱动器类型掩码
	SystemTime stBeginTime;	///< 查询开始时间
	SystemTime stEndTime;		///< 查询结束时间
};

enum 
{
	MAX_SEARCHED_FILES = 64,	///< 一次最大的查询文件数量
};

/// 文件列表信息
struct FileList
{
	int iNumFiles;		///< 文件数量
	struct FileInfo
	{
		int iDiskNo;	///< 磁盘物理序号
		int iSerialNo;	///< 磁盘序列号
		uint uiFileLength;			///< 文件长度，以K为单位
		char sFileName[108];		///< 文件名
		SystemTime stBeginTime;	///< 文件开始时间
		SystemTime stEndTime;		///< 文件结束时间
	} Files[MAX_SEARCHED_FILES];
};

//按时间段查询
struct SearchByTime
{
	int nHighChannel;			///< 33~64录像通道号掩码
	int nLowChannel;			///< 1~32录像通道号掩码
	char sType[24];			///< 查询文件类型
	char sEvent[32];			///< 查询事件类型
	SystemTime stBeginTime;	    ///< 查询开始时间
	SystemTime stEndTime;		///< 查询结束时间
	int    iSync;               ///< 是否需要同步
};


//每个通道的录像信息
struct SearchByTimeInfo
{
	int iChannel;			    ///< 录像通道号
	///< 录像记录用720个字节的5760位来表示一天中的1440分钟
	///< 0000:无录像 0001:F_COMMON 0002:F_ALERT 0003:F_DYNAMIC 0004:F_CARD 0005:F_HAND
	uchar cRecordBitMap[720];
};

struct SearchByTimeResult
{
	int nInfoNum;			          ///< 通道的录像记录信息个数
	SearchByTimeInfo ByTimeInfo[N_SYS_CH];    ///< 通道的录像记录信息
};

/// 日志查询种类
enum LOG_SEARCH_KIND
{
	LOG_SEARCH_KIND_TYPE_ALL,		///< 所有日志类型
	LOG_SEARCH_KIND_SYSTEM,			///< 系统日志
	LOG_SEARCH_KIND_TYPE_CONFIG,	///< 配置日志
	LOG_SEARCH_KIND_TYPE_STORAGE,	///< 存储日志
	LOG_SEARCH_KIND_TYPE_ALAEM,		///< 报警日志
	LOG_SEARCH_KIND_TYPE_RECORD,	///< 录像日志
	LOG_SEARCH_KIND_TYPE_ACCOUNT,	///< 用户管理
	LOG_SEARCH_KIND_TYPE_FILE,		///< 问价操作
	LOG_SEARCH_KIND_TYPE_NR,
};

/// 日志查询条件
struct LogSearchCondition
{
	int iType;					///< 日志类型,见LOG_SEARCH_KIND
	int iLogPosition;			///< 从上次查询的结束时的日志指针
	SystemTime stBeginTime;	///< 查询日志开始时间
	SystemTime stEndTime;		///< 查询日志结束时间
};

enum 
{
	MAX_RETURNED_LOGLIST = 128,	///< 一次查询返回最多的日志条数
};

struct LogList
{
	int iNumLog;
	struct LogItem
	{
		char sType[24];	///< 日志类型
		char sUser[32];	///< 日志用户
		char sData[68];	///< 日志数据
		SystemTime stLogTime;	///< 日志时间
		int iLogPosition;
	} Logs[MAX_RETURNED_LOGLIST];
};

/// 时间动作类型
enum EventActionTypes
{
	EVENT_ACTION_START  = 0,		// 事件开始
	EVENT_ACTION_STOP,				// 事件结束
	EVENT_ACTION_CONFIG,			// 事件配置变化，不作为接收参数
	EVENT_ACTION_LATCH,				// 事件延时结束，不作为发起参数
	EVENT_ACTION_NR,
};

/// 报警事件码
enum EventCodeTypes
{
	EVENT_CODE_INIT = 0,
	EVENT_CODE_LOCAL_ALARM = 1,
	EVENT_CODE_NET_ALARM,
	EVENT_CODE_MANUAL_ALARM,
	EVENT_CODE_VIDEO_MOTION,
	EVENT_CODE_VIDEO_LOSS,
	EVENT_CODE_VIDEO_BLIND,
	EVENT_CODE_VIDEO_TITLE,
	EVENT_CODE_VIDEO_SPLIT,
	EVENT_CODE_VIDEO_TOUR,
	EVENT_CODE_STORAGE_NOT_EXIST,
	EVENT_CODE_STORAGE_FAILURE,
	EVENT_CODE_LOW_SPACE,
	EVENT_CODE_NET_ABORT,
	EVENT_CODE_COMM,
	EVENT_CODE_STORAGE_READ_ERROR,
	EVENT_CODE_STORAGE_WRITE_ERROR,
	EVENT_CODE_NET_IPCONFLICT,
	EVENT_CODE_ALARM_EMERGENCY,
	EVENT_CODE_DEC_CONNECT,
	EVENT_CODE_NR,
};

struct AlarmInfo
{
	int nChannel;
	int iEvent;
	int iStatus;
	SystemTime SysTime;
};


/// 按键值, 不能随便更改
enum NetKeyBoardValue
{
	NET_KEY_0, NET_KEY_1, NET_KEY_2, NET_KEY_3, NET_KEY_4, NET_KEY_5, NET_KEY_6, NET_KEY_7, NET_KEY_8, NET_KEY_9,
	NET_KEY_10, NET_KEY_11, NET_KEY_12, NET_KEY_13, NET_KEY_14, NET_KEY_15, NET_KEY_16, NET_KEY_10PLUS,
	NET_KEY_UP = 20, NET_DOWN, NET_LEFT, NET_RIGHT, NET_KEY_SHIFT, NET_KEY_PGUP, NET_KEY_PGDN, NET_KEY_RET, NET_KEY_ESC, NET_KEY_FUNC,
	NET_KEY_PLAY, NET_KEY_BACK, NET_KEY_STOP, NET_KEY_FAST, NET_KEY_SLOW, NET_KEY_NEXT, NET_KEY_PREV,
	NET_KEY_REC = 40, NET_KEY_SEARCH, NET_KEY_INFO, NET_KEY_ALARM, NET_KEY_ADDR, NET_KEY_BACKUP, NET_KEY_SPLIT, NET_KEY_SPLIT1, NET_KEY_SPLIT4, NET_KEY_SPLIT8,
	NET_KEY_SPLIT9, NET_KEY_SPLIT16, NET_KEY_SHUT, NET_KEY_MENU,NET_KEY_SPLIT25,NET_KEY_SPLIT36,
	NET_KEY_PTZ = 60, NET_KEY_TELE, NET_KEY_WIDE, NET_KEY_IRIS_SMALL, NET_KEY_IRIS_LARGE, NET_KEY_FOCUS_NEAR, NET_KEY_FOCUS_FAR, NET_KEY_BRUSH, NET_KEY_LIGHT, NET_KEY_SPRESET,
	NET_KEY_GPRESET, NET_KEY_DPRESET, NET_KEY_PATTERN, NET_KEY_SCAN, NET_KEY_AUTOTOUR, NET_KEY_AUTOPAN,
};

/// 按键状态
enum NetKeyBoardState
{
	NET_KEYBOARD_KEYDOWN,	// 按键按下
	NET_KEYBOARD_KEYUP,		// 按键松开
};

struct NetKeyBoardData
{
	int iValue;
	int iState;
};

/// 网络报警
struct NetAlarmInfo
{
	int iEvent;   //告警事件类型，目前未使用
	int iState;   //每bit表示一个通道,bit0:第一通道,0-无报警 1-有报警, 依次类推
};

/// 网络手动抓图
struct NetSnap
{
	int iChannel;  //通道号
};

enum TransCommOpr
{
	TRANS_COMM_START,
	TRANS_COMM_STOP,
};

enum TransCommType
{
	TRANS_COMM_RS232,
	TRANS_COMM_RS485,
};

struct TransparentComm
{
	int iTCommType;
	int iTCommOpr;
};

enum UpLoadDataType  //上传数据类型
{
	VEHICLE_INFO,	//车载信息
};

struct UpLoadData		//上传数据
{
	int InfoType;
};

enum NatStatusType
{
	NAT_STATUS_DISENABLE,
	NAT_STATUS_PROBING,
	NAT_STATUS_CONNECTING,
	NAT_STATUS_CONNECTED,
};

struct NatStatusInfo
{
	int	iNatStatus;
	std::string NatInfoCode;
};

#endif
