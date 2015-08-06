
/*----------------------------------------------------------------------------------------------
* 2014 07 02 lgy	Add VM_PANO_2560_1920	VM_PANO_4000_3000
* 2014 11 04 lgy	Check all mirror
*---------------------------------------------------------------------------------------------*/

#ifndef __PUWELLTYPEDEF_H__
#define __PUWELLTYPEDEF_H__

#include "pw_datatype.h"
//////////////////////////////////////////////////////////////////////////
/*			Common Define												*/

typedef struct __tag_rect
{
	long left;
	long top;
	long right;
	long bottom;
} GRECT, *LPGRECT;

typedef struct __tag_point
{ 
	long x; 
	long y; 
} GPOINT, *LPGPOINT;

typedef struct __tag_fpoint
{ 
	float x; 
	float y; 
} GFPOINT, *LPGFPOINT;

typedef struct __tag_size
{ 
	long Wid; 
	long Hei; 
} GSIZE, *LPGSIZE;

typedef struct __tag_fsize
{ 
	float Wid; 
	float Hei; 
} GFSIZE, *LPGFSIZE;

typedef struct __tag_point3D
{ 
	long x;								
	long y;							
	long z;							
} GPOINT3D, *LPGPOINT3D;

typedef struct __tag_color
{ 
	unsigned char b;								
	unsigned char g;							
	unsigned char r;	
	unsigned char a;
} GCOLOR, *LPGCOLOR;

typedef struct __tag_fpoint3D
{ 
	float x;
	float y;
	float z;
} GFPOINT3D, *LPFGPOINT3D;

typedef struct __tag_fpoint4D
{ 
	float x;
	float y;
	float z;
	float a;
} GFPOINT4D, *LPFGPOINT4D;

typedef struct __tag_line
{ 
	GPOINT P1; 
	GPOINT P2; 
} GLINE, *LPGLINE;

typedef struct __tag_fline
{ 
	GFPOINT P1; 
	GFPOINT P2; 
} GFLINE, *LPGFLINE;

typedef struct __tag_fline3d
{ 
	GFPOINT3D P1; 
	GFPOINT3D P2; 
} GFLINE3D, *LPGFLINE3D;

typedef struct __tag_elli
{
	double Para[6];	//	Para[0]*X^2 + Para[1]*X + Para[2]*Y^2 + Para[3]*Y + Para[4]*X*Y = Para[5]
	GFPOINT Center;
	GFLINE LongAxis;
	GFLINE ShortAxis;
} GCOMELLI;

typedef struct _pwfspinsex
{
	GFPOINT		ScreenPoint; 
	GSIZE		ScreenSize;
}GPOINTINSCREEN;

typedef struct _pwlspinsex
{
	GPOINT		ScreenPoint; 
	GSIZE		ScreenSize;
}GLPOINTINSCREEN;

typedef enum pw_str_type_
{
	charpoint = 0,
	bytepoint = 1,
	wcharpoint = 2,
} GSTRTYPE;

typedef struct _pwstr
{
	long		StrLen; 
	GSTRTYPE	StrType;
	union
	{
		char	cpStr[64];
		unsigned char	bpStr[64];
		unsigned short	wpStr[32];
	}StringP;
}GString;

#define FS_MAX_CORRECT_GROUP_POINT_NUM 128
#define FS_MAX_SERIAL_LIST		4
#define FS_MAX_SERIAL_STRLEN	256
#define FS_MAX_MSGRETURN		256

typedef struct _fs_single_serialcom
{
	long StrLenth;
	unsigned char SerialComStr[FS_MAX_SERIAL_STRLEN];
	unsigned char MsgReturn[FS_MAX_MSGRETURN];
	long WaitMilliSecond;
}GFSSINGLESERIALCOM;

typedef struct _fs_serialcomlist
{
	long SingleComNum;
	GFSSINGLESERIALCOM SerialCom[FS_MAX_SERIAL_LIST];
}GFSSERIALCOMLIST;

//////////////////////////////////////////////////////////////////////////
/*			Image / Frame Define										*/
#ifndef VM_FORMAT_
#define VM_FORMAT_
#define VM_FORMAT_PLANAR		0x0100
#define VM_FORMAT_BEPLANAR_		0x0f00

#define VM_FORMAT_GRAY8			0x001		//	Y
#define VM_FORMAT_B8G8R8		0x002		//	B8	G8	R8
#define VM_FORMAT_B8G8R8A8		0x003		//	B8	G8	R8	A8
#define VM_FORMAT_R8G8B8		0x004		//	R8	G8	B8
#define VM_FORMAT_R8G8B8A8		0x005		//	R8	G8	B8	A8
#define VM_FORMAT_YUY2			0x011		//	Y0	U0	Y1	V0	Y2	U1	Y3	V1
#define VM_FORMAT_UYVY			0x012		//	Y0	U0	Y1	V0	Y2	U1	Y3	V1
#define VM_FORMAT_YUV422		0x013		//	Y0	U0	Y1	V0

#define VM_FORMAT_YUV420_P		(0x011 | VM_FORMAT_PLANAR)		
#define VM_FORMAT_YUV12420_P	(0x012 | VM_FORMAT_PLANAR)		
#define VM_FORMAT_YUV422_P		(0x013 | VM_FORMAT_PLANAR)		
#endif

typedef struct _vm_screen
{
	long		lWidth;
	long		lHeight;
	long		ImgFormat;
	void		*pBuffHandle;
	union
	{
		struct  
		{
			long	lLineBytes;
			void	*pPixel;	
		}chunky;
		struct  
		{
			long	lLineBytesArray[4];
			void	*pPixelArray[4];	
		}planar;
	}PixelArray;
}GVM_SCREEN, *LPGVM_SCREEN;

typedef union _pw_data_un_
{
	GUInt64		Data_UI64;
	GInt64		Data_I64;
	GUInt32		Data_UI32[2];
	GInt32		Data_I32[2];
	GUInt16		Data_UI16[4];
	GInt16		Data_I16[4];
	GUInt8		Data_UI8[8];
	GInt8		Data_I8[8];
	GDouble		Data_D;
	GFloat		Data_F[2];	
}PW_DATA_UN;

//////////////////////////////////////////////////////////////////////////
//	2014 11 04 lgy add new definition

typedef struct _pw_pano_type_01_
{
	unsigned int	lDefNo		:4;
	unsigned int	lResRateW	:5;		//	Resolution is lResRateW : lResRateH; Follow the frame size if either of them is 0
	unsigned int	lResRateH	:5;
	unsigned int	lWidth_8	:11;	//	Width/8
	unsigned int	lRadius_4	:11;	//	Radius/4
	unsigned int	lLensType	:4;		//	0:HM;	1:Fish;		2:montage sphere
	unsigned int	lLensNo		:6;		//	
	int		lCenterOffX_2		:6;		//	( CenterX - Width / 2 ) / 2
	int		lCenterOffY_2		:6;		//	( CenterY - Height / 2 ) / 2
	unsigned int	lExten		:6;		//	
//	unsigned int	lExten		:18;	//	
}GVM_PANO_TYPE_01;


GInt32 pwGetPanoTypeVer( GUInt64 dwPanoramicType );
GBool pwGetPanoLens( GUInt64 dwPanoramicType, GInt32 *plLensType, GInt32 *plLensNo );
GBool pwGetPanoSize( GUInt64 dwPanoramicType, GSIZE *pstFrameSize );
GBool pwGetImageRate( GSIZE stImageSize, GSIZE *pstRate );


#define VM_PANO_DEF_EX_STEP			60
#define VM_PANO_DEF_ORI_EX01		0x01//	type definition version 01

#define VM_PANO_DEF_EX01			(VM_PANO_DEF_ORI_EX01<<VM_PANO_DEF_EX_STEP)

#define VM_PANO_RERA_WSTEP_EX01		55	
#define VM_PANO_RERA_HSTEP_EX01		50	

#define VM_PANO_WID_STEP_EX01		39	
#define VM_PANO_WID_TIME_EX01		8	//	As: 352 / 8
#define VM_PANO_RAD_STEP_EX01		28	
#define VM_PANO_RAD_TIME_EX01		4	//	As: 352 / 4

#define VM_PANO_352_ORI_EX01		44		//	352 / 16 = 22
#define VM_PANO_352_EX01			(VM_PANO_352_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_704_ORI_EX01		88		//	704 / 16 = 44
#define VM_PANO_704_EX01			(VM_PANO_704_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_720_ORI_EX01		90		//	720 / 16 = 45
#define VM_PANO_720_EX01			(VM_PANO_720_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_768_ORI_EX01		96		//	768 / 16 = 48
#define VM_PANO_768_EX01			(VM_PANO_768_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_1024_ORI_EX01		128		//	1024 / 16 = 64
#define VM_PANO_1024_EX01			(VM_PANO_1024_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_1280_ORI_EX01		160		//	1280 / 16 = 80
#define VM_PANO_1280_EX01			(VM_PANO_1280_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_1536_ORI_EX01		192		//	1536 / 16 = 96
#define VM_PANO_1536_EX01			(VM_PANO_1536_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_1920_ORI_EX01		240		//	1920 / 16 = 120
#define VM_PANO_1920_EX01			(VM_PANO_1920_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_2048_ORI_EX01		256		//	2048 / 16 = 128
#define VM_PANO_2048_EX01			(VM_PANO_2048_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_2560_ORI_EX01		320		//	2560 / 16 = 160
#define VM_PANO_2560_EX01			(VM_PANO_2560_ORI_EX01<<VM_PANO_WID_STEP_EX01)

#define VM_PANO_4000_ORI_EX01		500		//	4000 / 16 = 250
#define VM_PANO_4000_EX01			(VM_PANO_4000_ORI_EX01<<VM_PANO_WID_STEP_EX01)


#define VM_PANO_768_576_EX01		( ( 4 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 3 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_768_EX01 )	
#define VM_PANO_720_480_EX01		( ( 3 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 2 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_720_EX01 )	
#define VM_PANO_1024_1024_EX01		( ( 1 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 1 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_1024_EX01 )	
#define VM_PANO_1280_960_EX01		( ( 4 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 3 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_1280_EX01 )	
#define VM_PANO_1280_1024_EX01		( ( 5 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 4 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_1280_EX01 )	
#define VM_PANO_1536_1536_EX01		( ( 1 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 1 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_1536_EX01 )	
#define VM_PANO_1920_1080_EX01		( ( 16 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 9 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_1920_EX01 )	
#define VM_PANO_2048_1536_EX01		( ( 4 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 3 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_2048_EX01 )	
#define VM_PANO_2560_1920_EX01		( ( 4 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 3 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_2560_EX01 )	
#define VM_PANO_4000_3000_EX01		( ( 4 <<VM_PANO_RERA_WSTEP_EX01 ) | ( 3 <<VM_PANO_RERA_HSTEP_EX01 ) | VM_PANO_4000_EX01 )	
/////
#define VM_PANO_LENS_TYPE_STEP_EX01	24	

#define VM_PANO_LENS_TYPE_M_ORI_EX01	0x0
#define VM_PANO_LENS_TYPE_F_ORI_EX01	0x1
#define VM_PANO_LENS_TYPE_S_ORI_EX01	0x2

#define VM_PANO_LENS_TYPE_M_EX01		( VM_PANO_LENS_TYPE_M_ORI_EX01 << VM_PANO_LENS_TYPE_STEP_EX01 )
#define VM_PANO_LENS_TYPE_F_EX01		( VM_PANO_LENS_TYPE_F_ORI_EX01 << VM_PANO_LENS_TYPE_STEP_EX01 )
#define VM_PANO_LENS_TYPE_S_EX01		( VM_PANO_LENS_TYPE_S_ORI_EX01 << VM_PANO_LENS_TYPE_STEP_EX01 )

#define VM_PANO_LENS_NO_STEP_EX01	18	

#define VM_PANO_00_65_NO_EX01		0x00	
#define VM_PANO_00_65_EX01			( VM_PANO_LENS_TYPE_M_EX01 | ( VM_PANO_00_65_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )	
#define VM_PANO_N20_70_NO_EX01		0x01	
#define VM_PANO_N20_70_EX01			( VM_PANO_LENS_TYPE_M_EX01 | ( VM_PANO_N20_70_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )		
#define VM_PANO_N20_45_NO_EX01		0x02
#define VM_PANO_N20_45_EX01			( VM_PANO_LENS_TYPE_M_EX01 | ( VM_PANO_N20_45_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )

		
#define VM_PANO_N25F_NO_EX01		0x00	
#define VM_PANO_N25F_EX01			( VM_PANO_LENS_TYPE_F_EX01 | ( VM_PANO_N25F_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )		
#define VM_PANO_N213F_NO_EX01		0x01	
#define VM_PANO_N2130F_EX01			( VM_PANO_LENS_TYPE_F_EX01 | ( VM_PANO_N213F_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )		
#define VM_PANO_N113F_NO_EX01		0x02	
#define VM_PANO_N113F_EX01			( VM_PANO_LENS_TYPE_F_EX01 | ( VM_PANO_N113F_NO_EX01 << VM_PANO_LENS_NO_STEP_EX01 ) )		

//////////////////////////////////////////////////////////////////////////
/*			Sensor Style												*/
#define VM_SENSOR_PANORAMIC		0x00001	
#define VM_SENSOR_PTZ			0x00002	
#define VM_SENSOR_CAMERA		0x00003	

#define VM_SENSOR_PANORAMIC_S	"PANORAMIC"
#define VM_SENSOR_PTZ_S			"PTZ"	
#define VM_SENSOR_CAMERA_S		"CAMERA"	


//////////////////////////////////////////////////////////////////////////
/*			Panoramic Type Define										*/
#define VM_PANO_RESO_MASK		0xff000	
#define VM_PANO_768_576			0x01000	
#define VM_PANO_1280_1024		0x02000	
#define VM_PANO_2048_1536		0x03000	
#define VM_PANO_1280_960		0x04000	
#define VM_PANO_2448_2048		0x05000	
#define VM_PANO_1920_1080		0x06000	
#define VM_PANO_1024_1024		0x07000	
#define VM_PANO_1536_1536		0x08000
#define VM_PANO_2560_1920		0x09000
#define VM_PANO_4000_3000		0x0a000

#define VM_PANO_MIRROR_MASK		0x0ff0	
#define VM_PANO_MIRROR_STEP		0x04
#define VM_PANO_00_65			0x0000	
#define VM_PANO_N10_65			0x0010	
#define VM_PANO_N20_70			0x0020	
#define VM_PANO_00_75			0x0030	
#define VM_PANO_N20_45			0x0040	
#define VM_PANO_N02_90F			0x0200	

//////////////////////////////////////////////////////////////////////////
//	以下定义基本作废，尽量用上面的定义组合
//	最新的C30面型是VM_PANO_N20_70

#define VM_PANO_PW_LF_A00		(VM_PANO_768_576 | VM_PANO_N02_90F)
#define VM_PANO_PW_LF_A01		(VM_PANO_768_576 | VM_PANO_N02_90F )
#define VM_PANO_PW_LF_A02		(VM_PANO_768_576 | VM_PANO_N02_90F )
#define VM_PANO_PW_LF_B00		(VM_PANO_768_576 | VM_PANO_N02_90F )
#define VM_PANO_PW_EM_A00		(VM_PANO_768_576 | VM_PANO_00_75 )	

#define VM_PANO_PW_HM_B10		(VM_PANO_1280_960 | VM_PANO_00_65 )	
#define VM_PANO_PW_HM_C10		(VM_PANO_1280_960 | VM_PANO_N20_45 )
#define VM_PANO_PW_HF_A10		(VM_PANO_1280_960 | VM_PANO_N02_90F )	
#define VM_PANO_PW_HF_T10		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T11		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_B10		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_B11		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_B30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_B31		(VM_PANO_2048_1536 | VM_PANO_N02_90F )

#define VM_PANO_PW_HF_T20		(VM_PANO_1920_1080 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T21		(VM_PANO_1920_1080 | VM_PANO_N02_90F )
	
#define VM_PANO_PW_HM_B30		(VM_PANO_2048_1536 | VM_PANO_00_65 )	
#define VM_PANO_PW_HM_C30		(VM_PANO_2048_1536 | VM_PANO_N20_45 )	
#define VM_PANO_PW_HF_A30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T31		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T32		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T33		(VM_PANO_2048_1536 | VM_PANO_N02_90F )

#define VM_PANO_PW_HM_B50		(VM_PANO_2448_2048 | VM_PANO_00_65 )	
#define VM_PANO_PW_HM_C50		(VM_PANO_2448_2048 | VM_PANO_N20_45 )	
#define VM_PANO_PW_HF_A50		(VM_PANO_2448_2048 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T50		(VM_PANO_2448_2048 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_T51		(VM_PANO_2448_2048 | VM_PANO_N02_90F )

#define VM_PANO_PW_EF_A10		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_A11		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_A30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_A31		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_B10		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_B11		(VM_PANO_1280_960 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_B30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_EF_B31		(VM_PANO_2048_1536 | VM_PANO_N02_90F )

#define VM_PANO_PW_HF_X30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )
#define VM_PANO_PW_HF_Q30		(VM_PANO_2048_1536 | VM_PANO_N02_90F )

//////////////////////////////////////////////////////////////////////////
// New type define

#define PW_PANO_PW_F_A1X_T		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_M_B1X_T		(VM_PANO_1280_960 | VM_PANO_00_65 | 0x00 )
#define PW_PANO_PW_M_C1X_T		(VM_PANO_1280_960 | VM_PANO_N20_45 | 0x00 )

#define PW_PANO_PW_F_A3X_T		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_M_B3X_T		(VM_PANO_2048_1536 | VM_PANO_00_65 | 0x00 )
#define PW_PANO_PW_M_C3X_T		(VM_PANO_2048_1536 | VM_PANO_N20_70 | 0x00 )

#define PW_PANO_PW_F_A1X_P		(VM_PANO_1280_1024 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_M_B1X_P		(VM_PANO_1280_1024 | VM_PANO_00_65 | 0x00 )
#define PW_PANO_PW_M_C1X_P		(VM_PANO_1280_1024 | VM_PANO_N20_70 | 0x00 )

#define PW_PANO_PW_F_A3X_P		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_M_B3X_P		(VM_PANO_2048_1536 | VM_PANO_00_65 | 0x00 )
#define PW_PANO_PW_M_C3X_P		(VM_PANO_2048_1536 | VM_PANO_N20_70 | 0x00 )

#define PW_PANO_PW_LF_A00		(VM_PANO_768_576 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_LF_A01		(VM_PANO_768_576 | VM_PANO_N02_90F | 0x01 )
#define PW_PANO_PW_LF_A02		(VM_PANO_768_576 | VM_PANO_N02_90F | 0x02 )
#define PW_PANO_PW_LF_B00		(VM_PANO_768_576 | VM_PANO_N02_90F | 0x03 )
#define PW_PANO_PW_EM_A00		(VM_PANO_768_576 | VM_PANO_00_75  | 0x00 )	

#define PW_PANO_PW_HM_B10		(VM_PANO_1280_960 | VM_PANO_00_65  | 0x00 )	
#define PW_PANO_PW_HM_C10		(VM_PANO_1280_960 | VM_PANO_N20_70 | 0x00 )
#define PW_PANO_PW_HF_A10		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x00 )	
#define PW_PANO_PW_HF_T10		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x01 )
#define PW_PANO_PW_HF_T11		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x02 )
#define PW_PANO_PW_HF_B10		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x03 )
#define PW_PANO_PW_HF_B11		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x04 )
#define PW_PANO_PW_EF_A10		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x05 )
#define PW_PANO_PW_EF_A11		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x06 )
#define PW_PANO_PW_EF_B10		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x07 )
#define PW_PANO_PW_EF_B11		(VM_PANO_1280_960 | VM_PANO_N02_90F | 0x08 )

#define PW_PANO_PW_HF_T20		(VM_PANO_1920_1080 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_HF_T21		(VM_PANO_1920_1080 | VM_PANO_N02_90F | 0x01 )

#define PW_PANO_PW_HM_B30		(VM_PANO_2048_1536 | VM_PANO_00_65 | 0x00 )	
#define PW_PANO_PW_HM_C30		(VM_PANO_2048_1536 | VM_PANO_N20_70 | 0x00 )	
#define PW_PANO_PW_HF_A30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_HF_T30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x01 )
#define PW_PANO_PW_HF_T31		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x02 )
#define PW_PANO_PW_HF_T32		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x03 )
#define PW_PANO_PW_HF_T33		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x04 )
#define PW_PANO_PW_HF_X30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x05 )
#define PW_PANO_PW_HF_Q30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x06 )

#define PW_PANO_PW_EF_A30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x07 )
#define PW_PANO_PW_EF_A31		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x08 )

#define PW_PANO_PW_HF_B30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x09 )
#define PW_PANO_PW_HF_B31		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x0a )

#define PW_PANO_PW_EF_B30		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x0b )
#define PW_PANO_PW_EF_B31		(VM_PANO_2048_1536 | VM_PANO_N02_90F | 0x0c )

#define PW_PANO_PW_HM_B50		(VM_PANO_2560_1920 | VM_PANO_00_65 | 0x00 )	
#define PW_PANO_PW_HM_C50		(VM_PANO_2560_1920 | VM_PANO_N20_70 | 0x00 )	
#define PW_PANO_PW_HF_A50		(VM_PANO_2560_1920 | VM_PANO_N02_90F | 0x00 )
#define PW_PANO_PW_HF_T50		(VM_PANO_2560_1920 | VM_PANO_N02_90F | 0x01 )
#define PW_PANO_PW_HF_T51		(VM_PANO_2560_1920 | VM_PANO_N02_90F | 0x02 )
//////////////////////////////////////////////////////////////////////////

#define VM_PANO_EM40075AT0_S	"VM_PANO_EM40075AT0"		

#define VM_PANO_PW_EM_A00_S		"VM_PANO_PW_EM_A10"	
#define VM_PANO_PW_HM_B10_S		"VM_PANO_PW_HM_B10"	
#define VM_PANO_PW_HM_B30_S		"VM_PANO_PW_HM_B30"	
#define VM_PANO_PW_HM_C30_S		"VM_PANO_PW_HM_C30"		

#define VM_PANO_PW_LF_A00_S		"VM_PANO_PW_LF_A00"		
#define VM_PANO_PW_LF_A01_S		"VM_PANO_PW_LF_A01"	
#define VM_PANO_PW_LF_A32_S		"VM_PANO_PW_LF_A32"	

#define VM_PANO_PW_HF_A10_S		"VM_PANO_PW_HF_A10"	
#define VM_PANO_PW_HF_A30_S		"VM_PANO_PW_HF_A30"	
#define VM_PANO_PW_HF_T10_S		"VM_PANO_PW_HF_T10"	
#define VM_PANO_PW_HF_T30_S		"VM_PANO_PW_HF_T30"	



#define HK_PROTOCOL	       		0xB5	//海康
#define DH_PROTOCOL	       		0x3A	//大华

#define VM_BITERATE_1			300
#define VM_BITERATE_2			600
#define VM_BITERATE_3			1200
#define VM_BITERATE_4			2400
#define VM_BITERATE_5			4800
#define VM_BITERATE_6			9600
#define VM_BITERATE_7			19200
#define VM_BITERATE_8			38400
#define VM_BITERATE_9			43000
#define VM_BITERATE_A			56000
#define VM_BITERATE_B			57600
#define VM_BITERATE_C			115200
#define VM_BITERATE_D			230400


#endif //__PUWELLTYPEDEF_H__