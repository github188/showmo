// 	Description:	
// 	Revisions:		Year-Month-Day  SVN-Author  Modification
//  Modify:			能力集相关配置
//

#include "../Include/ExchangeAL/NetIPAbilitySet.h"
#include "../Include/ExchangeAL/Exchange.h"
#include "../Include/ExchangeAL/CommExchange.h"
#include "../Include/Types/Defs.h"
#include <assert.h>
#include "../Include/PAL/Capture.h"

static ConfigPair s_vEncodeTypeMap[] = 
{
	{"DoubleStream", ENCODE_FUNCTION_TYPE_DOUBLE_STREAM},
	{"CombineStream", ENCODE_FUNCTION_TYPE_COMBINE_STREAM},
	{"SnapStream", ENCODE_FUNCTION_TYPE_SNAP_STREAM},
	{"WaterMark",ENCODE_FUNCTION_TYPE_WATER_MARK},
	{NULL,}
};

static ConfigPair s_vAlarmTypeMap[] =
{
	{"MotionDetect", ALARM_FUNCTION_TYPE_MOTION_DETECT},
	{"BlindDetect", ALARM_FUNCTION_TYPE_BLIND_DETECT},
	{"LossDetect", ALARM_FUNCTION_TYPE_LOSS_DETECT},
	{"AlarmConfig", ALARM_FUNCTION_TYPE_LOCAL_ALARM},
	{"NetAlarm", ALARM_FUNCTION_TYPE_NET_ALARM},
	{"NetIpConflict", ALARM_FUNCTION_TYPE_IP_CONFLICT},
	{"NetAbort", ALARM_FUNCTION_TYPE_NET_ABORT},
	{"StorageNotExist", ALARM_FUNCTION_TYPE_STORAGE_NOTEXIST},
	{"StorageLowSpace", ALARM_FUNCTION_TYPE_STORAGE_LOWSPACE},
	{"StorageFailure", ALARM_FUNCTION_TYPE_STORAGE_FAILURE},
	{"VideoAnalyze", ALARM_FUNCTION_TYPE_VIEDO_ANALYZE},
	{NULL},
};

static ConfigPair s_vNetServerMap[] =
{
	{"NetIPFilter", NET_SERVER_TYPES_IPFILTER},
	{"NetDHCP", NET_SERVER_TYPES_DHCP},
	{"NetDDNS", NET_SERVER_TYPES_DDNS},
	{"NetEmail", NET_SERVER_TYPES_EMAIL},
	{"NetMutliCast", NET_SERVER_TYPES_MULTICAST},
	{"NetNTP", NET_SERVER_TYPES_NTP},
	{"NetPPPoE", NET_SERVER_TYPES_PPPOE},
	{"NetDNS", NET_SERVER_TYPES_DNS},
	{"NetARSP", NET_SERVER_TYPES_ARSP},
	{"Net3G", NET_SERVER_TYPES_3G},
	{"NetMobile", NET_SERVER_TYPES_MOBILE},
	{"NetUPNP", NET_SERVER_TYPES_UPNP},	
	{"NetFTP", NET_SERVER_TYPES_FTP},
	{"NetWifi", NET_SERVER_TYPES_WIFI},	
	{"NetAlarmCenter", NET_SERVER_TYPES_ALARM_CENTER},	
	{"NetPlatMega", NET_SERVER_TYPES_NETPLAT_MEGA},	
	{"NetPlatXingWang", NET_SERVER_TYPES_NETPLAT_XINWANG},	
	{"NetPlatShiSou", NET_SERVER_TYPES_NETPLAT_SHISOU},	
	{"NetPlatVVEye", NET_SERVER_TYPES_NETPLAT_VVEYE},
	{"NetRTSP", NET_SERVER_TYPES_RTSP},	
	{"NetPhoneShortMsg", NET_SERVER_TYPES_SHORT_MSG},	
	{"NetPhoneMultimediaMsg", NET_SERVER_TYPES_MULTIMEDIA_MSG},	
	{"NetDAS", NET_SERVER_TYPES_DAS},
	{"NetLocalSdkPlatform",NET_SERVER_TYPES_LOCALSDK_PLATFORM}, 
	{"NetGodEyeAlarm",NET_SERVER_TYPES_GOD_EYE}, 
	{"NetNat",NET_SERVER_TYPES_NAT}, 
	{"NetVPN",NET_SERVER_TYPES_VPN},
	{"NetMediaStream",NET_SERVER_TYPES_MEDIASTREAM},
	{NULL},
};

static ConfigPair s_vPreviewTypeMap[] = 
{
	{"Tour", PREVIEW_TYPES_TOUR},
	{"GUISet", PREVIEW_TYPES_TALK},
	{NULL},
};

static ConfigPair s_vCommTypeMap[] = 
{
	{"CommRS485", COMM_TYPES_RS485},
	{"CommRS232", COMM_TYPES_RS232},
	{NULL},
};

static ConfigPair s_vInputMethodMap[] = 
{
	{"NoSupportChinese", NO_SUPPORT_CHINESE},
	{NULL},
};

static ConfigPair s_vTipShowMap[] = 
{
	{"NoBeepTipShow", NO_BEEP_TIP_SHOW},
	{"NoFTPTipShow", NO_FTP_TIP_SHOW},
	{"NoEmailTipShow", NO_EMAIL_TIP_SHOW},
	{NULL},
};

static ConfigPair s_vMobileDVRMap[] = 
{
	{"StatusExchange", MOBILEDVR_STATUS_EXCHANGE},
	{"DelaySet", MOBILEDVR_DELAY_SET},
	{"CarPlateSet",MOBILEDVR_CARPLATE_SET},
	{"GpsTiming",MOBILEDVR_GPS_TIMING},
	{NULL},
};

static ConfigPair s_vOtherFunctionMap[] = 
{
	{"DownLoadPause", OTHER_DOWNLOADPAUSE},
	{"USBsupportRecord", OTHER_USB_SUPPORT_RECORD},
	{"SDsupportRecord", OTHER_SD_SUPPORT_RECORD},
	{"SupportOnvifClient", OTHER_ONVIF_CLIENT_SUPPORT},
	{NULL},
};


template<> void exchangeTable<SystemFunction>(CConfigTable &table, SystemFunction &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < ENCODE_FUNCTION_TYPE_NR; i++)
	{
		exchanger.exchange(table["EncodeFunction"], s_vEncodeTypeMap[i].name, config.vEncodeFunction[i]);
	}
	for (int i = 0; i < ALARM_FUNCTION_TYPE_NR; i++)
	{
		exchanger.exchange(table["AlarmFunction"], s_vAlarmTypeMap[i].name, config.vAlarmFunction[i]);
	}
	for (int i = 0; i < NET_SERVER_TYPES_NR; i++)
	{
		exchanger.exchange(table["NetServerFunction"], s_vNetServerMap[i].name, config.vNetServerFunction[i]);
	}
	for (int i = 0; i < PREVIEW_TYPES_NR; i++)
	{
		exchanger.exchange(table["PreviewFunction"], s_vPreviewTypeMap[i].name, config.vPreviewFunction[i]);
	}
	for(int i=0; i < COMM_TYPES_NR; i++)
	{
		exchanger.exchange(table["CommFunction"], s_vCommTypeMap[i].name, config.vCommFunction[i]);
	}
	for(int i=0; i < NO_SUPPORT_NR; i++)
	{
		exchanger.exchange(table["InputMethod"], s_vInputMethodMap[i].name, config.vInputMethodFunction[i]);
	}
	for(int i=0; i < NO_TIP_SHOW_NR; i++)
	{
		exchanger.exchange(table["TipShow"], s_vTipShowMap[i].name, config.vTipShowFunction[i]);
	}
	for(int i=0; i < MOBILEDVR_NR; i++)
	{
		exchanger.exchange(table["MobileDVR"], s_vMobileDVRMap[i].name, config.vMobileDVRFunction[i]);
	}
	for(int i=0; i < OTHER_NR; i++)
	{
		exchanger.exchange(table["OtherFunction"], s_vOtherFunctionMap[i].name, config.vOtherFunction[i]);
	}

}

template<> void exchangeTable<BlindDetectFunction>(CConfigTable &table, BlindDetectFunction &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "BlindCoverNum", config.iBlindConverNum);
}

template<> void exchangeTable<MotionDetectFunction>(CConfigTable &table, MotionDetectFunction &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "GridRow", config.iGridRow);
	exchanger.exchange(table, "GridColumn", config.iGridColumn);
}

template<> void exchangeTable<DDNSServiceFunction>(CConfigTable &table, DDNSServiceFunction &config, int state)
{
	assert(state == CKeyExchange::ES_LOADING);
	for (uint i = 0; i < table.size(); i++)
	{
		config.vDDNSTypes.push_back(table[i].asString());
	}
}
/*
static ConfigPair s_commProtocol[] = 
{
	{"Console", CONSOLE},
	{"KeyBoard", KEYBOARD},
	{NULL,	}
};*/

template<> void exchangeTable<CommFunction>(CConfigTable &table, CommFunction &config, int state)
{
	assert(state == CKeyExchange::ES_LOADING);
	for (uint i = 0; i < table.size(); i++)
	{
		config.vCommProtocols.push_back(table[i].asString());
	}
}

template<> void exchangeTable<PTZProtocolFunction>(CConfigTable &table, PTZProtocolFunction &config, int state)
{
	assert(state == CKeyExchange::ES_LOADING);
	for (uint i = 0; i < table.size(); i++)
	{
		config.vPTZProtocols.push_back(table[i].asString());
	}
}

template<> void exchangeTable<MultiLangFunction>(CConfigTable &table, MultiLangFunction &config, int state)
{
	//assert(state == CKeyExchange::ES_LOADING);
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{
		for (uint i = 0; i < table.size(); i++)
		{
			config.vMultiLanguage.push_back(table[i].asString());
		}
	}
	else
	{
		int i = 0;
		for (std::vector<std::string>::const_iterator pi = config.vMultiLanguage.begin(); pi != config.vMultiLanguage.end(); i++, pi++)
		{
			table[i] = (*pi).c_str();
		}
	}
}

template<> void exchangeTable<MultVstdFunction>(CConfigTable &table, MultVstdFunction &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{
		const char *str = table.asCString();
		const char *psz = str;
		std::string item;
			
		while(*psz)
		{
			if(*psz++ == '|')
			{
				item.assign(str, psz - str - 1);
				config.vMultiVstd.push_back(item);
				str = psz;
			}
		}
		item.assign(str, psz - str);
		config.vMultiVstd.push_back(item);
	}
	else
	{
		int i = 0;
		std::string item;

		for (std::vector<std::string>::const_iterator pi = config.vMultiVstd.begin(); pi != config.vMultiVstd.end(); i++, pi++)
		{
			if (pi != config.vMultiVstd.begin())
			{
				item += "|";
			}
			item += (*pi);
		}
		table = item;
	}
}

static ConfigPair s_encodeStreamTypeMap[] =
{
	{"MainStream", CAPTURE_CHN_MAIN},
	{"ExtraStream2", CAPTURE_CHN_2END},
	{"ExtraStream3", CAPTURE_CHN_3IRD},
	{"ExtraStream4", CAPTURE_CHN_4RTH},
	{"JPEGStream", CAPTURE_CHN_JPEG},
	{NULL,}
};

static void exchangeStreamInfo(CConfigTable &table, EncodeInfo &config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable); 
	exchanger.exchange(table, "StreamType", config.iStreamType, s_encodeStreamTypeMap);
	exchanger.exchange(table, "HaveAudio", config.bHaveAudio);
	exchanger.exchange(table, "CompressionMask", config.uiCompression);
	exchanger.exchange(table, "ResolutionMask", config.uiResolution);
	
}

template<> void exchangeTable<EncodeAbility>(CConfigTable &table, EncodeAbility &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "MaxEncodePower", config.iMaxEncodePower);
	exchanger.exchange(table, "ChannelMaxSetSync", config.iChannelMaxSetSync);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(table["MaxEncodePowerPerChannel"], i, config.nMaxPowerPerChannel[i]);
		exchanger.exchange(table["ImageSizePerChannel"], i, config.ImageSizePerChannel[i]);
		exchanger.exchange(table["ExImageSizePerChannel"], i, config.ExImageSizePerChannel[i]);
	}
	for (uint i = 0; i < CAPTURE_CHN_NR; i++)
	{
		if (state == CKeyExchange::ES_LOADING)
		{
			if (i < table["EncodeInfo"].size())
			{
				exchangeStreamInfo(table["EncodeInfo"][i], config.vEncodeInfo[i], state);
			}
			else
			{	
				config.vEncodeInfo[i].bEnable = false;
			}

			if (i < table["CombEncodeInfo"].size())
			{
				exchangeStreamInfo(table["CombEncodeInfo"][i], config.vCombEncInfo[i], state);
			}
			else
			{	
				config.vCombEncInfo[i].bEnable = false;
			}
		}
		else
		{
			exchangeStreamInfo(table["EncodeInfo"][i], config.vEncodeInfo[i], state);
			exchangeStreamInfo(table["CombEncodeInfo"][i], config.vCombEncInfo[i], state);
		}
	}
	exchanger.exchange(table, "MaxBitrate", config.iMaxBps);

	for (int i = 0; i < N_SYS_CH; i++)
	{
		for (int j = 0; j < CAPTURE_SIZE_EXT_NR; j++)
		{
			exchanger.exchange(table["ExImageSizePerChannelEx"][i], j, config.ExImageSizePerChannelEx[i][j]);
		}
	}
}

template<> void exchangeTable<TalkAudioFormatFunction>(CConfigTable &table, TalkAudioFormatFunction &config, int state)
{
	::exchangeTable(table, config.audioFormat, state);
}

template<> void exchangeTable<NetOrderFunction>(CConfigTable &table, NetOrderFunction &config, int state)
{
       CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "NetOrder", config.bNetOrder);
}

template<> void exchangeTable<NetConnectAbility>(CConfigTable &table, NetConnectAbility &config, int state)
{
       CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "CurTcpNum", config.nCurTcpNum);
	exchanger.exchange(table, "MaxTcpNum", config.nMaxTcpNum);
	exchanger.exchange(table, "CurNatNum", config.nCurNatNum);
	exchanger.exchange(table, "MaxNatNum", config.nMaxNatNum);
}

template<> void exchangeTable<CarStatusNum>(CConfigTable &table, CarStatusNum &config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "CarStatusNum", config.iCarStatusNum);
}

template<> void exchangeTable<VGAResolutionAbility>(CConfigTable &table, VGAResolutionAbility &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{
		for (uint i = 0; i < table.size(); i++)
		{
			config.vAbilityVGA.push_back(table[i].asString());
		}
	}
	else
	{
		int i = 0;
		for (std::vector<std::string>::const_iterator pi = config.vAbilityVGA.begin(); pi != config.vAbilityVGA.end(); i++, pi++)
		{
			table[i] = (*pi).c_str();
		}
	}
}

