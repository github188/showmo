#ifndef __EXCHANGEAL_CAPTUREEXCHANGE_H__
#define __EXCHANGEAL_CAPTUREEXCHANGE_H__

#include "../Types/Defs.h"

// 全景参数
typedef struct tagCAPTURE_PANOATTR
{
	int	CenterX;		//圆心X坐标
	int	CenterY;		//圆心Y坐标
	int Radius;			//圆半径
	uint PanoMask;		//面型、最大分辨率掩码
	int PanoTilt;		//安装姿态
	int ElliPara[6];	//椭圆系数
}CAPTURE_PANOATTR;

typedef struct tagCAPTURE_PANOATTR_ALL
{
	CAPTURE_PANOATTR vCapturePanoAll[N_SYS_CH];
}CapturePanoAttrAll;

#endif
