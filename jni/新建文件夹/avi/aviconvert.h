#ifndef AVICONVERT_H
#define AVICONVERT_H
#include "pw_datatype.h"
#include <vector>
#include <stdio.h>
#include <string>
typedef void *CoverFileCallBack(GInt32 CurrentPos, GInt32 TotoalPos,GInt32 dwUser);//转换进度
#define SHOWE

#ifdef SHOWE
#define FRAME_I			0
#define FRAME_P			1
#define FRAME_A			3
#else
#define FRAME_I			0x01FC
#define FRAME_P			0x01FD
#define FRAME_A			0x01FA
#endif

#define PHONE_MAKEFOURCC(ch0, ch1, ch2, ch3)              \
    ((GInt32)(BYTE)(ch0) | ((GInt32)(BYTE)(ch1) << 8) |   \
    ((GInt32)(BYTE)(ch2) << 16) | ((GInt32)(BYTE)(ch3) << 24 ))

#define PHONE_WAVE_FORMAT_PCM     1
#define AVIIF_KEYFRAME	0x00000010L
#define AVIF_HASINDEX	0x00000010	// Index at end of file?
#define AVIF_TRUSTCKTYPE   0x00000800      /* Use CKType to find key frames */
#define HEADERBYTES     2048

#define CHUNK_ID_BYTES		4
#define CHUNK_SIZE_BYTES	4
#define CHUNK_TYPE_BYTES	4

//不通过sizeof函数去计算大小是避免不同编译器产生的4字节对齐问题
#define AVIH_BYTES			56
#define STRH_AUDIO_BYTES	56
#define STRH_VEDIO_BYTES	56
#define STRF_AUDIO_BYTES	18
#define STRF_VEDIO_BYTES	40
#define IDXL_BYTES			16

#define STRUCT_AVIH_FRAME_BYTE_OFFSET	16
#define STRUCT_STRH_FRAME_BYTE_OFFSET	32
#define STRUCT_AVIH_SUGGEST_BUFFER_BYTE_OFFSET	28		//dwSuggestedBufferSize
#define STRUCT_STRH_SUGGEST_BUFFER_BYTE_OFFSET	36		//dwSuggestedBufferSize

#define AVI_RIFF_SIZE_BYTE_OFFSET	4			//riff文件大小在所在文件字节流中的偏移量
#define AVI_MOVI_SIZE_BYTE_OFFSET	CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                    CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                    CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+AVIH_BYTES+ \
                                    CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+ \
                                    CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_VEDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_VEDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+\
                                    CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_AUDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_AUDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*LIST*/

#define AVI_AVIH_FRAME_BYTE_OFFSET	CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                    CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                    CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+STRUCT_AVIH_FRAME_BYTE_OFFSET


#define AVI_STRH_VFRAME_BYTE_OFFSET	CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                    CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                    CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+AVIH_BYTES+ \
                                    CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+ \
                                    CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRUCT_STRH_FRAME_BYTE_OFFSET


#define AVI_STRH_AFRAME_BYTE_OFFSET CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                    CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                    CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+AVIH_BYTES+ \
                                    CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+ \
                                    CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_VEDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_VEDIO_BYTES+ \
                                    CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+\
                                    CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRUCT_STRH_FRAME_BYTE_OFFSET


#define AVI_AVIH_SUGGEST_BUFFER_BYTE_OFFSET	CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                            CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                            CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+STRUCT_AVIH_SUGGEST_BUFFER_BYTE_OFFSET


#define AVI_STRH_VSUGGEST_BUFFER_BYTE_OFFSET CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
                                             CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+CHUNK_TYPE_BYTES/*hdrl*/+ \
                                             CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+AVIH_BYTES+ \
                                             CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/+ \
                                             CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRUCT_STRH_SUGGEST_BUFFER_BYTE_OFFSET

typedef struct
{
    GUInt16    left;
    GUInt16    top;
    GUInt16    right;
    GUInt16    bottom;
}PHONE_SRECT, *LPPHONE_SRECT;

//avih:avi header
typedef struct
{
    GInt32 dwMicroSecPerFrame ;		//显示每帧所需的时间(微秒)，定义avi的显示速率
    GInt32 dwMaxBytesPerSec;		// 最大的数据传输率
    GInt32 dwPaddingGranularity;	//记录块的长度需为此值的倍数，通常是2048
    GInt32 dwFlages;				//AVI文件的特殊属性，如是否包含索引块，音视频数据是否交叉存储
    GInt32 dwTotalFrame;			//文件中的总帧数
    GInt32 dwInitialFrames;			//说明在开始播放前需要多少帧
    GInt32 dwStreams;				//文件中包含的数据流种类
    GInt32 dwSuggestedBufferSize;	//建议读取本文件的缓存大小（应能容纳最大的块）
    GInt32 dwWidth;					//图像宽
    GInt32 dwHeight;				//图像高
    GInt32 dwReserved[4];			//保留值
}PHONE_AVIHEADER;

//strh:stream header
typedef struct {
    GInt32		fccType;			//4字节，表示数据流的种类 vids 表示视频数据流,auds 音频数据流
    GInt32      fccHandler;			//4字节，表示数据流解压缩的驱动程序代号
    GInt32      dwFlags;			//数据流属性, Contains AVITF_* flags
    GInt16		wPriority;			//此数据流的播放优先级
    GInt16		wLanguage;			//音频的语言代号
    GInt32		dwInitialFrames;	//说明在开始播放前需要多少帧
    GInt32      dwScale;			//数据量，视频每桢的大小或者音频的采样大小
    GInt32      dwRate;				//dwScale /dwRate = 每秒的采样数
    GInt32      dwStart;			//数据流开始播放的位置，以dwScale为单位
    GInt32      dwLength;			//数据流的数据量，以dwScale为单位
    GInt32      dwSuggestedBufferSize;	//建议缓冲区的大小
    GInt32      dwQuality;			//解压缩质量参数，值越大，质量越好
    GInt32      dwSampleSize;		//音频的采样大小
    PHONE_SRECT        rcFrame;			//视频图像所占的矩形
} PHONE_AVI_STREAM_HEADER, *LPPHONE_AVI_STREAM_HEADER;


//strf:stream format
typedef struct
{
    GInt32  biSize;
    GInt32  biWidth;
    GInt32  biHeight;
    GInt16  biPlanes;
    GInt16  biBitCount;
    GInt32  biCompression;
    GInt32  biSizeImage;
    GInt32  biXPelsPerMeter;
    GInt32  biYPelsPerMeter;
    GInt32  biClrUsed;
    GInt32  biClrImportant;
}PHONE_AVI_VEDIO_FORMAT;

typedef struct
{
    GInt16	wFormatTag;
    GInt16	nChannels;		//声道数
    GInt32	nSamplesPerSec; //采样率
    GInt32  nAvgBytesPerSec;// for buffer estimation
    GInt16	nBlockAlign;	//数据块的对齐标志
    GInt16	wBitsPerSample; //WAVE声音中每秒的数据量
    GInt16	biSize;			//此结构的大小
}PHONE_AVI_AUDIO_FORMAT;

typedef struct
{
    GInt32 ckid;			//记录数据块中子块的标记
    GInt32 dwFlags;			//表示chid所指子块的属性
    GInt32 dwChunkOffset;	//子块的相对位置
    GInt32 dwChunkLength;	//子块长度
}PHONE_AVI_INDEX;


typedef struct
{
    FILE *m_hAVIFile;
    bool m_foundIFrame;
    bool m_bWriteAVIStreamHeader;
}AVICONVERTINFO;


typedef struct
{
    int nVedioWid;
    int nVedioHei;
    int nVediofps;
}VEDIO_INFO;

class AVIConvert
{
public:
    AVIConvert(const GChar *pAVIFile);
    virtual ~AVIConvert();

    void writeFrame(int type, char *pbuf, int len);
private:
    enum{AVI_IDL, AVI_RUN, AVI_STOP};

public:
    CoverFileCallBack m_coverPosCallback;

    char m_szRecordFile[128];
    int m_nStatus;
    AVICONVERTINFO m_aviinfo;
    int    m_iAviInfoChunkBytes;			//avi信息块总大小
    int    m_iAviDataChunkBytes;			//avi数据块总大小
    int    m_iAviIndexChunkBytes;			//avi索引块总大小
    int    m_iAviTotolChunkBytes;			//avi文件总大小
    int	   m_iAviAudioFrames;				//记录总共多少视频帧,包括I\P帧
    int	   m_iAviVedioFrames;				//记录总共多少音频帧
    int    m_iAviSuggestBufferSize;			//建议缓冲区大小，需容纳最大帧的大小
    int	   m_iAviIndexOffset;				//记录某一帧数据在数据块中的偏移量,从movi开始计算,包括movi
    std::vector<PHONE_AVI_INDEX> m_vectorIndex;	//索引容器
    char  *m_frame_buffer;
public:
    int  start();
    void stop();
    void SetRIFFHeader();
    void SetAVIInfoChunk(int iWid, int iHei, int fps);
    void SetAVIDataChunk();
    void SetAVIIndexChunk();
    void SetAVIJunk();
    void SetAVIHeader(int iWid, int iHei, int fps);
    void SetAVIVedioStreamHeader(int iWid, int iHei, int fps);
    void SetAVIAudioStreamHeader();
    void SetAVIVedioStreamFormat(int iWid, int iHei, int fps);
    void SetAVIAudioStreamFormat();
    void WriteData( char *pbuf, int len );
    void WriteData_P( char *pbuf, int len, int iStreamType, VEDIO_INFO info );
};

#endif // AVICONVERT_H
