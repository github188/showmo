#ifndef __EXCHANGEAL_COMM_EXCHANGE_H__
#define __EXCHANGEAL_COMM_EXCHANGE_H__

#include "CommExchange.h"
#include "../Types/Defs.h"
#include "../Types/Types.h"
#include "../Infra/Time.h"
#include "../Manager/EventManager.h"

#define SHOW_STR_LEN 32


/// 工作表名称索引
enum WorksheetName
{
	RECORD_WORKSHEET = 0,		///< 录像工作表
	ALARM_WORKSHEET = 1,		///< 报警工作表
	MOTION_WORKSHEET = 2,		///< 动检工作表
	BLIND_WORKSHEET = 3,		///< 遮挡工作表
	VIDEOLOSS_WORKSHEET = 4,	///< 视频丢失工作表
	NETALARM_WORKSHEET = 5,	    ///< 网络报警工作表
	VIDEOANALYZE_WORKSHEET = 6,	///<视频分析工作表
	WORKSHEET_NR,
};


/// 云台联动类型
enum PtzLinkTypes
{
	PTZ_LINK_NONE,			// 不需要联动 
	PTZ_LINK_PRESET,		// 转至预置点 
	PTZ_LINK_TOUR,			// 巡航 
	PTZ_LINK_PATTERN		// 轨迹 
};


///< 基本事件结构
struct GenericEventConfig
{
	bool bEnable;			///< 使能
	EventHandler hEvent;	///< 处理参数
};

/// 所有通道的基本时间结构
struct GenericEventConfigAll
{
	GenericEventConfig vGenericEventConfig[N_SYS_CH];
};

///< 动态检测设置
struct MotionDetectConfig
{
	bool bEnable;							// 动态检测开启 
	int iLevel;								// 灵敏度: 1~6
	uint mRegion[MD_REGION_ROW];			// 区域，每一行使用一个二进制串 	
	EventHandler hEvent;					// 动态检测联动 
};

/// 全通道动态检测配置
struct MotionDetectConfigAll
{
	MotionDetectConfig vMotionDetectAll[N_SYS_CH];
};

///< 遮挡检测配置
struct BlindDetectConfig
{
	bool	bEnable;		///< 遮挡检测开启
	int		iLevel;			///< 灵敏度: 1~6
	EventHandler hEvent;	///< 遮挡检测联动参数
};

/// 全通道遮挡检测配置
struct BlindDetectConfigAll
{
	BlindDetectConfig vBlindDetectAll[N_SYS_CH];
};

struct	LossShowStrConfig
{
	char ShowStr[SHOW_STR_LEN];
};

struct LossShowStrConfigALL
{
	LossShowStrConfig vLossShowStrAll[N_SYS_CH];
};

///< 本地报警配置
struct AlarmConfig
{
	bool	bEnable;		///< 报警输入开关
	int		iSensorType;	///< 传感器类型常开 or 常闭
	EventHandler hEvent;	///< 报警联动
};

///< 所有通道的报警配置
struct AlarmConfigAll
{
	AlarmConfig vAlarmConfigAll[N_ALM_IN];
};

///< 本地报警输出配置
struct AlarmOutConfig
{
	int nAlarmOutType;		///< 报警输出类型: 配置,手动,关闭
	int nAlarmOutStatus;    ///< 报警状态: 0:打开 1;闭合
};

///< 所有通道的报警输出配置
struct AlarmOutConfigAll
{
	AlarmOutConfig vAlarmOutConfigAll[N_ALM_OUT];
};

///< 硬盘容量不足事件结构
struct StorageLowSpaceConfig
{
	bool bEnable;
	int iLowerLimit;		///< 硬盘剩余容量下限, 百分数
	EventHandler hEvent;	///< 处理参数
	bool bRecordTimeEnable;
	int iRecordTime;		///<录像天数
};

///<硬盘出错事件结构
struct StorageFailConfig
{
	bool 	bEnable;
	EventHandler hEvent;	///< 处理参数
	bool	bRebootEnable;		//系统重启全能:hutianhao
};

// 串口协议
enum CommProtocol
{
	CONSOLE = 0,
	KEYBOARD,
	COM_TYPES,
};

enum PtzConfigType
{
	CONFIG_PTZ_LOCAL,
	CONFIG_PTZ_ALARM,
	CONFIG_PTZ_PRESETINFO,
	CONFIG_PTZ_TOURINFO,
};
#define PTZ_CHANNELS 8
#define	PTZ_PRESETNUM 80
#define MAX_PROTOCOL_LENGTH 32

struct COMMATTRI
{
	int	iDataBits;	// 数据位取值为5,6,7,8 
	int	iStopBits;	// 停止位
	int	iParity;	// 校验位
	int	iBaudRate;	// 实际波特率
};

// 串口配置
struct CONFIG_COMM_X
{
	char sProtocolName[MAX_PROTOCOL_LENGTH];	// 串口协议:“Console” 
	int iPortNo;		// 端口号 
	COMMATTRI aCommAttri;		// 串口属性 
};

struct CommConfigAll
{
	CONFIG_COMM_X vCommConfig[COM_TYPES];
};

// 云台设置
struct CONFIG_PTZ
{
	char sProtocolName[MAX_PROTOCOL_LENGTH];	// 协议名称 	
	int	ideviceNo;				// 云台设备地址编号 	
	int	iNumberInMatrixs;		// 在矩阵中的统一编号	
	int iPortNo;				// 串口端口号	[1, 4] 	
	COMMATTRI dstComm;			// 串口属性 	
};

struct PTZConfigAll
{
	CONFIG_PTZ ptzAll[N_SYS_CH];
};

struct RS485ConfigAll
{
	CONFIG_PTZ rs485All[N_SYS_CH];
};


//车辆状态
enum CAR_STATUS_TYPE
{
	CAR_WORKING,             //是否在运行
	CAR_LIGHT_LEFT_TURN,     //左转灯是否点亮
	CAR_LIGHT_RIGHT_TURN,    //右转灯是否点亮
	CAR_DOOR_LEFT_FRONT,     //左前门是否打开
	CAR_DOOR_RIGHT_FRONT,    //右前门是否打开
	CAR_DOOR_LEFT_BACK,      //左后门是否打开
	CAR_DOOR_RIGHT_BACK,     //右后门是否打开
	CAR_DOOR_BACK,           //后门是否打开
	CAR_BRAKE,			     //是否踩刹车
	CAR_URGENCY_ALARM,       //紧急报警
	CAR_STATUS_NR, //状态种类数目
};

//外部输入类型
enum IO_INPUT_TYPE
{
	LOCAL_ALARM_INPUT,//本地报警输入
	RS232_INPUT,      //通过232串口输入
	RS485_INPUT,      //通过485串口输入
};

//外部信息输入与车辆状态的对应关系
struct CarStatusExchange
{
	int  statusType; //哪一种车辆状态，比如左转灯
	bool bExist;     //是否有该种状态的信息输入，根据车辆的实际情况进行设置
	bool bEnable;    //是否检测该种状态
	int  inputType;  //该种状态对应的信息输入类型，从IO_INPUT_TYPE枚举的值中取
	int  addr;       //地址，比如是本地报警输入口一对应0，输入口二对应1
	int  sensorType; //常开(NO)或常闭(NC)，当inputType是本地报警输入时有效
};

struct CarStatusExchangeAll
{
	CarStatusExchange exchangeAll[16];
};

struct CarDelayTimeConfig
{
	bool bStartDelay;
	bool bCloseDelay;
	
	int timeStartDelay;		//单位:分钟
	int timeCloseDelay;		//单位:分钟
};

//GPS校时配置
struct GPSTimingConfig
{
	bool bEnable;      // 是否启用
	int  timeChange;   // 相对于UTC时间需要改变多少，单位:秒
	int  updatePeriod; // 更新周期  单位:分钟
};


//点间巡航
struct TourState
{
	bool bRunning;
	int  lineID;   //点间巡航线路编号
};

//巡迹
struct PatternState
{
	bool bRunning;
	int  lineID;  //巡迹线路编号 (未使用)
};

//线扫
struct ScanState
{
	bool bRunning;
	int  lineID;  //线扫线路编号 (未使用)
};

//水平旋转
struct PanonState
{
	bool bRunning;
	int  lineID;  //水平旋转线路编号 (未使用)
};

//一个通道的云台操作状态
struct PtzState
{
	TourState    tourState;
	PatternState patternState;
	ScanState    scanState;
	PanonState   panonState;
};

//重启后恢复之前的云台操作状态
struct ResumePtzState
{
	bool     bEnable;                    //是否启用设备重启后恢复云台状态功能
	PtzState ptzStateAll[N_SYS_CH];
};

#endif
