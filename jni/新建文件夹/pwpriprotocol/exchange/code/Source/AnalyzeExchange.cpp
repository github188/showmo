#include "../ExchangeAL/AnalyzeExchange.h"
#include "../ExchangeAL/Exchange.h"

extern void exchangeEventHandler(CConfigTable& table, EventHandler& hEvent, int state);
extern void exchangeCover(CConfigTable& table, VIDEO_WIDGET& vw, int state);
extern void exchangeSysTime(CKeyExchange& configExchange, CConfigTable& table, SystemTime& systime);

//------------------------  PEA算法规则解析 ---------------------------------

void exchangePoint(CConfigTable& table, URP_IMP_POINT_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "x", config.s16X);
	exchanger.exchange(table, "y", config.s16Y);
}

void exchangeLine(CConfigTable& table, URP_LINE_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchangePoint(table["StartPt"], config.stStartPt, state);
	exchangePoint(table["EndPt"], config.stEndPt, state);
}

void exchangePerimeterArea(CConfigTable& table, URP_PERIMETER_LIMIT_BOUNDARY_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PointNum", config.s32BoundaryPtNum);
	for(int i = 0; i < IMP_MAX_BOUNDARY_POINT_NUM; i++)
	{
		exchangePoint(table["Points"][i], config.astBoundaryPts[i], state);
	}
}

void exchangePerimeterPara(CConfigTable& table, URP_PERIMETER_LIMIT_PARA_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "MinDist", config.s32MinDist);
	exchanger.exchange(table, "MinTime", config.s32MinTime);
	exchanger.exchange(table, "DirectionLimit", config.s32DirectionLimit);
	exchanger.exchange(table, "ForbiddenDirection", config.s32ForbiddenDirection);
	exchangePerimeterArea(table["Boundary"], config.stBoundary, state);
}

void exchangePerimeter(CConfigTable& table, URP_PERIMETER_RULE_PARA_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "TypeLimit", config.s32TypeLimit);
	exchanger.exchange(table, "TypeHuman", config.s32TypeHuman);
	exchanger.exchange(table, "TypeVehicle", config.s32TypeVehicle);
	exchanger.exchange(table, "Mode", config.s32Mode);
	exchangePerimeterPara(table["LimitPara"], config.stLimitPara, state);
}

void exchangeTripWireLimit(CConfigTable& table, URP_TRIPWIRE_LIMIT_PARA_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "MinDist", config.s32MinDist);
	exchanger.exchange(table, "MinTime", config.s32MinTime);
}

void exchangeTripWire(CConfigTable& table, URP_TRIPWIRE_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Valid", config.s32Valid);
	exchanger.exchange(table, "IsDoubleDir", config.s32IsDoubleDirection);
	exchanger.exchange(table, "ForbiddenDir", config.s32ForbiddenDirection);
	exchangeLine(table["Line"], config.stLine, state);
}

void exchangeTripWirePara(CConfigTable& table, URP_TRIPWIRE_RULE_PARA_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "TypeLimit", config.s32TypeLimit);
	exchanger.exchange(table, "TypeHuman", config.s32TypeHuman);
	exchanger.exchange(table, "TypeVehicle", config.s32TypeVehicle);
	exchangeTripWireLimit(table["Limit"], config.stLimitPara, state);
	for(int i = 0; i < IMP_URP_MAX_TRIPWIRE_CNT; i++)
	{
		exchangeTripWire(table["TripWire"][i], config.astLines[i], state);
	}
}

void exchangeRulePEA(CConfigTable& table, PEA_RULE_S& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ShowTrack", config.iShowTrack);
	exchanger.exchange(table, "ShowRule", config.iShowRule);
	exchanger.exchange(table, "Level", config.iLevel);
	exchanger.exchange(table, "PerimeterEnable", config.iPerimeterEnable);
	exchangePerimeter(table["PerimeterRule"], config.stPerimeterRulePara, state);
	exchanger.exchange(table, "TripWireEnable", config.iTripWireEnable);
	exchangeTripWirePara(table["TripWireRule"], config.stTripwireRulePara, state);
}

//------------------------  OSC算法规则解析 ---------------------------------

void exchangeParaOSCLimit(CConfigTable & table, URP_OSC_LMT_PARA_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SizeMin", config.s32SizeMin);
	exchanger.exchange(table, "SizeMax", config.s32SizeMax);
	exchanger.exchange(table, "TimeMin", config.s32TimeMin);
}

void exchangePolygonRegion(CConfigTable & table, URP_POLYGON_REGION_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Valid", config.s32Valid);
	exchanger.exchange(table, "PointNu", config.s32PointNum);
	for(int i = 0; i < IMP_MAX_BOUNDARY_POINT_NUM; i++)
	{
		exchangePoint(table["Points"][i], config.astPoint[i], state);
	}
}

void exchangeSpeclRegions(CConfigTable & table, URP_OSC_SPECL_REGIONS_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Valid", config.s32Valid);
	exchanger.exchange(table, "Name", config.as8Name);
	exchangePolygonRegion(table["OscRg"], config.stOscRg, state);
	exchangePolygonRegion(table["SubRgA"], config.astSubRgA, state);
	exchangePolygonRegion(table["SubRgB"], config.astSubRgB, state);
	exchangePolygonRegion(table["SubRgC"], config.astSubRgC,state);
}

void exchangeParaOSCRule(CConfigTable & table, URP_OSC_RULE_PARA_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SceneType", config.s32SceneType);
	exchanger.exchange(table, "CameraType", config.s32CameraType);
	exchangeParaOSCLimit(table["OscPara"], config.stOscPara, state);
	for(int i = 0; i < IMP_MAX_OSC_NUM; i++)
	{
		exchangeSpeclRegions(table["SpclRgs"][i], config.astSpclRgs[i], state);
	}
}

void exchangeRuleOSC(CConfigTable & table, OSC_RULE_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ShowTrack", config.iShowTrack);
	exchanger.exchange(table, "ShowRule", config.iShowRule);
	exchanger.exchange(table, "Level", config.iLevel);
	exchanger.exchange(table, "AbandumEnable", config.iAbandumEnable);
	exchangeParaOSCRule(table["AbandumRule"], config.stObjAbandumRulePara, state);
	exchanger.exchange(table, "StolenEnable", config.iStolenEnable);
	exchangeParaOSCRule(table["StolenRule"], config.stObjStolenRulePara, state);
	exchanger.exchange(table, "NoParkingEnable", config.iNoParkingEnable);
	exchangeParaOSCRule(table["NoParkingRule"], config.stNoParkingRulePara, state);
}

//------------------------  AVD算法规则解析 ---------------------------------

void exchangeRuleAVD(CConfigTable & table, AVD_RULE_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Level", config.iLevel);
	exchanger.exchange(table, "tBrightAbnmlEnable", config.itBrightAbnmlEnable);
	exchanger.exchange(table, "ClarityEnable", config.iClarityEnable);
	exchanger.exchange(table, "NoiseEnable", config.iNoiseEnable);
	exchanger.exchange(table, "ColorEnable", config.iColorEnable);
	exchanger.exchange(table, "FreezeEnable", config.iFreezeEnable);
	exchanger.exchange(table, "NosignalEnable", config.iNosignalEnable);
	exchanger.exchange(table, "ChangeEnable", config.iChangeEnable);
	exchanger.exchange(table, "InterfereEnable", config.iInterfereEnable);
	exchanger.exchange(table, "PtzLoseCtlEnable", config.iPtzLoseCtlEnable);
}

//------------------------  CPC算法规则解析 ---------------------------------

void exchangeRuleCPC(CConfigTable & table, CPC_RULE_S & config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Sizemin", config.s32Sizemin);
	exchanger.exchange(table, "Sizemax", config.s32Sizemax);
	exchanger.exchange(table, "Countmax", config.s32Countmax);
	exchanger.exchange(table, "Sensitivity", config.s32Sensitivity);
	exchanger.exchange(table, "Flag", config.u32Flag);
	exchanger.exchange(table, "EnterDirection", config.s32EnterDirection);
	for(int i = 0; i < IMP_MAX_POINT_NUM; i++)
	{
		exchangePoint(table["Points"][i], config.stRulePoint[i], state);
	}
}

void exchangeAnalyzeRule(CConfigTable& table, RuleConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchangeRulePEA(table["PEARule"], config.stRulePea, state);
	exchangeRuleOSC(table["OSCRule"], config.stRuleOSC, state);
	exchangeRuleAVD(table["AVDRule"], config.stRuleAVD, state);
	exchangeRuleCPC(table["CPCRule"], config.stRuleCPC, state);
}

template<> void exchangeTable<AnalyzeConfig>(CConfigTable &table, AnalyzeConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "ModuleType", config.moduleType);
	exchangeEventHandler(table["EventHandler"], config.hEvent, state);
	exchangeAnalyzeRule(table["RuleConfig"], config.stRuleConfig, state);
}

template<> void exchangeTable<AnalyzeConfigAll>(CConfigTable &table, AnalyzeConfigAll &configAll, int state)
{
	for(int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], configAll.vAnalyzeAll[i], state);
	}
}

template<> void exchangeTableV2<AnalyzeConfigAll>(CConfigTable &table, AnalyzeConfigAll &configAll, int state, int nSize)
{
	for(int i = 0; i < nSize; i++)
	{
		/*const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}*/
		exchangeTable(table[i], configAll.vAnalyzeAll[i], state);
	}
}

//-------------------------------------------------------------------------------------------

template<> void exchangeTable<AnalyzeAbility>(CConfigTable &table, AnalyzeAbility &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "IntelAVD", config.uIntelAVD);
	exchanger.exchange(table, "AlgorithmAVD", config.uAlgorithmAVD);
	exchanger.exchange(table, "IntelCPC", config.uIntelCPC);
	exchanger.exchange(table, "AlgorithmCPC", config.uAlgorithmCPC);
	exchanger.exchange(table, "IntelOSC", config.uIntelOSC);
	exchanger.exchange(table, "AlgorithmOSC", config.uAlgorithmOSC);
	exchanger.exchange(table, "IntelPEA", config.uIntelPEA);
	exchanger.exchange(table, "AlgorithmPEA", config.uAlgorithmPEA);
}

//-------------------------------------------------------------------------------------------

void exchangeCPCLink(CConfigTable& table, CPCLink& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	
	exchanger.exchange(table, "NoteTime", config.NoteTime);
	exchangeCover(table["CPCInfo"], config.CPCInfo, state);
}

template<> void exchangeTable<AnalyzeLink>(CConfigTable& table, AnalyzeLink& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);

	exchangeCPCLink(table["CPCLink"], config.CPCLinkConfig, state);
}

template<> void exchangeTable<AnalyzeLinkAll>(CConfigTable& table, AnalyzeLinkAll& config, int state)
{
	for(int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vAnalyzeLinkAll[i], state);
	}
}

template<> void exchangeTableV2<AnalyzeLinkAll>(CConfigTable& table, AnalyzeLinkAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vAnalyzeLinkAll[i], state);
	}
}

void exchangeCPCData(CConfigTable& table, CPCDataItem& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "channel", config.channel);
	exchanger.exchange(table, "inNum", config.inNum);
	exchanger.exchange(table, "outNum", config.outNum);
	exchangeSysTime(exchanger, table["startTime"], config.startTime);
	exchangeSysTime(exchanger, table["endTime"], config.endTime);
}

template<> void exchangeTable<CPCDataAll>(CConfigTable& table, CPCDataAll& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	
	exchanger.exchange(table, "ItemNum", config.nItemNum);
	for(int i = 0; i < config.nItemNum; i++)
	{
		exchangeCPCData(table["CPCData"][i], config.CPCData[i], state);
	}
}

