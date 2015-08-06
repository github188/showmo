

#ifndef __MANAGER_EVENT_H__
#define __MANAGER_EVENT_H__

#include "../Infra/Signals.h"
#include "../Infra/Time.h"
#include "../Types/Defs.h"
#include "../Json/value.h"
#include <string>

//  云台联动结构
struct PtzLinkConfig
{
	int iType;		// 联动的类型，见PtzLinkTypes
	int iValue;		// 联动的类型对应的值 
};

/// 工作表
struct WorkSheet
{	
	TimeSection	tsSchedule[N_WEEKS][N_TSECT];
};

/// 事件处理参数
struct EventHandler
{
	uint	dwRecord;				// 录象掩码 
	int		iRecordLatch;			// 录像延时：10～300 sec  	
	uint	dwTour;					// 轮巡掩码 	
	uint	dwSnapShot;				// 抓图掩码 
	uint	dwAlarmOut;				// 报警输出通道掩码 
	uint	dwMatrix;				// 矩阵掩码 
	int		iEventLatch;			// 联动开始延时时间，s为单位 
	int		iAOLatch;				// 报警输出延时：10～300 sec  
	PtzLinkConfig PtzLink[N_SYS_CH];		// 云台联动项 
	WorkSheet schedule;		// 录像时间段

	bool	bRecordEn;				// 录像使能 
	bool	bTourEn;				// 轮巡使能 
	bool	bSnapEn;				// 抓图使能 	
	bool	bAlarmOutEn;			// 报警使能 
	bool	bPtzEn;					// 云台联动使能 
	bool	bTip;					// 屏幕提示使能 	
	bool	bMail;					// 发送邮件 	
	bool	bMessage;				// 发送消息到报警中心 	
	bool	bBeep;					// 蜂鸣 	
	bool	bVoice;					// 语音提示 		
	bool	bFTP;					// 启动FTP传输 
	bool	bMatrixEn;				// 矩阵使能 
	bool	bLog;					// 日志使能
	bool	bMessagetoNet;			// 消息上传给网络使能 

	bool	bShowInfo;              // 是否在GUI上和编码里显示报警信息
	uint	dwShowInfoMask;         // 要联动显示报警信息的通道掩码
	char	pAlarmInfo[CHANNEL_NAME_MAX_LEN];//要显示的报警信息

	bool    bShortMsg;              //发送短信
	bool    bMultimediaMsg;         //发送彩信
};

typedef struct tagEVENT_INFO
{
	std::string		code;
	int				index;
	const EventHandler*	parameter;	// 事件处理参数
}EVENT_INFO;

#define MAX_EVENT_LATCH_NUMBER 4

class IEventManager
{
public:
	typedef TSignal5<std::string, int, appEventAction, const EventHandler *, Json::Value*> SIG_EVENT;
	typedef SIG_EVENT::SigProc Proc;

	static IEventManager* instance();
	virtual ~IEventManager();
	virtual bool attach(Proc proc) = 0;
	virtual bool detach(Proc proc) = 0;
	virtual bool checkTimeSection(const WorkSheet*workSheet) = 0;
	virtual void notify(std::string code, int index = 0, appEventAction action = appEventStart, const EventHandler *param = NULL, const WorkSheet *workSheet = NULL, Json::Value* data = NULL) = 0;
	virtual void getNextEvent() = 0;
	virtual bool getNextEvent(void* handler, EVENT_INFO* info) = 0;
	virtual void latchEvent(void *handler, IEventManager::Proc proc, std::string code, int index, int second) = 0;
};

#endif
