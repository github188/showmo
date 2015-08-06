#ifndef __EXCHANGE_AL_EXCHANGE_H__
#define __EXCHANGE_AL_EXCHANGE_H__

#include "../Types/Defs.h"
#include "../Infra/Time.h"
#include "../Json/reader.h"
#include "../Json/writer.h"
#include <stdio.h>

class CKeys
{
	friend class CKeyExchange;
public:
	CKeys(int index)
	{
		m_kind = kindIndex;
		m_value.index = index;
	};

	CKeys(const char* name)
	{
		m_kind = kindName;
		m_value.name = name;
	};

private:
	enum Kind
	{
		kindNone = 0,
		kindIndex,
		kindName
	}m_kind;

	union
	{
		int index;
		const char *name;
	}m_value;
};

struct ConfigPair
{
	const char* name;
	int value;
};

typedef Json::Value CConfigTable;
typedef Json::Reader CConfigReader;
#ifdef _DEBUG
	typedef Json::StyledWriter CConfigWriter;
#else
	typedef Json::FastWriter CConfigWriter;
#endif

/// \class CKeyExchange
/// \brief Json键值交换
class CKeyExchange
{
public:
	/// 配置交换状态，换入或者换出
	enum EXCHANGE_STATE
	{
		ES_SAVING,
		ES_LOADING,
	};

	void setState(int state);

	int getState() const;

	void exchange(CConfigTable& table, CKeys key, int& value, ConfigPair* map);

	void exchange(CConfigTable& table, CKeys key, int& value);

	void exchange(CConfigTable& table, CKeys key, std::string& value);
	
	void exchange(CConfigTable& table, CKeys key, char* value);

	void exchange(CConfigTable& table, CKeys key, bool& value);
	
	void exchange(CConfigTable& table, CKeys key, uchar& value);	
	void exchange(CConfigTable& table, CKeys key, ushort& value);

	void exchange(CConfigTable& table, CKeys key, uint& value);

	const char* getString(CConfigTable& table, CKeys key);

	int getInt(CConfigTable& table, CKeys key);

	bool getBool(CConfigTable& table, CKeys key);

	CConfigTable& getTable(CConfigTable& table, CKeys key);

	void exchange(CConfigTable& table, CKeys key, short& value);

private:
	int m_iState;
};

/// 默认请求包格式
struct DefaultRequest
{
	uint uiSessionId;
	std::string sName;		///< 配置名称
};

/// 默认响应包格式
struct DefaultResponse
{
	uint uiSessionId;		///< 会话ID
	int iRet;				///< 返回值
	std::string sName;		///< 配置名称
};

template <class T> void exchangeTable(CConfigTable& table, T& config, int state);
template <class T> void exchangeTableV2(CConfigTable& table, T& config, int state, int nSize);


//////////////////////////////////////////////////////////////////////////
/// T 配置结构类型
template <class T> class TExchangeAL
{
public:
	TExchangeAL()
	{
	}
	virtual ~TExchangeAL()
	{
	}

	/// 解析字符串，将字符串转换成相应的结构体，通常本地用于生成解析web请求
	/// \param [in] stream 要解析的字符串
	/// \param [in] key 配置对象所在键值
	/// \param [out] uiSessionID 会话ID号
	/// \param [out] config 转换好后的结构
	static void parseConfig(const std::string &stream, const std::string &key, uint &uiSessionID, T &config)
	{
		CConfigTable table;
		CConfigReader reader;

		reader.parse(stream, table);
		sscanf(table["SessionID"].asCString(), "0x%x", &uiSessionID);
		
		if (table[key].type() != Json::nullValue)
		{
			::exchangeTable(table[key], config, CKeyExchange::ES_LOADING);
		}
	}

	/// 序列化字符串，将结构体转换成字符串.通常web用来生成本地请求
	/// \param [in] uiSessionID 会话ID号
	/// \param [in] config 要转换的结构
	/// \param [in] key 对象所在键值
	/// \param [out] stream 转换后输出的字符串
	static void serizalConfig(uint uiSessionID, /*const*/T& config, const std::string &key, std::string &stream)
	{
		CConfigTable table;
		CConfigWriter writer(stream);
		char buf[32];

		sprintf(buf, "0x%x", uiSessionID);
		table["SessionID"] = buf;
		table["Name"] = key;
		::exchangeTable(table[key], config, CKeyExchange::ES_SAVING);
		writer.write(table);
	}

	/// 解析字符串，将字符串转换成相应的结构体，通常web用来解析本地响应
	/// \param [in] stream 要解析的字符串
	/// \param [in] key 配置对象所在的键值
	/// \param [out] 解析后得到的会话ID号
	/// \param [out] 解析后得到的用户返回值
	/// \param [out] 解析后的结构体
	static void parseConfig(const std::string &stream, const std::string &key, uint &uiSessionID, int &ret, T&config)
	{
		CConfigTable table;
		CConfigReader reader;

		reader.parse(stream, table);
		sscanf(table["SessionID"].asCString(), "0x%x", &uiSessionID);
		ret = table["Ret"].asInt();
		if (table[key].type() != Json::nullValue)
		{
			::exchangeTable(table[key], config, CKeyExchange::ES_LOADING);
		}
	}

	/// 序列化字符串，将结构体转换成字符串.通常本地用来生成web响应
	/// \param [in]
	static void serizalConfig(uint uiSessionID, int ret, /*const*/T& config, const std::string &key, std::string &stream)
	{
		CConfigTable table;
		CConfigWriter writer(stream);
		char buf[32];

		sprintf(buf, "0x%x", uiSessionID);
		table["SessionID"] = buf;
		table["Ret"] = ret;
		table["Name"] = key;
		::exchangeTable(table[key], config, CKeyExchange::ES_SAVING);
		writer.write(table);
	}

	/// 将Json表格转换成结构体
	/// \param [in] table 要转换的Json表格
	/// \param [out] 转换后的结构体
	static void getConfig(const CConfigTable &table, T &config)
	{
		union Panic_cast
		{
			const CConfigTable *in;
			CConfigTable *out;
		} cast;

		cast.in = &table;

		::exchangeTable(*cast.out, config, CKeyExchange::ES_LOADING);
	}

	/// 将结构体转换成Json表格
	/// \param [in] config 要转换的结构体
	/// \param [out] table 转换的配置表格
	static void setConfig(const T &config, CConfigTable &table)
	{
		union Panic_cast
		{
			const T *in;
			T *out;
		} cast;
		cast.in = &config;
		::exchangeTable(table, *cast.out, CKeyExchange::ES_SAVING);
	}

	/// 纯粹的结构体转化成字符串
	static void serizalConfig(const T &config, std::string &stream)
	{
		CConfigTable table;
		CConfigWriter writer(stream);

		union Panic_cast
		{
			const T *in;
			T *out;
		} cast;

		cast.in = &config;
		::exchangeTable(table, *cast.out, CKeyExchange::ES_SAVING);
		writer.write(table);
	}

	/// 将结构体转换成Json表格
	/// \param [in] config 要转换的结构体
	/// \param [out] table 转换的配置表格
	static void setConfigV2(const T &config, CConfigTable &table, int nSize)
	{
		union Panic_cast
		{
			const T *in;
			T *out;
		} cast;
		cast.in = &config;
		::exchangeTableV2(table, *cast.out, CKeyExchange::ES_SAVING, nSize);
	}

	/// 纯粹的结构体转化成字符串
	static void serizalConfigV2(const T &config, std::string &stream, int nSize)
	{
		CConfigTable table;
		CConfigWriter writer(stream);

		union Panic_cast
		{
			const T *in;
			T *out;
		} cast;

		cast.in = &config;
		::exchangeTableV2(table, *cast.out, CKeyExchange::ES_SAVING, nSize);
		writer.write(table);
	}

    /// 序列化字符串，将结构体转换成字符串.通常web用来生成本地请求
	/// \param [in] uiSessionID 会话ID号
	/// \param [in] config 要转换的结构
	/// \param [in] key 对象所在键值
	/// \param [out] 转换后输出的字符串
	/// \param [in] nSize config中需要转换的元素个数
	static void serizalConfigV2(uint uiSessionID, /*const*/T& config, const std::string &key, std::string &stream, int nSize)
	{
		CConfigTable table;
		CConfigWriter writer(stream);
		char buf[32] = {0};

		sprintf(buf, "0x%x", uiSessionID);
		table["SessionID"] = buf;
		table["Name"] = key;
		::exchangeTableV2(table[key], config, CKeyExchange::ES_SAVING, nSize);
		writer.write(table);
	}

	/// 字符串转换成配置结构
	static void parseConfig(const std::string &stream, T &config)
	{
		CConfigTable table;
		CConfigReader reader;

		reader.parse(stream, table);
		::exchangeTable(table, config, CKeyExchange::ES_LOADING);
	}

	/// 将Json表格转换成结构体
	/// \param [in] table 要转换的Json表格
	/// \param [out] 转换后的结构体
	static void getConfigV2(const CConfigTable &table, T &config, int nSize)
	{
		union Panic_cast
		{
			const CConfigTable *in;
			CConfigTable *out;
		} cast;

		cast.in = &table;

		::exchangeTableV2(*cast.out, config, CKeyExchange::ES_LOADING, nSize);
	}
	/// 字符串转换成配置结构
	static void parseConfigV2(const std::string &stream, T &config, int nSize)
	{
		CConfigTable table;
		CConfigReader reader;

		reader.parse(stream, table);
		::exchangeTableV2(table, config, CKeyExchange::ES_LOADING, nSize);
	}
};

void exchangeSysTime(CKeyExchange& configExchange, CConfigTable& table, SystemTime& systime);
int getIndex(ConfigPair *pairs, const char * name);
int getIndex(ConfigPair *pairs, int value);
void getString(const CConfigTable &table, std::string &stream);

#endif

