#include "../Include/ExchangeAL/CommExchange.h"
#include "../Include/ExchangeAL/Exchange.h"

static ConfigPair s_ptzLinkTypeMap[] = 
{
	{"None", PTZ_LINK_NONE},
	{"Preset", PTZ_LINK_PRESET},
	{"Tour", PTZ_LINK_TOUR},
	{"Pattern", PTZ_LINK_PATTERN},
	{NULL,	}
};

void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state)
{
	char buf[32];
	CKeyExchange exchanger;

	exchanger.setState(state);
	switch(state) 
	{
	case CKeyExchange::ES_SAVING:
		snprintf(buf,
			sizeof(buf) - 1,
			"%d %02d:%02d:%02d-%02d:%02d:%02d",
			timesection.enable,
			timesection.startHour,
			timesection.startMinute,
			timesection.startSecond,
			timesection.endHour,
			timesection.endMinute,
			timesection.endSecond);
		table = buf;
		break;

	case CKeyExchange::ES_LOADING:
		sscanf(table.asString().c_str(),
			"%d %02d:%02d:%02d-%02d:%02d:%02d",
			&timesection.enable,
			&timesection.startHour,
			&timesection.startMinute,
			&timesection.startSecond,
			&timesection.endHour,
			&timesection.endMinute,
			&timesection.endSecond);
		break;

	default:
		break;
	}
}

void exchangeEventHandler(CConfigTable& table, EventHandler& hEvent, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "RecordMask", hEvent.dwRecord);
	exchanger.exchange(table, "RecordLatch", hEvent.iRecordLatch);
	exchanger.exchange(table, "TourMask", hEvent.dwTour);
	exchanger.exchange(table, "SnapShotMask", hEvent.dwSnapShot);
	exchanger.exchange(table, "AlarmOutMask", hEvent.dwAlarmOut);

	exchanger.exchange(table, "AlarmOutLatch", hEvent.iAOLatch);
	CConfigTable& tb1 = table["PtzLink"];

	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(tb1[i], 0, hEvent.PtzLink[i].iType, s_ptzLinkTypeMap);
		exchanger.exchange(tb1[i], 1, hEvent.PtzLink[i].iValue);
	}
	exchanger.exchange(table, "RecordEnable",hEvent.bRecordEn);
	exchanger.exchange(table, "TourEnable", hEvent.bTourEn);
	exchanger.exchange(table, "SnapEnable", hEvent.bSnapEn);
	exchanger.exchange(table, "AlarmOutEnable", hEvent.bAlarmOutEn);
	exchanger.exchange(table, "PtzEnable", hEvent.bPtzEn);
	exchanger.exchange(table, "TipEnable", hEvent.bTip);
	exchanger.exchange(table, "MailEnable", hEvent.bMail);
	exchanger.exchange(table, "MessageEnable", hEvent.bMessage);
	exchanger.exchange(table, "BeepEnable", hEvent.bBeep);
	exchanger.exchange(table, "VoiceEnable", hEvent.bVoice);
	exchanger.exchange(table, "FTPEnable", hEvent.bFTP);
	for (int i = 0; i < N_WEEKS; i++)
	{
		for (int j = 0; j < N_TSECT; j++)
		{
			exchangeTimeSection(table["TimeSection"][i][j],	hEvent.schedule.tsSchedule[i][j], state);
		}
	}
	
	exchanger.exchange(table, "MatrixMask", hEvent.dwMatrix);
	exchanger.exchange(table, "MatrixEnable", hEvent.bMatrixEn);
	exchanger.exchange(table, "LogEnable", hEvent.bLog);
	exchanger.exchange(table, "EventLatch", hEvent.iEventLatch);
	exchanger.exchange(table, "MsgtoNetEnable", hEvent.bMessagetoNet);

	exchanger.exchange(table, "ShowInfo", hEvent.bShowInfo);
	exchanger.exchange(table, "ShowInfoMask", hEvent.dwShowInfoMask);
	exchanger.exchange(table, "AlarmInfo", hEvent.pAlarmInfo);

	exchanger.exchange(table, "ShortMsgEnable", hEvent.bShortMsg);
	exchanger.exchange(table, "MultimediaMsgEnable", hEvent.bMultimediaMsg);
}

template<> void exchangeTable<GenericEventConfig>(CConfigTable &table, GenericEventConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}

template<> void exchangeTable<GenericEventConfigAll>(CConfigTable &table, GenericEventConfigAll &config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vGenericEventConfig[i], state);
	}
}


template<> void exchangeTableV2<GenericEventConfigAll>(CConfigTable &table, GenericEventConfigAll &config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vGenericEventConfig[i], state);
	}
}

template<> void exchangeTable<MotionDetectConfig>(CConfigTable &table, MotionDetectConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Level", config.iLevel);
	CConfigTable& tableMDRegion = table["Region"];
	for(int i = 0; i < MD_REGION_ROW; i++)
	{
		exchanger.exchange(tableMDRegion, i, config.mRegion[i]);
	}
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}
template<> void exchangeTable<MotionDetectConfigAll>(CConfigTable &table, MotionDetectConfigAll &config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vMotionDetectAll[i], state);
	}
}


template<> void exchangeTableV2<MotionDetectConfigAll>(CConfigTable &table, MotionDetectConfigAll &config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vMotionDetectAll[i], state);
	}
}

template<> void exchangeTable<LossShowStrConfig>(CConfigTable &table, LossShowStrConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ShowStr", config.ShowStr);

}
template<> void exchangeTable<LossShowStrConfigALL>(CConfigTable &table, LossShowStrConfigALL &config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vLossShowStrAll[i], state);
	}
}

template<> void exchangeTableV2<LossShowStrConfigALL>(CConfigTable &table, LossShowStrConfigALL &config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vLossShowStrAll[i], state);
	}
}

template<> void exchangeTable<BlindDetectConfig>(CConfigTable &table, BlindDetectConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Level", config.iLevel);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}
template<> void exchangeTable<BlindDetectConfigAll>(CConfigTable &table, BlindDetectConfigAll &config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vBlindDetectAll[i], state);
	}
}


template<> void exchangeTableV2<BlindDetectConfigAll>(CConfigTable &table, BlindDetectConfigAll &config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vBlindDetectAll[i], state);
	}
}

static ConfigPair s_sensorTypeMap[] = 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	{"NC", 0},
	{"NO", 1},
	{NULL, }
};

template<> void exchangeTable<AlarmConfig>(CConfigTable &table, AlarmConfig &config, int state)
{
	CKeyExchange exchanger;


	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "SensorType", config.iSensorType, s_sensorTypeMap);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}

template<> void exchangeTable<AlarmConfigAll>(CConfigTable &table, AlarmConfigAll &config, int state)
{
	for (int i = 0; i < N_ALM_IN; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vAlarmConfigAll[i], state);
	}
}
template<> void exchangeTableV2<AlarmConfigAll>(CConfigTable &table, AlarmConfigAll &config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		exchangeTable(table[i], config.vAlarmConfigAll[i], state);
	}
}

static ConfigPair s_AlarmOutTypeMap[] = 
{
	{"AUTO", 0},
	{"MANUAL", 1},
	{"CLOSE", 2},
	{NULL, }
};

static ConfigPair s_AlarmOutStatusMap[] = 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	{"OPEN", 0},
	{"CLOSE", 1},
	{NULL, }
};

template<> void exchangeTable<AlarmOutConfig>(CConfigTable &table, AlarmOutConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "AlarmOutType", config.nAlarmOutType, s_AlarmOutTypeMap);
	exchanger.exchange(table, "AlarmOutStatus", config.nAlarmOutStatus, s_AlarmOutStatusMap);
}

template<> void exchangeTable<AlarmOutConfigAll>(CConfigTable &table, AlarmOutConfigAll &config, int state)
{
	for (int i = 0; i < N_ALM_OUT; i++)
	{
		exchangeTable(table[i], config.vAlarmOutConfigAll[i], state);
	}
}

template<> void exchangeTable<StorageLowSpaceConfig>(CConfigTable &table, StorageLowSpaceConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "LowerLimit", config.iLowerLimit);
	exchanger.exchange(table, "RecordTimeEnable", config.bRecordTimeEnable);
	exchanger.exchange(table, "RecordTime", config.iRecordTime);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}

template<> void exchangeTable<StorageFailConfig>(CConfigTable &table, StorageFailConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "RebootEnable", config.bRebootEnable);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
}

enum ENUMPARITY
{
	NOPARITY = 0,
	ODDPARITY,
	EVENPARITY,
	MARKPARITY,
	SPACEPARITY,
	PARITYS
};

enum 
{
	CA_BOUDBASE = 0,
	CA_PARITY = 1,
	CA_DATABITS = 2,
	CA_STOPBITS = 3,
};

static ConfigPair s_commPariity[] = 
{
	{"None", NOPARITY},
	{"Odd", ODDPARITY},
	{"Even", EVENPARITY},
	{"Mark", MARKPARITY},
	{"Space", SPACEPARITY},
	{NULL, }
};
void CommExchange(CConfigTable& table, COMMATTRI& commattri, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, CA_BOUDBASE,	commattri.iBaudRate);
	exchanger.exchange(table, CA_PARITY, commattri.iParity, s_commPariity);
	exchanger.exchange(table, CA_DATABITS,	commattri.iDataBits);
	exchanger.exchange(table, CA_STOPBITS,	commattri.iStopBits);
}

template<> void exchangeTable<CONFIG_COMM_X>(CConfigTable& table, CONFIG_COMM_X& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ProtocolName", config.sProtocolName);
	exchanger.exchange(table, "PortNo", config.iPortNo);
	CommExchange(table["Attribute"], config.aCommAttri, state);
}

template<> void exchangeTable<CommConfigAll>(CConfigTable& table, CommConfigAll& config, int state)
{
	for (int i = 0; i < COM_TYPES; i++)
	{
		exchangeTable(table[i], config.vCommConfig[i], state);
	}
}

template<> void exchangeTable<CONFIG_PTZ>(CConfigTable& table, CONFIG_PTZ& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ProtocolName", config.sProtocolName);
	exchanger.exchange(table,"DeviceNo",config.ideviceNo);
	exchanger.exchange(table, "NumberInMatrixs", config.iNumberInMatrixs);
	exchanger.exchange(table, "PortNo", config.iPortNo);
	CommExchange(table["Attribute"], config.dstComm, state);
}
template<> void exchangeTable<PTZConfigAll>(CConfigTable& table, PTZConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.ptzAll[i], state);
	}
}

template<> void exchangeTableV2<PTZConfigAll>(CConfigTable& table, PTZConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.ptzAll[i], state);
	}
}

template<> void exchangeTable<RS485ConfigAll>(CConfigTable& table, RS485ConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.rs485All[i], state);
	}
}

template<> void exchangeTableV2<RS485ConfigAll>(CConfigTable& table, RS485ConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.rs485All[i], state);
	}
}


//外部输入信息与车辆状态的对应关系

template<> void exchangeTable<CarStatusExchange>(CConfigTable& table, CarStatusExchange& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "StatusType",config.statusType);
	exchanger.exchange(table, "Exist", config.bExist);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "InputType", config.inputType);
	exchanger.exchange(table, "Addr", config.addr);
	exchanger.exchange(table, "SensorType", config.sensorType);
}

template<> void exchangeTable<CarStatusExchangeAll>(CConfigTable& table, CarStatusExchangeAll& config, int state)
{
	for (int i = 0; i < CAR_STATUS_NR; i++)
	{
		if (table[i].type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.exchangeAll[i], state);
	}
}

template<> void exchangeTable<CarDelayTimeConfig>(CConfigTable& table, CarDelayTimeConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "StartDelayEnable", config.bStartDelay);
	exchanger.exchange(table, "CloseDelayEnable", config.bCloseDelay);
	
	exchanger.exchange(table, "TimeStartDelay",config.timeStartDelay);
	exchanger.exchange(table, "TimeCloseDelay", config.timeCloseDelay);
}

//  GPS校时
template<> void exchangeTable<GPSTimingConfig>(CConfigTable& table, GPSTimingConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "TimeChange", config.timeChange);
	exchanger.exchange(table, "UpdatePeriod",config.updatePeriod);
}

//  点间巡航
template<> void exchangeTable<TourState>(CConfigTable& table, TourState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Running", config.bRunning);
	exchanger.exchange(table, "LineID", config.lineID);
}
//  巡迹
template<> void exchangeTable<PatternState>(CConfigTable& table, PatternState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Running", config.bRunning);
	exchanger.exchange(table, "LineID", config.lineID);
}
//  线扫
template<> void exchangeTable<ScanState>(CConfigTable& table, ScanState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Running", config.bRunning);
	exchanger.exchange(table, "LineID", config.lineID);
}
//  水平旋转
template<> void exchangeTable<PanonState>(CConfigTable& table, PanonState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Running", config.bRunning);
	exchanger.exchange(table, "LineID", config.lineID);
}
//  云台状态
template<> void exchangeTable<PtzState>(CConfigTable& table, PtzState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchangeTable(table[Json::StaticString("TourState")],    config.tourState, state);
	exchangeTable(table[Json::StaticString("PatternState")], config.patternState, state);
	exchangeTable(table[Json::StaticString("ScanState")],    config.scanState, state);
	exchangeTable(table[Json::StaticString("PanonState")],   config.panonState, state);
}

//  恢复云台状态
template<> void exchangeTable<ResumePtzState>(CConfigTable& table, ResumePtzState& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	for(int i = 0; i < N_SYS_CH; i++)
	{
//		trace("i = %d\n", i);
		if (table[Json::StaticString("PtzState")][i].type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[Json::StaticString("PtzState")][i], config.ptzStateAll[i], state);
	}
}
