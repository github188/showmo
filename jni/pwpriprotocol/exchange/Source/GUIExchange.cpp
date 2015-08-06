//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "../Include/ExchangeAL/MediaExchange.h"
#include "../Include/ExchangeAL/ManagerExchange.h"
#include "../Include/ExchangeAL/GUIExchange.h"

template<> void exchangeTable<GUISetConfig>(CConfigTable &table, GUISetConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "WindowAlpha", config.iWindowAlpha);
	exchanger.exchange(table, "TimeTitleEnable", config.bTimeTitleEn);
	exchanger.exchange(table, "ChannelTitleEnable", config.bChannelTitleEn);
	exchanger.exchange(table, "AlarmStateEnable", config.bAlarmStatus);
	exchanger.exchange(table, "RecordStateEnable", config.bRecordStatus);
	exchanger.exchange(table, "ChanStateLckEnable", config.bChanStateLckEn);
	exchanger.exchange(table, "ChanStateVlsEnable", config.bChanStateVlsEn);
	exchanger.exchange(table, "ChanStateRecEnable", config.bChanStateRecEn);
	exchanger.exchange(table, "ChanStateMtdEnable", config.bChanStateMtdEn);
	exchanger.exchange(table, "ChanStateBitRateEnable", config.bBitRateEn);
	exchanger.exchange(table, "RemoteEnable", config.bRemoteEnable);
	exchanger.exchange(table, "Deflick", config.bDeflick);
}

template<> void exchangeTable<Guideconfig>(CConfigTable &table, Guideconfig &config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "GuideEnable", config.bEnable);
}

template<> void exchangeTable<VGAresolution>(CConfigTable &table, VGAresolution &config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table["Mode"], "Width", config.nWidth);
	exchanger.exchange(table["Mode"], "Height", config.nHeight);
}

//系统语言列表
static ConfigPair s_language[]= 
{
	{"English", ENGLISH},
	{"SimpChinese", CHINESE_S},
	{"TradChinese", CHINESE_T},
	{"Italian", ITALIAN},
	{"Spanish", SPANISH},
	{"Japanese", JAPANESE},
	{"Russian", RUSSIAN},
	{"French", FRENCH},
	{"German", GERMAN},
	{"Portugal",PORTUGAL},
	{"Turkey",TURKEY},
	{"Poland",POLAND},
	{"Romanian",ROMANIAN},
	{"Hungarian",HUNGARIAN},
	{"Finnish",FINNISH},
	{"Estonian",ESTONIAN},
	{"Korean",KOREAN},
	{"Farsi",FARSI},
	{"Dansk",DANSK},
	{"Thai", THAI},
	{"Greek", GREEK},
	{"Vietnamese", VIETNAMESE},
	{"Ukrainian", UKRAINIAN},
	{"Brazilian", BRAZILIAN},
	{"Hebrew", HEBREW},
	{"Indonesian", INDONESIAN},
	{"Arabic", ARABIC},
	{"Swedish", SWEDISH},
	{"Czech", CZECH},
	{"Bulgarian", BULGARIAN},
	{"Slovakia", SLOVAKIA},
	{"Dutch", DUTCH},
	{NULL,}
};

//获取系统语言接口函数
ConfigPair *getSystemLanguageList()
{
	return  s_language;
};
