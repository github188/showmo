#include "StreamAdapter.h"
#include "jniutil.h"
#include <stdlib.h>
#include <string.h>
#include "jniutil.h"
#include<unistd.h>
//#include "./gl_montage_play_lib/avilib.h"
#define MAX_SCHEME_NAME_LEN 256
#define MAX_ID_LENGTH		32
#define MAX_CHANNEL_DEVICE  256
#define SDK_MAX_NAME_LEN		256
#define MAX_RECORD_NAME_LEN 64


#ifdef __cplusplus
extern "C" {
#endif
//static pthread_cond_t thread_cond;

StreamAdapter::StreamAdapter() :CameraSessionId(-1), cameraIndex(-1),lSourceID(-1), lEngineID(-1){
    //	LOGI("%s::StreamAdapter().", __FUNCTION__);
    //pthread_mutex_init(&thread_mutex, NULL);
   // pthread_cond_init(&thread_cond, NULL);
    //RealVideoCB = NULL;
    //RealAudioCB = NULL;
    //PlaybackVideoCB = NULL;
    //PlaybackAudioCB = NULL;
   // audioData = new jbyte[2048];
}

GBool StreamCBFun( const GInt8* pcBuff, GInt32 lBuffSize, PMP_CB_INFO* pstInfo, GUInt64 dw2UserData ){

    return true;
}

GVoid ErrorCBFun(GInt32 lEngineID, MP_ERROR_TYPE eErrType, const GChar *pstrErrorDescrib, GUInt64 dw2UserData) {
    LOGSTART();
    LOGI("%s:%d,%s.", __FUNCTION__, eErrType, pstrErrorDescrib);
    LOGEND();
}


void StreamAdapter::init(){
    LOGSTART();
    initMontagePlay();
    LOGEND();

}
void StreamAdapter::setCameraSessionId(jint SessionId) {
    CameraSessionId = SessionId;
    LOGD("%s:CameraSessionId is %d.", __FUNCTION__, CameraSessionId);
}

jint StreamAdapter::getCameraSessionId() {
    return CameraSessionId;
}

jint StreamAdapter::getMontagePlaySourceId() {
    return lSourceID;
}


void StreamAdapter::inputStreamData(const char *buff, const jint length) {
    LOGSTART();
    //LOGD("%s:input size is %d.lSourceID(%d)", __FUNCTION__, length, lSourceID);
    pstMontagePlay->pw_mp_InputData((const GInt32) lSourceID, (const GInt8 *) buff, (GUInt32) length, "video_h264_single");
    LOGEND();

    //LOGD("%s: pstMontagePlay  %d", __FUNCTION__, pstMontagePlay->pw_mp_InputData((const GInt32) lSourceID, (const GInt8 *) buff, (GUInt32) length, "mix_stream_pw1"));
}
jboolean StreamAdapter::initMontagePlay() {
    LOGSTART();
    pstMontagePlay = new PMP_MONTAGE_PLAY;
    if (pstMontagePlay == NULL) {
        goto ERROR;
    }
    if ( GFalse == pstMontagePlay->pw_mp_Create()) {
        LOGI("%s:pstMontagePlay create fail.", __FUNCTION__);
        goto ERROR;
    }
    pstMontagePlay->pw_mp_SetErrorCallBack(ErrorCBFun, (GUInt64) this);
    lEngineID = pstMontagePlay->pw_mp_GetEngineID();
    if ( PMP_ERR_ID == lEngineID) {
        LOGI("%s:pstMontagePlay get engine id error.", __FUNCTION__);
        goto ERROR;
    }
    lSourceID = pstMontagePlay->pw_mp_AddDataSource("single", "h264&g711a");
    if ( PMP_ERR_ID == lSourceID) {
        LOGI("%s:pstMontagePlay get source id error.", __FUNCTION__);
        goto ERROR;
    }
    pstMontagePlay->pw_mp_SetInfo( lSourceID, "buff_cache_size", "1" );
    if ( GFalse == pstMontagePlay->pw_mp_SetInfo( lSourceID, "steamcb_type", "block" )) {
        LOGI("%s:pstMontagePlay set callback type error.", __FUNCTION__);
        goto ERROR;
    }

    if ( GFalse == pstMontagePlay->pw_mp_SetInfo(lSourceID, "queue_full_method", "real_play")) {
        LOGI("%s:pstMontagePlay set callback type error.", __FUNCTION__);
        goto ERROR;
    }
    if ( GFalse == pstMontagePlay->pw_mp_SetInfo( lSourceID, "audio_decode_parsing", "n" )) {
        LOGI("%s:pstMontagePlay set pw_mp_SetInfo type error.", __FUNCTION__);
        goto ERROR;
    }
    if ( GFalse == pstMontagePlay->pw_mp_SetSteamCallBack( lSourceID, StreamCBFun, NULL )) {
        LOGI("%s:pstMontagePlay set pw_mp_SetSteamCallBack error.", __FUNCTION__);
        goto ERROR;
    }
    pstMontagePlay->pw_mp_Play(lSourceID);
    LOGEND();
    InitPlayerGL();
    return true;
ERROR:
    if (pstMontagePlay != NULL) {
        delete pstMontagePlay;
        pstMontagePlay = NULL;
    }
    return false;
}
void StreamAdapter::setGlPpi(float ppi){
    char value[64]={0};
    sprintf(value,"%f",ppi);
    LOGE("setGlPpi:%s",value);
    pstMontagePlay->pw_mp_SetInfo(lSourceID,"pixel_per_inche",value);
}
void StreamAdapter::setGlPpiXy(float ppix,float ppiy){
    char valuex[64]={0},valuey[64]={0};
    sprintf(valuex,"%f",ppix);
    sprintf(valuey,"%f",ppiy);
    LOGE("setGlPpiXy:%s,%s",valuex,valuey);
    pstMontagePlay->pw_mp_SetInfo(lSourceID,"pixel_per_inche_x",valuex);
    pstMontagePlay->pw_mp_SetInfo(lSourceID,"pixel_per_inche_y",valuey);
}

void StreamAdapter::InitPlayerGL(){
    if(pstMontagePlay != NULL){
        bool bRes=1;
        pstMontagePlay->pw_mpgl_Init();
        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "effect", "lv0" );

//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_type", "8704" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_tilt", "90.0" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_centerx", "640.0" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_centery", "512.0" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_radius", "512.0" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "add_project", "" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "lock_group", "all" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "lock_member", "all" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "set_texture_name", "single" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "pano_info_ready", "y" );
//        bRes &= pstMontagePlay->pw_mpgl_SetInfo( "gl_pattern_play", "" );
    }
}

void StreamAdapter::UninitPlayerGL(){
    if(pstMontagePlay != NULL){
        pstMontagePlay->pw_mpgl_Uninit();
    }
}

bool StreamAdapter::PatternCtrlFlingerDown(GLPOINTINSCREEN *fingerList, int flingerCount, int TickCount)
{
    bool bRes;

    LOGD("%s:---- > flingerList is %d; flingerCount is %d; TickCount is %d ", __FUNCTION__, fingerList, flingerCount, TickCount);
    if (fingerList == NULL) {
        return false;
    }
    if (pstMontagePlay == NULL) {
        return false;
    }
    bRes = pstMontagePlay->pw_mpgl_PatternCtrlFingerDown(fingerList, flingerCount, TickCount);

    if (flingerCount == 2) {
        LOGD("@@@@ PointCount %d: %d, %d, %d, %d, %d, %d, %d, %d ", flingerCount, fingerList[0].ScreenPoint.x, fingerList[0].ScreenPoint.y, fingerList[0].ScreenSize.Wid, fingerList[0].ScreenSize.Hei,
                fingerList[1].ScreenPoint.x, fingerList[1].ScreenPoint.y, fingerList[1].ScreenSize.Wid, fingerList[1].ScreenSize.Hei);
    } else if (flingerCount == 1) {
        LOGD("@@@@ PointCount %d: %d, %d, %d, %d ", flingerCount, fingerList[0].ScreenPoint.x, fingerList[0].ScreenPoint.y, fingerList[0].ScreenSize.Wid, fingerList[0].ScreenSize.Hei);
    } else if (flingerCount == 0)
        LOGD("@@@@ PointCount 0");

    //	LOGD("%s:---- > bRes is %d; ", __FUNCTION__, bRes );
    return bRes;
}


PW_PATTERN_OUT* StreamAdapter::GetPattern(int TickCount, int height, int width){
    int bRes = 0;
    if (pstMontagePlay != NULL) {
        GSIZE screenSize;
        screenSize.Hei = height;
        screenSize.Wid = width;
        bRes = pstMontagePlay->pw_mpgl_GetPatternWithTime(TickCount, &screenSize, &m_pattern);
        LOGD("%s:---- > Group Count is %d", __FUNCTION__, bRes);
        if (bRes)
            return &m_pattern;
        else
            return NULL;
    }
    return NULL;
}


void StreamAdapter::MontagePlayStop()
{
    pstMontagePlay->pw_mpgl_SetInfo("reset_camera", "");
    pstMontagePlay->pw_mpgl_SetInfo("show_single", "n");
    pstMontagePlay->pw_mp_SetInfo(lSourceID, "clear", "all");
    pstMontagePlay->pw_mp_Pause(lSourceID);
    pstMontagePlay->pw_mp_SetInfo(lSourceID, "clear", "pano_info");
    pstMontagePlay->pw_mpgl_SetInfo("pano_info_ready", "n");
}
void StreamAdapter::ResetPlayerGl(){
    pstMontagePlay->pw_mpgl_SetInfo("reset_camera", "");
}

void StreamAdapter::MontagePlayStart()
{
    pstMontagePlay->pw_mpgl_SetInfo("show_single", "y");
}

StreamAdapter::~StreamAdapter() {
    if (pstMontagePlay != NULL) {
        pstMontagePlay->pw_mp_Stop(lSourceID);
        delete pstMontagePlay;
    }
   // delete[] audioData;
    //FormatIsReady = false;
}

#ifdef __cplusplus
}
#endif
