#ifndef _EXCHANGEAL_CHAINEXCHANGE_H
#define _EXCHANGEAL_CHAINEXCHANGE_H

#include "CaptureExchange.h"

typedef enum OPChain_e
{
	OP_SET_PTZINFO,
	OP_ADD_REFOBJ,
	OP_CLEAR_REFOBJ,
	OP_TRANS_PIC2PTZ,
	OP_CHAIN_NR
}OPChain;

typedef struct CHAIN_CAPS
{
	int PtzNum;//支持最大通道数
	int Rev[12];
}CHAIN_CAPS;

typedef struct CHAIN_PTZATTR
{
	bool	NegAngle;		//false-不支持负角度, true-支持(默认值)
	int		FixHeight;		//云台安装高度 默认值为0
	int		PitchAngleDir;	//是否Z轴向上 1：向上，0表示向下(默认值)
}CHAIN_PTZATTR;

typedef struct CHAIN_PICRECT
{
	int PicX;			//像素坐标X
	int PicY;			//像素坐标Y
	int PicW;			//像素坐标W
	int PicH;			//像素坐标H
}CHAIN_PICRECT;

typedef struct CHAIN_PTZANG
{
	int PitchAng;		//俯仰角角度
	int HoriAng;		//水平角角度
}CHAIN_PTZANG;

typedef struct CHAIN_REFOBJ
{
	CHAIN_PICRECT PicRect;
	CHAIN_PTZANG PtzAng;
}CHAIN_REFOBJ;

typedef struct ChainParam
{
	CHAIN_PTZATTR	chainPtzAttr;
	CHAIN_REFOBJ	chainRefobj[32];
}ChainParam;

typedef struct PtzInfoReq_t
{
	int				ptz;
	CHAIN_PTZATTR	chainPtzAttr;
}PtzInfoReq;

typedef struct RefobjReq_t
{
	int				ptz;
	CHAIN_REFOBJ	chainRefobj;
}AddRefobjReq;

typedef struct TransPic2PtzAngleReq_t
{
	int				ptz;
	int				autoZoom;
	int				viewScale;
	CHAIN_PICRECT	picRect;
}TransPic2PtzAngleReq;

typedef struct TransPic2PtzAngleRsp_t
{
	CHAIN_PTZANG	chainPtzAngle;
	int				ptzZoom;
}TransPic2PtzAngleRsp;

const char* getChainOpName(int iOpIndex);

int getChainOpIndex(const char *sOpName);

#endif // _EXCHANGEAL_CHAINEXCHANGE_H

