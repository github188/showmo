#include "aviconvert.h"
#include <errno.h>
#include"../jniutil.h"
static unsigned int  last_stream_type = 0xfffffff;            //上一次流类型
static unsigned int  current_stream_type = 0xfffffff;         //当前流类型
static unsigned int  frame_len = 0;     //帧长，不包括私有帧头
static unsigned int  frame_fps = 25;
static unsigned int  frame_height = 0;
static unsigned int  frame_witdh = 0;
static bool bIFrameFound = false;

signed short AVI_A2l[256] =
{
    -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
    -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
    -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
    -3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
    -22016,-20992,-24064,-23040,-17920,-16896,-19968,-18944,
    -30208,-29184,-32256,-31232,-26112,-25088,-28160,-27136,
    -11008,-10496,-12032,-11520, -8960, -8448, -9984, -9472,
    -15104,-14592,-16128,-15616,-13056,-12544,-14080,-13568,
    -344,  -328,  -376,  -360,  -280,  -264,  -312,  -296,
    -472,  -456,  -504,  -488,  -408,  -392,  -440,  -424,
    -88,   -72,  -120,  -104,   -24,    -8,   -56,   -40,
    -216,  -200,  -248,  -232,  -152,  -136,  -184,  -168,
    -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
    -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
    -688,  -656,  -752,  -720,  -560,  -528,  -624,  -592,
    -944,  -912, -1008,  -976,  -816,  -784,  -880,  -848,
    5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
    7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
    2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
    3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
    22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
    30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
    11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
    15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
    344,   328,   376,   360,   280,   264,   312,   296,
    472,   456,   504,   488,   408,   392,   440,   424,
    88,    72,   120,   104,    24,     8,    56,    40,
    216,   200,   248,   232,   152,   136,   184,   168,
    1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
    1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
    688,   656,   752,   720,   560,   528,   624,   592,
    944,   912,  1008,   976,   816,   784,   880,   848,
};


int AVI_g711a_Decode(unsigned char *src, char *dest, int srclen, int *dstlen)
{
    int	i;

    unsigned short *pd=(unsigned short*)dest;

    for(i=0; i<srclen; i++)
    {
        pd[i]=(unsigned short)AVI_A2l[src[i]];
    }

    *dstlen = srclen<<1;

    return 1;
}

AVIConvert::AVIConvert(const GChar *pAVIFile)
{
    m_iAviInfoChunkBytes = 0;
    m_iAviDataChunkBytes = 0;
    m_iAviTotolChunkBytes = 0;
    m_iAviAudioFrames = 0;
    m_iAviVedioFrames = 0;
    m_iAviSuggestBufferSize = 0;
    m_iAviIndexChunkBytes = 0;
    m_iAviIndexOffset = CHUNK_TYPE_BYTES;
    memcpy(m_szRecordFile, pAVIFile, sizeof(m_szRecordFile));
    m_frame_buffer = new char[1024 * 1024];			//存储一帧的大小

    last_stream_type = 0xfffffff;            //上一次流类型
    current_stream_type = 0xfffffff;         //当前流类型
    frame_len = 0;     //帧长，不包括私有帧头
    frame_fps = 25;
    frame_height = 0;
    frame_witdh = 0;
    bIFrameFound = false;
}

AVIConvert::~AVIConvert()
{
    if (m_frame_buffer)
    {
        delete[] m_frame_buffer;
    }
    if (m_aviinfo.m_hAVIFile)
    {
        fclose(m_aviinfo.m_hAVIFile) ;
        m_aviinfo.m_hAVIFile = NULL ;
    }
}

int AVIConvert::start()
{
    m_aviinfo.m_hAVIFile = NULL;
    m_aviinfo.m_foundIFrame = false;
    m_aviinfo.m_bWriteAVIStreamHeader = false;
    m_nStatus = AVI_IDL;

    m_aviinfo.m_hAVIFile =  fopen(m_szRecordFile, "wb+");
    if(m_aviinfo.m_hAVIFile == NULL)
    {
        LogV("JNI", "AVIConvert::start "+errno);
        return -1;
    }
    LogV("JNI", "AVIConvert::start 1");
    m_nStatus = AVI_RUN;

    return 1;
}

void AVIConvert::stop()
{
//    SetAVIIndexChunk();
//    qDebug()<<"idxl num"<<m_iAviIndexChunkBytes/IDXL_BYTES;
//    for(int i=0; i<m_iAviIndexChunkBytes/IDXL_BYTES; i++) {
//        fwrite(&(m_vectorIndex[i]), IDXL_BYTES, 1, m_aviinfo.m_hAVIFile);
//    }

//    //修改部分参数
//    m_iAviDataChunkBytes += CHUNK_TYPE_BYTES/*movi*/;
//    m_iAviTotolChunkBytes = CHUNK_TYPE_BYTES/*AVI*/+CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+m_iAviInfoChunkBytes \
//                                    +CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+m_iAviDataChunkBytes + \
//                                    +CHUNK_ID_BYTES/*idxl*/+CHUNK_SIZE_BYTES/*idxl*/+m_iAviIndexChunkBytes;
//    fseek(m_aviinfo.m_hAVIFile, AVI_RIFF_SIZE_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviTotolChunkBytes), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_MOVI_SIZE_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviDataChunkBytes), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_AVIH_FRAME_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviVedioFrames), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_AVIH_SUGGEST_BUFFER_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviSuggestBufferSize), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_STRH_VFRAME_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviVedioFrames), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_STRH_VSUGGEST_BUFFER_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviSuggestBufferSize), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    fseek(m_aviinfo.m_hAVIFile, AVI_STRH_AFRAME_BYTE_OFFSET, SEEK_SET);
//    fwrite(&(m_iAviAudioFrames), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

//    //int iIndexByteOffset =	CHUNK_ID_BYTES/*RIFF*/+CHUNK_SIZE_BYTES/*RIFF*/+CHUNK_TYPE_BYTES/*AVI*/+ \
//    //						CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+m_iAviInfoChunkBytes+ \
//    //						CHUNK_ID_BYTES/*LIST*/+CHUNK_SIZE_BYTES/*LIST*/+m_iAviDataChunkBytes + \
//    //						CHUNK_ID_BYTES/*idxl*/;
//    //fseek(m_aviinfo.m_hAVIFile, iIndexByteOffset, SEEK_SET);
//    //fwrite(&(m_iAviIndexChunkBytes), CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

    if (m_aviinfo.m_hAVIFile)
    {
        fclose(m_aviinfo.m_hAVIFile);
        m_aviinfo.m_hAVIFile=NULL;
        m_aviinfo.m_bWriteAVIStreamHeader = false;
    }

    m_nStatus = AVI_STOP;
    m_iAviInfoChunkBytes = 0;
    m_iAviDataChunkBytes = 0;
    m_iAviTotolChunkBytes = 0;
    m_iAviAudioFrames = 0;
    m_iAviVedioFrames = 0;
    m_iAviSuggestBufferSize = 0;
    m_iAviIndexChunkBytes = 0;
    m_iAviIndexOffset = 0;
}


void AVIConvert::writeFrame(int type, char *pbuf, int len)
{
    char szIFrame[4] = {'0', '0', 'd', 'b'};
    char szPFrame[4] = {'0', '0', 'd', 'c'};
    char szAFrame[4] = {'0', '1', 'w', 'b'};
    char temp;
    int dstLen = len;
    int offsetlen = len;		//数据实际长度+2字节补齐字节数

    //movi块数据添加
    if (type == FRAME_A) {

        char s_dest[512] = {0};
        AVI_g711a_Decode((unsigned char *)pbuf, s_dest, len, &dstLen);  //注意这里的pbuf一定要转成无符号,否则如果是负数就会有问题
        fwrite(szAFrame, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(&dstLen, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(s_dest, dstLen, 1, m_aviinfo.m_hAVIFile);
        if(len%2) {			//为了2字节补齐
            temp = 0x0;
            fwrite(&temp, 1, 1, m_aviinfo.m_hAVIFile);
            offsetlen = dstLen+1;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+dstLen+1;
        } else{
            offsetlen = dstLen;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+dstLen;
        }
        m_iAviAudioFrames++;

    } else if(type == FRAME_P){

        fwrite(szPFrame, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(&len, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(pbuf, len, 1, m_aviinfo.m_hAVIFile);
        if(len%2) {			//为了2字节补齐
            temp = 0x0;
            fwrite(&temp, 1, 1, m_aviinfo.m_hAVIFile);
            offsetlen = len+1;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+len+1;
        } else{
            offsetlen = len;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+len;
        }

        m_iAviVedioFrames++;

    } else {

        fwrite(szIFrame, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(&len, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
        fwrite(pbuf, len, 1, m_aviinfo.m_hAVIFile);
        if(len%2) {			//为了2字节补齐
            temp = 0x0;
            fwrite(&temp, 1, 1, m_aviinfo.m_hAVIFile);
            offsetlen = len+1;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+len+1;
        } else {
            offsetlen = len;
            m_iAviDataChunkBytes += CHUNK_TYPE_BYTES+CHUNK_SIZE_BYTES+len;
        }
        m_iAviVedioFrames++;

    }
    m_iAviSuggestBufferSize = m_iAviSuggestBufferSize>dstLen?m_iAviSuggestBufferSize:dstLen;

    //idxl块数据存储到VECTOR中
    PHONE_AVI_INDEX index = {0};

    if (type == FRAME_A) {
        index.ckid = PHONE_MAKEFOURCC('0', '1', 'w', 'b');
        index.dwFlags = AVIIF_KEYFRAME;
    } else if(type == FRAME_P){
        index.ckid = PHONE_MAKEFOURCC('0', '0', 'd', 'c');
        index.dwFlags = 0;
    } else {
        index.ckid = PHONE_MAKEFOURCC('0', '0', 'd', 'b');
        index.dwFlags = AVIIF_KEYFRAME;
    }
    index.dwChunkOffset = m_iAviIndexOffset;
    index.dwChunkLength = dstLen;
    m_iAviIndexOffset += offsetlen + CHUNK_ID_BYTES/*ckid*/ + CHUNK_SIZE_BYTES/*dwChunkLength*/;
    m_iAviIndexChunkBytes += IDXL_BYTES;
    m_vectorIndex.push_back(index);

    fflush(m_aviinfo.m_hAVIFile);

}

void AVIConvert::WriteData_P( char *pbuf, int len, int iStreamType, VEDIO_INFO info )
{
    if(iStreamType != FRAME_I && !bIFrameFound) {
        return;
    }

    if(info.nVedioHei==0 || info.nVedioWid == 0)
        return;

    if(iStreamType == FRAME_I) {
        bIFrameFound = true;

        if(!m_aviinfo.m_bWriteAVIStreamHeader) {
            frame_fps = 12;
            frame_witdh = info.nVedioWid;
            frame_height = info.nVedioHei;

            SetRIFFHeader();
            SetAVIInfoChunk(frame_witdh, frame_height, frame_fps);
            SetAVIDataChunk();
            m_aviinfo.m_bWriteAVIStreamHeader = true;
            fflush(m_aviinfo.m_hAVIFile);
        }
    }

    switch(iStreamType)
    {
    case FRAME_P:
        writeFrame(FRAME_P,  pbuf, len);
        break;
    case FRAME_I:
        writeFrame(FRAME_I,  pbuf, len);
        break;
    case FRAME_A:
        writeFrame(FRAME_A,  pbuf, len);
        break;
    default:
        break;
    }
}

//找到下一帧才将上一帧的数据保存，如果每次都保存当前数据包的话，生成的文件可能存在最后一帧不完整的情况
void AVIConvert::WriteData( char *pbuf, int len )
{
    unsigned int  iSrcBufIndex = 0;
    while (iSrcBufIndex != len)
    {
        //如果数据接收不流畅，出现丢包的情况，那么可能存在接收数据溢出缓冲区的情况，需要重置
        if(frame_len>=1024*1024) {
            last_stream_type = 0xfffffff;            //上一次流类型
            current_stream_type = 0xfffffff;         //当前流类型
            frame_fps = 25;
            frame_height = 0;
            frame_witdh = 0;
            bIFrameFound = false;
            frame_len = 0;
            iSrcBufIndex = 0;
        }

        //保证第一帧写入的是I帧
        if(bIFrameFound) {
            m_frame_buffer[frame_len] = pbuf[iSrcBufIndex];
            frame_len++;
        }

        current_stream_type = ((current_stream_type << 8)+((unsigned char*)pbuf)[iSrcBufIndex]);    //注意这里的pbuf一定要转成无符号,否则如果是负数就会有问题
        iSrcBufIndex++;
        if (current_stream_type == 0x01FA || current_stream_type == 0x01FB || current_stream_type == 0x01F0
                ||current_stream_type == 0x01FC || current_stream_type == 0x01FD || current_stream_type == 0x01FE)
        {
            if (bIFrameFound == true)
            {
                frame_len = frame_len - 4;
                switch(last_stream_type)        //检测到的是当前的流类型，写的数据是上一次的流数据，所以要通过上一次流类型来进行判断
                {
                case FRAME_P:
                    writeFrame(FRAME_P,  m_frame_buffer, frame_len);
                    break;
                case FRAME_I:
                    writeFrame(FRAME_I,  m_frame_buffer, frame_len);
                    break;
                case FRAME_A:
                    writeFrame(FRAME_A,  m_frame_buffer, frame_len);
                    break;
                default:
                    break;
                }
            }

            switch(current_stream_type)
            {
            case FRAME_P:
            case FRAME_A:
            {
                iSrcBufIndex += 4;          //pbuf中往后移四个字节
                frame_len = 0;
                last_stream_type = current_stream_type;
            }
                break;
            case FRAME_I:
            {
                bIFrameFound = true;
                last_stream_type = current_stream_type;
                frame_len = 0;

                frame_fps = pbuf[iSrcBufIndex+1] & 0x1F;
                frame_witdh = (pbuf[iSrcBufIndex+2] & 0xFF) * 8;
                frame_height = (pbuf[iSrcBufIndex+3] & 0xFF) * 8;
                iSrcBufIndex += 4;          //读取frame_fps、frame_witdh、frame_height
                iSrcBufIndex += 8;
                if(!m_aviinfo.m_bWriteAVIStreamHeader) {
                    SetRIFFHeader();
                    SetAVIInfoChunk(frame_witdh, frame_height, frame_fps);
                    SetAVIDataChunk();
                    m_aviinfo.m_bWriteAVIStreamHeader = true;
                    fflush(m_aviinfo.m_hAVIFile);
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

//RIFF
void AVIConvert::SetRIFFHeader()
{
    char szRiff[4] = {'R', 'I', 'F', 'F'};
    char szSize[4] = "";
    char szType[4]  = {'A', 'V', 'I', ' '};

    fwrite(szRiff, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
}

//信息块
void AVIConvert::SetAVIInfoChunk(int iWid, int iHei, int fps)
{
    char szList[4] = {'L', 'I', 'S', 'T'};
    char szType[4]  = {'h', 'd', 'r', 'l'};

    //头信息+vedio信息+audio信息
    m_iAviInfoChunkBytes = CHUNK_TYPE_BYTES/*hdrl*/+CHUNK_ID_BYTES/*avih*/+CHUNK_SIZE_BYTES/*avih*/+AVIH_BYTES \
                            +CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/ \
                            +CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_VEDIO_BYTES \
                            +CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_VEDIO_BYTES \
                            +CHUNK_ID_BYTES/*list*/+CHUNK_SIZE_BYTES/*list*/+CHUNK_TYPE_BYTES/*strl*/\
                            +CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_AUDIO_BYTES \
                            +CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_AUDIO_BYTES ;

    fwrite(szList, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&m_iAviInfoChunkBytes, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);

    SetAVIHeader(iWid, iHei, fps);
}

//数据块
void AVIConvert::SetAVIDataChunk()
{
    char szList[4] = {'L', 'I', 'S', 'T'};
    char szType[4]  = {'m', 'o', 'v', 'i'};
    int iSize = 0;

    fwrite(szList, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
}

//索引块
void AVIConvert::SetAVIIndexChunk()
{
    char szIdxl[4] = {'i', 'd', 'x', '1'};

    fwrite(szIdxl, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&m_iAviIndexChunkBytes, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
}

//设置avih
void AVIConvert::SetAVIHeader(int iWid, int iHei, int fps)
{
    //avi header
    char szavi[4] = {'a', 'v', 'i', 'h'};
    int iSize = AVIH_BYTES;
    fwrite(szavi, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    PHONE_AVIHEADER aviHeader;
    memset(&aviHeader, 0, AVIH_BYTES);
    aviHeader.dwPaddingGranularity = 2048;
    aviHeader.dwStreams = 2;
    aviHeader.dwWidth = iWid;
    aviHeader.dwHeight = iHei;
    aviHeader.dwMicroSecPerFrame =(1*1000*1000)/fps;	//计算总时间
    aviHeader.dwFlages = AVIF_TRUSTCKTYPE|AVIF_HASINDEX;
    fwrite(&aviHeader, AVIH_BYTES, 1, m_aviinfo.m_hAVIFile);

    char szList[4] = {'L', 'I', 'S', 'T'};
    char szType[4]  = {'s', 't', 'r', 'l'};
    iSize = CHUNK_TYPE_BYTES/*strl*/+CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_VEDIO_BYTES \
        +CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_VEDIO_BYTES;
    //vedio
    fwrite(szList, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    SetAVIVedioStreamHeader(iWid, iHei, fps);
    SetAVIVedioStreamFormat(iWid, iHei, fps);

    //audio
    iSize = CHUNK_TYPE_BYTES/*strl*/+CHUNK_ID_BYTES/*strh*/+CHUNK_SIZE_BYTES/*strh*/+STRH_AUDIO_BYTES \
        +CHUNK_ID_BYTES/*strf*/+CHUNK_SIZE_BYTES/*strf*/+STRF_AUDIO_BYTES;
    fwrite(szList, CHUNK_ID_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    SetAVIAudioStreamHeader();
    SetAVIAudioStreamFormat();
}

//设置vedio strh
void AVIConvert::SetAVIVedioStreamHeader(int iWid, int iHei, int fps)
{
    char szType[4]  = {'s', 't', 'r', 'h'};
    int iSize = STRH_VEDIO_BYTES;
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

    PHONE_AVI_STREAM_HEADER aviVideoStreamInfo;
    memset(&aviVideoStreamInfo, 0, STRH_VEDIO_BYTES);
    aviVideoStreamInfo.fccType			= PHONE_MAKEFOURCC('v', 'i', 'd', 's');
    aviVideoStreamInfo.fccHandler		= PHONE_MAKEFOURCC('H','2','6','4');
    aviVideoStreamInfo.dwFlags			= 0;	 //数据流属性
    aviVideoStreamInfo.wPriority		= 0;	 //数据流播放优先级
    aviVideoStreamInfo.wLanguage		= 0;	 //音频的语言代号
    aviVideoStreamInfo.dwScale			= 1;	 //视频每帧的大小或者音频的采样大小
    aviVideoStreamInfo.dwRate			= fps;	 //每秒的采样数，5帧
    aviVideoStreamInfo.dwQuality		= -1;	 //表示解压缩质量参数，值越大质量越好
    aviVideoStreamInfo.dwStart			= 0;	 //数据流开始播放的位置
    aviVideoStreamInfo.dwInitialFrames	= 0;	 //在播放前需要多少帧
    aviVideoStreamInfo.dwSampleSize		= 0;	 //音频的采样大小
    aviVideoStreamInfo.rcFrame.left		= 0;
    aviVideoStreamInfo.rcFrame.top		= 0;
    aviVideoStreamInfo.rcFrame.right	= iWid;
    aviVideoStreamInfo.rcFrame.bottom	= iHei;
    aviVideoStreamInfo.dwSuggestedBufferSize= 0;
    aviVideoStreamInfo.dwLength = 0;

    fwrite(&aviVideoStreamInfo, STRH_VEDIO_BYTES, 1, m_aviinfo.m_hAVIFile);
}

//设置vedio strf
void AVIConvert::SetAVIVedioStreamFormat(int iWid, int iHei, int fps)
{
    char szType[4]  = {'s', 't', 'r', 'f'};
    int iSize = STRF_VEDIO_BYTES;
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

    PHONE_AVI_VEDIO_FORMAT format;
    memset(&format, 0, STRF_VEDIO_BYTES);
    format.biSize	 = sizeof(PHONE_AVI_VEDIO_FORMAT);
    format.biWidth	 = iWid;
    format.biHeight	 = iHei;
    format.biPlanes	 = 1;
    format.biBitCount	 = 24;
    format.biCompression = PHONE_MAKEFOURCC('H','2','6','4');
    format.biSizeImage	 = iWid*iHei*3;
    format.biXPelsPerMeter	= 0;
    format.biYPelsPerMeter	= 0;
    format.biClrUsed		= 0;
    format.biClrImportant	= 0;

    fwrite(&format, STRF_VEDIO_BYTES, 1, m_aviinfo.m_hAVIFile);
}


//设置audio strh
void AVIConvert::SetAVIAudioStreamHeader()
{
    char szType[4]  = {'s', 't', 'r', 'h'};
    int iSize = STRH_AUDIO_BYTES;
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

    PHONE_AVI_STREAM_HEADER aviAudioStreamInfo;
    memset(&aviAudioStreamInfo, 0, STRH_AUDIO_BYTES);
    aviAudioStreamInfo.fccType	 = PHONE_MAKEFOURCC('a', 'u', 'd', 's');
    aviAudioStreamInfo.fccHandler= PHONE_WAVE_FORMAT_PCM;
    aviAudioStreamInfo.dwFlags	 = 0;
    aviAudioStreamInfo.wPriority = 0;
    aviAudioStreamInfo.wLanguage = 0;
    aviAudioStreamInfo.dwScale	 = 1;			//声道数
    aviAudioStreamInfo.dwRate	 = 8000;		//Hz
    aviAudioStreamInfo.dwStart	 = 0;
    aviAudioStreamInfo.dwInitialFrames	= 0;
    aviAudioStreamInfo.dwSuggestedBufferSize = 320;
    aviAudioStreamInfo.dwQuality	 = 0;
    aviAudioStreamInfo.dwSampleSize	 = 1;
    aviAudioStreamInfo.rcFrame.bottom= 0;
    aviAudioStreamInfo.rcFrame.left	 = 0;
    aviAudioStreamInfo.rcFrame.right = 0;
    aviAudioStreamInfo.rcFrame.top	 = 0;
    aviAudioStreamInfo.dwLength	 = 0;

    fwrite(&aviAudioStreamInfo, STRH_AUDIO_BYTES, 1, m_aviinfo.m_hAVIFile);
}

//设置audio strf
void AVIConvert::SetAVIAudioStreamFormat()
{
    char szType[4]  = {'s', 't', 'r', 'f'};
    int iSize = STRF_AUDIO_BYTES;
    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);

    //设置音频帧格式
    PHONE_AVI_AUDIO_FORMAT wave_format;
    wave_format.wFormatTag      = PHONE_WAVE_FORMAT_PCM;
    wave_format.nChannels       = 1;
    wave_format.nSamplesPerSec  = 8000;
    wave_format.wBitsPerSample  = 16;
    wave_format.nBlockAlign     = (wave_format.wBitsPerSample / 8) * wave_format.nChannels;
    wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;

    fwrite(&wave_format, STRF_AUDIO_BYTES, 1, m_aviinfo.m_hAVIFile);
}

void AVIConvert::SetAVIJunk()
{
//    char szType[4]  = {'J', 'U', 'N', 'K'};
//    /* Calculate the needed amount of junk bytes, output junk */
//    int njunk = HEADERBYTES - nhb - 8 - 12;

//    fwrite(szType, CHUNK_TYPE_BYTES, 1, m_aviinfo.m_hAVIFile);
//    fwrite(&iSize, CHUNK_SIZE_BYTES, 1, m_aviinfo.m_hAVIFile);
//    fwrite(&wave_format, STRF_AUDIO_BYTES, 1, m_aviinfo.m_hAVIFile);
}

