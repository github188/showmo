#include "../Include/ExchangeAL/CameraExchange.h"
#include "../Include/ExchangeAL/Exchange.h"

//曝光配置解析
void exchangeExposure(CConfigTable& table, ExposureParam& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Level", config.level);
	exchanger.exchange(table, "LeastTime", config.leastTime);
	exchanger.exchange(table, "MostTime", config.mostTime);
}

//增益配置解析
void exchangeGain(CConfigTable& table, GainParam& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Gain", config.gain);
	exchanger.exchange(table, "AutoGain", config.autoGain);
}

extern void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state);

// 曝光属性解析		zzb 2013.11.5 add
void exchangeExposureAttr(CConfigTable& table, CamAEAttr_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "ExpType", config.expType);
	exchanger.exchange(table, "GainCtrl", config.gainCtrl);
	exchanger.exchange(table, "Shutter", config.shutter);
	exchanger.exchange(table, "AeMode", config.aeMode);
}

// IRCut属性解析
void exchangeIRCutAttr(CConfigTable& table, IRCutAttr_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Mode", config.mode);
	exchanger.exchange(table, "DncThr", config.dncThr);
	exchanger.exchange(table, "NdcThr", config.ndcThr);

	for(uint i = 0; i < MAX_IRCUT_MODE; i++)
	{
		exchanger.exchange(table["DncThrArray"], i, config.dncThrArray[i]);
		exchanger.exchange(table["NdcThrArray"], i, config.ndcThrArray[i]);
	}
}


void exchangeIRCutCtrlAttr(CConfigTable& table, IRCUT_CTRL_ATTR_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "DncThr", config.dncThr);
	exchanger.exchange(table, "NdcThr", config.ndcThr);
	exchanger.exchange(table, "IsKeepColor", config.IsKeepColor);
	exchanger.exchange(table, "Sensitivity", config.Sensitivity);
	exchanger.exchange(table, "ForceCut", config.ForceCut);
	exchanger.exchange(table["NSTime"], "hour", config.NightStartTime.hour);
	exchanger.exchange(table["NSTime"], "minute", config.NightStartTime.minute);
	exchanger.exchange(table["NSTime"], "second", config.NightStartTime.second);
	exchanger.exchange(table["NETime"], "hour", config.NightEndTime.hour);
	exchanger.exchange(table["NETime"], "minute", config.NightEndTime.minute);
	exchanger.exchange(table["NETime"], "second", config.NightEndTime.second);
}

void exchangeIRCutAttr(CConfigTable& table, IRCUT_CTRL_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "IRCutCtrlMode", config.IRCutCtrlMode);

	for(uint i = 0; i < IRCUT_CTRL_M_NR; i++)
	{
		exchangeIRCutCtrlAttr(table["IRCutCtrlAttr"][i], config.IRCutCtrAttr[i], state);
	}
}


// 视频属性解析
void exchangeVideoAttr(CConfigTable& table, VIDEO_COLOR& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Brightness", config.Brightness);
	exchanger.exchange(table, "Contrast", config.Contrast);
	exchanger.exchange(table, "Saturation", config.Saturation);
	exchanger.exchange(table, "Hue", config.Hue);
	exchanger.exchange(table, "Gain", config.Gain);
	exchanger.exchange(table, "WhiteBalance", config.WhiteBalance);
	exchanger.exchange(table, "Acutance", config.Acutance);
}

// 宽动态属性解析
void exchangeWdrAttr(CConfigTable& table, WdrAttr_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Strength", config.strength);
	exchangeTimeSection(table["TimeSection"], config.tSection, state);
}

// Gamma属性解析
void exchangeGammaAttr(CConfigTable& table, GammaAttr_t& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.enable);
	exchanger.exchange(table, "Mode", config.mode);
}

template<> void exchangeTable<VIDEO_COLOR>(CConfigTable &table, VIDEO_COLOR &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchanger.exchange(table, "Brightness", config.Brightness);
	exchanger.exchange(table, "Contrast", config.Contrast);
	exchanger.exchange(table, "Saturation", config.Saturation);
	exchanger.exchange(table, "Hue", config.Hue);
	exchanger.exchange(table, "Gain", config.Gain);
	exchanger.exchange(table, "WhiteBalance", config.WhiteBalance);
	exchanger.exchange(table, "Acutance", config.Acutance);
}

//网络摄像头参数解析
template<> void exchangeTable<CameraParam>(CConfigTable &table, CameraParam &config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	
	exchanger.exchange(table, "WhiteBalance", config.whiteBalance);
	exchanger.exchange(table, "DayNightColor", config.dayNightColor);
	exchanger.exchange(table, "ElecLevel", config.elecLevel);
	exchanger.exchange(table, "ApertureMode", config.apertureMode);
	exchanger.exchange(table, "BLCMode", config.BLCMode);
	exchanger.exchange(table, "PictureMirror", config.PictureMirror);
	exchanger.exchange(table, "PictureFlip", config.PictureFlip);
	exchanger.exchange(table, "RejectFlicker", config.RejectFlicker);
	exchanger.exchange(table, "EsShutter", config.EsShutter);
	exchanger.exchange(table, "IRCUTMode", config.ircut_mode);
	exchanger.exchange(table, "DncThr", config.dnc_thr);
	exchanger.exchange(table, "AeSensitivity", config.ae_sensitivity);
	exchanger.exchange(table, "Day_nfLevel", config.Day_nfLevel);
	exchanger.exchange(table, "Night_nfLevel", config.Night_nfLevel);
	exchanger.exchange(table, "IrcutSwap", config.Ircut_swap);

	exchangeExposure(table["ExposureParam"], config.exposureParam, state);
	exchangeGain(table["GainParam"], config.gainParam, state);

	// zzb 2013.11.5 add
	exchanger.exchange(table, "SceneMode", config.sceneMode);
	exchanger.exchange(table, "RotateAttr", config.rotateAttr);
	exchanger.exchange(table, "TimeDomain3d", config.timeDomain3d);
	exchanger.exchange(table, "NoiseDropSth2d", config.noiseDropSth2d);
	exchanger.exchange(table, "AntiFogStrength", config.antiFogSth);
	exchanger.exchange(table, "AntiPseudoColorStrength", config.antiPseudoColorSth);

	exchangeExposureAttr(table["ExposureAttr"], config.exposureAttr, state);
	exchangeIRCutAttr(table["IRCutAttr"], config.ircutAttr, state);
	exchangeVideoAttr(table["VideoAttr"], config.videoAttr, state);
	exchangeWdrAttr(table["WdrAttr"], config.wdrAttr, state);
	exchangeGammaAttr(table["GammaAttr"], config.gammaAttr, state);
}

template<> void exchangeTable<CameraDM8127>(CConfigTable &table, CameraDM8127 &config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	
	exchanger.exchange(table, "BlcMode", config.blcMode);
	exchanger.exchange(table, "DreMode", config.dreMode);
	exchanger.exchange(table, "DreStrength", config.dreStrength);
	exchanger.exchange(table, "AwbMode", config.awbMode);
	exchanger.exchange(table, "AeMode", config.aeMode);
	exchanger.exchange(table, "AewPriority", config.aewPriority);
	exchanger.exchange(table, "BinNingMode", config.binNingMode);
	exchanger.exchange(table, "RotateAttr", config.rotateAttr);
	exchangeTable<VIDEO_COLOR>(table["VideoColor"], config.videoAttr, state);
	exchangeIRCutAttr(table["IRCutAttr"], config.ircutAttr, state);
}

//新的DM8127参数解析
template<> void exchangeTable<CameraDM8127V2>(CConfigTable &table, CameraDM8127V2 &config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	
	exchanger.exchange(table, "BlcMode", config.blc);
	exchanger.exchange(table, "DreMode", config.dreMode);
	exchanger.exchange(table, "DreStrength", config.dreStrength);
	exchanger.exchange(table, "AwbMode", config.awbMode);
	exchanger.exchange(table, "AeMode", config.AeMode);
	exchanger.exchange(table, "AewPriority", config.aewPriority);
	exchanger.exchange(table, "BinNingMode", config.binningMode);
	exchanger.exchange(table, "Env50_60hz", config.env50_60hz);
	exchanger.exchange(table, "MirrorMode", config.mirrorMode);
	exchangeTable<VIDEO_COLOR>(table["VideoColor"], config.videoAttr, state);
	exchangeIRCutAttr(table["IRCutAttr"], config.IRCutCtrl, state);
}


// HI3518平台所有摄像头参数解析
template<> void exchangeTable<CameraParamAll>(CConfigTable &table, CameraParamAll &configAll, int state)
{
	for(int i = 0; i < N_SYS_CH; i++)
	{
		if(table[i] == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], configAll.vCameraParamAll[i], state);
	}
}

// DM8127平台所有摄像头参数解析
template<> void exchangeTable<CameraAllDM8127>(CConfigTable &table, CameraAllDM8127 &configAll, int state)
{
	for(int i = 0; i < N_SYS_CH; i++)
	{
		if(table[i] == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], configAll.vCameraParamAll[i], state);
	}
}

// HI3518平台摄像头参数解析V2
template<> void exchangeTableV2<CameraParamAll>(CConfigTable &table, CameraParamAll &configAll, int state, int nSize)
{
	for(int i = 0; i < nSize; i++)
	{
		if(table[i] == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], configAll.vCameraParamAll[i], state);
	}
}

// DM8127平台摄像头参数解析V2
template<> void exchangeTableV2<CameraAllDM8127>(CConfigTable &table, CameraAllDM8127 &configAll, int state, int nSize)
{
	for(int i = 0; i < nSize; i++)
	{
		if(table[i] == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], configAll.vCameraParamAll[i], state);
	}
}

// 摄像头能力集解析
template<> void exchangeTable<CameraAbility>(CConfigTable &table, CameraAbility &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	//曝光速度
	exchanger.exchange(table, "Count", config.count);
	for(int i = 0; i < config.count; i++)
	{
		exchanger.exchange(table["Speeds"], i, config.speeds[i]);
	}

	//工作状态: >= 0 正常, < 0 异常
	exchanger.exchange(table, "Status", config.status);

	//参考电平值
	exchanger.exchange(table, "ElecLevel", config.elecLevel);

	//平均亮度
	exchanger.exchange(table, "Luminance", config.luminance);

	//2a版本
	exchanger.exchange(table, "Version", config.pVersion);
}
