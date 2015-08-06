#ifndef __EXCHANGE_AL_NETWORK_EXCHANGE_H__
#define __EXCHANGE_AL_NETWORK_EXCHANGE_H__

#include "../Types/Defs.h"
#include "CommExchange.h"
#include "Exchange.h"
#include "MediaExchange.h"
#include "../PAL/ExDec.h"
#include "../PAL/Net.h"

//需要了在加
#if 0
enum NetWorkErrorNo
{
	ACCOUNT_NOT_LOGIN = ERROR_BEGIN_NETCOMMON + 1,
	ACCOUNT_PASSWORD_NOT_VALID = ERROR_BEGIN_NETCOMMON + 2,
	ACCOUNT_USER_NOT_VALID = ERROR_BEGIN_NETCOMMON + 4,
	ACCOUNT_USER_LOCKED = ERROR_BEGIN_NETCOMMON + 5,
	ACCOUNT_USER_IN_BLACKLIST = ERROR_BEGIN_NETCOMMON + 6,
	ACCOUNT_USER_HAS_USED = ERROR_BEGIN_NETCOMMON + 7,
};
#endif

//结构定义规则统一改为不带前缀的格式

#define  NAME_PASSWORD_LEN  64
#define  DECODER_NAME_LEN	64
#define  EMAIL_ADDR_LEN  32
#define  N_MIN_TSECT 2
#define  N_ALARMSERVERKEY 5
#define  MAX_FILTERIP_NUM 64
#define  MAX_MAC_LEN 32

///< 服务器结构定义
struct RemoteServerConfig
{
	char ServerName[NAME_PASSWORD_LEN];	///< 服务名
	IPAddress ip;						///< IP地址
	int Port;							///< 端口号
	char UserName[NAME_PASSWORD_LEN];		///< 用户名
	char Password[NAME_PASSWORD_LEN];		///< 密码
	bool Anonymity;							///< 是否匿名登录
};

/// 录象下载策略
enum RecordDownloadPolicy
{
	DOWNLOADING_AT_NORMALSPEED = 0,	//普通下载
	DOWNLOADING_AT_HIGHSPEED,		//高速下载
};

/// 传输策略
enum TransferPolicy
{
	TRANSFER_POLICY_AUTO,		///< 自适应
	TRANSFER_POLICY_QUALITY,	///< 质量优先
	TRANSFER_POLICY_FLUENCY,	///< 流量优先
	TRANSFER_POLICY_TRANSMISSION,///<网传优先
	TRANSFER_POLICY_NR,
};

///< 普通网络设置
struct NetCommonConfig
{
	char HostName[NAME_PASSWORD_LEN];	///< 主机名
	IPAddress HostIP;		///< 主机IP
	IPAddress Submask;		///< 子网掩码
	IPAddress Gateway;		///< 网关IP
	int HttpPort;			///< HTTP服务端口
	int TCPPort;			///< TCP侦听端口
	int SSLPort;			///< SSL侦听端口
	int UDPPort;			///< UDP侦听端口
	int MaxConn;			///< 最大连接数
	int MonMode;			///< 监视协议 {"TCP","UDP","MCAST",…}
	int MaxBps;				///< 限定码流值
	int TransferPlan;		///< 传输策略 见TransferPolicy
	bool bUseHSDownLoad;	///< 是否启用高速录像下载测率
	char sMac[MAX_MAC_LEN]; ///< MAC地址
};

struct NetDevList
{
	std::vector<NetCommonConfig> vNetDevList;
};

///< IP权限设置
struct NetIPFilterConfig
{
	bool Enable;		///< 是否开启
	IPAddress BannedList[MAX_FILTERIP_NUM];		///< 黑名单列表
	IPAddress TrustList[MAX_FILTERIP_NUM];		///< 白名单列表
};

///< 组播设置
struct NetMultiCastConfig
{
	bool Enable;		///< 是否开启
	RemoteServerConfig Server;		///< 组播服务器
};

///< pppoe设置
struct NetPPPoEConfig
{
	bool Enable;		///< 是否开启
	RemoteServerConfig Server;		///< PPPOE服务器
	IPAddress addr;		///< 拨号后获得的IP地址
};

///< DDNS设置
struct NetDDNSConfig
{
	bool Enable;	///< 是否开启
	bool Online;		///< 是否在线
	char DDNSKey[NAME_PASSWORD_LEN];	///< DDNS类型名称
	char HostName[NAME_PASSWORD_LEN];	///< 主机名
	RemoteServerConfig Server;		///< DDNS服务器
};

enum TransferProtocol
{
	TRANSFER_PROTOCOL_TCP,
	TRANSFER_PROTOCOL_UDP,
	TRANSFER_PROTOCOL_NR,
};


enum TransferProtocol_V2
{
	TRANSFER_PROTOCOL_NETIP,
	TRANSFER_PROTOCOL_ONVIF,
	TRANSFER_PROTOCOL_NAT,
	TRANSFER_PROTOCOL_DAHUA,
	TRANSFER_PROTOCOL_NR_V2,
};

///< 解码器地址设置
struct NetDecorderConfig
{
	bool Enable;						///< 是否开启
	char UserName[NAME_PASSWORD_LEN];	///< DDNS类型名称, 目前有:
	char PassWord[NAME_PASSWORD_LEN];	///< 主机名
	char Address[NAME_PASSWORD_LEN];
	int Protocol;
	int Port;							///< 解码器连接端口
	int Channel;						///< 解码器连接通道号
	int Interval;                       ///< 轮巡的间隔时间(s),0:表示永久
};

/// 解码器地址设置
struct NetDecorderConfigAll
{
	NetDecorderConfig vNetDecorderConfig[N_DECORDR_CH];
};


//解码器地址设置v2
struct NetDecorderConfigAll_V2
{
	std::vector<NetDecorderConfig> vNetDecorderVector[N_DECORDR_CH];
};

enum DevType
{
	DEV_TYPE_IPC,
	DEV_TYPE_DVR,
	DEV_TYPE_HVR,
	DEV_TYPE_NR
};

///< 解码器地址设置
struct NetDecorderConfigV3
{
	bool Enable;						///< 是否开启
	char UserName[NAME_PASSWORD_LEN];	///< DDNS类型名称
	char PassWord[NAME_PASSWORD_LEN];	///< 主机名
	char Address[NAME_PASSWORD_LEN];
	int Protocol;
	int Port;							///< 解码器连接端口
	int Channel;						///< 解码器连接通道号
	int Interval;                       ///< 轮巡的间隔时间(s),0:表示永久
	char ConfName[DECODER_NAME_LEN];	///<配置名称
	int DevType;						///<设备类型
	int StreamType;						///<连接的码流类型CaptureChannelTypes
};

/*解码器连接类型*/
enum DecorderConnType
{
	CONN_SINGLE = 0, 	/*单连接*/
	CONN_MULTI = 1,		/*多连接轮巡*/
	CONN_TYPE_NR,
};

/*数字通道的配置*/
struct NetDigitChnConfig
{
	bool Enable;		/*数字通道是否开启*/
	int ConnType;		/*连接类型，取DecoderConnectType的值*/
	int TourIntv;		/*多连接时轮巡间隔*/
	uint SingleConnId;	/*单连接时的连接配置ID, 从1开始，0表示无效*/
	bool EnCheckTime;	/*开启对时*/
	std::vector<NetDecorderConfigV3> vNetDecorderConf; /*网络设备通道配置表*/
};

/*所有数字通道的配置*/
struct NetDecorderConfigAll_V3
{
	NetDigitChnConfig DigitChnConf[N_DECORDR_CH];
};

/*通道模式配置*/
struct NetDecorderChnModeConfig
{
	CAPTURE_TOTAL_HVRCAP 	HVRTotalCap;
	int HVRCurCapMode;
};

/*数字通道状态*/
struct NetDecorderChnStatus
{
	char ChnName[CHANNEL_NAME_MAX_LEN];
	char pMaxResName[50];
	char pCurResName[50];
	char pStatus[50];
};


/*所有数字通道状态*/
struct NetDecorderChnStatusAll
{
	NetDecorderChnStatus ChnStatusAll[MAX_HVR_CHNCAP];
};

//Pos设备类型
enum PosDevType
{
	POS_TYPE_MANY_LINES, //计算完总金额后才把商品信息一起发送过来
	POS_TYPE_ONE_LINE,   //每统计一件商品就把该商品的信息发送过来
	POS_NR
};

//文字编码格式
enum WordEncode
{
	WORD_ENCODE_GB2312,  //汉字编码GB2312
	WORD_ENCODE_UNICODE, //万国码 Unicode
	WORD_ENCODE_UTF8,    //UTF-8
};

// pos相关配置
struct NetPosConfig
{
	bool Enable;		/*pos机通道使能*/
	int Devtype;		//pos机类型
	int Protocol;		//通信协议
	int Port;			//协议端口号
	bool SnapEnable;    //抓拍使能
	int  StartLine;     //对收到的信息从多少行开始显示
	int  WordEncodeType;//文字编码格式，如枚举值 WordEncodeType 所示
	int res;			//保留
};
//所有pos机相关配置
struct NetPosConfigAll
{
	NetPosConfig PosConfig[N_SYS_CH];
};

///< 报警中心设置
struct NetAlarmCenterConfig
{
	bool bEnable;		///< 是否开启
	char sAlarmServerKey[NAME_PASSWORD_LEN];	///< 报警中心协议类型名称,
	///< 报警中心服务器
	RemoteServerConfig Server;
	bool bAlarm;
	bool bLog;
};

struct NetAlarmServerConfigAll
{
	NetAlarmCenterConfig vAlarmServerConfigAll[MAX_ALARMSERVER_TYPE];
};

///< ftp设置
struct NetFtpServerConfig{
	bool bEnable;        ///< 服务器使能
	RemoteServerConfig Server;	///< FTP服务器
	char cRemoteDir[MAX_PATH_LENGTH];	///< 远程目录
	int iMaxFileLen;	///< 文件最大长度
};

///< NTP设置

struct NetNTPConfig
{
	///< 是否开启
	bool Enable;
	///< PPPOE服务器
	RemoteServerConfig Server;
	///< 更新周期
	int UpdatePeriod;
	///< 时区
	int TimeZone;
};

#define  MAX_EMAIL_TITLE_LEN 64
#define  MAX_EMAIL_RECIEVERS  5
///< EMAIL设置
struct NetEmailConfig
{
	///< 是否开启
	bool Enable;
	///< smtp 服务器地址使用字符串形式填充
	///< 可以填ip,也可以填域名
	RemoteServerConfig Server;
	bool bUseSSL;
	///< 发送地址
	char SendAddr[EMAIL_ADDR_LEN];
	///< 接收人地址
	char Recievers[MAX_EMAIL_RECIEVERS][EMAIL_ADDR_LEN];
	///< 邮件主题
	char Title[MAX_EMAIL_TITLE_LEN];
	///< email有效时间
	TimeSection Schedule[N_MIN_TSECT];
};

//抓包配置结构
struct NetSnifferConfig
{
	IPAddress		SrcIP;		//抓包源地址
	int				SrcPort;	//抓包源端口
	IPAddress		DestIP;		//抓包目标地址
	int				DestPort;	//抓包目标端口
};

#define MAX_ETH_NUM 4

//DHCP
struct NetDHCPConfig
{
	bool bEnable;
	char ifName[32];
};

/// 所有网卡的DHCP配置
struct NetDHCPConfigAll
{
	NetDHCPConfig vNetDHCPConfig[MAX_ETH_NUM];
};

///< NTP设置
struct NetDNSConfig
{
	IPAddress		PrimaryDNS;
	IPAddress		SecondaryDNS;
};


struct DDNSTypeConfigAll
{
	NetDDNSConfig vDDNSTypeAll[MAX_DDNS_TYPE];
};

///< ARSP(主动注册服务器)设置包括：乐泰DNS
struct NetARSPConfig
{
	bool bEnable;	///< 是否开启
	char sARSPKey[NAME_PASSWORD_LEN];	///< DNS类型名称
	int iInterval;	///< 保活间隔时间
	char sURL[NAME_PASSWORD_LEN];    ///< 本机域名
	RemoteServerConfig Server;		///< DDNS服务器
	int nHttpPort;                  ///< 服务器HTTP端口
};

struct NetARSPConfigAll
{
	NetARSPConfig vNetARSPConfigAll[MAX_ARSP_TYPE];
};

enum Net3GType
{
	WIRELESS_AUTOSEL=0,	    ///< 自动选择
	WIRELESS_TD_SCDMA=1,	///< TD-SCDMA网络
	WIRELESS_WCDMA=2,		///< WCDMA网络
	WIRELESS_CDMA_1X=3,     ///< CDMA 1.x网络
	WIRELESS_EDGE=4,		///< GPRS网络
	WIRELESS_EVDO=5,		///< EVDO网络
	WIRELESS_MAX
};

struct Net3GConfig
{
	bool bEnable;			  ///< 无线模块使能标志
	int iNetType;			  ///< 无线网络类型
	std::string strAPN;		  ///< 接入点名称
	std::string strDialNum;   ///< 拨号号码
	std::string strUserName;  ///< 拨号用户名
	std::string strPWD;  	  ///< 拨号密码
	IPAddress addr;			  ///< 拨号后获得的IP地址
};

///< 手机监控设置包括：乐泰DNS
struct NetMoblieConfig
{
	bool bEnable;	///< 是否开启
	RemoteServerConfig Server;		///< 服务器
};

struct NetUPNPConfig
{
	bool bEnable;			  ///< 使能标志
	bool bState;              ///< 状态
	int iHTTPPort;			  ///< HTTP端口
	int iMediaPort;			  ///< 媒体端口
	int iMobliePort;		  ///< 手机监控端口
};

//WIFI
struct NetWifiConfig
{
	bool bEnable;
	std::string strSSID;            //SSID Number
	int nChannel;                   //channel
	std::string strNetType;         //Infra, Adhoc
	std::string strEncrypType;      //NONE, WEP, TKIP, AES
	std::string strAuth;            //OPEN, SHARED, WEPAUTO, WPAPSK, WPA2PSK, WPANONE, WPA, WPA2
	int  nKeyType;                  //0:Hex 1:ASCII
	std::string strKeys;
	IPAddress HostIP;		///< host ip
	IPAddress Submask;		///< netmask
	IPAddress Gateway;		///< gateway
};

struct NetWifiStatus
{
	int nWifiStatus;
};
struct NetAPSwapWifi
{
	int nAPSwapWifi;
};
// 报警中心消息类型
enum AlarmCenterMsgType
{
	ALARMCENTER_ALARM,
	ALARMCENTER_LOG,
};

// 报警中心消息类型
enum AlarmCenterStatus
{
	AC_START,
	AC_STOP,
};

// 告警中心消息内容
struct NetAlarmCenterMsg
{
	IPAddress HostIP;		///< 设备IP
	int nChannel;           ///< 通道
	int nType;              ///< 类型 见AlarmCenterMsgType
	int nStatus;            ///< 状态 见AlarmCenterStatus
	SystemTime Time;        ///< 发生时间
	std::string strEvent;    ///< 事件
	std::string strSerialID; ///< 设备序列号
	std::string strDescrip;  ///< 描述
};

// RTSP
struct NetRtspConfig
{
	bool bServer;
	bool bClient;
	RemoteServerConfig Server;		///< 服务器模式
	RemoteServerConfig Client;		///< 客户端模式
};

struct DASSerInfo
{
	bool enable;
 	char serAddr[NAME_PASSWORD_LEN];
	int  port;
	char userName[NAME_PASSWORD_LEN];
	char passwd[NAME_PASSWORD_LEN];
	char devID[NAME_PASSWORD_LEN];
};

struct NetMediaStreamConfig
{
	bool enable;
	char serAddr[NAME_PASSWORD_LEN];
	int  port;
	char devID[NAME_PASSWORD_LEN];
};


//手机短信配置
struct NetShortMsgCfg
{
	bool bEnable;       //发送手机短信的功能是否启用
	char pDesPhoneNum[MAX_RECIVE_MSG_PHONE_COUNT][16]; //接收短信的手机号，现支持3个手机号
	int  sendTimes;     //需要向每个手机发送多少次短信
};

//手机彩信配置
struct NetMultimediaMsgCfg
{
	bool bEnable;				// 发送手机彩信的功能是否启用
	char pDesPhoneNum[MAX_RECIVE_MSG_PHONE_COUNT][16]; //接收彩信的手机号，现支持3个手机号
	char pGateWayDomain[40];	// 网关地址，域名或IP
	int  gateWayPort;			// 网关端口
	char pMmscDomain[40];		// 彩信服务器地址，IP或域名
	int  mmscPort;				// 彩信服务器端口号
};


//网络优先级
struct NetOrderConfig
{
	bool         bEnable;          //是否设置网络优先级
	int          netCount;         //网络类型数目
	NetLinkOrder pNetOrder[NM_NR]; //网络优先级
	NetLinkOrder pReserve[7-NM_NR];//给新的网络类型预留
};

//网络平台信息设置
typedef struct LocalSdkNetPlatformConfig
{
	int Enable;
	int nISP;
	char sServerName[32];
	char ID[32];
	char sUserName[32];
	char sPassword[32];
	IPAddress HostIP;		///< host ip
	int port;
}LOCALSDK_NET_PLATFORM_CONFIG;

//神眼接警中心系统
struct	GodEyeConfig
{
	bool bEnable;
	char MainServerName[NAME_PASSWORD_LEN]; //主域名
	int MainPort;	//主端口
	char ExServerName[NAME_PASSWORD_LEN]; //备用域名
	int ExPort;	//备用端口
};

enum  DigManagerShowStatus
{
	SHOW_NONE,
	SHOW_ALL,
};


//数字通道显示状态
struct DigitalManagerShow
{
	int  nDigitalManagerShowSta;
};

//NAT使能，MTU值
struct NatConfig
{
	bool bEnable;
	int nMTU;
};

struct VPNConfig
{
	bool Enable;		///< 是否开启
	IPAddress ServiceIp;					///< 服务器IP地址
	char UserName[NAME_PASSWORD_LEN];		///< 用户名
	char Password[NAME_PASSWORD_LEN];		///< 密码
	IPAddress addr;		///< 拨号后获得的IP地址
};

void exchangeServer(CKeyExchange& configExchange, CConfigTable& table, RemoteServerConfig& server);

#endif

