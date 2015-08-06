
#ifndef __DVRDEF_H__
#define __DVRDEF_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Types.h"

typedef unsigned int			Param;
typedef void *					Handle;
typedef int						Bool;

#ifdef WIN32
	#define snprintf _snprintf
#endif

#define FOR(i, max)				for (i = 0; i < (max); i++)
#define BITMSK(bit)				(int)(1 << (bit))

#define INTEGER_MIN (int)(0x80000000)
#define INTEGER_MAX (int)(0x7FFFFFFF)
#define UINTEGER_MAX (unsigned int)0xFFFFFFFF

#define TRUE					1
#define FALSE					0

#ifndef NULL
#define NULL					0
#endif

#define ON						1
#define OFF						0

#define MPG1					1
#define MPG4					4

#define CIF1					1	//1 CIF
#define CIF2					2	//2 CIF
#define CIF4					4	//4 CIF

#define NC						0	//常关

#define NO						1	//常开

#ifndef MAX
#define MAX(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))
#endif

#define ABS(x) ((x)>=0?(x):-(x))

#ifdef FUNC_BUILD_CHANNEL
#define channel_utol(x) (2 * x - x % 2)
#define channel_ltou(x) ((x + 1) / 2)
#else
#define channel_utol(x) (x)
#define channel_ltou(x) (x)
#endif

#define trace printf
#ifdef DEBUG
	#define tracepoint() 	do {trace("tracepoint: %s,%d\n",__FILE__,__LINE__); } while(0)
#else
	#define tracepoint()
#endif

#define NAME_LEN		32
#define CHANNEL_NAME_SIZE		64
#define LABEL_LEN		32

/// 全程定义常量
enum DVRGlobal
{
	MAX_PATH_LENGTH		= 260,		///< 路径最大长度
	MAX_USERNAME_LENGTH = 128,		///< 用户名最大长度，字节
	MAX_PASSWORD_LENGTH	= 128,		///< 用户密码最大长度
	N_SYS_CH			= 32,		///< 最大通道数
	N_CHMAX_PERLINE		= 16,		///< 一行最大通道数
	N_SYS_COMB_CH		= 2,		///< 最大组合编码通道数
	N_ALM_IN			= 32,		///< 最大报警输入通道数
	N_ALM_OUT			= 32,		///< 最大报警输出通道数
	N_PLY_CH			= 32,		///< 最大回放路数
	N_DECORDR_CH		= 32,		///< 解码器最大路数
	N_AUDIO_IN			= 32,		///< 最大音频输入路数
	N_PTZ_ALARM			= 16,		///< 最大云台报警路数
	N_WEEKS				= 7,		///< 一周天数
	N_TSECT				= 6,		///< 时间段数量
	N_SPLIT				= 15,		///< 最大分割模式
	N_VIDEO_OUT			= 4,		///< 最大视频输出
	N_MATRIX_OUT		= 4,		///< 最大举证输出路数
	MAX_DDNS_TYPE		= 5,		///< 最大DDNS种类
	MAX_ALARMSERVER_TYPE= 5,		///< 最大报警服务器种类
	MAX_ARSP_TYPE		= 5,		///< 最大主动注册服务器种类
	MAX_CHANNEL_WIDTH 	= 320,		///< 通道标题最大宽度
	MD_REGION_ROW		= 32,		///< 动检最大行数
	MAX_EXTRA_STREAM_TYPE = 3,		///< 辅码流类型
	MAX_COVER_COUNT		= 8,		///< 最大覆盖区域数
	CHANNEL_NAME_MAX_LEN = 64,		///< 通道名称最大长度
	N_COLOR_SECTION		= 2,		///< 视频颜色时间段数
	MAX_DRIVER_PER_DISK = 2,		///< 每个磁盘最多的分区数
	MAX_DISK_PER_MACHINE = 8,		///< 最多支持8块硬盘
	MAX_AP_NUMBER		= 32,		///< 最大AP节点个数
	MAX_SPOT_NUMBER		= 4,		///<最大的SPOT接口数
	MAX_DEV_DEC_CHNS	= (N_PLY_CH+N_DECORDR_CH),
	MAX_RECIVE_MSG_PHONE_COUNT = 3, ///<最大支持的用于接收报警提示信息的手机数目
	MAX_CAR_INFO_WIDTH  = 24,       ///<车辆信息最多能占用的字节数
};

/// 星期
enum WeekDays
{
	WEEK_DAY_SUNDAY,		///< 星期日
	WEEK_DAY_MONDAY,
	WEEK_DAY_TUESDAY,
	WEEK_DAY_WEDNESDAY,	
	WEEK_DAY_THRUSDAY,
	WEEK_DAY_FRIDAY,
	WEEK_DAY_SATURDAY,
	WEEK_DAY_NR,	
};

#define RELATIVE_MAX_X 0x7FF
#define RELATIVE_MAX_Y 0x7FF

//应用程序事件
typedef enum app_event_action
{
	appEventStart = 0,		// 事件开始
	appEventStop,			// 事件结束
	appEventConfig,			// 事件配置变化，不作为接收参数
	appEventLatch,			// 事件延时结束，不作为发起参数
	appEventPulse,			// 脉冲事件，没有开始和结束
}appEventAction;

//thread priority fifo 0-29
#define TP_COM			 1		//1
#define TP_TIMER		 2		//1
#define TP_ALARM		 3		//1
#define TP_PLAY			 4		//4

#define TP_REC			 6
#define TP_CAPTURE	20		//16
#define TP_MEDIAFILE		24		//1
#define TP_GUI			25		//1
#define TP_PTZ			26

//thread priority other 30-99
#define TP_SMTP     30
#define WATCH_DOG   31

#define TP_BACKUP		35		//1
#define TP_SVR			37		//2
#define TP_NET			39		//16
#define TP_TEL			55		//1
#define TP_FRONTBOARD	56		//1
#define TP_MOUSE		57		//1
#define TP_FTP			58
#define TP_WEB          58
#define TP_DEFAULT		64
#define TP_PRE			 127

enum fmt_time_flag{
	FT_NORMAL = 0,
	FT_HALF_YEAR = 0x0001,
	FT_NO_SECOND = 0x0002,
	FT_NO_SIGNS  = 0x0004,
	FT_ONLY_DATE = 0x0008,
	FT_ONLY_TIME = 0x0010,
	FT_FULL_AMPM = 0x0020
};

enum LanguageTypes
{
	ENGLISH,		//英语
	CHINESE_S,		//简体中文
	CHINESE_T,		//繁体中文
	ITALIAN,		//意大利语
	SPANISH,		//西班牙语
	JAPANESE,		//日语
	RUSSIAN,		//俄语
	FRENCH,			//法语
	GERMAN,			//德语
	PORTUGAL,		//葡萄牙文
	TURKEY,			//土耳其文
	POLAND,			//波兰文
	ROMANIAN,       //罗马尼亚
	HUNGARIAN,      //匈牙利语
	FINNISH,		//芬兰语
	ESTONIAN,	    //爱沙尼亚语
	KOREAN,		    //韩语
	FARSI,          //波斯语 
	DANSK,			//丹麦语
	THAI,			//泰国语
	GREEK,			//希腊语
	VIETNAMESE,		//越南语
	UKRAINIAN,      //乌克兰语
	BRAZILIAN,      //巴西语
	HEBREW,         //希伯来语
	INDONESIAN,     //印尼语
	ARABIC,         //阿拉伯语
	SWEDISH,        //瑞典语
	CZECH,          //捷克语
	BULGARIAN,      //保加利亚
	SLOVAKIA,		//斯洛伐克语
	DUTCH,			//荷兰语
	LANG_NR
};

/// 模块错误号
enum ModuleErrorNumber
{
	ERROR_OK = 100,						///< 正常
	ERROR_BEGIN_ACCOUNT = 201,			///< Manager起始错误号
	ERROR_BEGIN_NETCOMMON = 301,		///< 通用网络开始错误号
	ERROR_BEGIN_NETSERVICE = 401,		///< 网络服务开始错误号
	ERROR_BEGIN_NETOPERATION = 501,		///< 操作开始错误号
	ERROR_BEGIN_CONFIG = 601,			///< 配置操作开始错误号
	ERROR_BEGIN_ABILITY = 701,			///< 能力集相关错误号
	ERROR_BEGIN_DEVINFO = 801,			///< 设备信息相关错误号
};

// Join two variables
#define MACRO_JOIN( X, Y ) MACRO_DO_JOIN( X, Y )
#define MACRO_DO_JOIN( X, Y ) MACRO_DO_JOIN2(X,Y)
#define MACRO_DO_JOIN2( X, Y ) X##Y

#endif //__DVRDEF_H__
