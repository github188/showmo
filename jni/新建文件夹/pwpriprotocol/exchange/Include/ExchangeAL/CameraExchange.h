#ifndef _EXCHANGEAL_CAMERAEXCHANGE_H
#define _EXCHANGEAL_CAMERAEXCHANGE_H

#include "../Types/Defs.h"
#include "../Types/Types.h"
#include "../PAL/Video.h"
#include "../Infra/Time.h"

typedef enum CAMERA_SCENE //白平衡
{
	SCENE_AUTO,
	SCENE_INDOOR, //室内
	SCENE_OUTDOOR //室外
}CAMERA_SCENE;

typedef enum DNC_MODE //日夜模式取值
{
	DNC_AUTO,			// 自动切换
	DNC_MULTICOLOR,		// 彩色
	DNC_BLACKWHITE,		// 强制为黑白模式
	DNC_NR
}DNC_MODE;

typedef enum APERTURE_MODE //自动光圈模式
{
	APERTURE_AUTO_ON,
	APERTURE_AUTO_OFF,
}APERTURE_MODE;

typedef enum BLC_MODE //背光补偿模式
{
	BLC_AUTO,
	BLC_ON,
	BLC_OFF
}BLC_MODE;

//曝光配置
struct ExposureParam
{
	int  level;    //曝光等级
	uint leastTime;//自动曝光时间下限或手动曝光时间，单位微秒
	uint mostTime; //自动曝光时间上限，单位微秒
};

//增益配置
struct GainParam
{
	int gain;    //自动增益上限(自动增益启用)或固定增益值
	int autoGain;//自动增益是否启用，0:不开启  1:开启
};


//情景模式设置
typedef enum SCENE_MODE_e
{
	SM_STANDARD = 0,	// 标准
	SM_NEUTRAL,			// 柔和
	SM_VIVID,			// 鲜艳
	SM_BW,				// 黑白
}SceneMode_t;

//图像旋转属性
typedef enum ROTATE_ATTR_e
{
	RA_NONE = 0,
	RA_90,
	RA_180,
	RA_270,
	RA_BUTT
}RotateAttr_t;

// 3D降噪模式
typedef enum CAM_3DMODE_e
{
	STRM_NONE = 0,		// 关闭
	STRM_LOW,
	STRM_MIDLOW,
	STRM_MID,
	STRM_MIDHIGH,
	STRM_HIGH,
	STRM_BUTT
}CAM_3DMODE_t;

// 2D降噪强度
typedef enum CAM_2DMODE_e
{
	STRM2D_NONE = 0,	// 自动模式
	STRM2D_LOW,
	STRM2D_MID,
	STRM2D_HIGH,
	STRM2D_BUTT
}CAM_2DMODE_t;

/*
	设置曝光属性
	a)	自动/手动
	b)	自动时, 可设置增益控制(低/较低/中/较高/高)
	c)	手动时, 可设置:
					增益控制(低/较低/中/较高/高): 1~5
					快门(曝光时间): 0~12
					亮度控制(0-128)
*/
typedef struct CAM_AEATTR_s
{
	int expType;        // 0-自动, 1-手动
	int gainCtrl;       // 增益: 0~4
	int shutter;        // 快门(曝光时间): 0~11
	int aeMode;			// 曝光优先模式(0-噪声优先(default), 1-帧率优先)
	int rev[5];
}CamAEAttr_t;

#define MAX_IRCUT_MODE	16

typedef enum IRCUT_MODE_e	// IRCut Mode
{
	IRCUTMODE_AUTO = 0,		// 自动(default)
	IRCUTMODE_COLOR,		// 彩色
	IRCUTMODE_BW,			// 黑白
	IRCUTMODE_ALARM,		// 外部触发
	IRCUTMODE_NR
}IRCutMode_t;

typedef struct IRCUR_ATTR_s			// IR-Cut(日夜切换)属性
{
	int mode;						// 模式(自动(default)/彩色/黑白/外部触发)
	int dncThr;						// 当前模式对应的日夜转换阈值
	int ndcThr;						// 当前模式对应的夜日转换阈值
	int dncThrArray[MAX_IRCUT_MODE];// 日夜转换阈值
	int ndcThrArray[MAX_IRCUT_MODE];// 夜日转换阈值
}IRCutAttr_t;

//宽动态属性
typedef struct WDR_ATTR_s
{
	TimeSection	tSection;	// 时间段, 格式如: "1 00:00:00-24:00:00"
	int			strength;	// 强度
}WdrAttr_t;

// gamma模式枚举
typedef enum GAMMA_MODE_e
{
	CURVE_1_6 = 0,
	CURVE_1_8,
	CURVE_2_0,
	CURVE_2_2,
	CURVE_DEFAULT,
	CURVE_SRGB,
	CURVE_USER,
	CURVE_BUFF,
}GammaMode_t;

// gamma属性值
typedef struct GAMMA_ATTR_s
{
	int enable;
	int	mode;
}GammaAttr_t;

// 摄像头调试控制信息
typedef struct CAM_MODCTRL_s
{
	char	Name[64];
	int		DataLen;
	char	Data[4096];
}CAM_MODCTRL_t;

//网络摄像头参数
struct CameraParam				// HI3518平台
{
	uint whiteBalance;          //白平衡
	uint dayNightColor;         //日夜模式，取值有彩色、自动切换和黑白
	int  elecLevel;             //参考电平值
	uint apertureMode;          //自动光圈模式
	uint BLCMode;               //背光补偿模式
	ExposureParam exposureParam;//曝光配置
	GainParam     gainParam;    //增益配置
	uint PictureFlip;		//图片上下翻转
	uint PictureMirror;	//图片左右翻转
	uint RejectFlicker;	//日光灯防闪功能
	uint EsShutter;		//电子慢快门功能
	int ircut_mode;		//IR-CUT切换 0 = 红外灯同步切换 1 = 自动切换
	int dnc_thr;			//日夜转换阈值
	int ae_sensitivity;	//ae灵敏度配置
	int Day_nfLevel;		//noise filter 等级，0-5,0不滤波，1-5 值越大滤波效果越明显
	int Night_nfLevel;
	int Ircut_swap;		//ircut 正常序= 0        反序= 1

	//  zzb 2013.11.5 add
    CamAEAttr_t 	exposureAttr;		// 曝光属性
	IRCutAttr_t		ircutAttr;			// IR-Cut属性(自动模式时, dncThr=43, ndcThr=119, 且不可更改; 外部触发模式时, dncThr=80, ndcThr=72, 可由用户修改)
	int 			sceneMode;			// 情景模式(图像色彩模式: 标准(default)/柔和/鲜艳/黑白)
	VIDEO_COLOR		videoAttr;			// 图像属性
	WdrAttr_t		wdrAttr;			// 宽动态属性
	GammaAttr_t		gammaAttr;			// gamma属性
	int				rotateAttr;			// 图像旋转属性(关闭/水平/垂直/180度旋转)
	int				timeDomain3d;		// 3D时域(参考"enum CAM_3DMODE_e")
	int				noiseDropSth2d;		// 2D降噪强度(参考"enum CAM_2DMODE_e", 强度strength缩写为sth)
	int				antiFogSth;			// 去雾强度(0~100)
	int				antiPseudoColorSth;	// 去伪彩强度(0~100)
};

// -------- DM8127 Camera Params begin ------------
enum WhiteBalanceMode_e
{
	AWB_AUTO = 0,				// default
	DAY_D65,
	DAY_D55,
	FLORESCENT,
	INCANDESCENT
};

struct CameraDM8127				// DM8127平台摄像头参数
{
	int			blcMode;		// 背光补偿模式: 0-Min, 1-Mid, 2-Max(default)
	int			dreMode;		// DRE模式:	0-Off, 1-High Speed(default), 2-High Quality
	int			dreStrength;	// DRE强度: 0-Low, 1-Medium(default), 2-High
	int 		awbMode;		// 白平衡模式, 取值参考WhiteBlanceMode_e(default: AWB_AUTO)
	int			aeMode;			// 自动曝光模式: 0-AE_NIGHT, 1-AE_DAY(default)
	int			aewPriority;	// 自动曝光优先级: 0-帧率优先, 1-画质优先(default)
	int			binNingMode;	// BinNing模式: 0-BingNing(default), 1-Skip
	int			rotateAttr;		// 图像旋转属性(0-关闭/1-水平镜像/2-垂直镜像/3-水平+垂直镜像)
	VIDEO_COLOR	videoAttr;		// 图像属性
	IRCutAttr_t	ircutAttr;		// IR-Cut属性(自动模式时, dncThr=43, ndcThr=119, 且不可更改; 外部触发模式时, dncThr=80, ndcThr=72, 可由用户修改)
};
// -------- DM8127 Camera Params end ------------

// -------- dm8127 new Params -------------------
typedef enum IRCUT_CTRL_MODE_e
{
	IRCUT_CTRL_M_LDR = 0, //光敏电阻，同主动模式
	IRCUT_CTRL_M_DAN = 1, //日夜模式
	IRCUT_CTRL_M_PASV = 2, // 被动模式，通过前端图像获取亮度值作判断
	IRCUT_CTRL_M_MANU = 3, //手动模式 
	IRCUT_CTRL_M_RPASV = 4, //反向被动模式 
	IRCUT_CTRL_M_NR
}IRCUT_CTRL_MODE_t;

typedef struct IRCut_Time_s
{
	int hour;
	int minute;
	int second;
}IRCut_Time;

typedef struct IRCUT_CTRL_ATTR_s
{
	int dncThr;//向前兼容
	int ndcThr;//向前兼容
	int IsKeepColor;//是否保持彩色 [0 表示否 1表示 是]
	int Sensitivity;//灵敏度 [0 ~ 100]
	int ForceCut;//0表示强制切换为白天 1表示强制切换为夜晚
	IRCut_Time NightStartTime;//夜间开启时间
	IRCut_Time NightEndTime;//夜间结束时间 
}IRCUT_CTRL_ATTR_t;

typedef struct IRCUT_CTRL_s
{
	int IRCutCtrlMode;
	IRCUT_CTRL_ATTR_t IRCutCtrAttr[IRCUT_CTRL_M_NR];
}IRCUT_CTRL_t;

typedef struct tagCameraDM8127V2			// 新DM8127平台摄像头参数
{
	int env50_60hz;//0 表示 50HZ 1 表示 60HZ
	int AeMode; // 0 表示晚上 1表示 白天 
	int awbMode;// 0 自动 1 室内 2 室外
	int aewPriority;	// 自动曝光优先级: 0-帧率优先, 1-画质优先(default)
	int binningMode; //0 表示 binning 1表示skip
	int dreMode;		// DRE模式: 0-Off, 1-High Speed(default), 2-High Quality
	int dreStrength;	// DRE强度: 0-Low, 1-Medium(default), 2-High
	int blc; // 0 LOW 1 NORMAL 2 high
	int mirrorMode; // 0 Normal 1 mirror 2 flip 3 Rotate
	VIDEO_COLOR videoAttr;
	IRCUT_CTRL_t IRCutCtrl;
}CameraDM8127V2;


// -------- dm8127 new Params -------------------


//多个摄像头的参数
struct CameraParamAll
{
	CameraParam vCameraParamAll[N_SYS_CH];
};

struct CameraAllDM8127
{
	CameraDM8127V2 vCameraParamAll[N_SYS_CH];
};

//摄像头能力集
struct CameraAbility
{
	int  count;      //支持曝光速度数量
	uint speeds[16]; //曝光速度
	int  status;     //工作状态  >= 0 正常    < 0 异常
	int  elecLevel;  //参考电平值
	int  luminance;  //平均亮度
	char pVersion[64];//2a版本
	char reserve[32];//保留
};

#endif // _EXCHANGEAL_CAMERAEXCHANGE_H
