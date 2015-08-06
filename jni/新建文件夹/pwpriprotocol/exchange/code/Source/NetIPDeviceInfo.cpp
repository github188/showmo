//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "../Include/ExchangeAL/NetIPDeviceInfo.h"
#include "../Include/ExchangeAL/Exchange.h"

template<> void exchangeTable<SystemInformation>(CConfigTable &table, SystemInformation &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SoftWareVersion", config.sSoftWareVersion);
	exchanger.exchange(table, "HardWareVersion", config.sHardWareVersion);
	exchanger.exchange(table, "EncryptVersion", config.sEncryptVersion);
	exchangeSysTime(exchanger, table["BuildTime"], config.tmBuildTime);
	exchanger.exchange(table, "SerialNo", config.sSerialNo);
	exchanger.exchange(table, "VideoInChannel", config.iVideoInChannel);
	exchanger.exchange(table, "VideoOutChannel", config.iVideoOutChannel);
	exchanger.exchange(table, "AlarmInChannel", config.iAlarmInChannel);
	exchanger.exchange(table, "AlarmOutChannel", config.iAlarmOutChannel);
	exchanger.exchange(table, "TalkInChannel", config.iTalkInChannel);
	exchanger.exchange(table, "TalkOutChannel", config.iTalkOutChannel);
	exchanger.exchange(table, "ExtraChannel", config.iExtraChannel);
	exchanger.exchange(table, "AudioInChannel", config.iAudioInChannel);
	exchanger.exchange(table, "CombineSwitch", config.iCombineSwitch);
	exchanger.exchange(table, "DigChannel", config.iDigChannel);
	exchanger.exchange( table,"DeviceRunTime",config.uiDeviceRunTime);
	exchanger.exchange(table, "HardWare", config.sHardWare);
	exchanger.exchange(table, "UUID", config.sUuid);

}

template<> void exchangeTable<DriverInformation>(CConfigTable &table, DriverInformation &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "DirverType", config.iDriverType);
	exchanger.exchange(table, "IsCurrent", config.bIsCurrent);
	exchanger.exchange(table, "TotalSpace", config.uiTotalSpace);
	exchanger.exchange(table, "RemainSpace", config.uiRemainSpace);
	exchanger.exchange(table, "Status", config.iStatus);
	exchanger.exchange(table, "LogicSerialNo", config.iLogicSerialNo);
	exchangeSysTime(exchanger, table["NewStartTime"], config.tmStartTimeNew);
	exchangeSysTime(exchanger, table["NewEndTime"], config.tmEndTimeNew);
	exchangeSysTime(exchanger, table["OldStartTime"], config.tmStartTimeOld);
	exchangeSysTime(exchanger, table["OldEndTime"], config.tmEndTimeOld);
}

template<> void exchangeTable<StorageDeviceInformation>(CConfigTable &table, StorageDeviceInformation &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PlysicalNo", config.iPhysicalNo);
	exchanger.exchange(table, "PartNumber", config.iPartNumber);
	for (int i = 0; i < MAX_DRIVER_PER_DISK; i++)
	{
		exchangeTable(table["Partition"][i], config.diPartitions[i], state);
	}
}

template<> void exchangeTable<StorageDeviceInformationAll>(CConfigTable &table, StorageDeviceInformationAll  &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{
		config.iDiskNumber = table.size();
	}

	if (config.iDiskNumber)
	{
		for (int i = 0; i < config.iDiskNumber; i++)
		{
			exchangeTable(table[i], config.vStorageDeviceInfoAll[i], state);
		}
	}
	else
	{
		// 当没有硬盘时，table表为空，WEB端解析时会挂掉，考虑到兼容性
		// 这里使用一个容量为0的硬盘信息表示没有安装磁盘
		memset(&config.vStorageDeviceInfoAll[0], 0, sizeof(StorageDeviceInformation));
		exchangeTable(table[0u], config.vStorageDeviceInfoAll[0u], state);
	}
}

template<> void exchangeTable<ChannelState>(CConfigTable &table, ChannelState &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Record", config.bRecord);
	exchanger.exchange(table, "Bitrate", config.iBitrate);
}

template<> void exchangeTable<WorkState>(CConfigTable &table, WorkState &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchangeTable(table["ChannelState"][i], config.vChnState[i], state);
	}

	CConfigTable& tb = table["AlarmState"];
	exchanger.exchange(tb, "VideoMotion", config.vAlarmState.iVideoMotion);
	exchanger.exchange(tb, "VideoBlind", config.vAlarmState.iVideoBlind);
	exchanger.exchange(tb, "VideoLoss", config.vAlarmState.iVideoLoss);
	exchanger.exchange(tb, "AlarmIn", config.vAlarmState.iAlarmIn);
	exchanger.exchange(tb, "AlarmOut", config.vAlarmState.iAlarmOut);
	//ExChgDebug("iVideoMotion: %d, iVideoBlind: %d, iVideoLoss: %d, iAlarmIn: %d, iAlarmOut: %d\n",
	//	config.vAlarmState.iVideoMotion, config.vAlarmState.iVideoBlind, config.vAlarmState.iVideoLoss,
	//	config.vAlarmState.iAlarmIn, config.vAlarmState.iAlarmOut);
}

ConfigPair s_WifiRSSI[] =
{
	{"NoSingnal", RSSI_NO_SIGNAL},
	{"VeryLow", RSSI_VERY_LOW},
	{"Low", RSSI_LOW},
	{"Good", RSSI_GOOD},
	{"VeryGood", RSSI_VERY_GOOD},
	{"Excellent", RSSI_EXCELLENT},
	{NULL,	}
};

template<> void exchangeTable<NetWifiDevice>(CConfigTable& table, NetWifiDevice& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SSID", config.strSSID);
	exchanger.exchange(table, "RSSI", config.nRSSI, s_WifiRSSI);
	exchanger.exchange(table, "Channel", config.nChannel);
	exchanger.exchange(table, "NetType", config.strNetType);
	exchanger.exchange(table, "EncrypType", config.strEncrypType);
	exchanger.exchange(table, "Auth", config.strAuth);
}

template<> void exchangeTable<NetWifiDeviceAll>(CConfigTable& table, NetWifiDeviceAll& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Numbers", config.nDevNumber);
	for (int i = 0; (i < config.nDevNumber) && (i < MAX_AP_NUMBER); i++)
	{
		exchangeTable(table["WifiAP"][i], config.vNetWifiDeviceAll[i], state);
	}
}

template<> void exchangeTable<OEMInfo>(CConfigTable& table, OEMInfo& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "OEMID", config.nOEMID);
	exchanger.exchange(table, "Name", config.strName);
	exchanger.exchange(table, "Telephone", config.strTel);
	exchanger.exchange(table, "Address", config.strAddr);
}
