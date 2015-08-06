// 	Description:	
// 	Revisions:		Year-Month-Day  SVN-Author  Modification
//	Modify:			网络操作码Json格式交换
//

#include "../Include/ExchangeAL/NetIPOperation.h"
#include "../Include/ExchangeAL/Exchange.h"
#include "../Include/ExchangeAL/MediaExchange.h"
#include "../Include/Types/Defs.h"

static ConfigPair s_monitorActionMap[] =
{
	{"Start", MONITOR_ACTION_START},
	{"Stop", MONITOR_ACTION_STOP},
	{"Claim", MONITOR_ACTION_CLAIM},
	{"Pause", MONITOR_ACTION_PAUSE},
	{"Continue", MONITOR_ACTION_CONTINUE},
    {"Request", MONITOR_ACTION_REQUEST},   
	{NULL},
};

static ConfigPair s_monitorStreamTypeMap[] =
{
	{"Main", CAPTURE_CHN_MAIN},
	{"Extra1", CAPTURE_CHN_2END},
	{"Extra2", CAPTURE_CHN_3IRD},
	{NULL},
};

static ConfigPair s_monitorTransTypeMap[] = 
{
	{"TCP", MONITOR_TRANSMODE_TCP},
	{"UDP", MONITOR_TRANSMODE_UDP},
	{"MCAST", MONITOR_TRANSMODE_MCAST},
	{"RTP", MONITOR_TRANSMODE_RTP},
	{NULL,	}
};

static ConfigPair s_combinTypeMap[] = 
{
	{"COMBIN_1", COMBIN_1},
	{"COMBIN_2", COMBIN_2},
	{"COMBIN_3", COMBIN_3},
	{"COMBIN_4", COMBIN_4},
	{"COMBIN_5", COMBIN_5},
	{"COMBIN_6", COMBIN_6},
	{"COMBIN_7", COMBIN_7},
	{"COMBIN_8", COMBIN_8},
	{"COMBIN_9", COMBIN_9},
	{"COMBIN_10", COMBIN_10},
	{"COMBIN_11", COMBIN_11},
	{"COMBIN_12", COMBIN_12},
	{"COMBIN_13", COMBIN_13},
	{"COMBIN_14", COMBIN_14},
	{"COMBIN_15", COMBIN_15},
	{"COMBIN_16", COMBIN_16},
	{"COMBIN_1_4", COMBIN_1_4},
	{"COMBIN_5_8", COMBIN_5_8},
	{"COMBIN_9_12", COMBIN_9_12},
	{"COMBIN_13_16", COMBIN_13_16},
	{"COMBIN_1_8", COMBIN_1_8},
	{"COMBIN_9_16", COMBIN_9_16},
	{"COMBIN_1_9", COMBIN_1_9},
	{"COMBIN_8_16", COMBIN_8_16},
	{"COMBIN_1_16", COMBIN_1_16},
	{"CONNECT_ALL", CONNECT_ALL},
	{"NONE", COMBIN_NONE},
	{NULL,	}
};

template<> void exchangeTable<MonitorControl>(CConfigTable &table, MonitorControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_monitorActionMap);
	exchanger.exchange(table["Parameter"], "Channel", config.iChannel);
	exchanger.exchange(table["Parameter"], "StreamType", config.iStreamType, s_monitorStreamTypeMap);
	exchanger.exchange(table["Parameter"], "TransMode", config.iTransMode, s_monitorTransTypeMap);
	exchanger.exchange(table["Parameter"], "CombinMode", config.iCombinType, s_combinTypeMap);
}

static ConfigPair s_playBackActionMap[] =
{
	{"Start", PLAY_BACK_ACTION_START},
	{"Stop", PLAY_BACK_ACTION_STOP},
	{"Pause", PLAY_BACK_ACTION_PAUSE},
	{"Continue", PLAY_BACK_ACTION_CONTINUE},
	{"Locate", PLAY_BACK_ACTION_LOCATE},
	{"EOF", PLAY_BACK_ACTION_EOF},
	{"Claim", PLAY_BACK_ACTION_CLAIM},
	{"DownloadStart", PLAY_BACK_ACTION_DOWNLOADSTART},
	{"DownloadStop", PLAY_BACK_ACTION_DOWNLOADSTOP},
	{"Fast", PLAY_BACK_ACTION_FAST},
	{"Slow", PLAY_BACK_ACTION_SLOW},
	{"Request", PLAY_BACK_ACTION_REQUEST},
    {"DownooadRequest", PLAY_BACK_ACTION_DOWNLOAD_REQUEST},
	{"DownloadPause", PLAY_BACK_ACTION_DOWNLOAD_PAUSE},
	{"DownloadContinue", PLAY_BACK_ACTION_DOWNLOAD_CONTINUE},
	{NULL}
};

static ConfigPair s_playBackModeMap[] =
{
	{"ByTime", PLAYBACK_BY_TIME},
	{"ByName", PLAYBACK_BY_NAME},
	{NULL}
};


template<> void exchangeTable<PlayBackControl>(CConfigTable &table, PlayBackControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_playBackActionMap);
	exchanger.exchange(table["Parameter"], "TransMode", config.iTransMode, s_monitorTransTypeMap);
	exchanger.exchange(table["Parameter"], "PlayMode", config.iPlayMode, s_playBackModeMap);
	exchanger.exchange(table["Parameter"], "Value", config.iValue);
	exchanger.exchange(table["Parameter"], "FileName", config.sFileName);
	exchangeSysTime(exchanger, table["StartTime"], config.stStartTime);
	exchangeSysTime(exchanger, table["EndTime"], config.stEndTime);
}

static ConfigPair s_machineActionMap[] =
{
	{"Reboot", MACHINE_ACTION_REBOOT},
	{"ShutDown", MACHINE_ACTION_SHUTDOWN},
	{NULL,}
};

template<> void exchangeTable<MachineControl>(CConfigTable &table, MachineControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_machineActionMap);
}

static ConfigPair s_defaultConfigTypeMap[] =
{
	{"General", DEFAULT_CFG_GENERAL},
	{"Encode", DEFAULT_CFG_ENCODE},
	{"Record", DEFAULT_CFG_RECORD},
	{"CommPtz", DEFAULT_CFG_PTZCOMM},
	{"NetServer", DEFAULT_CFG_NET_SERVICE},
	{"NetCommon", DEFAULT_CFG_NET_COMMON},
	{"Alarm", DEFAULT_CFG_ALARM},
	{"Account", DEFAULT_CFG_USERMANAGER},
	{"Preview", DEFAULT_CFG_PREVIEW},
	{"CameraPARAM", DEFAULT_CFG_CAMERA_PARAM},
	{NULL},
};

template<> void exchangeTable<DefaultConfigControl>(CConfigTable &table, DefaultConfigControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < DEFAULT_CFG_END; i++)
	{
		exchanger.exchange(table, s_defaultConfigTypeMap[getIndex(s_defaultConfigTypeMap, i)].name, config.vDefaultConfig[i]);
	}	
}

static ConfigPair s_talkControlTypesMap[] =
{
	{"Start", TALK_CONTROL_TYPES_START},
	{"Stop", TALK_CONTROL_TYPES_STOP},
	{"Claim", TALK_CONTROL_TYPES_CLAIM},
    {"Request", TALK_CONTROL_TYPES_REQUEST},   
	{NULL},
};

template<> void exchangeTable<TalkControl>(CConfigTable &table, TalkControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_talkControlTypesMap);
	AudioInFormatExchange(table["AudioFormat"], config.afAudioFormat, state);
}

static ConfigPair s_upgradeActionMap[] =
{
	{"Start", UPGRADE_ACTION_TYPES_START},
	{"Abort", UPGRADE_ACTION_TYPES_ABORT},
	{NULL},
};

static ConfigPair s_upgradeTypesMap[] =
{
	{"System", UPGRADE_TYPES_SYSTEM},
	{NULL},
};

template<> void exchangeTable<UpgradeControl>(CConfigTable &table, UpgradeControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_upgradeActionMap);
	exchanger.exchange(table, "Type", config.iType, s_upgradeTypesMap);
}

template<> void exchangeTable<UpgradeInfo>(CConfigTable &table, UpgradeInfo &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Serial", config.strSerial);
	exchanger.exchange(table, "Hardware", config.strHardware);
	exchanger.exchange(table, "Vendor", config.strVendor);

	if (state == CKeyExchange::ES_LOADING)
	{
		if (table["LogoArea"]["Begin"].type() == Json::stringValue &&
			table["LogoArea"]["End"].type() == Json::stringValue)
		{
			exchanger.exchange(table["LogoArea"], "Begin", config.uiLogoArea[0]);
			exchanger.exchange(table["LogoArea"], "End", config.uiLogoArea[1]);
		}
		else
		{
			config.uiLogoArea[0] = 0;
			config.uiLogoArea[1] = 0;
		}
	}
	else
	{
		exchanger.exchange(table["LogoArea"], "Begin", config.uiLogoArea[0]);
		exchanger.exchange(table["LogoArea"], "End", config.uiLogoArea[1]);
	}
}

template<> void exchangeTable<SearchCondition>(CConfigTable &table, SearchCondition &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Channel",  config.iChannel);
	exchanger.exchange(table, "Type", config.sType);
	exchanger.exchange(table, "Event", config.sEvent);
	exchanger.exchange(table, "DriverTypeMask", config.uiDriverTypeMask);
	exchangeSysTime(exchanger, table["BeginTime"], config.stBeginTime);
	exchangeSysTime(exchanger, table["EndTime"], config.stEndTime);
}

template<> void exchangeTable<SearchByTime>(CConfigTable &table, SearchByTime &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "HighChannel",  config.nHighChannel);
	exchanger.exchange(table, "LowChannel", config.nLowChannel);
	exchanger.exchange(table, "Type", config.sType);
	exchanger.exchange(table, "Event", config.sEvent);
	exchanger.exchange(table, "Sync", config.iSync);
	exchangeSysTime(exchanger, table["BeginTime"], config.stBeginTime);
	exchangeSysTime(exchanger, table["EndTime"], config.stEndTime);
}

template<> void exchangeTable<FileList>(CConfigTable &table, FileList &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{	
		memset(&config, 0, sizeof(FileList));
		config.iNumFiles = table.size();
	}
	for (int i = 0; i < config.iNumFiles; i++)
	{
		exchanger.exchange(table[i], "DiskNo", config.Files[i].iDiskNo);
		exchanger.exchange(table[i], "SerialNo", config.Files[i].iSerialNo);
		exchanger.exchange(table[i], "FileLength", config.Files[i].uiFileLength);
		exchanger.exchange(table[i], "FileName", config.Files[i].sFileName);
		exchangeSysTime(exchanger, table[i]["BeginTime"], config.Files[i].stBeginTime);
		exchangeSysTime(exchanger, table[i]["EndTime"], config.Files[i].stEndTime);
	}
}

static ConfigPair s_logSearchKindMap[] =
{
	{"LogAll", LOG_SEARCH_KIND_TYPE_ALL},
	{"LogSystem", LOG_SEARCH_KIND_SYSTEM},
	{"LogConfig", LOG_SEARCH_KIND_TYPE_CONFIG},
	{"LogStorage", LOG_SEARCH_KIND_TYPE_STORAGE},
	{"LogAlarm", LOG_SEARCH_KIND_TYPE_ALAEM},
	{"LogRecord", LOG_SEARCH_KIND_TYPE_RECORD},
	{"LogAccount", LOG_SEARCH_KIND_TYPE_ACCOUNT},
	{"LogFile", LOG_SEARCH_KIND_TYPE_FILE},
	{NULL}
};

template<> void exchangeTable<LogSearchCondition>(CConfigTable &table, LogSearchCondition &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Type", config.iType, s_logSearchKindMap);
	exchanger.exchange(table, "LogPosition", config.iLogPosition);	
	exchangeSysTime(exchanger, table["BeginTime"], config.stBeginTime);
	exchangeSysTime(exchanger, table["EndTime"], config.stEndTime);
}

template<> void exchangeTable<LogList>(CConfigTable &table, LogList &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	if (state == CKeyExchange::ES_LOADING)
	{	
		memset(&config, 0, sizeof(LogList)); // 确定没有用到C++中的string等对象
		config.iNumLog = table.size();
	}
	for (int i = 0; i < config.iNumLog; i++)
	{
		exchanger.exchange(table[i], "Type", config.Logs[i].sType);
		exchanger.exchange(table[i], "User", config.Logs[i].sUser);
		exchanger.exchange(table[i], "Data", config.Logs[i].sData);
		exchanger.exchange(table[i], "Position", config.Logs[i].iLogPosition);

		exchangeSysTime(exchanger, table[i]["Time"], config.Logs[i].stLogTime);
	}
}

static ConfigPair s_logControlActionMaps[] =
{
	{"RemoveAll", LOG_CONTROL_REMOVEALL},
	{NULL,}
};

template<> void exchangeTable<LogControl>(CConfigTable &table, LogControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_logControlActionMaps);
}

static ConfigPair s_storageControlTypeMaps[] =
{
	{"SetType", STORAGE_DEVICE_CONTROL_SETTYPE},
	{"Recover", STORAGE_DEVICE_CONTROL_RECOVER},
	{"Partition", STORAGE_DEVICE_CONTROL_PARTITIONS},
	{"Clear", STORAGE_DEVICE_CONTROL_CLEAR},
	{NULL,}
};

static ConfigPair s_storageControlTypeMaps22[] =
{
	{"ReadWrite", 0},
	{"ReadOnly", 1},
	{"Events", 2},
	{"Redundant", 3},
	{"SnapShot", 4},
	{NULL,},
};

static ConfigPair s_storageControlTypeMaps33[] =
{
	{"Data", STORAGE_DEVICE_CLEAR_DATA},
	{"Partition", STORAGE_DEVICE_CLEAR_PARTITIONS},
	{NULL,},
};

static const char s_storageControlTypeMaps44[][16] = {"Record", "SnapShot"};

template<> void exchangeTable<StorageDeviceControl>(CConfigTable &table, StorageDeviceControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Action", config.iAction, s_storageControlTypeMaps);
	exchanger.exchange(table, "SerialNo", config.iSerialNo);
	exchanger.exchange(table, "PartNo", config.iPartNo);
	switch (config.iAction)
	{
	case STORAGE_DEVICE_CONTROL_SETTYPE:
		exchanger.exchange(table, "Type", config.iType, s_storageControlTypeMaps22);
		break;
	case STORAGE_DEVICE_CONTROL_RECOVER:
		break;
	case STORAGE_DEVICE_CONTROL_PARTITIONS:
		{
			for (int i = 0; i < MAX_DRIVER_PER_DISK; i++)
			{				
				exchanger.exchange(table["PartitionSize"][i], s_storageControlTypeMaps44[i], config.iPartSize[i]); 
			}
		}
		break;
	case STORAGE_DEVICE_CONTROL_CLEAR:
		exchanger.exchange(table, "Type", config.iType, s_storageControlTypeMaps33);
		break;
	};
}

static ConfigPair s_ptzControlSetPatternStatusMaps[] =
{
	{"Start", PTZ_PATTERN_STATUS_START},
	{"Stop", PTZ_PATTERN_STATUS_STOP},
	{NULL,}
};

static ConfigPair s_ptzMenuOperator[] = 
{
	{"Enter", PTZ_MENU_OPT_ENTER},
	{"Leave", PTZ_MENU_OPT_LEAVE},
	{"OK", PTZ_MENU_OPT_OK},
	{"Cancel", PTZ_MENU_OPT_CANCEL},
	{"Up", PTZ_MENU_OPT_UP},
	{"Down", PTZ_MENU_OPT_DOWN},
	{"Left", PTZ_MENU_OPT_LEFT},
	{"Right", PTZ_MENU_OPT_RIGHT},
	{NULL}
};

static ConfigPair s_ptzOperationCommand[] = 
{
	{"DirectionUp", PTZ_OPERATION_DIRECTION_UP},	
	{"DirectionDown", PTZ_OPERATION_DIRECTION_DOWN},	
	{"DirectionLeft", PTZ_OPERATION_DIRECTION_LEFT},	
	{"DirectionRight", PTZ_OPERATION_DIRECTION_RIGHT},	
	{"DirectionLeftUp", PTZ_OPERATION_DIRECTION_LEFTUP},	
	{"DirectionLeftDown", PTZ_OPERATION_DIRECTION_LEFTDOWN},	
	{"DirectionRightUp", PTZ_OPERATION_DIRECTION_RIGHTUP},	
	{"DirectionRightDown", PTZ_OPERATION_DIRECTION_RIGHTDOWN},	
	{"ZoomWide", PTZ_OPERATION_ZOOMWIDE},
	{"ZoomTile", PTZ_OPERATION_ZOOMTILE},
	{"FocusNear", PTZ_OPERATION_FOCUSNEAR},
	{"FocusFar", PTZ_OPERATION_FOCUSFAR},	
	{"IrisLarge", PTZ_OPERATION_IRISLARGE},	
	{"IrisSmall", PTZ_OPERATION_IRISSMALL},	
	{"Alarm", PTZ_OPERATION_ALARM},	
	{"LightOn", PTZ_OPERATION_LIGHTON},	
	{"LightOff", PTZ_OPERATION_LIGHTOFF},
	{"SetPreset", PTZ_OPERATION_SETPRESET},
	{"ClearPreset", PTZ_OPERATION_CLEARPRESET},	
	{"GotoPreset", PTZ_OPERATION_GOTOPRESET},	
	{"AutoPanOn", PTZ_OPERATION_AUTOPANON},	
	{"AutoPanOff", PTZ_OPERATION_AUTOPANOFF},	
	{"SetLimitLeft", PTZ_OPERATION_SETLIMITLEFT},	
	{"SetLimitRight", PTZ_OPERATION_SETLIMITRIGHT},	
	{"AutoScanOn", PTZ_OPERATION_AUTOSCANON},	
	{"AutoScanOff", PTZ_OPERATION_AUTOSCANOFF},	
	{"AddTour", PTZ_OPERATION_ADDTOUR},	
	{"DeleteTour", PTZ_OPERATION_DELETETOUR},	
	{"StartTour", PTZ_OPERATION_STARTTOUR},	
	{"StopTour", PTZ_OPERATION_STOPTOUR},	
	{"ClearTour", PTZ_OPERATION_CLEARTOUR},	
	{"Position", PTZ_OPERATION_POSITION},	
	{"Aux", PTZ_OPERATION_AUX},	
	{"Menu", PTZ_OPERATION_MENU},	
	{"Flip", PTZ_OPERATION_FLIP},	
	{"Reset", PTZ_OPERATION_RESET},	
	{NULL,}
};

static ConfigPair s_PtzAuxStatus[] = 
{
	{"On", PTZ_AUX_ON},
	{"Off", PTZ_AUX_OFF},
	{NULL}
};

template<> void exchangeTable<PTZControl>(CConfigTable &table, PTZControl &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Command", config.iCommand, s_ptzOperationCommand);
	exchanger.exchange(table[Json::StaticString("Parameter")], "Step", config.parameter.iStep);
	exchanger.exchange(table[Json::StaticString("Parameter")], "Channel", config.parameter.iChannel);
	exchanger.exchange(table[Json::StaticString("Parameter")], "Preset", config.parameter.iPreset);
	exchanger.exchange(table[Json::StaticString("Parameter")], "Tour", config.parameter.iTour);
	exchanger.exchange(table[Json::StaticString("Parameter")], "Pattern", config.parameter.iPattern, s_ptzControlSetPatternStatusMaps);
	exchanger.exchange(table[Json::StaticString("Parameter")], "MenuOpts", config.parameter.iMenuOpts, s_ptzMenuOperator);
	exchanger.exchange(table[Json::StaticString("Parameter")]["AUX"], "Number", config.parameter.AUX.iNumber);
	exchanger.exchange(table[Json::StaticString("Parameter")]["AUX"], "Status", config.parameter.AUX.iStatus, s_PtzAuxStatus);
}

// TODO 完整的报警事件表
static ConfigPair s_eventTypeMap[] = 
{
	{"VideoMotion", appEventVideoMotion},
	{"VideoBlind", appEventVideoBlind},
	{"VideoLoss", appEventVideoLoss},
	{"LocalIO", appEventAlarmLocal},
	{"NetAlarm", appEventAlarmNet},
	{"CommAlarm", appEventComm},
	{"NetAbort", appEventNetAbort},
	{"StorageNotExist", appEventStorageNotExist},
	{"StorrageLowSpace", appEventStorageLowSpace},
	{"StorageFailure", appEventStorageFailure},
	{"ManualAlarm", appEventAlarmManual},
	{"VideoTitle", appEventVideoTitle},
	{"VideoSplit", appEventVideoSplit},
	{"VideoTour", appEventVideoTour},
	{"NetAbort", appEventNetAbort},
	{"CommAlarm", appEventComm},
	{"StorageFailureRead", appEventStorageReadErr},
	{"StorageFailureWrite", appEventStorageWriteErr},
	{"NetIPConflict", appEventIPConflict},
	{"VideoAnalyze",appEventVideoAnalyze},
	{NULL,	}
};

static ConfigPair s_eventStatusMap[] = 
{
	{"Start", appEventStart},
	{"Stop", appEventStop},
	{NULL,	}
};

template<> void exchangeTable<AlarmInfo>(CConfigTable &table, AlarmInfo &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Channel", config.nChannel);
	exchanger.exchange(table, "Event", config.iEvent, s_eventTypeMap);
	exchanger.exchange(table, "Status", config.iStatus, s_eventStatusMap);
	exchangeSysTime(exchanger, table["StartTime"], config.SysTime);
}

static ConfigPair s_kbValueMap[] = 
{
	{"0", NET_KEY_0},{"1", NET_KEY_1},{"2", NET_KEY_2},{"3", NET_KEY_3},{"4", NET_KEY_4},
	{"5", NET_KEY_5},{"6", NET_KEY_6},{"7", NET_KEY_7},{"8", NET_KEY_8},{"9", NET_KEY_9},
	{"10", NET_KEY_10},{"11", NET_KEY_11},{"12", NET_KEY_12},{"13", NET_KEY_13},{"14", NET_KEY_14},
	{"15", NET_KEY_15},{"16", NET_KEY_16},{"10Plus", NET_KEY_10PLUS},
	{"Up", NET_KEY_UP},			// 上或者云台向上
	{"Down", NET_DOWN},			// 下或者云台向下
	{"Left", NET_LEFT},			// 左或者云台向左
	{"Right", NET_RIGHT},		// 右或者云台向右
	{"Shift", NET_KEY_SHIFT},
	{"PageUp", NET_KEY_PGUP},   // 上一页
	{"PageDown", NET_KEY_PGDN}, // 下一页
	{"Enter", NET_KEY_RET},     // 确认
	{"Esc", NET_KEY_ESC},       // 取消或退出
	{"Func", NET_KEY_FUNC},     // 切换输入法
	{"Play", NET_KEY_PLAY},     // 播放/暂停
	{"Back", NET_KEY_BACK},     // 倒放
	{"Stop", NET_KEY_STOP},     // 停止
	{"Fast", NET_KEY_FAST},     // 快放
	{"Slow", NET_KEY_SLOW},     // 慢放
	{"Next", NET_KEY_NEXT},     // 下一个文件
	{"Prev", NET_KEY_PREV},     // 上一个文件
	{"Record", NET_KEY_REC},    // 录像设置
	{"Search", NET_KEY_SEARCH}, // 录像查询
	{"Info", NET_KEY_INFO},     // 系统信息
	{"Alarm", NET_KEY_ALARM},   // 告警输出
	{"Address", NET_KEY_ADDR},  // 遥控器地址设置
	{"Backup", NET_KEY_BACKUP}, // 备份
	{"SPLIT", NET_KEY_SPLIT},   // 画面分割模式切换，每按一次切换到下一个风格模式
	{"SPLIT1", NET_KEY_SPLIT1}, // 单画面
	{"SPLIT4", NET_KEY_SPLIT4}, // 四画面
	{"SPLIT8", NET_KEY_SPLIT8}, // 八画面
	{"SPLIT9", NET_KEY_SPLIT9}, // 九画面
	{"SPLIT16", NET_KEY_SPLIT16},// 16画面
	{"SPLIT25", NET_KEY_SPLIT25},// 25画面
	{"SPLIT32", NET_KEY_SPLIT36},// 36画面
	{"ShutDown", NET_KEY_SHUT}, // 关机
	{"Menu", NET_KEY_MENU},     // 菜单
	{"PTZ", NET_KEY_PTZ},       // 进入云台控制模式
	{"ZoomTele", NET_KEY_TELE},     // 变倍减
	{"ZoomWide", NET_KEY_WIDE},     // 变倍加
	{"IrisSmall", NET_KEY_IRIS_SMALL}, // 光圈增
	{"IrisLarge", NET_KEY_IRIS_LARGE}, // 光圈减
	{"FocusNear", NET_KEY_FOCUS_NEAR}, // 聚焦远
	{"FocusFar", NET_KEY_FOCUS_FAR},   // 聚焦近
	{"Brush", NET_KEY_BRUSH},          // 雨刷
	{"Light", NET_KEY_LIGHT},          // 灯光
	{"Preset", NET_KEY_SPRESET},       // 设置预置点
	{"GotoPreset", NET_KEY_GPRESET},   // 转至预置点
	{"DelPrest", NET_KEY_DPRESET},     // 清除预置点 
	{"Pattern", NET_KEY_PATTERN},      // 模式
	{"AutoScan", NET_KEY_SCAN},        // 自动扫描开始/结束
	{"AutoTour", NET_KEY_AUTOTOUR},    // 自动巡航
	{"AutoPan", NET_KEY_AUTOPAN},      // 线扫开始/结束
	{NULL,	}
};

static ConfigPair s_kbStatusMap[] = 
{
	{"KeyDown", NET_KEYBOARD_KEYDOWN},
	{"KeyUp", NET_KEYBOARD_KEYUP},
	{NULL,	}
};

template<> void exchangeTable<NetKeyBoardData>(CConfigTable &table, NetKeyBoardData &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Value", config.iValue, s_kbValueMap);
	exchanger.exchange(table, "Status", config.iState, s_kbStatusMap);
}

template<> void exchangeTable<NetAlarmInfo>(CConfigTable &table, NetAlarmInfo &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Event", config.iEvent);
	exchanger.exchange(table, "State", config.iState);
}

template<> void exchangeTable<NetSnap>(CConfigTable &table, NetSnap &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Channel", config.iChannel);
}

static ConfigPair s_tcommTypeMap[] = 
{
	{"RS232", TRANS_COMM_RS232},
	{"RS485", TRANS_COMM_RS485},
	{NULL,	}
};

static ConfigPair s_tcommOprMap[] = 
{
	{"Start", TRANS_COMM_START},
	{"Stop", TRANS_COMM_STOP},
	{NULL,	}
};

template<> void exchangeTable<TransparentComm>(CConfigTable &table, TransparentComm &config, int state)
{
    CKeyExchange exchanger;

    exchanger.setState(state);
    exchanger.exchange(table, "CommName", config.iTCommType, s_tcommTypeMap);
    exchanger.exchange(table, "Action", config.iTCommOpr, s_tcommOprMap);
}

#if 0
static ConfigPair s_UPLoadDataTypeMap[] = 
{
	{"VehicleInfomation", VEHICLE_INFO},
	{NULL,	}
};
#endif
template<> void exchangeTable<UpLoadData>(CConfigTable &table, UpLoadData &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "UpLoadDataType", config.InfoType);

}

static ConfigPair s_NatStatusTypeMap[] = 
{
	{"DisEnable",NAT_STATUS_DISENABLE},
	{"Probing",NAT_STATUS_PROBING},
	{"Connecting",NAT_STATUS_CONNECTING},
	{"Conneted",NAT_STATUS_CONNECTED},	
};

template<> void exchangeTable<NatStatusInfo>(CConfigTable &table, NatStatusInfo &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchanger.exchange(table, "NatStatus", config.iNatStatus,s_NatStatusTypeMap);
	exchanger.exchange(table, "NaInfoCode", config.NatInfoCode);
}

