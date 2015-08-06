#ifndef __EXCHANGEAL_IMP_EXCHANGE_H__
#define __EXCHANGEAL_IMP_EXCHANGE_H__

#include "../PAL/Imp.h"
#include "GUIExchange.h"
#include "../Manager/EventManager.h"
#include "MediaExchange.h"
#include "../Infra/Timer.h"

enum _module_type
{
	MODULE_TYPE_PEA = 0,
	MODULE_TYPE_OSC,
	MODULE_TYPE_AVD,
	MODULE_TYPE_CPC,
	MODULE_TYPE_ALL,
};

enum
{
	maxCPCDataItem = 2048,
	maxCPCDataReturn = 512,
	itemtotalLength = 96,	//2位通道号，20位开始时间，20位结束时间，和两个13位的数据,加冗余
};

//各种算法的规则配置集合
typedef struct tagRuleConfig
{
	PEA_RULE_S stRulePea;   //周界检测与绊线配置
	OSC_RULE_S stRuleOSC; //物品盗移、滞留，非法停车配置
	AVD_RULE_S stRuleAVD;//异常视频信号诊断配置
	CPC_RULE_S stRuleCPC;//人头计数算法配置
}RuleConfig;


//一个通道的智能分析配置
typedef struct tagAnalyzeConfig
{
	bool         bEnable;      //是否启用视频分析功能
	int          moduleType;   //使用的那种算法模块
	EventHandler hEvent;  //联动事件
	RuleConfig   stRuleConfig; //各种算法的规则配置
}AnalyzeConfig;

//所有通道的智能分析配置
typedef struct tagAnalyzeConfigAll
{
	AnalyzeConfig vAnalyzeAll[N_SYS_CH];
}AnalyzeConfigAll;

//智能分析能力
struct AnalyzeAbility
{
	uint uIntelPEA; //智能PEA支持，位对应通道
	uint uAlgorithmPEA;//PEA支持的算法类型，例如拌线
	uint uIntelOSC; //智能OSC支持，位对应通道
	uint uAlgorithmOSC;//OSC支持的算法类型
	uint uIntelAVD; //智能AVD支持，位对应通道
	uint uAlgorithmAVD;//AVD支持的算法类型
	uint uIntelCPC; //人流量记数支持，位对应通道
	uint uAlgorithmCPC;//CPC支持的算法类型
};

struct CPCLink
{
	int NoteTime;
	VIDEO_WIDGET CPCInfo;
};

struct AnalyzeLink
{
	CPCLink CPCLinkConfig;
};

struct AnalyzeLinkAll
{
	AnalyzeLink vAnalyzeLinkAll[N_SYS_CH];
};

struct CPCDataItem		//人数统计项结构体
{
	int channel;			//对应的通道号
	SystemTime startTime;	//开始时间
	SystemTime endTime;	//结束时间
	int inNum;			//进入人数
	int outNum;			//离开人数
};

struct CPCDataAll
{
	int nItemNum;
	CPCDataItem CPCData[maxCPCDataReturn];
};

#endif //__EXCHANGEAL_IMP_EXCHANGE_H__
