#ifndef __EXCHANGEAL_MANAGER_EXCHANGE_H__
#define __EXCHANGEAL_MANAGER_EXCHANGE_H__

#include <vector>
#include <string>
#include "../Types/Types.h"
#include "../Types/Defs.h"
#include "../Infra/Time.h"

enum AccountErrorNo
{
	ACCOUNT_OK = 100,
	ACCOUNT_NOT_LOGIN = ERROR_BEGIN_ACCOUNT + 1,
	ACCOUNT_PASSWORD_NOT_VALID = ERROR_BEGIN_ACCOUNT + 2,
	ACCOUNT_USER_NOT_VALID = ERROR_BEGIN_ACCOUNT + 4,
	ACCOUNT_USER_LOCKED = ERROR_BEGIN_ACCOUNT + 5,
	ACCOUNT_USER_IN_BLACKLIST = ERROR_BEGIN_ACCOUNT + 6,
	ACCOUNT_USER_HAS_USED = ERROR_BEGIN_ACCOUNT + 7,
	ACCOUNT_INPUT_NOT_VALID,	 /// 输入不合法
	ACCOUNT_OVERLAP,			/// 索引重复如要增加的用户已经存在等
	ACCOUNT_OBJECT_NONE,		/// 不存在对象, 用于查询时
	ACCOUNT_OBJECT_NOT_VALID,	/// 对象不存在
	ACCOUNT_OBJECT_IN_USE,		/// 目标正在使用中,如组被用，不能删除
	ACCOUNT_SUBSET_OVERLAP,		/// 子集超范围 (如组的权限超过权限表，用户权限超出组的权限范围等等)
	ACCOUNT_PWD_NOT_VALID,		/// 密码不正确
	ACCOUNT_PWD_NOT_MATCH,		/// 密码不匹配
	ACCOUNT_RESERVED,			/// 保留帐号
};

/// 密码保存方式，加密或者不加密
enum PasswordEncrypt
{
	PASSWORD_FLAG_PLAIN,	///< 密码不加密方式
	PASSWORD_FLAG_MD5,		///< MD5密码加密方式
	PASSWORD_FLAG_3DES,		///< 3DES加密方式
	PASSWORD_FLAG_HSQD,		///<客户黄山奇盾特殊加密方式(只判断，不校验)
	PASSWORD_FLAG_NR,		///< 加密种类
};

/// 用户登陆方式
enum LoginType
{
	LOGIN_TYPE_GUI,			///< 本地GUI登陆
	LOGIN_TYPE_CONSOLE,		///< 控制台登陆
	LOGIN_TYPE_WEB,			///< WEB登陆
	LOGIN_TYPE_SNS,			///< SNS登陆
	LOGIN_TYPE_MOBIL,		///< 移动终端登陆
	LOGIN_TYPE_NETKEYBOARD,	///< 网络键盘登陆
	LOGIN_TYPE_SERVER,		///< 中心服务器登陆
	LOGIN_TYPE_AUTOSEARCH,	///< IP自动搜索工具登陆
	LOGIN_TYPE_UPGRADE,		///< 升级工具登陆
	LOGIN_TYPE_MEGAEYE,		///< 全球眼登陆
	LOGIN_TYPE_NR,			///< 登陆类型
};

///< 用户登陆请求信息
struct LoginRequest
{
	std::string sUserName;		///< 登陆名字
	std::string sPassword;		///< 登陆密码
	int iEncryptType;			///< 密码加密方式，加密或者不加密
	int iLoginType;				///< 登陆方式
};

struct AutoLogin
{
	int iEnable; 				///<自动登录是否开启。1开启，0关闭
	LoginRequest LoginInfo;
};
/// 设备类型
enum DeviceType
{
	DEVICE_TYPE_DVR,	///< 普通DVR设备
	DEVICE_TYPE_NVS,	///< NVS设备
	DEVICE_TYPE_IPC,	///< IPC设备
	DEVICE_TYPE_HVR,	///<混合dvr
	DEVICE_TYPE_IVR,	///<智能dvr
	DEVICE_TYPE_MVR,	///<车载dvr
	DEVICE_TYPE_NR
};

/// 登陆请求响应
struct LoginResponse
{
	int iChannelNum;		///< 机器路数
	int iAliveInterval;		///< 保活周期(s)
	int iRet;				///< 返回值
	int iDeviceType;		///< 设备类型
	uint uiSessionId;		///< 会话ID
	int iExtraChannel;		///< 扩展通道数
};

/// 修改用户密码请求
struct ModifyPasswordRequest
{
	uint uiSessionId;
	int iEncryptType;
	std::string sUserName;
	std::string sPassword;
	std::string sNewPassword;
};

/// 用户配置信息
struct UserConfig
{
	UserConfig &operator=(const UserConfig &user);
	std::vector<std::string> authorityList;		// 用户权限列表
	std::string	group;		// 组名
	std::string memo;		// 提示信息
	std::string userName;	// 用户名
	std::string password;	// 密码
	bool		reserved;	// 是否是保留用户
	bool		shareable;	// 该用户是否可复用
};

struct ModifyUserConfig
{
	std::string sUserName;
	UserConfig User;
};

/// 组配置权限
struct GroupConfig
{
	GroupConfig &operator=(const GroupConfig &group);
	std::vector<std::string> authorityList;		// 用户权限列表
	std::string memo;		// 提示信息
	std::string name;		// 组名
};

struct ModifyGroupConfig
{
	std::string sGroupName;
	GroupConfig Group;
};

// 删除账号, 用来删除用户或者组
struct DeleteAccountRequest
{
	uint uiSessionID;
	std::string sName;	
};

const char *getUserLoginType(int iLoginType);
const char *getUserEncType(int iEncType);


/// 硬盘满处理策略
enum DiskFullPolicy
{
	DISK_FULL_POLICY_STOP_RECORD,	///< 停止录像
	DISK_FULL_POLICY_OVERWRITE,		///< 覆盖
	DISK_FULL_POLICY_NR
};

/// 视频输出适配器类型
enum VideoOutputTypes
{
	VIDEOOUT_AUTO = 0,	///< 自动检测设备
	VIDEOOUT_VGA,		///< 切换到VGA输出
	VIDEOOUT_TV,		///< 切换到TV输出
	VIDEOOUT_VGA_LCD,	///< 切换到LCD VGA输出
	VIDEOOUT_NR,
};

/// 视频制式类型
enum VideoStandardTypes
{
	VIDEO_STD_PAL = 0,	///< PAL制式
	VIDEO_STD_NTSC,		///< NTSC制式
	VIDEO_STD_SECAM		///< SECAM制式
};

///< 普通配置
struct GeneralConfig
{
	int iLocalNo;				///< 本机编号:[0, 998]
	int iOverWrite;				///< 硬盘满时的处理策略DiskFullPolicy
	int iSnapInterval;			///< 定时抓图的时间间隔，以秒为单位
	std::string sMachineName;	///< 机器名
	int iVideoStartOutPut;		///< 输出模式
	int iAutoLogout;			///< 本地菜单自动注销(分钟)	[0, 120]
};

/// 夏令时规则
enum DST_RULE_TYPES
{
	DST_RULE_OFF = 0,	///< 关闭夏令时规则
	DST_RULE_ON,		///< 启动夏令时规则
	DST_RULE_NR
};

///< 夏令时结构
struct GeneralDSTPoint
{
	int	iYear;
	int	iMonth;
	int	iWeek;		///<周1:first  to2 3 4 -1:last one   0:表示使用按日计算的方法[-1,4]
	int	iWeekDay;	///<weekday from sunday=0	[0, 6]
	int Hour;
	int Minute;
};

///<区域设置
struct LocationConfig
{
	int iVideoFormat;		///< 视频制式
	int iLanguage;			///< 语言选择
	int iDateFormat;		///< 日期格式
	int iDateSeparator;		///< 日期分割符:“.”, “-”, “/”
	int iTimeFormat;		///< 时间格式:“12”, “24”
	int iDSTRule;			///< 夏令时规则
	int iWorkDay;			///< 工作日
	GeneralDSTPoint dDSTStart;
	GeneralDSTPoint dDSTEnd;
};

///< 自动维护设置
struct AutoMaintainConfig
{
	int iAutoRebootDay;				///< 自动重启设置日期
	int iAutoRebootHour;			///< 重启整点时间	[0, 23]
	int iAutoDeleteFilesDays;		///< 自动删除文件时间[0, 30]
};

//////////////////////////////////////////////////////////////////////////
/// 车载相关配置项
struct CarPlates
{
	std::string sPlateName;
};

//////////////////////////////////////////////////////////////////////////
/// 系统能力相关配置
struct DeviceDesc
{
	int nAudioIn;			///< 音频输入路数
	int nAlarmIn;			///< 报警输入路数
	int nAlarmOut;			///< 报警输出路数
	int nTrailDay;			///< 使用时间
	std::string sGUITheme;	///< GUI主题
	bool	bAutoBoot;		///< 是否带自动开关机功能
	int nRemoteType;        //遥控器类型
	int nPenaltyDay;       ///< 惩罚周期
	int nlocalPalyMax;	//本地回放最大路数
	int nlocalPalyDefault;  //本地回放默认路数
	int nCapture;           //捕获通道数
	int nPadType;           //前面板类型
	bool bEablePlayDefault; //是否启用回放默认路数
	bool bEnableComm;       //串口232功能是否启用
	bool bEnablePtz;        //云台功能是否启用
};

/// 记录系统上次运行状态
struct SystemLastState
{
	int nLastSpt;   ///< 记录上次手动切换分割模式,设备启动时切换到最后一次分割状态
	int nLastSubSpt;	///< 记录上次手动切换分割模式,设备启动时切换到最后一次分割状态
	TimeSection tsWorkHour;			///< DVR工作时间段(24小时制，如果要表示晚上到第二天凌晨如[20, 8])
};

//配置应用结果，按位表示，由配置的观察者设置，配置的提交者检查
enum CONFIG_APPLY_RET
{
	CONFIG_APPLY_OK = 0,						/// 成功
	CONFIG_APPLY_REBOOT = 0x00000002,			/// 需要重启系统
	CONFIG_APPLY_FILE_ERROR = 0x00000004,		/// 写文件出错
	CONFIG_APPLY_CAPS_ERROR = 0x00000008,		/// 特性不支持
	CONFIG_APPLY_GATEWAY_ERROR = 0x00000010,	///网关设置失败,仍然归类为成功
	CONFIG_APPLY_VALIT_ERROR = 0x00000020,		/// 验证失败
	CONFIG_APPLY_NOT_EXSIST = 0x00000040,		/// 配置不存在
	CONFIG_APPLY_NOT_SAVE = 0x00000080,			/// 不要保存到文件
	CONFIG_APPLY_DELAY_SAVE = 0x00000100,		///延时保存..把位占掉取值为0x180的原因是在配置保存的时候需要互斥
};

struct AbilitySerialNo
{
	std::string serialNo;
	std::string productType;
};

struct ProduceTest
{
	bool TestEnable; //生产测试模式
};

struct DASLoginInfo
{
	std::string userName;		///< 登陆名字
	std::string password;		///< 登陆密码
	int encryptType;			///< 密码加密方式,PasswordEncrypt
	std::string deviceID;		///<设备ID
	int devicType;				///<DeviceType
	uint channelNums;			///<设备通道数
};

struct LedLight
{
	int light;					//led亮度
};


#endif
