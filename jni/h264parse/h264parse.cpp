
#include "h264parse.h"
#include "h264def.h"

#ifndef HNull
#define HNull 0
#endif
int H264NalParse( unsigned char *pbuff, unsigned long dwBufflen, LPGVM_H264NALINFO pstNalInfoList, long lListLen, long *plNalNum )
{
	unsigned char *pCur = pbuff, *pLast=HNull;
	unsigned long dwLen = dwBufflen;
	unsigned long dwNalHead1 = 0xffffffff, dwNalHead2 = 0xffffffff;
	unsigned long dwLastType=H264_NAL_NALL, dwNalNum=0;

	while(dwLen)
	{
		unsigned char byTemp = *pCur; pCur++;
		dwLen--;

		dwNalHead2 = dwNalHead1;
		dwNalHead1 = (dwNalHead1<<8) + byTemp;
		if( 0x00000001 == dwNalHead2 )
		{	
			if( H264_NAL_NALL != dwLastType )
			{
				if(dwNalNum<lListLen)
				{
					pstNalInfoList[dwNalNum].pNalStart = pLast;
					pstNalInfoList[dwNalNum].dwNalType = dwLastType;
					pstNalInfoList[dwNalNum].dwNalLength = pCur - pLast - 5;
					dwNalNum++;
				}
			}
			byTemp = dwNalHead2 & 0x31;
			switch (byTemp)
			{
			case H264_NAL_P:
			case H264_NAL_I:
			case H264_NAL_SPS:
			case H264_NAL_PPS:
				pLast = pCur;
				dwLastType = byTemp;
				break;
			case H264_NAL_SEI:
				pLast = pCur;
				dwLastType = byTemp;
				if(dwLen>0)
				{
					byTemp = *pCur; pCur++;
					dwLen--;
					if(byTemp>H264_NAL_SEI_SELF)
						dwLastType = H264_NAL_SEI_SELF;
				}
				break;
			}
		}

	}
	if( H264_NAL_NALL != dwLastType )
	{
		if(dwNalNum<lListLen)
		{
			pstNalInfoList[dwNalNum].pNalStart = pLast;
			pstNalInfoList[dwNalNum].dwNalType = dwLastType;
			pstNalInfoList[dwNalNum].dwNalLength = pCur - pLast - 5;
			dwNalNum++;
		}
	}
	*plNalNum = dwNalNum;

	return 0;
}



int H264ParseSps( unsigned char *pbuff, unsigned long dwBufflen, LPGVM_H264SPSINFO pstH264Info )
{
	MpegEncContext test={0};
	long bit_size = 8 * dwBufflen;
	GVM_H264SPSINFO stDefault = { 0, 0, 0 };
	SPS stSPS = {0};
	int buffer_size = (bit_size+7)>>3;
	int iRes;

	if(!pstH264Info) return H264_PARA_ERR;

	*pstH264Info = stDefault;

    test.gb.buffer       = pbuff;
    test.gb.size_in_bits = bit_size;
    test.gb.size_in_bits_plus8 = bit_size + 8;
    test.gb.buffer_end   = pbuff + buffer_size;
    test.gb.index        = 0;

	iRes = ff_h264_decode_seq_parameter_set( &test, &stSPS );

	if(stSPS.mb_width>0) pstH264Info->dwFrameWidth = stSPS.mb_width * 16;
	if(stSPS.mb_height>0) pstH264Info->dwFrameHeight = stSPS.mb_height * 16;
	if( stSPS.num_units_in_tick>0 && stSPS.time_scale>0 )
		pstH264Info->dwFrameRate = stSPS.time_scale / (stSPS.num_units_in_tick * 2);
	if(iRes)
		return H264_NOT_COMPLETE;

	return H264_OK;
}
