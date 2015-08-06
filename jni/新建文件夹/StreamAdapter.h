#ifndef _STREAM_ADAPTER_H_
#define _STREAM_ADAPTER_H_

#include <stdio.h>
#include <jni.h>
#include "./gl_montage_play_lib/montage_play.h"
#include "./gl_montage_play_lib/pwmacro.h"
#include "./gl_montage_play_lib/pwerror.h"
#include <pthread.h>

#define  CALLBACK
typedef void (CALLBACK* RealPlayCodecCallback)(int nSessionId, jint type, void *data, void* user);
typedef void (CALLBACK* PlayBackCodecCallback)(int nSessionId, jint type, void *data, void* user);
typedef enum
{
    Dev_DVR,
    Dev_IPC,
    Dev_NVS,
    Dev_MCE,
    Dev_Decoder
}EnumDevType;

typedef struct _MediaCodec {
    jint width;
    jint height;
    GUInt16 by2FPS;
    GUInt32 SampleRate;
    jbyte spsData[256];
    jbyte ppsData[256];
} MediaCodec;

class StreamAdapter {
public:
    StreamAdapter();
    //	StreamAdapter(VSDevice_Info_t *DeviceInfo);
    ~StreamAdapter();
    void init();
    void setGlPpi(float ppi);
    void setGlPpiXy(float ppix,float ppiy);
    void inputStreamData(const char *buff, const jint length);
    void InitPlayerGL();
    void UninitPlayerGL();
    bool PatternCtrlFlingerDown(GLPOINTINSCREEN *fingerList, int flingerCount, int TickCount = 0);
    PW_PATTERN_OUT* GetPattern(int TickCount = 0, int height = 0, int width = 0);

    void setCameraSessionId(jint SessionId);
    jint getCameraSessionId();
    jint getMontagePlaySourceId();
    void MontagePlayStart();
    void MontagePlayStop();
    void ResetPlayerGl();
private:
    jboolean initMontagePlay();
    PMP_MONTAGE_PLAY* pstMontagePlay;
    PW_PATTERN_OUT m_pattern;
    //当请求新的视频流的时候，保留请求的会话id
    jint CameraSessionId;

    GInt32 lEngineID;
    GInt32 lSourceID;
    //belong to
    jint cameraIndex;
};
#endif
