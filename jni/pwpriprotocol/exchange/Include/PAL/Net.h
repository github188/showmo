

#ifndef __PAL_NETWORK_H__
#define __PAL_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup NetAPI API Net
/// 网络接口、DDNS与PPPoE的参数设置与获取。
/// @{

/// 设置主机的IP地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [in] pHostIp 主机IPpHostIp
/// \param [in] pNetMask 子网掩码点分字符串
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetHostIP(const char *pEthName,
				 const char *pHostIp,
                 const char *pNetMask);


/// 设置网关地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [in] pGateway 网关地址点分字符串
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetGateway(const char *pEthName,
				  const char *pGateway);


/// 获取主机的IP地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [out] pHostIp 主机IP地址点分字符串
/// \param [in] iHLen 主机IP地址字符串缓冲长度
/// \param [out] pNetMask 子网掩码点分字符串
/// \param [in] iNLen 子网掩码字符串缓冲长度
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetHostIP(const char *pEthName,
				 char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// 获取网关地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [out] pGateway 网关地址点分字符串
/// \param [in] iGLen 网关地址字符串缓冲长度
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetGateway(const char *pEthName,
				  char *pGateway,
                  const int iGLen);


/// 发送ARP包
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \retval 0  发送成功
/// \retval <0  发送失败
int NetSendarp(const char *pEthName);

/// 取得本机的Mac地址
/// 
/// \param [in] pEthName 接口名称，如eth0,eth1。，NULL表示当前使用的网卡
/// \param [out] pEthMac Mac16进制地址的字符串，形如00:05:0e:11:12:15。
/// \param [in] iEthMacLen MAC地址掩码长度。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetMAC(const char *pEthName,
                 char *pEthMac,
                 const int iEthMacLen);


/// 设置本机的Mac地址
/// 
/// \param [in] pEthName 接口名称，如eth0,eth1。，NULL表示当前使用的网卡
/// \param [in] pEthMac Mac16进制地址的字符串，形如00:05:0e:11:12:15
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetMAC(const char *pEthName, const char *pEthMac);

/// 获取DNS地址
/// 
/// \param [out] pPrimary 首选的DNS服务器地址字符串。
/// \param [in] iPlen 长度,要大于16
/// \param [out] pSecondary 备选的DNS服务器地址字符串。
/// \param [in] iSlen 长度,要大于16
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetGetDNSAddress(char *pprimary, int iPlen ,char *psecondary, int iSlen);
/// 设置DNS地址
/// 
/// \param [in] pPrimary 首选的DNS服务器地址字符串。
/// \param [in] pSecondary 备选的DNS服务器地址字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSAddress(const char * pPrimary, const char * pSecondary);

/// 设置DNS域名
/// 
/// \param [in] pDomain DNS域名字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSDomain(const char * pDomain);


/// 设置计算机名
/// 
/// \param [in] pHostName 计算机名字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSHostName(const char * pHostName);

//int NetSetMac(const char * pMac);

/// 初始化DVR web控件
/// 
/// \param [in] iWebPort web服务端口号
/// \param [in] iAppPort 应用程序端口号
/// \param [in] iLanguage web控件语言序号
/// \retval 0  设置成功
/// \retval <0  设置失败

//int	WebInitialize(int iWebPort, int iAppPort, int iLanguage);

/// WEN配置结构
typedef struct WEB_PARAM_CONFIG
{
	int		iWebPort;		 ///< HTTP 端口
	int 	iTCPPort;	     ///< TCP 端口
	int 	iUDPPort;	     ///< UDP 端口
	int 	iRtspPort;	     ///< RTSP 端口
	int 	iLanguage;	     ///< 语言
	char*   strWebRoot;		 ///< WEB资源文件的目录
	char*   strLangFile;     ///< 语言文件
	int     iResev[4];       ///< 保留
}WEB_PARAM;

int WebConfig(int cfgKind, const char *pConfig, int nSize);
int WebStart(int iWebPort, int iAppPort, int iLanguage, const char* strWebRoot, const char* webFile);
int WebStartV2(int iWebPort, int iAppPort, int iRtspPort, int iLanguage, const char* strWebRoot, const char* webFile);

int WebStartV3(WEB_PARAM *pWebParam);
int WebStop(void);
int WebRestart(void);
int WebStatus(void);

typedef int (*funcHttpProc)(int nCmd, void *pParm, int nLen, unsigned int dwUser);

int WebSetCallBack(funcHttpProc, void*);

/// 设置DVR web控件的映射端口
/// 
/// \param [in] iWebPort web经过映射后的公网端口号
/// \param [in] iAppPort 应用程序经过映射后的公网端口号
/// \retval 0  设置成功
/// \retval <0  设置失败
int WebSetMapPort(int iWebPort, int iAppPort);

/// 设置DVR web控件下载的URL
/// 
/// \param [in] pURL WEB控件URL
/// \retval 0  设置成功
/// \retval <0  设置失败
int WebSetActiveXUrl(const char* pURL);

/////////////////////////////////////////////////////
/// 设置主机的IP/掩码/网关地址
///此组函数用于设置多网卡，多网卡时通过参数pEthName区分网卡，如"eth0","eth2"
///其它参数意义参见上面对应函数
int NetSetHostIP2(const char *pEthName,  
		   const char *pHostIp,
                 const char *pNetMask);


/// 设置网关地址
int NetSetGateway2(const char *pEthName,
				const char *pGateway);


/// 获取主机的IP地址
int NetGetHostIP2(const char *pEthName,
		   char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// 获取网关地址
int NetGetGateway2(const char *pEthName,
		    char *pGateway,
                  const int iGLen);

// 增加路由
/// 
/// \param [in] pEthName 网卡名
/// \param [in] addr IP
/// \param [in] mask 掩码
/// \param [in] gw 网关
/// \param [in] nFlag 0: host route 1: default route
/// \retval 0  设置成功
/// \retval <0  设置失败

int NetAddRoute(const char *pEthName, struct in_addr addr, struct in_addr mask, struct in_addr gw, int nFlag);

// 删除路由
/// 
/// \param [in] pEthName 网卡名
/// \param [in] nFlag 0: host route 1: default route
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetDelRoute(const char *pEthName, int nFlag);


//////////////////////////////////////////////


/// DDNS配置结构
typedef struct NET_DDNS_CONFIG
{
	int		enable;			///< 是否启动DDNS，0-不启动，1-启动
	char	hostname[32];	///< 主机名，带0结束符的字符串。
	char	username[32];	///< DDNS服务用户名，带0结束符的字符串。
	char	password[32];	///< DDNS服务密码，带0结束符的字符串。
	char	serverip[32];	///< DDNS服务器IP地址，带0结束符的字符串，点分形式，如192.168.0.6。
	int		serverport;		///< DDNS服务端口。
} NET_DDNS_CONFIG;


/// 设置DDNS（动态域名解析）配置。
/// 
/// \param [in] pConfig 配置结构NET_DDNS_CONFIG指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDDNSConfig(NET_DDNS_CONFIG *pConfig);


/// 获取DDNS（动态域名解析）配置， 如果没有配置，应当返回一个初始化的结构。
/// 
/// \param [out] pConfig 配置结构NET_DDNS_CONFIG指针。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetDDNSConfig(NET_DDNS_CONFIG *pConfig);

/// PPPoE配置结构
typedef struct NET_PPPOE_CONFIG
{
	int		enable;			///< 是否启动PPPoE，0-不启动，1-启动
	char	username[64];	///< 帐户用户名，带0结束符的字符串。
	char	password[64];	///< 帐户密码，带0结束符的字符串, 因为CMOS空间限制密码最多为8字节。
} NET_PPPOE_CONFIG;


/// 设置PPPoE（以太网点对点）配置。
/// 
/// \param [in] pConfig 配置结构NET_PPPOE_CONFIG指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// 获取PPPoE（以太网点对点）配置，如果没有配置，应当返回一个初始化的结构。
/// 
/// \param [out] pConfig 配置结构NET_PPPOE_CONFIG指针。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// 获取PPPoE拨号后分配的IP地址
int NetGetDialIpAddress(char *pDailIp);


/// 设置DHCP使能标志。
/// 
/// \param [in] enable Bit0~bit31分别代表对应第几块网卡的DHCP使能标志 0： 不使能 1： 使能
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDhcpEnable(int enable);

/// 获取DHCP的使能标志
/// 
/// \param [out] pEnable Bit0~bit31分别代表对应第几块网卡的DHCP使能标志 0： 不使能 1： 使能
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetDhcpEnable(int *pEnable);

/// 获取网线连接状态
/// 
/// \param [out] ethname 网口名, 一般为"eth0"
/// \retval 0 连接正常
/// \retval other 异常
//int mii_diag(char *ethname);          // zzb 2013.9.10 modify
int mii_diag(const char *ethname);



/// 检测IP地址是否冲突
/// 
/// \retval 0 正常
/// \retval other 冲突
int ArpCheck();


/// @} end of group


#define IW_ENCODING_TOKEN_MAX 128 //64

typedef struct 
{
	int nEnable;
	char	 SSID[36];
	int  nChannel;      //
	char  NetType[32];	 //Infra, Adhoc
	char  Encode[32];  // NONE, WEP, TKIP, AES
	char  Auth[32];    // OPEN, SHARED, WEPAUTO, WPAPSK, WPA2PSK, WPANONE, WPA, WPA2
	int  nKeyType;    //0:Hex 1:ASCII
	char   Keys[IW_ENCODING_TOKEN_MAX]; 
	//int  KeyID;
}CONFIG_WLAN;

typedef struct 
{
	char	SSID[36];
	int nRSSI;                          //SEE RSSI_SINGNAL
	int nChannel;
	char  NetType[32];	 //Infra, Adhoc
	char  Encode[32];   // NONE, WEP, TKIP, AES
	char  Auth[32];      // OPEN, SHARED, WEPAUTO, WPAPSK, WPA2PSK, WPANONE, WPA, WPA2
}WLAN_DEVICE;

int WLanOpen();
int WLanClose();
int WLanSetConfig(CONFIG_WLAN* pconfig);
int WLanGetConfig(CONFIG_WLAN* pconfig);
WLAN_DEVICE* WLanScanDevices(int *pNum);

//wifi设备是否存在
//返回值： 1  设备存在    0 设备不存在
int WLanIsDeviceExist(void);

//wifi是否连接成功
//返回值： 1  连接成功    0 连接失败
int WLanIsConnect(void);

//获取无线网卡名
int WLanGetIfname(char * ifname);

//此组函数用于多网卡配置
int NetSetHostIPEx(const char *pEthName,  
		   			const char *pHostIp,
                 			const char *pNetmask);
/// 设置网关地址
int NetSetGatewayEx(const char *pEthName,
					     const char *pGateway);


/// 获取主机的IP地址
int NetGetHostIPEx(const char *pEthName,
		   			 char *pHostIp,
                 			 const int iHLen,
                 			 char *pNetmask,
                 			 const int iNLen);
/// 获取网关地址
int NetGetGatewayEx(const char *pEthName,
		    			     char *pGateway,
                  			     const int iGLen);
//获取EthName，供应用层调用。可从系统获取，现写死
//int NetGetEthName(int EthNo, char *pEthName, int len_EthName);

//*pEthName :	存入支持的网卡名,以::分隔,如eth0::eth2::
//len_EthName:	存入支持的网卡名BUF长度,一定要够长,否则失败
//*Ethnum:		实际找到的网卡数量

int NetGetEthName( char *pEthName, int len_EthName, int *iEthnum);

int NetGetDefaultEth(char *pEthName);
int NetSetDefaultEth(const char *pEthName);

enum network_media {
 NM_WIRED = 0,  //有线网卡
 NM_WLAN,       //Wifi
 NM_3G,         //3G网卡
 NM_NR,    //种类数
};
#define NM_WIRELESS NM_WLAN

typedef struct tagNetDevice
{
	unsigned char valid;			//当前网卡是否连接
	unsigned char trans_media;	//传输媒介
	unsigned char reserved[6];	//保留
	char name[32];		//网卡名称
	
}NetDevice;

//网络连接优先级
typedef struct tagNetLinkOrder
{
	int netType;  //取值是network_media类型的
	int netOrder; //数字越小，优先级越高,0是最高优先级
}NetLinkOrder;

/********************************************************************
**
**	函数名	:	NetGetEthDevice
**	入参	:	
**				1.	pNetDev:指向网卡属性结构NetDevice的指针数组。
**				2.	iMax   :最大支持的网卡个数(可以设置为10,目前实际为3个)
**	返回	:	> 0 支持的实际网卡个数
**				= 0 支持网卡个数为0,即失败
**	功能说明:	此函数用于应用层获取网卡支持的个数,以及对应网卡的种类和当前是否可用
**
*********************************************************************/
int NetGetEthDevice(NetDevice *pNetDev, int iMax);


/*
* 作  用：网络类型修改处理函数指针
* 参  数：[in] newNetTpye 切换成功以后使用的网络类型，具体值如枚举值network_media所示
* 返回值：先忽略
* 说  明：程序里会创建一个这种格式的函数，然后将其注册到网络相关的库里，库在网络状态切换以后调用该函数指针，
*         参数newNetType是切换成功以后使用的网络类型
*/
typedef int (*pFunc_NetTypeChange)(network_media newNetType);

/*
* 作  用：注册网络类型切换回调函数
* 参  数：[in] changeEditProc 要注册的回调函数
* 返回值：0：注册成功  -1：注册失败
* 说  明：在网络类型切换完成后才会执行回调函数
*/
int NetAttchTypeChangeProc(pFunc_NetTypeChange changeEditProc);

/*
* 作  用：设置指定网络的启用顺序
* 参  数：[in] enable    是否启用该功能  0：启用  -1：不启用
*         [in] netCount  需要排序的网络类型数目
*         [in] pNetOrder 结构体数组，保存网络类型和其对应的优先级，数字越小，优先级越高
* 返回值：0：设置成功  -1：设置失败
* 说  明：当访问本地局域网地址时，如果几种网络对应的本地地址不在一个子网内，则设置的启用顺序无效。
*         如有线地址是10.6.2.38，子网掩码是255.255.0.0，Wifi的地址是192.168.1.5，子网掩码是255.255.255.0，
*         则当访问的地址是10.6.5.38时，数据会直接通过有线发送出去，因为有线的本地地址和目的地址在同一个子网内，此时网络优先级无效。
*/
int NetSetLinkOrder(int enable, int netCount, NetLinkOrder* pNetOrder);



/* 网络类型 */
typedef enum tagWirelessType
{
	WIRELESS_TYPE_AUTOSEL=0,		/* 自动选择*/
	WIRELESS_TYPE_TD_SCDMA=1,	/* TD-SCDMA网络 */
	WIRELESS_TYPE_WCDMA=2,		/* WCDMA网络 */
	WIRELESS_TYPE_CDMA_1x=3,		/* CDMA 1.x网络 */
	WIRELESS_TYPE_EDGE=4,		/* GPRS网络 */
	WIRELESS_TYPE_EVDO=5,		/*EVDO网络*/
	WIRELESS_TYPE_MAX
}WirelessType;

// 无线设备
typedef struct tagWIRELESS_DEVICE
{
	unsigned int	typeMask; ///< 支持的网络类型掩码，按位表示，每一位对应WirelessType类型
	char	info[32]; ///< 其他信息
	int reserved[23];
}WIRELESS_DEVICE;


typedef struct tagWIRELESS_CONFIG
{
	int iEnable;					/* 无线模块使能标志 */
	WirelessType iNetType;			/* 无线网络类型*/
	char szAPN[32];		         	/* 接入点名称，字符串, 包括'\0'结束符共32bytes */
	char szDialNum[32];  			/* 拨号号码，字符串, 包括'\0'结束符共32bytes */
	char  szUserName[128];  		/* 拨号用户名，字符串, 包括'\0'结束符共16bytes */
	char szPWD[128];  				/* 拨号密码，字符串, 包括'\0'结束符共16bytes */
	char Reserved[8]; 				/* 预留8字节，供扩展 */
}WIRELESS_CONFIG;


// 无线网络状态
typedef struct tagWIRELESS_STATUS
{
	int iAccessStat; /* 无线网络注册状态*/
	char szDevIP[16];   /* 前端设备拨号IP，字符串, 包括'\0'结束符共16byte*/
	char szSubNetMask [16];   /* 前端设备拨号子网掩码，字符串, 包括'\0'结束符共16byte */
	char szGateWay[16];  /* 前端设备拨号网关，字符串, 包括'\0'结束符共16byte */
	WirelessType iNetType : 8; /* 无线网络类型*/
	char Reserved1[3];   /* 保留字节，用于扩展*/
	char Reserved[4];   /* 保留字节，用于扩展*/
}WIRELESS_STATUS;

typedef  enum tagNetworkDevType
{
	NWDEV_ETH0 = 0,				/* 以太网 */
	NWDEV_WLAN = 8,				/* W-lan */
	NWDEV_3G_= 16				/* TD-SCDMA */
} NWDevType;
// 无线网络信号强度,返回各个制式网络信号强度指示，范围0-100，100表示满格。

typedef struct  tagWIRELESS_RSSI
{
	int td_scdma;
	int wcdma;  
	int cdma_1x;  
	int edge; 
	int evdo;  
	int reserved[11]; /*保留字节，用于扩展*/
}WIRELESS_RSSI;

int WirelessCreat( void );
int WirelessRelease( void );

int WirelessGetConfig(WIRELESS_CONFIG* pconfig );
int WirelessSetConfig(WIRELESS_CONFIG *pconfig );
int WirelessGetStatus(WIRELESS_STATUS* pstatus );
int GetNetWorkDevice(NWDevType *pType);
int WirelessGetRSSI (WIRELESS_RSSI* prssi);
int WirelessGetSMS (char* sender, char * pstring);
int WirelessPutSMS (const char* receivers[32], const char * pstring);
int PPPOEGetIP( char * ip, int const ip_len, char * netmask, int const mask_len );
int WirelessGetDialog(char* caller);
int WirelessNeedBlock(void);
int WirelessGetDevice( WIRELESS_DEVICE* device );

//作  用：判断3G设备是否存在
//返回值：0 设备存在  其它值：设备不存在
int isWirelessDeviceExist(void);

//作  用：判断3G设备是否拨号成功
//返回值：0 拨号成功  其它值：拨号失败
int isWirelessDialUp(void);

 // USB 3G模块接口描述，128字节
typedef struct usb3GDesc
{
	char		manufacturer[32];	//厂商描述
	char		product[32];		      //产品描述
	char		version[16];		      //版本号
	char		speed[8];		     //usb速度
	char		vendor[16];  
	char		prodID[16];
	char		reserved[40];	    //< 保留
} usb3GDesc;
 
/*********************************************************
*描述:检测wireless设备是否连接，连接成功返回设备信息
*参数desc: [out] 返回设备信息
*返回值: 	>0:   成功<0:     失败
*********************************************************/
int scan_wireless_device(usb3GDesc *desc);

/// 取得本机的Mac地址
/// 
/// \param [in] pEthName 接口名称，如eth0,eth1。，NULL表示当前使用的网卡
/// \param [out] pEthMac Mac16进制地址的字符串，形如00:05:0e:11:12:15。
/// \param [in] iEthMacLen MAC地址掩码长度。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetMAC2(const char *pEthName, char *pEthMac, const int iEthMacLen);

/// VPN配置结构
typedef struct NET_VPN_CONFIG
{
	int		enable;			///< 是否启动VPN，0-不启动，1-启动
	char	serverip[32];	///< VPN服务器IP地址，带0结束符的字符串，点分形式，如192.168.0.6。
	char	username[64];	///< VPN服务用户名，带0结束符的字符串。
	char	password[64];	///< VPN服务密码，带0结束符的字符串。
} NET_VPN_CONFIG;

//获得VPN的配置
//pConfig VPN结构体
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetSetVPNConfig(NET_VPN_CONFIG *pConfig);

//设置VPN的配置
//pConfig VPN结构体
/// \retval 0 设置成功
/// \retval <0  设置失败
int NetGetVPNConfig(NET_VPN_CONFIG *pConfig);

//
//获取VPN连接后分配的IP地址
//
int VPNGetIP( char * ip, int const ip_len, char * netmask, int const mask_len );

#ifdef __cplusplus
}
#endif

#endif 
