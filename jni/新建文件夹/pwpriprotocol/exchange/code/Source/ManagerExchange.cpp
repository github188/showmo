//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//					用户管理相关配置转换
//

#include "../Include/ExchangeAL/ManagerExchange.h"
#include <../Include/Json/reader.h>
#include <../Include/Json/writer.h>
#include "../Include/ExchangeAL/Exchange.h"
#include <assert.h>

// 登陆方式，和<<JsonConfigFormat保持一致>>
static ConfigPair s_loginTypeMap[] =
{
	{"GUI", LOGIN_TYPE_GUI},
	{"Console", LOGIN_TYPE_CONSOLE},
	{"IPNC-Web", LOGIN_TYPE_WEB},
	{"IPNC-CMS_B", LOGIN_TYPE_SNS},		// IPNC-SNS
	{"IPNC-Mobile", LOGIN_TYPE_MOBIL},
	{"IPNC-NKB", LOGIN_TYPE_NETKEYBOARD},
	{"IPNC-Server", LOGIN_TYPE_SERVER},
	{"IPNC-AutoSearch", LOGIN_TYPE_AUTOSEARCH},
	{"IPNC-Upgrade", LOGIN_TYPE_UPGRADE},
	{"CMS_A", LOGIN_TYPE_MEGAEYE},		// 大华CMS客户端: 全球眼
	{NULL,}
};

static ConfigPair s_passwordEncryptFlag[] =
{
	{"NONE", PASSWORD_FLAG_PLAIN},
	{"MD5", PASSWORD_FLAG_MD5},
	{"3DES", PASSWORD_FLAG_3DES},
	{"HSQD", PASSWORD_FLAG_HSQD},
	{NULL},
};

template<> void exchangeTable<LoginRequest>(CConfigTable &table, LoginRequest &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "UserName", config.sUserName);
	exchanger.exchange(table, "PassWord", config.sPassword);
	exchanger.exchange(table, "LoginType", config.iLoginType, s_loginTypeMap);
	exchanger.exchange(table, "EncryptType", config.iEncryptType, s_passwordEncryptFlag);
}

template<> void exchangeTable<AutoLogin>(CConfigTable &table, AutoLogin &config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "AutoLoginEnable", config.iEnable);
	exchangeTable<LoginRequest>(table, config.LoginInfo, state);
}

static ConfigPair s_deviceTypeMap[] =
{
	{"DVR", DEVICE_TYPE_DVR},
	{"NVS", DEVICE_TYPE_NVS},
	{"IPC", DEVICE_TYPE_IPC},
	{"HVR", DEVICE_TYPE_HVR},
	{"MVR", DEVICE_TYPE_MVR},
	{"IVR", DEVICE_TYPE_IVR},
	{NULL},
};

template<> void exchangeTable<LoginResponse>(CConfigTable &table, LoginResponse &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Ret", config.iRet);
	exchanger.exchange(table, "SessionID", config.uiSessionId);
	exchanger.exchange(table, "DeviceType ", config.iDeviceType, s_deviceTypeMap);
	exchanger.exchange(table, "ChannelNum", config.iChannelNum);
	exchanger.exchange(table, "AliveInterval", config.iAliveInterval);
	exchanger.exchange(table, "ExtraChannel", config.iExtraChannel);
}

template<> void exchangeTable<ModifyPasswordRequest>(CConfigTable &table, ModifyPasswordRequest &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SessionID", config.uiSessionId);
	exchanger.exchange(table, "UserName", config.sUserName);
	exchanger.exchange(table, "PassWord", config.sPassword);
	exchanger.exchange(table, "NewPassWord", config.sNewPassword);
	exchanger.exchange(table, "EncryptType", config.iEncryptType, s_passwordEncryptFlag);
}

/// 权限列表解析, 比较特殊，直接交换了
template<> void exchangeTable<std::vector<std::string> >(CConfigTable &table, std::vector<std::string> &config, int state)
{
	if (state == CKeyExchange::ES_LOADING)
	{
		for (uint i = 0; i < table.size(); i++)
		{
			config.push_back(table[i].asString());
		}
	}
	else if (state == CKeyExchange::ES_SAVING)
	{
		for (uint i = 0; i < config.size(); i++)
		{
			table[i] = config[i];
		}
	}
}

GroupConfig &GroupConfig::operator=(const GroupConfig &group)
{
	memo = group.memo;
	name = group.name;
	authorityList.clear();
	for (uint i = 0; i < group.authorityList.size(); i++)
	{
		authorityList.push_back(group.authorityList[i]);
	}
	return *this;
}

UserConfig &UserConfig::operator=(const UserConfig &user)
{
	authorityList.clear();
	for (uint i = 0; i < user.authorityList.size(); i++)
	{
		authorityList.push_back(user.authorityList[i]);
	}
	group = user.group;
	memo = user.memo;
	userName = user.userName;
	password = user.password;
	reserved = user.reserved;
	shareable = user.shareable;
	return *this;
}

template<> void exchangeTable<UserConfig>(CConfigTable &table, UserConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	::exchangeTable(table["AuthorityList"], config.authorityList, state);
	exchanger.exchange(table, "Name", config.userName);
	exchanger.exchange(table, "Password", config.password);
	exchanger.exchange(table, "Memo", config.memo);
	exchanger.exchange(table, "Group", config.group);
	exchanger.exchange(table, "Reserved", config.reserved);
	exchanger.exchange(table, "Sharable", config.shareable);
}

template<> void exchangeTable<std::vector<UserConfig> >(CConfigTable &table, std::vector<UserConfig> &config, int state)
{
	if (state == CKeyExchange::ES_LOADING)
	{
		for (uint i = 0; i < table.size(); i++)
		{
			UserConfig User;

			::exchangeTable(table[i], User, CKeyExchange::ES_LOADING);
			config.push_back(User);
		}
	}
	else if (state == CKeyExchange::ES_SAVING)
	{
		for (uint i = 0; i < config.size(); i++)
		{
			::exchangeTable(table[i], config[i], CKeyExchange::ES_SAVING);
		}
	}
}

template<> void exchangeTable<GroupConfig>(CConfigTable &table, GroupConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	::exchangeTable(table["AuthorityList"], config.authorityList, state);
	exchanger.exchange(table, "Name", config.name);
	exchanger.exchange(table, "Memo", config.memo);
}

template<> void exchangeTable<std::vector<GroupConfig> >(CConfigTable &table, std::vector<GroupConfig> &config, int state)
{
	if (state == CKeyExchange::ES_LOADING)
	{
		for (uint i = 0; i < table.size(); i++)
		{
			GroupConfig Group;

			::exchangeTable(table[i], Group, CKeyExchange::ES_LOADING);
			config.push_back(Group);
		}
	}
	else if (state == CKeyExchange::ES_SAVING)
	{
		for (uint i = 0; i < config.size(); i++)
		{
			::exchangeTable(table[i], config[i], CKeyExchange::ES_SAVING);
		}
	}
}

template<> void exchangeTable<ModifyUserConfig>(CConfigTable &table, ModifyUserConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "UserName", config.sUserName);
	::exchangeTable(table["User"], config.User, state);
}

template<> void exchangeTable<ModifyGroupConfig>(CConfigTable &table, ModifyGroupConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "GroupName", config.sGroupName);
	::exchangeTable(table["Group"], config.Group, state);
}

const char *getUserLoginType(int iLoginType)
{
	if (iLoginType < LOGIN_TYPE_NR)
	{
		int index = getIndex(s_loginTypeMap, iLoginType);

		assert(index >= 0);
		return s_loginTypeMap[iLoginType].name;
	}
	return "";
}

const char *getUserEncType(int iEncType)
{
	if (iEncType < PASSWORD_FLAG_NR)
	{
		//int index = getIndex(s_passwordEncryptFlag, iEncType);

		return s_passwordEncryptFlag[iEncType].name;
	}
	return "";
}

template<> void exchangeTable<DeleteAccountRequest>(CConfigTable &table, DeleteAccountRequest &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SessionID", config.uiSessionID);
	exchanger.exchange(table, "Name", config.sName);
}

template<> void exchangeTable<CarPlates>(CConfigTable& table, CarPlates& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PlateName", config.sPlateName);
}

template<> void exchangeTable<DeviceDesc>(CConfigTable& table, DeviceDesc& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "AudioInChannel", config.nAudioIn);
	exchanger.exchange(table, "AlarmInChannel", config.nAlarmIn);
	exchanger.exchange(table, "AlarmOutChannel", config.nAlarmOut);
	exchanger.exchange(table, "TrailDay", config.nTrailDay);
	exchanger.exchange(table, "GUITheme", config.sGUITheme);
	exchanger.exchange(table, "AutoBoot", config.bAutoBoot);
	exchanger.exchange(table, "RemoteType", config.nRemoteType);
	exchanger.exchange(table, "PenaltyPaDay", config.nPenaltyDay);
	exchanger.exchange(table, "localPalyMax", config.nlocalPalyMax);
	exchanger.exchange(table, "localPalyDefault", config.nlocalPalyDefault);
	exchanger.exchange(table, "CaptureInChannel", config.nCapture);
	exchanger.exchange(table, "PadType", config.nPadType);
	exchanger.exchange(table, "EnablePlayDefault", config.bEablePlayDefault);
	exchanger.exchange(table, "EnableComm", config.bEnableComm);
	exchanger.exchange(table, "EnablePtz", config.bEnablePtz);
}

template<> void exchangeTable<AbilitySerialNo>(CConfigTable& table, AbilitySerialNo& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SerialNo", config.serialNo);
	exchanger.exchange(table, "ProductType", config.productType);
}

template<> void exchangeTable<ProduceTest>(CConfigTable& table, ProduceTest& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Test", config.TestEnable);
}

template<> void exchangeTable <DASLoginInfo>(CConfigTable &table, DASLoginInfo &config, int state)
{
    CKeyExchange exchanger;

    exchanger.setState(state);
    exchanger.exchange(table, "UserName" , config.userName);
    exchanger.exchange(table, "PassWord" , config.password);
    exchanger.exchange(table, "EncryptType", config.encryptType, s_passwordEncryptFlag);
    exchanger.exchange(table, "DeviceType", config.devicType, s_deviceTypeMap);
    exchanger.exchange(table, "DeviceID", config.deviceID);
    exchanger.exchange(table, "ChannelNum", config.channelNums);
}

