#include "../Include/ExchangeAL/PCSExchange.h"
#include <../Include/Json/reader.h>
#include <../Include/Json/writer.h>
#include "../Include/ExchangeAL/Exchange.h"
#include <assert.h>

template<> void exchangeTable<DeviceCode>(CConfigTable &table, DeviceCode &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "device_code", config.device_code);
	exchanger.exchange(table, "user_code", config.user_code);
	exchanger.exchange(table, "verification_url", config.verification_url);
	exchanger.exchange(table, "qrcode_url", config.qrcode_url);
	exchanger.exchange(table, "expires_in", config.expires_in);
	exchanger.exchange(table, "interval", config.interval);
}

template<> void exchangeTable<AccessTokenCode>(CConfigTable &table, AccessTokenCode &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "access_token", config.access_token);
	exchanger.exchange(table, "expires_in", config.expires_in);
	exchanger.exchange(table, "refresh_token", config.refresh_token);
	exchanger.exchange(table, "scope", config.scope);
	exchanger.exchange(table, "session_key", config.session_key);
	exchanger.exchange(table, "session_secret", config.session_secret);
}

template<> void exchangeTable<QuotaCode>(CConfigTable &table, QuotaCode &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "quota", config.quota);
	exchanger.exchange(table, "used", config.used);
}

template<> void exchangeTable<SliceUploadCode>(CConfigTable &table, SliceUploadCode &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "md5", config.md5);
}

template<> void exchangeTable<BaiduPcsConfig>(CConfigTable &table, BaiduPcsConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "action", config.action);
	exchanger.exchange(table, "bind", config.bind);
	exchanger.exchange(table, "usercode", config.userCode);
	exchanger.exchange(table, "bConfirm", config.bConfirm);
	exchanger.exchange(table, "fileDetect", config.fileDetect);
	exchanger.exchange(table, "fileAlarm", config.fileAlarm);
	exchanger.exchange(table, "fileBlind", config.fileBlind);
	exchanger.exchange(table, "snapDetect", config.snapDetect);
	exchanger.exchange(table, "snapAlarm", config.snapAlarm);
	exchanger.exchange(table, "snapBlind", config.snapBlind);
}






