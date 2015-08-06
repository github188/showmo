//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//	Modify:			配置字符串资源管理
//

#include "../Include/ExchangeAL/ExchangeKind.h"
#include "../Include/ExchangeAL/Exchange.h"

static char sNull[] = "NULL"; //作为未找到时的返回值

static ConfigPair s_defaultConfigKeyMaps[] =
{
	{"General", DEFAULT_CFG_GENERAL},
	{"Encode", DEFAULT_CFG_ENCODE},
	{"Record", DEFAULT_CFG_RECORD},
	{"NetService", DEFAULT_CFG_NET_SERVICE},
	{"NetCommon", DEFAULT_CFG_NET_COMMON},
	{"Alarm", DEFAULT_CFG_ALARM},
	{"PtzComm", DEFAULT_CFG_PTZCOMM},
	{"Account", DEFAULT_CFG_USERMANAGER},
	{"Preview", DEFAULT_CFG_PREVIEW},
	{"CameraPARAM", DEFAULT_CFG_CAMERA_PARAM},
	{NULL,},
};

/// 根据默认配置种类DefaultConfigKinds取得相应的名称
const char *getDefaultKindName(int iDefaultKind)
{
    if(iDefaultKind >= 0 && iDefaultKind < DEFAULT_CFG_END)
    {
        int index = getIndex(s_defaultConfigKeyMaps, iDefaultKind);

    	if(index >= 0 && index < DEFAULT_CFG_END)
        {
            return s_defaultConfigKeyMaps[index].name;
        }
    }
    return sNull;
}

int getDefaultKindIndex(const char *sDefaultName)
{
	if(sDefaultName)
    {
        int index = getIndex(s_defaultConfigKeyMaps, sDefaultName);

    	if(index >= 0 && index < DEFAULT_CFG_END)
        {
    	    return s_defaultConfigKeyMaps[index].value;
        }
    }
	return -1;
}

static ConfigPair s_configKeyMaps[] =
{
	{"AVEnc.Encode", CFG_ENCODE},
	{"AVEnc.VideoWidget", CFG_WIDEOWIDGET},
	{"AVEnc.VideoColor", CFG_VIDEOCOLOR},
	{"Record", CFG_RECORD},
	{"Detect.MotionDetect", CFG_MOTIONDETECT},
	{"Detect.BlindDetect", CFG_BLINDDETECT},
	{"Detect.LossDetect", CFG_LOSSDETECT},
	{"Alarm.LocalAlarm", CFG_LOCALALAEM},
	{"Alarm.NetAlarm", CFG_NETALAEM},
	{"Alarm.NetIPConflict", CFG_NETIPCONFLICT},
	{"Alarm.NetAbort", CFG_NETABORT},
	{"Storage.StorageNotExist", CFG_STORAGENOTEXIST},
	{"Storage.StorageLowSpace", CFG_STORAGELOWSPACE},
	{"Storage.StorageFailure", CFG_STORAGEFAILURE},
	{"Alarm.PTZAlarmProtocol", CFG_PTZALARM},
	{"NetWork.NetCommon", CFG_NETCOMMON},
	{"NetWork.NetIPFilter", CFG_NETIPFILTER},
	{"NetWork.NetDHCP", CFG_NETDHCP},
	{"NetWork.NetDDNS", CFG_NETDDNS},
	{"NetWork.NetEmail", CFG_NETEMAIL},
	{"NetWork.NetNTP", CFG_NETNTP},
	{"NetWork.NetPPPoE", CFG_NETPPPOE},
	{"NetWork.NetDNS", CFG_NETDNS},
	{"Uart.Comm", CFG_COMM},
	{"Uart.PTZ", CFG_PTZ},
	{"Uart.PTZPreset", CFG_PTZPRESET},
	{"Uart.PTZTour", CFG_PTZTOUR},
	{"fVideo.Tour", CFG_TOUR},
	{"fVideo.GUISet", CFG_GUISET},
	{"fVideo.TVAdjust", CFG_TVADJUST},
	{"fVideo.AudioInFormat", CFG_AUDIOINFORMAT},
	{"fVideo.Play", CFG_PLAY},
	{"General.General", CFG_GENERAL},
	{"General.Location", CFG_LOCATION},
	{"General.AutoMaintain", CFG_AUTOMAINTAIN},
	{"ChannelTitle", CFG_CHANNELTITLE},			///< 通道名称，特殊!!!!!
	{"Storage.Snapshot", CFG_SNAPSHOT},
	{"AVEnc.CombineEncode", CFG_COMBINEENCODE},
	{"AVEnc.CombineEncodeParam", CFG_COMBINEENCODEMODE},
	{"NetWork.NetFTP", CFG_NETFTP},
	{"NetWork.NetARSP", CFG_NETARSP},
	{"NetPlat.MegaNet", CFG_NETPLAT_MEGANET},
	{"NetPlat.MegaMotion", CFG_NETPLAT_MEGAMOTION},
	{"NetWork.RemoteDevice", CFG_NETDECORDER},
	{"General.SupportLang", CFG_ABILITY_SUPPORTLANGUAGE},
	{"Ability.SupportVstd", CFG_ABILITY_SUPPORTVSTD},
	{"NetWork.Net3G", CFG_3GNET},
	{"NetWork.NetMobile", CFG_MOBILE},
	{"Car.CarPlate", CFG_CARPLATE},
	{"Media.VideoChannel", CFG_VIDEOCHANNEL},
	{"NetWork.Upnp", CFG_NETUPNP},
	{"Ability.DeviceDesc", CFG_ABILITY_DEVDESC},
	{"Storage.StoragePosition", CFG_STORAGE_POSITION},
	{"NetWork.Wifi", CFG_WIFI},
	{"General.SystemState", CFG_SYSTEM_STATE},
	{"fVideo.VideoOut", CFG_VIDEOOUT},
	{"Ability.VGAResolution", CFG_ABILITY_VGARESOLUTION},
	{"Ability.GUIThemeList", CFG_ABILITY_THEMELIST},
	{"Simplify.Encode", CFG_ENCODE_SIMPLIFY},
	{"Uart.RS485", CFG_RS485},
	{"NetWork.AlarmServer", CFG_NETALARMSERVER},
	{"Alarm.AlarmOut", CFG_ALARMOUT},
	{"NetPlat.XinWang", CFG_NETPLAT_XINWANG},
	{"NetPlat.ShiSou", CFG_NETPLAT_SHISOU},
	{"NetPlat.VVEye", CFG_NETPLAT_VVEYE},
	{"AVEnc.WaterMark", CFG_MEDIA_WATERMARK},
	{"NetWork.RemoteDeviceV2", CFG_NETDECORDER_V2},
	{"NetPlat.WellSun", CFG_NETPLAT_WELLSUN},
	{"Ability.EncodePower", CFG_ABILITY_ENOCDE},
	{"NetWork.RTSP", CFG_NET_RTSP},
	{"Ability.SerialNo", CFG_ABILITY_SERIALNO},
	{"NetWork.RemoteDeviceV3", CFG_NETDECORDER_V3},
	{"NetWork.ChnMode",CFG_NETDECORDER_CHNMODE},
	{"NetWork.ChnStatus",CFG_NETDECORDER_CHNSTATUS},
	{"Produce.Test", CFG_PRODUCE_TEST},
	{"NetWork.Pos", CFG_NET_POS},
	{"Camera.Param", CFG_CAMERA_PARAM},
	{"fVideo.OSDWidget", CFG_OSD_WIDGET},
   	{"NetWork.DAS", CFG_NET_DAS},
	{"NetPlat.NanRui", CFG_NETPLAT_NANRUI},
	{"fVideo.Spot", CFG_SPOT},
	{"fVideo.Volume", CFG_VOLUME},
	{"NetWork.ShortMsg", CFG_PHONE_SHORT_MSG},
	{"NetWork.MultimediaMsg", CFG_PHONE_MULTIMEDIA_MSG},
	{"Car.InputExchange", CFG_CAR_INPUT_EXCHANGE},
	{"Car.DelayTime", CFG_CAR_DELAY_TIME},
	{"NetWork.NetOrder", CFG_NET_ORDER},
	{"NetWork.LocalSdkPlatform",CFG_LOCALSDK_NET_PLATFORM},
	{"Car.GPSTiming", CFG_GPS_TIMING},
	{"Car.Display", CFG_CAR_DISPLAY},
	{"Detect.Analyze", CFG_VIDEO_ANALYZE},
	{"NetWork.GodEye",CFG_GODEYE_ALARM},
	{"Status.NatInfo",CFG_NAT_STATUS_INFO},
	{"AVEnc.EncodeStaticParam", CFG_ENCODE_STATICPARAM},
	{"fVideo.LossShowStr", CFG_LOSS_SHOW_STR},
	{"NetWork.DigManagerShow", CFG_DIGMANAGER_SHOW},
	{"fVideo.VideoOutPriority",CFG_VIDEOOUT_PRIORITY},
	{"NetWork.Nat",CFG_NAT},
	{"Detect.AnalyzeLink",CFG_ANALYZE_LINK},
	{"Guide",CFG_GUIDE},
	{"fVideo.VideoSeque", CFG_VIDEOSEQUE},
	{"General.AutoLogin",CFG_AUTOLOGIN},
	{"NetWork.VPN",CFG_VPN},
	{"General.ResumePtzState", CFG_RESUME_PTZ_STATE},
	{"Capture.Panoramic", CFG_CAPTURE_PANORAMIC},
	{"CameraTi.Param", CFG_CAMERA_DM8127},
	{"Chain.Param", CFG_CHAIN},
	{"NetWork.MediaStream", CFG_NET_MEDIA_STREAM},
	{"General.LedLight", CFG_LEDLIGHT}, //增加LED
	{"NetWork.WifiStatus", CFG_WIFISTATUS}, //增加wifi状态
	{NULL,}
};

/// 更具配置种类ConfigKinds取得相应名称
const char *getConfigName(int iConfigKind)
{
    if(iConfigKind >= 0 && iConfigKind < CFG_NR)
    {
        int index = getIndex(s_configKeyMaps, iConfigKind);
        if(index >= 0 && index < CFG_NR)
        {
            return s_configKeyMaps[index].name;
        }
    }

    return sNull;
}

int getConfigIndex(const char *sConfigKind)
{
    if(sConfigKind)
    {
        int index = getIndex(s_configKeyMaps, sConfigKind);

        if(index >= 0 && index < CFG_NR)
        {
            return s_configKeyMaps[index].value;
        }
    }
    return -1;
}

static ConfigPair s_abilityKindsMap[] =
{
	{"SystemFunction", ABILITY_SYSTEM_FUNCTION},
	{"EncodeCapability", ABILITY_ENCODE},
	{"BlindCapability", ABILITY_BLIND},
	{"MotionArea", ABILITY_MOTION},
	{"DDNSService", ABILITY_DDNS_TYPES},
	{"ComProtocol", ABILITY_COMM_PROTOCOLS},
	{"PTZProtocol", ABILITY_PTZ_PROTOCOLS},
	{"TalkAudioFormat", ABILITY_TALK_ATTRIBUTE},
	{"MultiLanguage", ABILITY_MULTI_LANG},
	{"LanguageList", ABILITY_LANG_LIST},
	{"MultiVstd", ABILITY_MULTI_VSTD},
	{"VstdList", ABILITY_VSTD_LIST},
	{"UartProtocol", ABILITY_UART_PROTOCOLS},
	{"Camera", ABILITY_CAMERA},
	{"NetOrder",ABILITY_NETORDER},
	{"Intelligent", ABILITY_INTELLIGENT},
	{"NetAbility",ABILITY_NET_CONNECT},
	{"CarStatusNum", ABILITY_CARSTATUSNUM},
	{"VGAresolution",ABILITY_VGARESOLUTION},
	{NULL},
};

const char *getAbilityName(int iAbilityKind)
{
    if(iAbilityKind >= 0 && iAbilityKind < ABILITY_KIND_NR)
    {
        int index = getIndex(s_abilityKindsMap, iAbilityKind);
        if(index >= 0 && index < ABILITY_KIND_NR)
        {
            return s_abilityKindsMap[index].name;
        }
    }
    return sNull;
}

int getAbilityIndex(const char *sAbilityKind)
{
    if(sAbilityKind)
    {
    	int index = getIndex(s_abilityKindsMap, sAbilityKind);

        if(index >= 0 && index < ABILITY_KIND_NR)
        {
            return s_abilityKindsMap[index].value;
        }
    }
    return -1;
}

static ConfigPair s_deviceInfoKindMap[] =
{
	{"SystemInfo", DEVICE_INFO_SYSTEM},
	{"StorageInfo", DEVICE_INFO_STORAGE},
	{"WorkState", DEVICE_INFO_WORKSTATE},
	{"WifiAP", DEVICE_INFO_WIFI_AP},
	{"OEMInfo", DEVICE_OEM_INFO},
	{NULL}
};

const char *getDeviceInfoName(int iDeviceInfoKind)
{
    if(iDeviceInfoKind >= 0 && iDeviceInfoKind < DEVICE_INFO_NR)
    {
        int index = getIndex(s_deviceInfoKindMap, iDeviceInfoKind);
        if(index >= 0 && index < DEVICE_INFO_NR)
        {
            return s_deviceInfoKindMap[index].name;
        }
    }
	return sNull;
}

int getDeviceInfoIndex(const char *sDeviceInfoKind)
{
    if(sDeviceInfoKind)
    {
        int index = getIndex(s_deviceInfoKindMap, sDeviceInfoKind);
        if(index >= 0 && index < DEVICE_INFO_NR)
        {
            return s_deviceInfoKindMap[index].value;
        }
    }
    return -1;
}

/// 基本系统操作
static ConfigPair s_systemOperationsMaps[] =
{
	{"OPMachine", OPERATION_MACHINE},
	{"OPDefaultConfig", OPERATION_DEFAULT_CONFIG},
	{"OPPTZControl", OPERATION_PTZ},
	{"OPMonitor", OPERATION_MONITOR},
	{"OPPlayBack", OPERATION_PLAYBACK},
	{"OPTalk", OPERATION_TALK},
	{"OPStorageManager", OPERATION_DISK_MANAGER},
	{"OPLogManager", OPERATION_LOG_MANAGER},
	{"OPSystemUpgrade", OPERATION_SYSTEM_UPGRADE},
	{"OPFileQuery", OPERATION_FILE_QUERY},
	{"OPLogQuery", OPERATION_LOG_QUERY},
	{"OPTimeSetting", OPERATION_TIME_SETTING},
	{"OPNetKeyboard", OPERATION_NET_KEYBOARD},
	{"OPNetAlarm", OPERATION_NET_ALARM},
	{"OPSNAP", OPERATION_SNAP},
	{"OPTrans", OPERATION_TRANS},
	{"OPTUpData",OPERATION_UPDATA},
	{"OPTimeSettingNoRTC", OPERATION_TIME_SETTING_NORTC},
	{"OPCPCData", OPERATION_CPCDATA},
	{NULL}
};

const char *getOperationName(int iOperationKind)
{
    if(iOperationKind >= 0 &&  iOperationKind < OPERATION_NR)
    {
        int index = getIndex(s_systemOperationsMaps, iOperationKind);
        if(index >= 0 &&  index < OPERATION_NR)
        {
            return s_systemOperationsMaps[index].name;
        }
    }
    return sNull;
}

int getOperationIndex(const char *sOperationKind)
{
    int rVal = -1;
    if(sOperationKind)
    {
        int index = getIndex(s_systemOperationsMaps, sOperationKind);
        if(index >= 0 && index < OPERATION_NR)
        {
            rVal = s_systemOperationsMaps[index].value;
        }
    }
    return rVal;
}

static ConfigPair s_eventTypeMaps[] =
{
	{"LocalAlarm", appEventAlarmLocal},
	{"NetAlarm", appEventAlarmNet},
	{"MotionDetect", appEventVideoMotion},
	{"LossDetect", appEventVideoLoss},
	{"BlindDetect", appEventVideoBlind},
	{"VideoSplit", appEventVideoSplit},
	{"VideoTour", appEventVideoTour},
	{"VideoTitle", appEventVideoTitle},
	//{"StorageNotExist", appEventVideoTour},
	{"StorageFailure", appEventStorageFailure},
	{"StorageLowSpace", appEventStorageLowSpace},
	{"StorageNotExist", appEventStorageNotExist},
	{"StorageReadError", appEventStorageReadErr},
	{"StorageWriteError", appEventStorageWriteErr},
	{"NetAbort", appEventNetAbort},
	{"NetIPConflict", appEventIPConflict},
	{"ShutDown", appEventShutdown},
	{"Reboot", appEventReboot},
	{"Upgrade", appEventUpgrade},
	{"Backup", appEventBackup},
	{"Comm", appEventComm},
	{"Emergency", appEventAlarmEmergency},	// FIXME NOW it is useless
	{"DecConnect", appEventDecConnect},		// FIXME
	{"ManualAlarm", appEventAlarmManual},	// FIXME
	{"NewFile", appEventNewFile},
	{"VideoAnalyze",appEventVideoAnalyze},
	{NULL,}
};


const char *getEventName(int kind)
{
    if(kind >= 0 && kind < appEventAll)
    {
        int index = getIndex(s_eventTypeMaps, kind);
    	if(index >= 0 && index < appEventAll)
        {
            return s_eventTypeMaps[index].name;
        }
    }
	return sNull;
}

int getEventIndex(const char *kinds)
{
	if(kinds)
    {
        int index = getIndex(s_eventTypeMaps, kinds);
    	if(index >= 0 && index < appEventAll)
        {
            return s_eventTypeMaps[index].value;
        }
    }
	return -1;
}

static ConfigPair s_eventTypeLogs[] =
{
	{"All", logTypeAll},
	{"SystemManager", logTypeSystem},
	{"ConfigOperation", logTypeConfig},
	{"StorageManager", logTypeStorage},
	{"AlarmEvent", logTypeAlarm},
	{"RecordOperation", logTypeRecord},
	{"UserManager", logTypeAccount},
	{"FileAccess", logTypeAccess}
};

/// 得到日志类型名称
const char *getLogTypeName(int iLogType)
{
	int index = getIndex(s_eventTypeLogs, iLogType);

	if (index >= 0 && index < (int)(sizeof(s_eventTypeLogs)/sizeof(s_eventTypeLogs[0])))
	{
		return s_eventTypeLogs[index].name;
	}
	trace("@@@@@@@@@@@@@@@@@@@@@@@@@@@@unknown logtype:%d\n", iLogType);
	return "NULL";
}

struct LogPair
{
	const char* name;
	int value;
	int type;
};

static LogPair s_LogPairItem[] =
{
	{"Reboot", logItemReboot, logTypeSystem},
	{"ShutDown", logItemShutDown, logTypeSystem},
	{"ClearLog", logItemClearLog, logTypeSystem},
	{"ModifyTime", logItemModifyTime, logTypeSystem},
	{"ZeroBitrate", logItemZeroBitrate, logTypeSystem},
	{"Upgrade", logItemUpgrade, logTypeSystem},
	{"Exception", logItemException, logTypeSystem},
	{"Update", logItemUpdate, logTypeSystem},
	{"SetTime", logItemSetTime, logTypeSystem},
	{"SaveConfig", logItemSaveConfig,logTypeConfig},
	{"SetDriverType", logItemSetDriverType, logTypeStorage},
	{"ClearDriver", logItemClearDriver, logTypeStorage},
	{"StorageDeviceError", logItemStorageDeviceError, logTypeStorage},
	{"DiskChanged", logItemDiskChanged, logTypeStorage},
	{"EventStart", logItemEventStart, logTypeAlarm},
	{"EventStop", logItemEventStop, logTypeAlarm},
	{"Record", logItemRecord, logTypeRecord},
	{"LogIn", logItemLogIn,logTypeAccount},
	{"LogOut", logItemLogOut,logTypeAccount},
	{"AddUser", logItemAddUser,logTypeAccount},
	{"DeleteUser", logItemDeleteUser,logTypeAccount},
	{"ModifyUser", logItemModifyUser,logTypeAccount},
	{"ModifyPassword", logItemModifyPassword},
	{"AddGroup", logItemAddGroup,logTypeAccount},
	{"DeleteGroup", logItemDeleteGroup,logTypeAccount},
	{"ModifyGroup", logItemModifyGroup,logTypeAccount},
	{"AccountRestore", logItemAccountRestore,logTypeAccount},
	{"FileAccessError", logItemFileAccessError,logTypeAccess},
	{"FileSearch", logItemFileSearch,logTypeAccess},
	{"FileAccess", logItemFileAccess,logTypeAccess},
	{"RecoverTime", logItemRecoverTime,logTypeSystem},
};

const char *getLogData(int iLogItemKind)
{
	uint i;

	for (i = 0; i < sizeof(s_LogPairItem)/sizeof(s_LogPairItem[0]); i++)
	{
		if (s_LogPairItem[i].value== iLogItemKind)
		{
			break;
		}
	}
	if (i >= sizeof(s_LogPairItem)/sizeof(s_LogPairItem[0]))
	{
		trace("Unknown Log Kind:%d\n", iLogItemKind);
		return NULL;
	}
	return s_LogPairItem[i].name;
}

int getLogType(const char *sLog)
{
	uint i;

	for (i = 0; i < sizeof(s_LogPairItem)/sizeof(s_LogPairItem[0]); i++)
	{
		if (!strcmp(s_LogPairItem[i].name, sLog))
		{
			break;
		}
	}
	if (i >= sizeof(s_LogPairItem)/sizeof(s_LogPairItem[0]))
	{
		trace("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Unknown Log Kind:%s\n", sLog);
		return (int)NULL;
	}
	return s_LogPairItem[i].type;
}


//系统调试相关----------------------------------------------------------
static ConfigPair s_systemDebugMaps[] =
{
	{"DebugCamera", DEBUG_CAMERA},
	{"DebugShell", DEBUG_SHELL},
	{"DebugCameraSaveCmd",DEBUG_CAMERA_SAVE_CMD},
};

const char *getDebugName(int iDebugKind)
{
	uint i;

	for (i = 0; i < sizeof(s_systemDebugMaps)/sizeof(s_systemDebugMaps[0]); i++)
	{
		if (s_systemDebugMaps[i].value == iDebugKind)
		{
			break;
		}
	}
	if (i >= sizeof(s_systemDebugMaps)/sizeof(s_systemDebugMaps[0]))
	{
		trace("Unknown Log Kind:%d\n", iDebugKind);
		return sNull;
	}
	return s_systemDebugMaps[i].name;
}

int getDebugKind(const char *pDebugName)
{
	uint i;

	for (i = 0; i < sizeof(s_systemDebugMaps)/sizeof(s_systemDebugMaps[0]); i++)
	{
		if (!strcmp(s_systemDebugMaps[i].name, pDebugName))
		{
			break;
		}
	}
	if (i >= sizeof(s_systemDebugMaps)/sizeof(s_systemDebugMaps[0]))
	{
		trace("Unknown Log Kind:%s\n", pDebugName);
		return -1;
	}
	return s_systemDebugMaps[i].value;
}

