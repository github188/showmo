//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//	Modify:			普通配置相关转化,包括General，Location，AutoMaintain三个配置结构
// 

#include "../Include/ExchangeAL/ManagerExchange.h"
#include "../Include/ExchangeAL/Exchange.h"
#include "../Include/Types/Defs.h"
#include "../Include/ExchangeAL/GUIExchange.h"

static ConfigPair s_videoOutput[] = 
{
	{"Auto", VIDEOOUT_AUTO},
	{"VGA", VIDEOOUT_VGA},
	{"TV", VIDEOOUT_TV},
	{NULL,	},
};

static ConfigPair s_diskFullPolicy[] = 
{
	{"StopRecord", DISK_FULL_POLICY_STOP_RECORD},
	{"OverWrite", DISK_FULL_POLICY_OVERWRITE},
	{NULL,	},
};

template<> void exchangeTable<GeneralConfig>(CConfigTable &table, GeneralConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "LocalNo",	config.iLocalNo);
#ifdef WIN32
	static bool compatR01 = false;
	
	// 兼容老的配置
	if (table["OverWrite"].type() == Json::intValue 
		|| table["OverWrite"].type() == Json::booleanValue)
	{
		compatR01 = true;
	}

	if (compatR01)
	{
		exchanger.exchange(table, "OverWrite", config.iOverWrite);
	}
	else
	{
		exchanger.exchange(table, "OverWrite", config.iOverWrite, s_diskFullPolicy);
	}
#else
	if (state == CKeyExchange::ES_LOADING && table["OverWrite"].type() == Json::intValue)
	{
		table["OverWrite"] = s_diskFullPolicy[table["OverWrite"].asInt()].name;
	}
	else
	{
		exchanger.exchange(table, "OverWrite", config.iOverWrite, s_diskFullPolicy);
	}

#endif
	exchanger.exchange(table, "SnapInterval", config.iSnapInterval);
	exchanger.exchange(table,"MachineName", config.sMachineName);
	exchanger.exchange(table,"VideoOutPut", config.iVideoStartOutPut, s_videoOutput);
	exchanger.exchange(table,"AutoLogout", config.iAutoLogout);
}

static ConfigPair s_videoFormat[] = 
{
	{"PAL", VIDEO_STD_PAL},
	{"NTSC", VIDEO_STD_NTSC},
	{"SECAM", VIDEO_STD_SECAM},
	{NULL,	},
};

static ConfigPair s_dateFormat[] = 
{
	{"YYMMDD", CTime::DF_YYMMDD},
	{"MMDDYY", CTime::DF_MMDDYY},
	{"DDMMYY", CTime::DF_DDMMYY},
	{NULL,	},
};

static ConfigPair s_dateSeparator[] = 
{
	{".", CTime::DS_DOT},
	{"-", CTime::DS_DASH},
	{"/", CTime::DS_SLASH},
	{NULL,	},
};

static ConfigPair s_timeFormat[] = 
{
	{"12", CTime::TF_12},
	{"24", CTime::TF_24},
	{NULL,	},
};

static ConfigPair s_dstRule[] = 
{
	{"Off", DST_RULE_OFF},
	{"On", DST_RULE_ON},
	{NULL,	},
};

void exchangeDST(CConfigTable& table, GeneralDSTPoint& dst, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Year",  dst.iYear);
	exchanger.exchange(table, "Month",  dst.iMonth);
	exchanger.exchange(table, "Week",  dst.iWeek);//默认为按周设置
	exchanger.exchange(table, "Day",  dst.iWeekDay);
	exchanger.exchange(table, "Hour",  dst.Hour);
	exchanger.exchange(table, "Minute",  dst.Minute);
}

template<> void exchangeTable<LocationConfig>(CConfigTable &table, LocationConfig &config, int state)
{
	CKeyExchange exchanger;
	ConfigPair *s_language = getSystemLanguageList();
	exchanger.setState(state);
	exchanger.exchange(table, "VideoFormat",	config.iVideoFormat, s_videoFormat);
	exchanger.exchange(table, "Language", config.iLanguage, s_language);
	exchanger.exchange(table, "DateFormat", config.iDateFormat, s_dateFormat);
	exchanger.exchange(table, "DateSeparator", config.iDateSeparator, s_dateSeparator);
	exchanger.exchange(table, "TimeFormat", config.iTimeFormat, s_timeFormat);
	exchanger.exchange(table, "DSTRule",	config.iDSTRule, s_dstRule);
	exchanger.exchange(table, "WorkDay", config.iWorkDay); //工作日默认1111100, 用位来表示
	exchangeDST(table["DSTStart"], config.dDSTStart, state);
	exchangeDST(table["DSTEnd"], config.dDSTEnd, state);
}

static ConfigPair s_autoRebootDay[] =
{
	{"Never", 0},
	{"Everyday", 1},
	{"Sunday", 2},
	{"Monday", 3},
	{"Tuesday", 4},
	{"Wednesday", 5},
	{"Thursday", 6},
	{"Friday", 7},
	{"Saturday", 8},
	{NULL, }
};

extern void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state);

template<> void exchangeTable<AutoMaintainConfig>(CConfigTable &table, AutoMaintainConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "AutoRebootDay", config.iAutoRebootDay, s_autoRebootDay);
	exchanger.exchange(table, "AutoRebootHour", config.iAutoRebootHour);
	exchanger.exchange(table, "AutoDeleteFilesDays", config.iAutoDeleteFilesDays);
}

template<> void exchangeTable<SystemLastState>(CConfigTable &table, SystemLastState &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "LastSpt", config.nLastSpt);
	exchanger.exchange(table, "LastSubSpt", config.nLastSubSpt);
	exchangeTimeSection(table["WorkTime"], config.tsWorkHour, state);
}

template<> void exchangeTable<LedLight>(CConfigTable &table, LedLight &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "light", config.light);
}

