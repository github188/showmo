#ifndef __EXCHANGE_AL_NETPLATFORM_OPERATION_H__
#define __EXCHANGE_AL_NETPLATFORM_OPERATION_H__

#include "../Types/Defs.h"
#include "NetWorkExchange.h"

struct CONFIG_NET_MEGA
{
	bool bEnable;
	bool bNetManEnable;
	IPAddress ServerIP;
	int iServerPort;
	char sDeviceId[32];
	char sUserName[24];
	char sPasswd[32];
	int iMaxCon;
	int iVideoPort;
	int iAudioPort;
	int iMsgPort;
	int iUpdatePort;
};

struct MegaMotion
{
	Rect mRect[MAX_COVER_COUNT];
};

struct MegaMotionAll
{
	MegaMotion MotionRect[N_SYS_CH];
};


// 新望平台
struct CONFIG_NET_XINWANG
{
	bool bEnable;
	bool bSyncTime;
	bool bSubStream;
	IPAddress ServerIP;
	int iServerPort;
	int iDownLoadPort;
	char sPasswd[32];
	char szSID[32];
};

// 视搜平台
struct CONFIG_NET_SHISOU
{
	bool bEnable;
	RemoteServerConfig Server;
	char szSID[MAX_USERNAME_LENGTH];
};

// VVEYE平台
struct CONFIG_NET_VVEYE
{
	bool bEnable;                
	bool bCorpEnable;            //只有在使用企业服务器时才需要设置Server
	RemoteServerConfig Server;
	char szDeviceName[MAX_USERNAME_LENGTH];
};

//WELLSUN平台
struct CONFIG_NET_WELLSUN
{
	bool bEnable;
	RemoteServerConfig Server;
	unsigned int    uiDevID;            //device id
	int nGPSHeartBeat;                  //gps hearbeat
	int nSysHeartBeat;                  //svr hearbeat
};

// 楠瑞平台
struct CONFIG_NET_NANRUI
{
	bool bEnable;
	RemoteServerConfig Server;
	char szSID[MAX_USERNAME_LENGTH];
};

#endif
