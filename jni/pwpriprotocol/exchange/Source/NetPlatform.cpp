// 	Description:	
// 	Revisions:		Year-Month-Day  SVN-Author  Modification
//	Modify:			网络操作码Json格式交换
//

#include "../Include/ExchangeAL/NetPlatform.h"
#include "../Include/ExchangeAL/Exchange.h"

template<> void exchangeTable<CONFIG_NET_MEGA>(CConfigTable &table, CONFIG_NET_MEGA &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "NetMEnable", config.bNetManEnable);
	exchanger.exchange(table, "ServerIP", config.ServerIP.l);
	exchanger.exchange(table, "ServerPort", config.iServerPort);
	exchanger.exchange(table, "DeviceId", config.sDeviceId);
	exchanger.exchange(table, "UserName", config.sUserName);
	exchanger.exchange(table, "Passwd", config.sPasswd);
	exchanger.exchange(table, "MaxCon", config.iMaxCon);
	exchanger.exchange(table, "VideoPort", config.iVideoPort);
	exchanger.exchange(table, "AudioPort", config.iAudioPort);
	exchanger.exchange(table, "MsgPort", config.iMsgPort);
	exchanger.exchange(table, "UpdatePort", config.iUpdatePort);
}

static void exchangeRect(CConfigTable& table, Rect& rect, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Left", rect.left);
	exchanger.exchange(table, "Top", rect.top);
	exchanger.exchange(table, "Right", rect.right);
	exchanger.exchange(table, "Bottom", rect.bottom);
}

template<> void exchangeTable<MegaMotionAll>(CConfigTable &table, MegaMotionAll &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		for (int j = 0; j < MAX_COVER_COUNT; j++)
		{
			exchangeRect(table[i]["RelativePos"][j], config.MotionRect[i].mRect[j], state);
		}
	}
}

template<> void exchangeTable<CONFIG_NET_XINWANG>(CConfigTable &table, CONFIG_NET_XINWANG &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "SyncTime", config.bSyncTime);
	exchanger.exchange(table, "SubStream", config.bSubStream);
	exchanger.exchange(table, "ServerIP", config.ServerIP.l);
	exchanger.exchange(table, "ServerPort", config.iServerPort);
	exchanger.exchange(table, "DownLoadPort", config.iDownLoadPort);
	exchanger.exchange(table, "Passwd", config.sPasswd);
	exchanger.exchange(table, "SID", config.szSID);
}

template<> void exchangeTable<CONFIG_NET_SHISOU>(CConfigTable &table, CONFIG_NET_SHISOU &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "SID", config.szSID);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<CONFIG_NET_VVEYE>(CConfigTable &table, CONFIG_NET_VVEYE &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "CorpEnable", config.bCorpEnable);
	exchanger.exchange(table, "DeviceName", config.szDeviceName);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<CONFIG_NET_WELLSUN>(CConfigTable &table, CONFIG_NET_WELLSUN &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "DevID", config.uiDevID);
	exchanger.exchange(table, "GPSHeartBeat", config.nGPSHeartBeat);
	exchanger.exchange(table, "SysHeartBeat", config.nSysHeartBeat);
	exchangeServer(exchanger, table["Server"], config.Server);
}


template<> void exchangeTable<CONFIG_NET_NANRUI>(CConfigTable &table, CONFIG_NET_NANRUI &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "SID", config.szSID);
	exchangeServer(exchanger, table["Server"], config.Server);
}
