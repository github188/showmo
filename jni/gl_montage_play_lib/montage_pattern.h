#ifndef _MONTAGE_PATTERN_SDK_
#define _MONTAGE_PATTERN_SDK_

#include "pw_datatype.h"
#include "pw_typedef.h"

#define PATTERN_VERSION		0x02
/*----------------------------------------------------------------------------------------------
*	Macro & Enum definition 
*---------------------------------------------------------------------------------------------*/

#define PMPA_TEX_NAME_MAX	16

#define PMPA_MEM_CFG_CF_NULL			0x00
#define PMPA_MEM_CFG_CF_FRONT_LEFT		0x01
#define PMPA_MEM_CFG_CF_FRONT_RIGHT		0x02
#define PMPA_MEM_CFG_CF_BACK_LEFT		0x03
#define PMPA_MEM_CFG_CF_BACK_RIGHT		0x04
#define PMPA_MEM_CFG_CF_FRONT			0x05
#define PMPA_MEM_CFG_CF_BACK			0x06
#define PMPA_MEM_CFG_CF_LEFT			0x07
#define PMPA_MEM_CFG_CF_RIGHT			0x08
#define PMPA_MEM_CFG_CF_FRONT_AND_BACK	0x09

typedef enum _pwpa_vtx_type_
{
	PMPA_VEC_GL_POINTS				= 0x0000,
	PMPA_VEC_GL_LINES				= 0x0001,
	PMPA_VEC_GL_LINE_LOOP			= 0x0002,
	PMPA_VEC_GL_LINE_STRIP			= 0x0003,
	PMPA_VEC_GL_TRIANGLES			= 0x0004,
	PMPA_VEC_GL_TRIANGLE_STRIP		= 0x0005,
	PMPA_VEC_GL_TRIANGLE_FAN		= 0x0006,
	PMPA_VEC_GL_QUADS				= 0x0007,
	PMPA_VEC_GL_QUAD_STRIP			= 0x0008,
	PMPA_VEC_GL_POLYGON				= 0x0009
}E_PWPA_VTX_TYPE;

/*----------------------------------------------------------------------------------------------
*	Strut & Unit definition 
*---------------------------------------------------------------------------------------------*/

/*--
	GFloat fScale, fWid_to_Hei;
	fWid_to_Hei = 1.0f * pstFrameSize->Hei / pstFrameSize->Wid;
	fScale = pstCamState->fMinDistance * tan( PW_FD2PI( pstCamState->fViewAngle * 0.5f ) );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	glViewport( 0, 0, (GLsizei)pstFrameSize->Wid, pstFrameSize->Hei ); 
	glFrustum( -fScale, fScale, -fScale * fWid_to_Hei, fScale * fWid_to_Hei, 
				pstCamState->fMinDistance, pstCamState->fMaxDistance );

	glRotatef( pstCamState->fTilt, 1.0f, 0.0f, 0.0f );		// Around X axis
	glRotatef( pstCamState->fPan, 0.0f, 0.0f, 1.0f );		//Around Z axis
	glRotatef( pstCamState->fRotate, 0.0f, 1.0f, 0.0f );	//Around Y axis
	glTranslatef( pstCamState->fPosiX, pstCamState->fPosiY, pstCamState->fPosiZ );// Move opposite direction
--*/

typedef struct _pwpa_cam_posture_
{
	GBool	bEnable;

	GFloat	fViewAngle;
	GFloat	fMinDistance;
	GFloat	fMaxDistance;

	GFloat	fPan;
	GFloat	fTilt;
	GFloat	fRotate;

	GFloat	fPosiX;
	GFloat	fPosiY;
	GFloat	fPosiZ;
}PWPA_CAM_POSTURE;

typedef struct _pwpa_posture_
{
	GUInt8	bNeedRotate;
	GUInt8	bNeedTranslate;
	GUInt8	bNeedScale;
	GUInt8	byExtern;
	GFloat	fRotate[3];		//	XYZ		//	Old version dRotateTilt = dRotate[0]
	GFloat	fTranslate[3];	//	XYZ		
	GFloat	fScale[3];		//	XYZ		
}PWPA_POSTURE;

typedef struct _pwpa_cam_posture_2_
{
	GUInt32	ulEnable		:1;			//	0 / 1
	GUInt32	ulPrioPan		:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt32	ulPrioTilt		:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt32	ulPrioRotate	:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt32	ulPrioPosiX		:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt32	ulPrioPosiY		:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt32	ulPrioPosiZ		:3;			//	0~6,0:No use; >0:Draw first which be higher.
	GUInt64	ullExtern		:13;

	GFloat	fViewAngle;
	GFloat	fMinDistance;
	GFloat	fMaxDistance;

	GFloat	fPan;
	GFloat	fTilt;
	GFloat	fRotate;

	GFloat	fPosiX;
	GFloat	fPosiY;
	GFloat	fPosiZ;
}PWPA_CAM_POSTURE_2;


typedef struct _pwpa_posture_2_
{
	GUInt64	ullPrioRotateX	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioRotateY	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioRotateZ	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioTransX	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioTransY	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioTransZ	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioScaleX	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioScaleY	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullPrioScaleZ	:4;			//	0~9,0:No use; >0:Draw first which be higher.
	GUInt64	ullEnable		:4;
	GUInt64	ullExtern		:24;
	GFloat	fRotate[3];		//	XYZ		//	Old version dRotateTilt = dRotate[0]
	GFloat	fTranslate[3];	//	XYZ		
	GFloat	fScale[3];		//	XYZ		
}PWPA_POSTURE_2;

typedef struct _pwpa_mem_cfg_
{
	GUInt64	dwCullFaceCfg	:4;		//	PMPA_MEM_CFG_CF_
	GUInt64	bNeedTexture	:4;		
	GUInt64	dwTransparency	:8;		//	0~255 = 0~1.0f: transparency of member 
	GUInt64	dwEffectLv		:4;
 	GUInt64	bLocalTexture	:1;
	GUInt64	dwExtern		:43;
}PWPA_MEM_CFG;

/*--  
	pstrTextureName:
	"PrivateTexture"			pstPrivateTex
--*/
typedef struct _pwpa_mem_tex_cfg_
{
	GChar*			pstrTextureName;
	LPGVM_SCREEN	pstPrivateTexture;	
}PWPA_MEM_TEX_CFG;

typedef struct _pw_pattern_mem_
{
	PWPA_MEM_CFG		stCfgMember;		
	E_PWPA_VTX_TYPE		ePatternType;
	PWPA_POSTURE		stMemberPosture;
	GChar				strTextureName[PMPA_TEX_NAME_MAX];
	GUInt32				dwPointCountRow;
	GUInt32				dwPointCountCol;
	GUInt32				dwPointCountLay;

	/*--	Value storage sequence.
			Lay0[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			Lay1[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			Lay2[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			.....
	--*/
	const GFloat		*pfVertexList_3;	//	... Xi, Yi, Zi ...
	const GFloat		*pfOriVertexList_3;	//	... Xi, Yi, Zi ...
	const GFloat		*pfColorList_4;		//	... Ri, Gi, Bi, Ai ...
	const GFloat		*pfTexCordList_2;	//	... Pi, Ti ...
}PW_PATTERN_MEM, *LP_PW_PATTERN_MEM;

typedef struct _pw_pattern_mem_2_
{
	PWPA_MEM_CFG		stCfgMember;		
	E_PWPA_VTX_TYPE		ePatternType;
	PWPA_POSTURE_2		stMemberPosture;
	GChar				strTextureName[PMPA_TEX_NAME_MAX];
	GUInt32				ulPointCountRow;
	GUInt32				ulPointCountCol;
	GUInt32				ulPointCountLay;

	/*--	Value storage sequence.
			Lay0[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			Lay1[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			Lay2[Col0(Row0, Row1, ... ) Col1(Row0, Row1, ... ) ...]
			.....
	--*/
	const GFloat		*pfVertexList_3;	//	... Xi, Yi, Zi ...
	const GFloat		*pfColorList_4;		//	... Ri, Gi, Bi, Ai ...
	const GFloat		*pfTexCordList_2;	//	... Pi, Ti ...
}PW_PATTERN_MEM_2, *LP_PW_PATTERN_MEM_2;

typedef struct _pw_pattern_group_
{
	GFloat				fBackColor[4];	//	GRBA:0~1 ; 0 == fBackColor[3] : no use
	PWPA_CAM_POSTURE	stCamState;
	PWPA_POSTURE		stGroupPosture;
	PWPA_POSTURE_2		stGroupPosture_2;

	/*-- Draw first which dwPriority is higher. --*/
	GUInt32				dwPriority;
	/*-- 0 == stCfgGroup ? config as pstPatternMember->stCfgMember --*/
	PWPA_MEM_CFG		stCfgGroup;			
	/*-- GNull == pstCfgTex ? Render texture follow pstPatternMember->pstCfgTex --*/
	GChar				strTextureName[PMPA_TEX_NAME_MAX];
	/*-- GNull != pstCfgTex ? Render texture by FBO, named as pstCfgTex->pstrTextureName --*/
	PWPA_MEM_TEX_CFG	stGenerateTexture;

	GBool				bNeedReloadPattern;
	GInt32				lMemberCount;
	GFloat				fVerToOri;			//	0~1, x = pfVertexList_3 * ( 1 - fVerToOri ) + pfOriVertexList_3 * fVerToOri
	const PW_PATTERN_MEM*	pstPatternMember;

	/*-- phNextAddEx point to a 'PW_PATTERN_GROUP' struct added provisionally.
			Will be drawn follow this group that ignore 'dwPriority'.
	--*/
	const GVoid			*phNextAddEx;
}PW_PATTERN_GROUP, *LP_PW_PATTERN_GROUP;

typedef struct _pw_pattern_group_2_
{
	GFloat				fBackColor[4];	//	GRBA:0~1 ; 0 == fBackColor[3] : no use
	PWPA_CAM_POSTURE_2	stCamState;
	PWPA_POSTURE_2		stGroupPosture;

	/*-- Draw first which dwPriority is higher. --*/
	GUInt32				ulPriority;
	/*-- 0 == stCfgGroup ? config as pstPatternMember->stCfgMember --*/
	PWPA_MEM_CFG		stCfgGroup;			
	/*-- GNull == pstCfgTex ? Render texture follow pstPatternMember->pstCfgTex --*/
	GChar				strTextureName[PMPA_TEX_NAME_MAX];
	/*-- GNull != pstCfgTex ? Render texture by FBO, named as pstCfgTex->pstrTextureName --*/
	PWPA_MEM_TEX_CFG	stGenerateTexture;

	GInt32				lMemberCount;
	const PW_PATTERN_MEM_2*	pstPatternMember;

	/*-- phNextAddEx point to a 'PW_PATTERN_GROUP' struct added provisionally.
			Will be drawn follow this group that ignore 'dwPriority'.
	--*/
	const GVoid			*phNextAddEx;
}PW_PATTERN_GROUP_2, *LP_PW_PATTERN_GROUP_2;


/*-- Pattern control component must output 'PW_PATTERN_OUT'
		'Montage play' will call 'opengl' or 'opengles 2.x' follow 'PW_PATTERN_OUT'
--*/
typedef struct _pw_pattern_out_
{
	GFloat					fBackColor[4];	//	GRBA:0~1
	PWPA_CAM_POSTURE		stCamState;
	PWPA_POSTURE			stWorldPosture;
	PWPA_POSTURE_2			stWorldPosture_2;

	GInt32					lTextureExCount;
	PWPA_MEM_TEX_CFG		*pstTextureExList;
	GInt32					lGroupCount;
	const PW_PATTERN_GROUP*	pstPatternGroup;

	/*-- pstPatternEx point to a 'PW_PATTERN_GROUP' struct added provisionally.
			Will be drawn at last that ignore 'dwPriority'.
	--*/
	const GVoid*			phPatternAddEx;

	GInt32						lGroupCount_2;
	const PW_PATTERN_GROUP_2*	pstPatternGroup_2;

}PW_PATTERN_OUT, *LP_PW_PATTERN_OUT;

typedef GInt32	( *GL_PA_CREATE )();
typedef GBool	( *GL_PA_DESTORY )( GInt32 );
typedef GBool	( *GL_PA_RESET )( GInt32 );
typedef GBool	( *GL_PA_SETINFO )( GInt32, const GChar*, const GChar* );
typedef GBool	( *GL_PA_GETINFO )( GInt32, const GChar*, const GChar** );
typedef GBool	( *GL_PA_PLAY )( GInt32 );
typedef GBool	( *GL_PA_PAUSE )( GInt32 );
typedef GBool	( *GL_PA_REPLAY )( GInt32 );
typedef GBool	( *GL_PA_STOP )( GInt32 );
typedef GBool	( *GL_PA_RUNTIME )( GInt32, GUInt32, const GSIZE*, PW_PATTERN_OUT* );
typedef GBool	( *GL_PA_FINGER )( GInt32, GLPOINTINSCREEN*, GUInt32, GUInt32 );
typedef GBool	( *GL_PA_SETEVENTCB )( GInt32, GChar*, GUInt64 );
typedef GBool	( *GL_PA_SETCOMPLEXINFO )( GInt32, const GChar*, GUInt64, GUInt64 );



#endif	//	_MONTAGE_PATTERN_SDK_
