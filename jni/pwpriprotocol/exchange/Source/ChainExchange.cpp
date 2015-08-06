#include "../Include/ExchangeAL/Exchange.h"
#include "../Include/ExchangeAL/ChainExchange.h"

static char sNull[] = "NULL";

static ConfigPair s_operationKeyMaps[] = 
{
	{"SetPtzInfo", OP_SET_PTZINFO},
	{"AddRefobj", OP_ADD_REFOBJ},
	{"ClearRefobj", OP_CLEAR_REFOBJ},
	{"TransPic2PtzAngle", OP_TRANS_PIC2PTZ},
	{"", OP_CHAIN_NR},
};

const char* getChainOpName(int iOpIndex)
{
	if(iOpIndex >= 0 && iOpIndex < OP_CHAIN_NR)
	{
		int index = getIndex(s_operationKeyMaps, iOpIndex);
		if(index >= 0 && index < OP_CHAIN_NR)
			return s_operationKeyMaps[index].name;
	}
	return sNull;
}

int getChainOpIndex(const char *sOpName)
{
	if(sOpName)
	{
		int index = getIndex(s_operationKeyMaps, sOpName);
		if(index >= 0 && index < OP_CHAIN_NR)
			return s_operationKeyMaps[index].value;
	}
	return -1;
}

template<> void exchangeTable<CHAIN_PTZATTR>(CConfigTable& table, CHAIN_PTZATTR &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "NegAngle", config.NegAngle);
	exchanger.exchange(table, "FixHeight", config.FixHeight);
	exchanger.exchange(table, "PitchAngleDir", config.PitchAngleDir);
}

void exchangePictureRect(CConfigTable& table, CHAIN_PICRECT &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PicX", config.PicX);
	exchanger.exchange(table, "PicY", config.PicY);
	exchanger.exchange(table, "PicW", config.PicW);
	exchanger.exchange(table, "PicH", config.PicH);
}

template<> void exchangeTable<CHAIN_CAPS>(CConfigTable& table, CHAIN_CAPS &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PtzNum", config.PtzNum);
}

template<> void exchangeTable<CHAIN_PTZANG>(CConfigTable& table, CHAIN_PTZANG &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PitchAngle", config.PitchAng);
	exchanger.exchange(table, "HoriAngle", config.HoriAng);
}

template<> void exchangeTable<CHAIN_REFOBJ>(CConfigTable& table, CHAIN_REFOBJ &config, int state)
{
	if(state == CKeyExchange::ES_LOADING)
	{
		if(table != Json::Value::null)
		{
			exchangePictureRect(table["PicRect"], config.PicRect, state);
			exchangeTable<CHAIN_PTZANG>(table["PtzAngle"], config.PtzAng, state);
		}
		else
			memset(&config, 0, sizeof(config));
	}
	else
	{
		CHAIN_REFOBJ tmp = {{0}};
		if(memcmp(&config, &tmp, sizeof(config)) != 0)
		{
			exchangePictureRect(table["PicRect"], config.PicRect, state);
			exchangeTable<CHAIN_PTZANG>(table["PtzAngle"], config.PtzAng, state);
		}
	}
}

template<> void exchangeTable<PtzInfoReq>(CConfigTable &table, PtzInfoReq &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Ptz", config.ptz);
	exchangeTable<CHAIN_PTZATTR>(table["ChainPtzAttr"], config.chainPtzAttr, state);
}

template<> void exchangeTable<ChainParam>(CConfigTable &table, ChainParam &config, int state)
{
	exchangeTable<CHAIN_PTZATTR>(table["ChainPtzAttr"], config.chainPtzAttr, state);
	if(state == CKeyExchange::ES_LOADING)
	{
		for(uint i = 0; i < table["ChainRefobj"].size(); i++)
			exchangeTable<CHAIN_REFOBJ>(table["ChainRefobj"][i], config.chainRefobj[i], state);
	}
	else
	{
		CHAIN_REFOBJ tmp = {{0}};
		for(uint i = 0; i < 32; i++)
		{
			if(memcmp(&config.chainRefobj[i], &tmp, sizeof(CHAIN_REFOBJ)) != 0)
				exchangeTable<CHAIN_REFOBJ>(table["ChainRefobj"][i], config.chainRefobj[i], state);
			else
				break;
		}
	}	
}

template<> void exchangeTable<AddRefobjReq>(CConfigTable &table, AddRefobjReq &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Ptz", config.ptz);
	exchangeTable<CHAIN_REFOBJ>(table["ChainRefobj"], config.chainRefobj, state);
}

template<> void exchangeTable<TransPic2PtzAngleReq>(CConfigTable &table, TransPic2PtzAngleReq &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Ptz", config.ptz);
	exchanger.exchange(table, "AutoZoom", config.autoZoom);
	exchanger.exchange(table, "ViewScale", config.viewScale);
	exchangePictureRect(table["PicRect"], config.picRect, state);
}

template<> void exchangeTable<TransPic2PtzAngleRsp>(CConfigTable &table, TransPic2PtzAngleRsp &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PtzZoom", config.ptzZoom);
	exchangeTable<CHAIN_PTZANG>(table["ChainPtzAngle"], config.chainPtzAngle, state);
}

