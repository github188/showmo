#include "../Include/ExchangeAL/Exchange.h"
#include "../Include/Types/Defs.h"

#ifdef WIN32
	#define stricmp _stricmp
#else
	#define stricmp strcasecmp
#endif

void CKeyExchange::setState(int state)
{
	m_iState = state;
}

int CKeyExchange::getState() const
{
	return m_iState;
}

// 配置非法时取最后一个有效的配置，正确的设计一般为默认值
void CKeyExchange::exchange(CConfigTable& table, CKeys key, int& value, ConfigPair* map)
{
	const char* str;

	switch(m_iState)
	{
	case ES_SAVING:
		while(map->name)
		{
			if(map->value == value)
			{
				getTable(table, key) = map->name;
				break;
			}
			map++;
		}
		if (!map->name)
		{
			map--;
			getTable(table, key) = map->name;
		}
		break;

	case ES_LOADING:
		str = getString(table, key);
		while(map->name)
		{
			if(stricmp(str, map->name) == 0)
			{
				value = map->value;
				break;
			}
			map++;
		}
		if (!map->name)
		{
			map--;
			value = map->value;
		}
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, int& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getInt(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, short& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getInt(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, ushort& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getInt(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, std::string& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getString(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, char* value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		strcpy(value, getString(table, key));
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, bool& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getBool(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, uchar& value)
{
	switch(m_iState)
	{
	case ES_SAVING:
		getTable(table, key) = value;
		break;

	case ES_LOADING:
		value = getInt(table, key);
		break;
	}
}

void CKeyExchange::exchange(CConfigTable& table, CKeys key, uint& value)
{
	char buf[16];
	const char* str;

	switch(m_iState)
	{
	case ES_SAVING:
		sprintf(buf, "0x%08X", value);
		getTable(table, key) = buf;
		break;

	case ES_LOADING:
		str = getString(table, key);
		sscanf(str, "0x%08X", &value);
		break;
	}
}

const char* CKeyExchange::getString(CConfigTable& table, CKeys key)
{
	CConfigTable& v = getTable(table, key);

	if (v.type() != Json::stringValue)
	{
	    //tracepoint();
		//std::string stream;

		//::getString(table, stream);
		//printf("1111111111 stream:%s\n", stream.c_str());
	}
	return v.asCString();
}

int CKeyExchange::getInt(CConfigTable& table, CKeys key)
{
	CConfigTable& v = getTable(table, key);
	return v.asInt();
}

bool CKeyExchange::getBool(CConfigTable& table, CKeys key)
{
	CConfigTable& v = getTable(table, key);
	return v.asBool();
}

CConfigTable& CKeyExchange::getTable(CConfigTable& table, CKeys key)
{
	if (key.m_kind == CKeys::kindIndex)
		return table[key.m_value.index];
	else
		return table[Json::StaticString(key.m_value.name)];
}

template<> void exchangeTable<DefaultRequest>(CConfigTable &table, DefaultRequest &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SessionID", config.uiSessionId);
	exchanger.exchange(table, "Name", config.sName);
}

template<> void exchangeTable<DefaultResponse>(CConfigTable &table, DefaultResponse &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SessionID", config.uiSessionId);
	exchanger.exchange(table, "Name", config.sName);
	exchanger.exchange(table, "Ret", config.iRet);
}


template<> void exchangeTable<std::string>(CConfigTable &table, std::string &str, int state)
{
	if(state == CKeyExchange::ES_SAVING)
		table = str;
	else if(state == CKeyExchange::ES_LOADING)
		str = table.asString();
}

void exchangeSysTime(CKeyExchange& configExchange, CConfigTable& table, SystemTime& systime)
{
	char buf[32];

	switch(configExchange.getState()) {
	case CKeyExchange::ES_SAVING:
		sprintf(buf,
			"%04d-%02d-%02d %02d:%02d:%02d",
			systime.year,
			systime.month,
			systime.day,
			systime.hour,
			systime.minute,
			systime.second);
		table = buf;
		break;

	case CKeyExchange::ES_LOADING:
		sscanf(table.asString().c_str(),
			"%04d-%02d-%02d %02d:%02d:%02d",
			&systime.year,
			&systime.month,
			&systime.day,
			&systime.hour,
			&systime.minute,
			&systime.second);
		break;
	}
}

static ConfigPair* find(ConfigPair *pairs, const char * name)
{
	while(pairs->name)
	{
		if(strcmp(pairs->name, name) == 0)
			return pairs;
		pairs++;
	}
	return 0;
}

static ConfigPair* find(ConfigPair *pairs, int value)
{
	while(pairs->name)
	{
		if(pairs->value == value)
			return pairs;
		pairs++;
	}
	return 0;
}

int getIndex(ConfigPair *pairs, const char * name)
{
	ConfigPair* pPairs = find(pairs, name);

	if (pPairs)
		return pPairs - pairs;
	return -1;
}

int getIndex(ConfigPair *pairs, int value)
{
	ConfigPair* pPairs = find(pairs, value);

	if (pPairs)
		return pPairs - pairs;
	return -1;
}

void getString(const CConfigTable &table, std::string &stream)
{
	CConfigWriter write(stream);
	write.write(table);
}

template<> void exchangeTable<SystemTime>(CConfigTable &table, SystemTime &config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchangeSysTime(exchanger, table, config);
}

