#ifndef __EXCHANAGEAL_GUI_EXCHANGE_H__
#define __EXCHANAGEAL_GUI_EXCHANGE_H__

#include "../Types/Defs.h"
#include "Exchange.h"


/// GUI设置
struct GUISetConfig
{
	int iWindowAlpha;			///< 窗口透明度	[128, 255]
	bool bTimeTitleEn;			///< 时间标题显示使能
	bool bChannelTitleEn;		///< 通道标题显示使能	
	bool bAlarmStatus;			///<  报警状态
	bool bRecordStatus;			///<  录像状态显示使能
	bool bChanStateRecEn;		///< 录像标志显示使能
	bool bChanStateVlsEn;		///< 视频丢失标志显示使能
	bool bChanStateLckEn;		///< 通道锁定标志显示使能	
	bool bChanStateMtdEn;		///< 动态检测标志显示使能
	bool bBitRateEn;			///< 码流显示使能
	bool bRemoteEnable;			///< 遥控器使能
	bool bDeflick;				///< 抗抖动
};

//向导设置
struct Guideconfig
{
	int bEnable; 				///<向导是否开启。1开启，0关闭
	int reserved[3];
};


//VGA分辨率
struct VGAresolution
{
	int nHeight;
	int nWidth;
};
/// 得到系统总的语言列表
ConfigPair *getSystemLanguageList();

#endif
