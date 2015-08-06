//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "../Include/ExchangeAL/NetWorkExchange.h"
#include "../Include/ExchangeAL/Exchange.h"

static ConfigPair s_netStreamTypeMap[] =
{
	{"TCP", 0},
	{"UDP", 1},
	{"MCAST", 2},
	{NULL,	}
};

static ConfigPair s_netTransferPlanMap[] =
{
	{"AutoAdapt", TRANSFER_POLICY_AUTO},
	{"Quality", TRANSFER_POLICY_QUALITY},
	{"Fluency", TRANSFER_POLICY_FLUENCY},
	{"Transmission",TRANSFER_POLICY_TRANSMISSION},
	{NULL,}
};


extern void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state);

template<> void exchangeTable<NetCommonConfig>(CConfigTable& table, NetCommonConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "HostName", config.HostName);
	exchanger.exchange(table, "HostIP", config.HostIP.l);

	exchanger.exchange(table, "Submask",	config.Submask.l);

	exchanger.exchange(table, "GateWay",	config.Gateway.l);
	exchanger.exchange(table, "HttpPort",config.HttpPort);
	exchanger.exchange(table, "TCPPort", config.TCPPort);
	exchanger.exchange(table, "TCPMaxConn", config.MaxConn);

	exchanger.exchange(table, "SSLPort", config.SSLPort);
	exchanger.exchange(table, "UDPPort",	config.UDPPort);
	exchanger.exchange(table, "MonMode",	config.MonMode, s_netStreamTypeMap);
	exchanger.exchange(table, "MaxBps",	config.MaxBps);
	exchanger.exchange(table, "TransferPlan", config.TransferPlan, s_netTransferPlanMap);
	exchanger.exchange(table, "UseHSDownLoad", config.bUseHSDownLoad);
	exchanger.exchange(table, "MAC", config.sMac);
}

void exchangeServer(CKeyExchange& configExchange, CConfigTable& table, RemoteServerConfig& server)
{
	configExchange.exchange(table, "Address", server.ip.l);
	configExchange.exchange(table, "Name", server.ServerName);
	configExchange.exchange(table, "Port", server.Port);
	configExchange.exchange(table, "UserName", server.UserName);
	configExchange.exchange(table, "Password", server.Password);
	configExchange.exchange(table, "Anonymity", server.Anonymity);
}

template<> void exchangeTable<NetIPFilterConfig>(CConfigTable& table, NetIPFilterConfig& config, int state)
{
	int i;
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);

	CConfigTable& tb = table["Banned"];

	for(i = 0; i < MAX_FILTERIP_NUM; i++)
	{
		exchanger.exchange(tb, i,config.BannedList[i].l);
	}

	CConfigTable& tb1 = table["Trusted"];

	for(i = 0; i < MAX_FILTERIP_NUM; i++)
	{
		exchanger.exchange(tb1, i,config.TrustList[i].l);
	}
}

template<> void exchangeTable<NetMultiCastConfig>(CConfigTable& table, NetMultiCastConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<NetPPPoEConfig>(CConfigTable& table, NetPPPoEConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchangeServer(exchanger, table["Server"], config.Server);
	exchanger.exchange(table, "HostIP", config.addr.l);
}

template<> void exchangeTable<NetDDNSConfig>(CConfigTable& table, NetDDNSConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "Online", config.Online);
	exchanger.exchange(table, "DDNSKey", config.DDNSKey);
	exchanger.exchange(table, "HostName", config.HostName);
	exchanger.exchange(table["Server"], "Name", config.Server.ServerName);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<NetFtpServerConfig>(CConfigTable& table, NetFtpServerConfig& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);

	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Directory", config.cRemoteDir);
	exchanger.exchange(table, "MaxFileLen", config.iMaxFileLen);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<NetNTPConfig>(CConfigTable& table, NetNTPConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "UpdatePeriod", config.UpdatePeriod);
	exchanger.exchange(table, "TimeZone", config.TimeZone);//中国加8
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<NetEmailConfig>(CConfigTable& table, NetEmailConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	int i;
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "UseSSL", config.bUseSSL);
	exchanger.exchange(table, "Title", config.Title);
	exchanger.exchange(table, "SendAddr", config.SendAddr);
	exchangeServer(exchanger, table["MailServer"], config.Server);
	CConfigTable& tb1 = table["Recievers"];
	for(i = 0; i < MAX_EMAIL_RECIEVERS; i++)
	{
		exchanger.exchange(tb1, i, config.Recievers[i]);
	}
	CConfigTable& tb2 = table["Schedule"];
	for(i = 0; i < N_MIN_TSECT; i++)
	{
		exchangeTimeSection(tb2[i], config.Schedule[i], state);
	}
}

template<> void exchangeTable <NetDHCPConfigAll>(CConfigTable & table, NetDHCPConfigAll& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < MAX_ETH_NUM; i++)
	{
		exchanger.exchange(table[i], "Enable", config.vNetDHCPConfig[i].bEnable);
		exchanger.exchange(table[i], "Interface", config.vNetDHCPConfig[i].ifName);
	}
}

template<> void exchangeTable <DDNSTypeConfigAll >(CConfigTable & table, DDNSTypeConfigAll& config, int state)
{
	for (int i = 0; i < MAX_DDNS_TYPE; i++)
	{
		exchangeTable(table[i], config.vDDNSTypeAll[i], state);
	}
}

ConfigPair s_alarmServerType[] =
{
	{"GENERAL", 0},
	{NULL,	}
};

template<> void exchangeTable<NetAlarmCenterConfig>(CConfigTable& table, NetAlarmCenterConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Protocol", config.sAlarmServerKey);
	exchanger.exchange(table, "Alarm", config.bAlarm);
	exchanger.exchange(table, "Log", config.bLog);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable <NetAlarmServerConfigAll>(CConfigTable & table, NetAlarmServerConfigAll& config, int state)
{
	for (int i = 0; i < MAX_ALARMSERVER_TYPE; i++)
	{
		exchangeTable(table[i], config.vAlarmServerConfigAll[i], state);
	}
}

template<> void exchangeTable <NetDNSConfig>(CConfigTable & table, NetDNSConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Address",  config.PrimaryDNS.l);
	exchanger.exchange(table, "SpareAddress",  config.SecondaryDNS.l);
}

template<> void exchangeTable<NetARSPConfig>(CConfigTable& table, NetARSPConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "ARSPKey", config.sARSPKey);
	exchanger.exchange(table, "Interval", config.iInterval);
	exchanger.exchange(table, "HttpPort", config.nHttpPort);
	exchanger.exchange(table, "URL", config.sURL);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable <NetARSPConfigAll>(CConfigTable & table, NetARSPConfigAll& config, int state)
{
	for (int i = 0; i < MAX_ARSP_TYPE; i++)
	{
		exchangeTable(table[i], config.vNetARSPConfigAll[i], state);
	}
}

static ConfigPair s_transferProtocolType[] =
{
	{"TCP", TRANSFER_PROTOCOL_TCP},
	{"UDP", TRANSFER_PROTOCOL_UDP},
	{NULL,	}
};

static ConfigPair s_transferProtocolType_V2[] =
{
	{"NAT", TRANSFER_PROTOCOL_NAT},
	{"DAHUA", TRANSFER_PROTOCOL_DAHUA},
	{"ONVIF", TRANSFER_PROTOCOL_ONVIF},
	{"TCP", TRANSFER_PROTOCOL_NETIP},//TCP为了兼容老的IE解析
	{NULL,	}
};

template<> void exchangeTable<NetDecorderConfig>(CConfigTable& table, NetDecorderConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "UserName", config.UserName);
	exchanger.exchange(table, "PassWord", config.PassWord);
	exchanger.exchange(table, "IPAddress", config.Address);
	exchanger.exchange(table, "Port", config.Port);
	exchanger.exchange(table, "Protocol", config.Protocol, s_transferProtocolType);
	exchanger.exchange(table, "Channel", config.Channel);
	exchanger.exchange(table, "Interval", config.Interval);
}

template<> void exchangeTable <NetDecorderConfigAll_V2>(CConfigTable & table, NetDecorderConfigAll_V2& config, int state)
{
	for (uint i = 0; i < N_DECORDR_CH; i++)
	{
		if (state == CKeyExchange::ES_LOADING)
		{
			config.vNetDecorderVector[i].clear();
			for (uint j = 0; j < table[i].size(); j++)
			{
				NetDecorderConfig DecoderCfg;
				exchangeTable(table[i][j], DecoderCfg, state);
				config.vNetDecorderVector[i].push_back(DecoderCfg);
			}
		}
		else
		{
			uint j = 0;
			for (std::vector<NetDecorderConfig>::const_iterator pi = config.vNetDecorderVector[i].begin(); pi != config.vNetDecorderVector[i].end(); j++, pi++)
			{
				NetDecorderConfig DecoderCfg = (*pi);
				exchangeTable(table[i][j], DecoderCfg, state);
			}
		}
	}
}

template<> void exchangeTable <NetDecorderConfigAll>(CConfigTable & table, NetDecorderConfigAll& config, int state)
{
	for (int i = 0; i < N_DECORDR_CH; i++)
	{
		exchangeTable(table[i], config.vNetDecorderConfig[i], state);
	}
}

template<> void exchangeTableV2 <NetDecorderConfigAll>(CConfigTable & table, NetDecorderConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vNetDecorderConfig[i], state);
	}
}


static ConfigPair s_StreamType[] =
{
	{"MAIN", CAPTURE_CHN_MAIN},
	{"2END", CAPTURE_CHN_2END},
	{"3IRD", CAPTURE_CHN_3IRD},
	{"4RTH", CAPTURE_CHN_4RTH},
	{"JPEG", CAPTURE_CHN_JPEG},
	{NULL,	}
};

static ConfigPair s_DevType[] =
{
	{"IPC", DEV_TYPE_IPC},
	{"DVR", DEV_TYPE_DVR},
	{"HVR", DEV_TYPE_HVR},
	{NULL,	}
};

template<> void exchangeTable<NetDecorderConfigV3>(CConfigTable& table, NetDecorderConfigV3& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "UserName", config.UserName);
	exchanger.exchange(table, "PassWord", config.PassWord);
	exchanger.exchange(table, "IPAddress", config.Address);
	exchanger.exchange(table, "Port", config.Port);
	exchanger.exchange(table, "Protocol", config.Protocol, s_transferProtocolType_V2);
	exchanger.exchange(table, "Channel", config.Channel);
	exchanger.exchange(table, "Interval", config.Interval);
	exchanger.exchange(table, "ConfName", config.ConfName);
	exchanger.exchange(table, "StreamType", config.StreamType, s_StreamType);
	exchanger.exchange(table, "DevType", config.DevType, s_DevType);
}

static ConfigPair s_ConnectType[] =
{
	{"SINGLE", CONN_SINGLE},
	{"MULTI", CONN_MULTI},
	{NULL,	}
};

template<> void exchangeTable<NetDigitChnConfig>(CConfigTable& table, NetDigitChnConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "ConnType", config.ConnType,s_ConnectType);
	exchanger.exchange(table, "TourIntv", config.TourIntv);
	exchanger.exchange(table, "SingleConnId", config.SingleConnId);
	exchanger.exchange(table, "EnCheckTime", config.EnCheckTime);

	CConfigTable& tb1 = table["Decoder"];

	if(state == CKeyExchange::ES_LOADING)
	{
		config.vNetDecorderConf.clear();
		for(uint i = 0; i < tb1.size(); i++)
		{
			NetDecorderConfigV3 DecoderCfg;
			exchangeTable(tb1[i], DecoderCfg, state);
			config.vNetDecorderConf.push_back(DecoderCfg);
		}
	}
	else
	{
		uint i = 0;
		for (std::vector<NetDecorderConfigV3>::const_iterator pi = config.vNetDecorderConf.begin(); pi != config.vNetDecorderConf.end(); i++, pi++)
		{
			NetDecorderConfigV3 DecoderCfg = (*pi);
			exchangeTable(tb1[i], DecoderCfg, state);
		}
	}
}
template<> void exchangeTable <NetDecorderConfigAll_V3>(CConfigTable & table, NetDecorderConfigAll_V3& config, int state)
{
	for (int i = 0; i < N_DECORDR_CH; i++)
	{
		exchangeTable(table[i], config.DigitChnConf[i], state);
	}
}

template<> void exchangeTableV2 <NetDecorderConfigAll_V3>(CConfigTable & table, NetDecorderConfigAll_V3& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.DigitChnConf[i], state);
	}
}

void exchangeChnMode(CKeyExchange& configExchange, CConfigTable& table, CAPTURE_HVRCAP& HVRCAP)
{
	configExchange.exchange(table, "AnalogCapD1", HVRCAP.AnalogCap.nD1Chn);
	configExchange.exchange(table, "AnalogCap960H", HVRCAP.AnalogCap.n960HChn);
	configExchange.exchange(table, "AnalogCap720P", HVRCAP.AnalogCap.n720PChn);
	configExchange.exchange(table, "AnalogCap1080P", HVRCAP.AnalogCap.n1080PChn);
	configExchange.exchange(table, "AnalogCapCIF", HVRCAP.AnalogCap.nCIFChn);
	configExchange.exchange(table, "AnalogCapHD1", HVRCAP.AnalogCap.nHD1Chn);

	configExchange.exchange(table, "DigitalCapD1", HVRCAP.DigitalCap.nD1Chn);
	configExchange.exchange(table, "DigitalCap960H", HVRCAP.DigitalCap.n960HChn);
	configExchange.exchange(table, "DigitalCap720P", HVRCAP.DigitalCap.n720PChn);
	configExchange.exchange(table, "DigitalCap1080P", HVRCAP.DigitalCap.n1080PChn);
	configExchange.exchange(table, "DigitalCapCIF", HVRCAP.DigitalCap.nCIFChn);
	configExchange.exchange(table, "DigitalCapHD1", HVRCAP.DigitalCap.nHD1Chn);

}

template<> void exchangeTable <NetDecorderChnModeConfig>(CConfigTable & table, NetDecorderChnModeConfig& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "TotalChnsModeNum", config.HVRTotalCap.nHVRCap);
	for (int i=0; i<MAX_HVR_CHNCAP; i++)
	{
		exchangeChnMode(exchanger,table["TotalChnsMode"][i],config.HVRTotalCap.HVRCap[i]);
	}

	exchanger.exchange(table, "CurChnsMode", config.HVRCurCapMode);
}

template<> void exchangeTable<NetDecorderChnStatus>(CConfigTable& table, NetDecorderChnStatus& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "ChnName", config.ChnName);
	exchanger.exchange(table, "MaxRes", config.pMaxResName);
	exchanger.exchange(table, "CurRes", config.pCurResName);
	exchanger.exchange(table, "Status", config.pStatus);
}

template<> void exchangeTable <NetDecorderChnStatusAll>(CConfigTable & table, NetDecorderChnStatusAll& config, int state)
{
	for (int i = 0; i < MAX_HVR_CHNCAP; i++)
	{
		exchangeTable(table[i], config.ChnStatusAll[i], state);
	}
}
////////////////////////////////////
static ConfigPair s_posDeviceType[] =
{
	{"ManyLines", POS_TYPE_MANY_LINES},
	{"OneLine",   POS_TYPE_ONE_LINE},
	{NULL,	}
};

//文字编码格式
static ConfigPair s_wordEncodeType[] =
{
	{"GB2312",  WORD_ENCODE_GB2312},
	{"Unicode", WORD_ENCODE_UNICODE},
	{"UTF-8",   WORD_ENCODE_UTF8},
	{NULL, }
};

template<> void exchangeTable<NetPosConfig>(CConfigTable& table, NetPosConfig& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "Devtype", config.Devtype, s_posDeviceType);
	exchanger.exchange(table, "Protocol", config.Protocol, s_transferProtocolType);
	exchanger.exchange(table, "Port", config.Port);
	exchanger.exchange(table, "SnapEnable", config.SnapEnable);
	exchanger.exchange(table, "StartLine", config.StartLine);
	exchanger.exchange(table, "WordEncodeType", config.WordEncodeType, s_wordEncodeType);
}

template<> void exchangeTable <NetPosConfigAll>(CConfigTable & table, NetPosConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchangeTable(table[i], config.PosConfig[i], state);
	}
}

template<> void exchangeTableV2 <NetPosConfigAll>(CConfigTable & table, NetPosConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.PosConfig[i], state);
	}
}
//////////////////////////////

ConfigPair s_3gNetType[] =
{
	{"AUTO", WIRELESS_AUTOSEL},
	{"TD_SCDMA", WIRELESS_TD_SCDMA},
	{"WCDMA", WIRELESS_WCDMA},
	{"CDMA_1X", WIRELESS_CDMA_1X},
	{"EDGE", WIRELESS_EDGE},
	{"EVDO", WIRELESS_EVDO},
	{NULL,	}
};

template<> void exchangeTable<Net3GConfig>(CConfigTable& table, Net3GConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "NetType", config.iNetType, s_3gNetType);
	exchanger.exchange(table, "APN", config.strAPN);
	exchanger.exchange(table, "DialNum", config.strDialNum);
	exchanger.exchange(table, "UserName", config.strUserName);
	exchanger.exchange(table, "Password", config.strPWD);
	exchanger.exchange(table, "DialIP", config.addr.l);
}

template<> void exchangeTable<NetMoblieConfig>(CConfigTable& table, NetMoblieConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchangeServer(exchanger, table["Server"], config.Server);
}

template<> void exchangeTable<NetUPNPConfig>(CConfigTable& table, NetUPNPConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "State", config.bState);
	exchanger.exchange(table, "HTTPPort", config.iHTTPPort);
	exchanger.exchange(table, "MediaPort", config.iMediaPort);
	exchanger.exchange(table, "MobilePort", config.iMobliePort);
}

template<> void exchangeTable<NetWifiConfig>(CConfigTable& table, NetWifiConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "SSID", config.strSSID);
	exchanger.exchange(table, "Channel", config.nChannel);
	exchanger.exchange(table, "NetType", config.strNetType);
	exchanger.exchange(table, "EncrypType", config.strEncrypType);
	exchanger.exchange(table, "Auth", config.strAuth);
	exchanger.exchange(table, "KeyType", config.nKeyType);
	exchanger.exchange(table, "Keys", config.strKeys);
	exchanger.exchange(table, "HostIP", config.HostIP.l);
	exchanger.exchange(table, "Submask", config.Submask.l);
	exchanger.exchange(table, "GateWay", config.Gateway.l);
}

ConfigPair s_ACType[] =
{
	{"Alarm", ALARMCENTER_ALARM},
	{"Log", ALARMCENTER_LOG},
	{NULL,	}
};

ConfigPair s_ACStatus[] =
{
	{"Start", AC_START},
	{"Stop", AC_STOP},
	{NULL,	}
};

//增加WifiStatus
template<> void exchangeTable<NetWifiStatus>(CConfigTable& table, NetWifiStatus& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "WifiStatus", config.nWifiStatus);
}
template<> void exchangeTable<NetAPSwapWifi>(CConfigTable& table, NetAPSwapWifi& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "APSwapWifi", config.nAPSwapWifi);
}
template<> void exchangeTable<NetAlarmCenterMsg>(CConfigTable& table, NetAlarmCenterMsg& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Address", config.HostIP.l);
	exchanger.exchange(table, "Channel", config.nChannel);
	exchanger.exchange(table, "Type", config.nType, s_ACType);
	exchanger.exchange(table, "Event", config.strEvent);
	exchanger.exchange(table, "Status", config.nStatus, s_ACStatus);
	exchangeSysTime(exchanger, table["StartTime"], config.Time);
	exchanger.exchange(table, "SerialID", config.strSerialID);
	exchanger.exchange(table, "Descrip", config.strDescrip);
}

template<> void exchangeTable<NetRtspConfig>(CConfigTable& table, NetRtspConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "IsServer", config.bServer);
	exchanger.exchange(table, "IsClient", config.bClient);
	exchangeServer(exchanger, table["Server"], config.Server);
	exchangeServer(exchanger, table["Client"], config.Client);
}

template<> void exchangeTable<DASSerInfo>(CConfigTable& table, DASSerInfo& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.enable);
	exchanger.exchange(table, "ServerAddr", config.serAddr);
    	exchanger.exchange(table, "Port", config.port);
	exchanger.exchange(table, "UserName", config.userName);
    	exchanger.exchange(table, "Password", config.passwd);
    	exchanger.exchange(table, "DeviceID", config.devID);
}

template<> void exchangeTable<NetMediaStreamConfig>(CConfigTable& table, NetMediaStreamConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enbale", config.enable);
	exchanger.exchange(table, "ServerAddr", config.serAddr);
	exchanger.exchange(table, "Port", config.port);
	exchanger.exchange(table, "DeviceID", config.devID);
}

//手机短信配置解析
template<> void exchangeTable<NetShortMsgCfg>(CConfigTable& table, NetShortMsgCfg& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchanger.exchange(table, "Enable", config.bEnable);
	for(int i = 0; i < MAX_RECIVE_MSG_PHONE_COUNT; i++)
	{
		exchanger.exchange(table["PhoneNum"], i, config.pDesPhoneNum[i]);
	}
	exchanger.exchange(table, "SendTimes", config.sendTimes);
}

//手机彩信配置解析
template<> void exchangeTable<NetMultimediaMsgCfg>(CConfigTable& table, NetMultimediaMsgCfg& config, int state)
{
	CKeyExchange exchanger;

    exchanger.setState(state);

	exchanger.exchange(table, "Enable", config.bEnable);
	for(int i = 0; i < MAX_RECIVE_MSG_PHONE_COUNT; i++)
	{
		exchanger.exchange(table["PhoneNum"], i, config.pDesPhoneNum[i]);
	}

	exchanger.exchange(table, "GateWayDomain", config.pGateWayDomain);
	exchanger.exchange(table, "GateWayPort", config.gateWayPort);

	exchanger.exchange(table, "MmscDomain", config.pMmscDomain);
	exchanger.exchange(table, "MmscPort", config.mmscPort);
}

//网络优先级配置
template<> void exchangeTable<NetOrderConfig>(CConfigTable& table, NetOrderConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "NetCount", config.netCount);
	for(int i = 0; i < NM_NR; i++)
	{
		exchanger.exchange(table["NetOrder"][i], "NetType", config.pNetOrder[i].netType);
		exchanger.exchange(table["NetOrder"][i], "NetOrder", config.pNetOrder[i].netOrder);
	}
}

//网络平台信息设置
template<> void exchangeTable<LocalSdkNetPlatformConfig>(CConfigTable& table, LocalSdkNetPlatformConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "ISP", config.nISP);
	exchanger.exchange(table, "ServerName", config.sServerName);
	exchanger.exchange(table, "ID", config.ID);
	exchanger.exchange(table, "UserName", config.sUserName);
	exchanger.exchange(table, "Password", config.sPassword);
	exchanger.exchange(table, "Address", config.HostIP.l);
	exchanger.exchange(table, "port", config.port);

}

//神眼接警中心系统
template<> void exchangeTable<GodEyeConfig>(CConfigTable& table, GodEyeConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "MainServerName", config.MainServerName);
	exchanger.exchange(table, "MainPort", config.MainPort);
	exchanger.exchange(table, "ExServerName", config.ExServerName);
	exchanger.exchange(table, "ExPort", config.ExPort);

}

static ConfigPair s_DigManagerShowMap[] =
{
	{"ShowNone",SHOW_NONE},
	{"ShowAll",SHOW_ALL},
	{NULL},
};

template<> void exchangeTable <DigitalManagerShow>(CConfigTable &table, DigitalManagerShow &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "DigManagerShow" , config.nDigitalManagerShowSta, s_DigManagerShowMap);

}

template<> void exchangeTable <NatConfig>(CConfigTable &table, NatConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table,"NatEnable", config.bEnable);
	exchanger.exchange(table, "XMeyeMTU" , config.nMTU);
}

template<> void exchangeTable<VPNConfig>(CConfigTable& table, VPNConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.Enable);
	exchanger.exchange(table, "ServiceIp", config.ServiceIp.l);
	exchanger.exchange(table, "UserName", config.UserName);
	exchanger.exchange(table, "Password", config.Password);
	exchanger.exchange(table, "addr", config.addr.l);
}

