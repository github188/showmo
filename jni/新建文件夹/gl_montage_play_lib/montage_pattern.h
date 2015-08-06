#ifndef _MONTAGE_PATTERN_SDK_
#define _MONTAGE_PATTERN_SDK_

#include "pw_datatype.h"
#include "pw_typedef.h"

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

typedef struct _pwpa_mem_cfg_
{
	GUInt64	dwCullFaceCfg	:4;		//	PMPA_MEM_CFG_CF_
	GUInt64	bNeedTexture	:4;		
	GUInt64	dwTransparency	:8;		//	0~255 = 0~1.0f: transparency of member 
	GUInt64	dwEffectLv		:4;
	GUInt64	dwExtern		:44;
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

typedef struct _pw_pattern_group_
{
	GFloat				fBackColor[4];	//	GRBA:0~1 ; 0 == fBackColor[3] : no use
	PWPA_CAM_POSTURE	stCamState;
	PWPA_POSTURE		stGroupPosture;

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


/*-- Pattern control component must output 'PW_PATTERN_OUT'
		'Montage play' will call 'opengl' or 'opengles 2.x' follow 'PW_PATTERN_OUT'
--*/
typedef struct _pw_pattern_out_
{
	GFloat				fBackColor[4];	//	GRBA:0~1
	PWPA_CAM_POSTURE	stCamState;
	PWPA_POSTURE		stWorldPosture;

	GInt32				lTextureExCount;
	PWPA_MEM_TEX_CFG	*pstTextureExList;
	GInt32				lGroupCount;
	const PW_PATTERN_GROUP*	pstPatternGroup;

	/*-- pstPatternEx point to a 'PW_PATTERN_GROUP' struct added provisionally.
			Will be drawn at last that ignore 'dwPriority'.
	--*/
	const GVoid*		phPatternAddEx;
}PW_PATTERN_OUT, *LP_PW_PATTERN_OUT;


#endif	//	_MONTAGE_PATTERN_SDK_
