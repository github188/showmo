
#ifndef _H264_PARSE_H_
#define _H264_PARSE_H_


#define H264_OK					0x00
#define H264_PARA_ERR			0x01
#define H264_NOT_COMPLETE		0x02

#define H264_NAL_NALL			0x00
#define H264_NAL_P				0x01
#define H264_NAL_I				0x05
#define H264_NAL_SPS			0x07
#define H264_NAL_PPS			0x08
#define H264_NAL_SEI			0x06
#define H264_NAL_SEI_SELF		0x80

typedef struct SLINCE_DATE_TIME_s					                        
{
	unsigned int	second	:6;		//	秒	0-59
	unsigned int	minute :6;		//	分	0-59
	unsigned int	hour :5;		//	时	0-23
	unsigned int	day :5;		//	日	1-31
	unsigned int	month :4;		//	月	1-12
	unsigned int	year :6;		//	年	2000-2063	
}SLINCE_DATE_TIME_t;

typedef struct _i_framehead
{
	unsigned char flags[4]; //0x00, 0x00, 0x01, 0xfc
	//CT-------
	//unsigned int codeType  :4;
	//unsigned int extWidth  :2;
	//unsigned int extHeight :2;
	//--------
	unsigned char ct;
	unsigned char fps;
	unsigned char width;   
	unsigned char heith;
	SLINCE_DATE_TIME_t clockTime;  
	unsigned int frameLen;  //include  head
}T_I_PwFrameHead;

typedef struct _p_framehead
{
	unsigned char flags[4];//0x00, 0x00, 0x01, 0xfd 
	unsigned int frameLen; 
}T_P_PwFrameHead;

typedef struct JFA_SLICE_HEADERS_s
{
	unsigned int SliceType;//帧类型 FA
	unsigned char CodecType;
	unsigned char SampRate;
	unsigned short avSliceLength;//码流数据 
}T_S_PwFrameHead;

typedef struct _nal_info_ 
{
	unsigned int	dwNalType;
	unsigned char	*pNalStart;
	unsigned int	dwNalLength;
}GVM_H264NALINFO, *LPGVM_H264NALINFO;

int H264NalParse( unsigned char *pbuff, unsigned long dwBufflen, LPGVM_H264NALINFO pstNalInfoList, long lListLen, long *plNalNum);

typedef struct _sps_info_ 
{
	unsigned int	dwFrameWidth;
	unsigned int	dwFrameHeight;
	unsigned int	dwFrameRate;
}GVM_H264SPSINFO, *LPGVM_H264SPSINFO;

int H264ParseSps( unsigned char *pbuff, unsigned long dwBufflen, LPGVM_H264SPSINFO pstH264Info );

#endif