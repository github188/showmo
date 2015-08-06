
#include "ipc365_app_showmo_jni_JniClient.h"
#include"jniutil.h"
#include"jniwraper.h"
#include"./pwnetsdk/pw_net_sdk.h"
#include"./pwpriprotocol/priprotocol.h"
#include"h264parse/h264parse.h"
#include"avi/aviconvert.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"jniHelper/JniObject.h"
#include<mutex>

#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

#define PW_PHONE_BL_SHOWMO_FINDFILE_MAXCOUNT 16
#define CODEC_H264_NAL_SPS 0x67
#define CODEC_H264_NAL_PPS 0x68
#define CODEC_H264_NAL_SEI 0x06
#define CODEC_H264_NAL_I   0x65
#include <stdarg.h>
#include"StreamAdapter.h"
StreamAdapter realplayAdapter;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct    // 远程回放结束  HIST_VIDEO_CAMERA_ACK_MSG
{
    uint32 camera_id;
    uint32 client_id;
    uint32 custom_id;
    uint32 device_id;
    uint32 id;
    uint32 option;
    uint32 value1;
    uint32 value2;
}PW_REMOTE_MESSAGE;

void printErr(char* msg,int flag=0);

jmethodID ID_RealDatacallback=NULL;
jobject obj_RealDatacallback=NULL;
JavaVM *realDatajvm=NULL;

jmethodID ID_DebugCallback=NULL;
jobject obj_DebugCallback=NULL;
JavaVM *debugjvm=NULL;
char debugpath[124]={0};
std::mutex debugMutex;

jboolean m_bRecord = false;
AVIConvert *m_aviConvert=NULL;
std::mutex m_aviConvertMutex;
jboolean m_bFoundIFrame=false;
VEDIO_INFO m_VedioInfo;

bool getH264Nal(char *pBuf, int nSize, LPGVM_H264NALINFO pSPS, LPGVM_H264NALINFO pPPS)
{
    int nSPSIndex = -1, nPPSIndex = -1, nSEIIndex = -1;
    GLong i=0, lCheckLen;

    lCheckLen = (nSize-5)<200?(nSize-5):200;

    GUInt32 lTemp, lVal, lTT;
    for (; i<lCheckLen; i++)
    {
        lTemp = pBuf[i];	lVal = lTemp;
        lTemp = pBuf[i+1];	lVal += lTemp<<8;
        lTemp = pBuf[i+2];	lVal += lTemp<<16;
        lTemp = pBuf[i+3];	lVal += lTemp<<24;
        lTT = pBuf[i+4];

        if(lVal==0x01000000 && CODEC_H264_NAL_SPS == lTT)
        {
            nSPSIndex = i;
        }

        if(lVal==0x01000000 && CODEC_H264_NAL_PPS == lTT)
        {
            nPPSIndex = i;
        }

        if(lVal==0x01000000 && CODEC_H264_NAL_SEI == lTT)   //第二个SEI会把第一个的索引信息覆盖
        {
            nSEIIndex = i;
        }
    }

    //    char log[256] = {0};
    //    sprintf(log, "getH264Nal lCheckLen:%d,nSPSIndex:%d,nPPSIndex:%d,nSEIIndex:%d", lCheckLen,nSPSIndex,nPPSIndex,nSEIIndex);
    //    LogV("JNI", log);
    if(nSPSIndex == -1 || nPPSIndex == -1 || nSEIIndex == -1)
        return false;

    pSPS->pNalStart = (unsigned char *)pBuf+nSPSIndex;
    pSPS->dwNalType = CODEC_H264_NAL_SPS;
    pSPS->dwNalLength = nPPSIndex-nSPSIndex;
    pPPS->pNalStart = (unsigned char *)pBuf+nPPSIndex;
    pPPS->dwNalType = CODEC_H264_NAL_PPS;
    pPPS->dwNalLength = nSEIIndex-nPPSIndex;
    return true;
}

int  jni_RealDataCallBack(char *pBuffer,long lStreamType,long lFrameNum,long lbufsize,long dwUser){
    JNIEnv * env;
    realplayAdapter.inputStreamData(pBuffer,lbufsize);
    realDatajvm->AttachCurrentThread(&env,NULL);
    //char msg[150];
    // sprintf(msg,"jni_RealDataCallBack :%d,%d,%d,%d",lStreamType,lFrameNum,lbufsize,dwUser);
    //LogV(JNITAG,msg);
    int iRet=1;
    if(!m_bFoundIFrame && lStreamType==0) {
        GVM_H264NALINFO sps={0},pps={0};
        if(getH264Nal(pBuffer, lbufsize, &sps, &pps)) {
            GVM_H264SPSINFO stH264Info = {0};
            if(sps.pNalStart==NULL || sps.dwNalLength==0)
                return NULL;
            H264ParseSps( sps.pNalStart+5, sizeof(sps.dwNalLength-5), &stH264Info );
            m_VedioInfo.nVedioWid = stH264Info.dwFrameWidth;
            m_VedioInfo.nVedioHei = stH264Info.dwFrameHeight;
            m_bFoundIFrame = true;
        }
    }
    if(m_bRecord && m_bFoundIFrame) {
        if(m_aviConvertMutex.try_lock()) {
            if(m_aviConvert) {
                m_aviConvert->WriteData_P(pBuffer, lbufsize, lStreamType, m_VedioInfo);
            }
            m_aviConvertMutex.unlock();
        }
    }

    if(ID_RealDatacallback!=NULL){
        jbyteArray jbyteArr=env->NewByteArray(lbufsize);
        env->SetByteArrayRegion(jbyteArr,0,lbufsize,(jbyte*)pBuffer);

        if(lStreamType!=3){
            free(pBuffer);
        }


        jlong arg1=lStreamType;
        jlong arg2=lFrameNum;
        jlong arg3=lbufsize;
        jlong arg4=100;
        // LOGE("jni RealData type: %d  size %d",lStreamType,lbufsize);
        iRet=env->CallIntMethod(obj_RealDatacallback,ID_RealDatacallback,jbyteArr,arg1,arg2,arg3,arg4);
        env->DeleteLocalRef(jbyteArr);
    }
    realDatajvm->DetachCurrentThread();
    return iRet;
}



void  cpperrordatacallback(char *pBuffer){
    JNIEnv * env=NULL;
    int iret=debugjvm->GetEnv((void**)(&env),JNI_VERSION_1_4);
    int attachRet=0;
    if(iret<0){
        //LOGE("debugjvm AttachCurrentThread");
        attachRet= debugjvm->AttachCurrentThread(&env,NULL);
    }
    if(env!=NULL){
        jstring jstrmsg=env->NewStringUTF(pBuffer);
        env->CallVoidMethod(obj_DebugCallback,ID_DebugCallback,jstrmsg);
        env->DeleteLocalRef(jstrmsg);
    }
    if(iret<0){
        //LOGE("debugjvm DetachCurrentThread");
        debugjvm->DetachCurrentThread();
    }

    //LOGE("debugjvm->GetEnv ret:%d,%d",iret,(int)env);
}

jmethodID ID_PlaybackDatacallback=NULL;
jobject obj_PlaybackDatacallback=NULL;
JavaVM *playbackDatajvm=NULL;

int  jni_PlaybackCallBack(char *pBuffer,long lStreamType,long lFrameNum,long lbufsize,long dwUser){
    JNIEnv * env;
    realplayAdapter.inputStreamData(pBuffer,lbufsize);
    playbackDatajvm->AttachCurrentThread(&env,NULL);
    //char msg[150];
    //sprintf(msg,"jni_PlaybackCallBack :%d,%d,%d,%d",lStreamType,lFrameNum,lbufsize,dwUser);
    // LogV(JNITAG,msg);
    int iRet=1;
    if(ID_PlaybackDatacallback!=NULL){
        jbyteArray jbyteArr=env->NewByteArray(lbufsize);
        env->SetByteArrayRegion(jbyteArr,0,lbufsize,(jbyte*)pBuffer);

        //free(pBuffer);
        jlong arg1=lStreamType;
        jlong arg2=lFrameNum;
        jlong arg3=lbufsize;
        jlong arg4=100;
        iRet=env->CallIntMethod(obj_PlaybackDatacallback,ID_PlaybackDatacallback,jbyteArr,arg1,arg2,arg3,arg4);
        env->DeleteLocalRef((jobject)jbyteArr);
    }

    playbackDatajvm->DetachCurrentThread();
    return iRet;
}

jmethodID ID_MsgDatacallback=NULL;
jobject obj_MsgDatacallback=NULL;
JavaVM *MsgDatajvm=NULL;

jstring AlarmImagePath=NULL;
int RecordId=-1;
jclass glb_class_msg_cb_wraper=NULL;

jobject glb_object_cb_wraper=NULL;
jmethodID glb_cb_mid=NULL;
jobject glb_cb_object_map=NULL;
jmethodID glb_mid_cb_map_new_class=NULL;
jmethodID glb_mid_cb_map_add_field=NULL;

/*
 * 将field和其值转换成键值对，值将以String形式存储,小于40的字符串可以用这个函数添加，大于的用其他函数
 * */
void jniGlbCbMapAddField(JNIEnv* env,char* fieldname,char* fieldFormat,...){
    jstring jsFieldName=env->NewStringUTF(fieldname);
    va_list arglist;
    va_start(arglist,fieldFormat);
    char fieldValue[128]={0};
    vsprintf(fieldValue,fieldFormat,arglist);
    va_end(arglist);
    jstring jsFieldValue=env->NewStringUTF(fieldValue);
    //free(fieldValue);
    env->CallVoidMethod(glb_cb_object_map,glb_mid_cb_map_add_field,jsFieldName,jsFieldValue);
}
void jniGlbCbMapAddStringField(JNIEnv* env,char* fieldname,jstring valuejstr){
    jstring jsFieldName=env->NewStringUTF(fieldname);
    env->CallVoidMethod(glb_cb_object_map,glb_mid_cb_map_add_field,jsFieldName,valuejstr);
}
void jniGlbCbMapAddCharField(JNIEnv* env,char* fieldname,char* valuecstr){
    jstring jsFieldName=env->NewStringUTF(fieldname);
    jstring jsFieldValue=env->NewStringUTF(valuecstr);
    env->CallVoidMethod(glb_cb_object_map,glb_mid_cb_map_add_field,jsFieldName,jsFieldValue);
}

void jniGlbCbMapNewClass(JNIEnv* env,char* classname){
    jstring className=env->NewStringUTF(classname);
    env->CallVoidMethod(glb_cb_object_map,glb_mid_cb_map_new_class,className);
}

void  jni_msgDataCallBack(char *pBuffer,long lmsgid,long lexternid,long lbufsize,long dwUser){
    JNIEnv* env;
    MsgDatajvm->AttachCurrentThread(&env,NULL);
    jobject objMsgOut=NULL;
    //LOGW("jni_msgDataCallBack:%d,%d",lmsgid,lbufsize);
    if(glb_cb_object_map!=NULL ){
        switch (lmsgid) {
        case UPDATE_MOBILE_INVITE_INFO:
        {
            update_2_mobile_invite *m_inviteInfo=(update_2_mobile_invite *)pBuffer;
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$Update_2_mobile_invite");
            jniGlbCbMapAddField(env,"deviceId","%d",(int)m_inviteInfo->device_id);
            jniGlbCbMapAddField(env,"customId","%d",(int)m_inviteInfo->custom_id);
            jniGlbCbMapAddCharField(env,"softwareVersion",m_inviteInfo->software_version);
            jniGlbCbMapAddField(env,"reserver1","%d",(int)m_inviteInfo->reserver1);
            jniGlbCbMapAddField(env,"reserver2","%d",(int)m_inviteInfo->reserver2);

        }
            break;
        case HIST_VIDEO_CAMERA_ACK_MSG:
        {
            LOGW("get playback complete msg");
            PW_REMOTE_MESSAGE *m_remote_msg=(PW_REMOTE_MESSAGE *)pBuffer;
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$Remote_Message");
            jniGlbCbMapAddField(env,"cameraId","%d",(int)m_remote_msg->camera_id);
            jniGlbCbMapAddField(env,"clientId","%d",(int)m_remote_msg->client_id);
            jniGlbCbMapAddField(env,"customId","%d",(int)m_remote_msg->custom_id);
            jniGlbCbMapAddField(env,"deviceId","%d",(int)m_remote_msg->device_id);
            jniGlbCbMapAddField(env,"id","%d",(int)m_remote_msg->id);
            jniGlbCbMapAddField(env,"option","%d",(int)m_remote_msg->option);
            jniGlbCbMapAddField(env,"value1","%d",(int)m_remote_msg->value1);
            jniGlbCbMapAddField(env,"value2","%d",(int)m_remote_msg->value2);

        }
            break;
        case CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG:
        {

            device_alarm_server_upload_msg *alarm=(device_alarm_server_upload_msg *)pBuffer;
            // LOGW("CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG:%d",alarm->device_id);
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$Device_alarm_server_upload_msg");
            jniGlbCbMapAddField(env,"clientId","%d",(int)alarm->client_id);
            jniGlbCbMapAddField(env,"recordId","%d",(int)alarm->record_id);
            jniGlbCbMapAddField(env,"deviceId","%d",(int)alarm->device_id);
            jniGlbCbMapAddField(env,"cameraId","%d",(int)alarm->camera_id);
            jniGlbCbMapAddField(env,"channelNo","%d",(int)alarm->tf_state);
            jniGlbCbMapAddField(env,"alarmType","%d",(int)alarm->alarm_type);
            jniGlbCbMapAddField(env,"beginTime","%d",(long)alarm->begin_time);
            jniGlbCbMapAddField(env,"endTime","%d",(long)alarm->end_time);
            jniGlbCbMapAddField(env,"alarmMode","%d",(int)alarm->alarm_mode);
            jniGlbCbMapAddField(env,"alarmCode","%d",(int)alarm->alarm_code);
            jniGlbCbMapAddField(env,"Ccid","%d",(int)alarm->ccid);

        }
            break;

        case CLIENT_MGR_CAMERA_OFFLINE_MSG:
        case CLIENT_MGR_CAMERA_ONLINE_MSG:
        {
            //LOGW("get CLIENT_MGR_CAMERA_ONLINE state ");
            user_2_mgr_disconn_device * devDisconnect=(user_2_mgr_disconn_device *)pBuffer;
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$User_2_mgr_disconn_device");
            jniGlbCbMapAddField(env,"device_id","%d",(int)devDisconnect->user_id);
            jniGlbCbMapAddField(env,"user_id","%d",(int)devDisconnect->device_id);
        }
            break;
        case PLAY_STOP:
        case MGR_OUTLINE:
            jniGlbCbMapNewClass(env,"null");

            break;
        case  DOWNLOAD_PIC_POS:		//下载图片进度
            //LOGW("DOWNLOAD_PIC_POS recordId:%d  lbufsize:%d",RecordId,lbufsize);
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$alarm_data_download_progress");
            jniGlbCbMapAddField(env,"recordId","%d",RecordId);
            jniGlbCbMapAddField(env,"pos","%d",lbufsize);
            break;
        case  DOWNLOAD_PIC_FAILED:		//下载图片失败
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$alarm_data_download");
            jniGlbCbMapAddField(env,"recordId","%d",RecordId);
            jniGlbCbMapAddField(env,"state","%d",0);

            break;
        case UPDATE_MOBILE_INVITE_ACK:
        {
            //UPDATE_MOBILE_INVITE_ACK callback the upgrade ack,
            //cmd equals UPDATE_DOWNPOS means downloadpos, UPDATE_FAILED ...
            SDK_CAMERA_UPDATE *m_updateAck=(SDK_CAMERA_UPDATE *)pBuffer;
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$SDK_CAMERA_UPDATE");
            jniGlbCbMapAddField(env,"cameraid","%d",(int)m_updateAck->cameraid);
            jniGlbCbMapAddField(env,"downpos","%d",(int)m_updateAck->downpos);
            jniGlbCbMapAddField(env,"cmd","%d",(int)m_updateAck->cmd);
            jniGlbCbMapAddField(env,"errcode","%d",(int)m_updateAck->errcode);
        }
            break;
        case DOWNLOAD_PIC_SUCCESS:
        {
            if(AlarmImagePath==NULL || RecordId <0){
                return;
            }
            const char* imgPath=env->GetStringUTFChars(AlarmImagePath,NULL);
            int filenameLen=strlen(imgPath)+30;
            char *imgFileName=(char*)malloc(sizeof(char)*filenameLen);
            memset(imgFileName,0,sizeof(char)*filenameLen);
            sprintf(imgFileName,"%s/%d%s",imgPath,RecordId,".jpg");
            LOGW("AlarmimgFileName:%s,bufsize %ld",imgFileName,(int)lbufsize);
            env->ReleaseStringUTFChars(AlarmImagePath,imgPath);
            env->DeleteGlobalRef(AlarmImagePath);
            AlarmImagePath=NULL;
            FILE*file=fopen(imgFileName,"ab+");

            if(file==NULL){
                LOGE("AlarmimgFileName:%s open err");
                return;
            }

            fwrite(pBuffer,lbufsize,1,file);

            fclose(file);
            jniGlbCbMapNewClass(env,"ipc365.app.showmo.jni.JniDataDef$alarm_data_download");
            jniGlbCbMapAddCharField(env,"alarmImgFilename",imgFileName);
            jniGlbCbMapAddField(env,"recordId","%d",RecordId);
            jniGlbCbMapAddField(env,"state","%d",1);
            free(imgFileName);
            RecordId=-1;
        }
            break;
        default:
            break;
        }
        env->CallVoidMethod(glb_object_cb_wraper,glb_cb_mid,glb_cb_object_map,lmsgid);
    }
    MsgDatajvm->DetachCurrentThread();

}



void printErr(char* msg,int flag){//flag 0 netsdk错误 1 私有协议错误
    char errmsg[215]={0};
    unsigned long err=0;
    if(flag==0){
        PW_NET_GetLastError(&err);
    }else{
        err=PW_PRI_GetLastError();
    }
    sprintf(errmsg,"error:%d In :",err);
    strcat(errmsg,msg);
    LOGE("%s",errmsg);
}




/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    pw_jni_call_test
 * Signature: ()I
 */
//void callTest2(char* dd,...){
//    va_list args;
//    va_start(args,dd);
//    int value1=va_arg(args,int);
//    int value2=va_arg(args,int);
//    va_end(args);
//    char *values=(char*)malloc(sizeof(char)*40);
//    memset(values,0,sizeof(char)*40);
//    sprintf(values,"callTest2:%d%d",__VA_ARGS__);
//    LogV(JNITAG,values);
//}

int callTest(JNIEnv *env,jobject obj,jmethodID mid,...){
    //    va_list vl;
    //    va_start(vl,mid);

    //    int value1=va_arg(vl,int);
    //    int value2=va_arg(vl,int);
    //    char *values=(char*)malloc(sizeof(char)*40);
    //    memset(values,0,sizeof(char)*40);
    //   // vsprintf(values,"%d%d",vl);
    //    sprintf(values,"%d%d",value1,value2);
    //    LogV(JNITAG,values);
    //   int res=env->CallIntMethod(obj,mid,__VA_ARGS__);
    //   callTest2("dsf",value1,value2);
    //  va_end(vl);
    char msg[100];
    //  sprintf(msg,"i get a int from testclass :%d",res);
    LogV(JNITAG,msg);
}

JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_pw_1jni_1call_1test
(JNIEnv *env, jclass){
    LogV(JNITAG,"Java_ipc365_app_showmo_jni_JniClient_pw_1jni_1call_1test");
    //jclass clazz;//=env->FindClass("ipc365/app/showmo/jni/JniDataDef$User_2_mgr_disconn_device");
    //jmethodID mid=env->GetMethodID(clazz,"<init>","()V");
    //jobject jobj=jniNewInitObject(env,"ipc365/app/showmo/jni/JniDataDef$User_2_mgr_disconn_device",&clazz);
    //jniSetIntField(env,clazz,jobj,"device_id",78787);
    //jniSetIntField(env,clazz,jobj,"user_id",1543523);
    jclass clazz;
    jobject obj=jniNewInitObject(env,"ipc365/app/showmo/jni/JniDataDef$TestClass",&clazz);
    jmethodID mid=env->GetMethodID(clazz,"func","(II)I");

    //env->CallIntMethod(obj,mid,10,12);
    callTest(env,obj,mid,10,12);

    return NULL;
}


//bool getH264Nal(char *pBuf, int nSize, LPGVM_H264NALINFO pSPS, LPGVM_H264NALINFO pPPS)
//{
//    int nSPSIndex = -1, nPPSIndex = -1, nSEIIndex = -1;
//    GLong i=0, lCheckLen;

//    lCheckLen = (nSize-5)<200?(nSize-5):200;

//    GUInt32 lTemp, lVal, lTT;
//    for (; i<lCheckLen; i++)
//    {
//        lTemp = pBuf[i];	lVal = lTemp;
//        lTemp = pBuf[i+1];	lVal += lTemp<<8;
//        lTemp = pBuf[i+2];	lVal += lTemp<<16;
//        lTemp = pBuf[i+3];	lVal += lTemp<<24;
//        lTT = pBuf[i+4];

//        if(lVal==0x01000000 && CODEC_H264_NAL_SPS == lTT)
//        {
//            nSPSIndex = i;
//        }

//        if(lVal==0x01000000 && CODEC_H264_NAL_PPS == lTT)
//        {
//            nPPSIndex = i;
//        }

//        if(lVal==0x01000000 && CODEC_H264_NAL_SEI == lTT)   //第二个SEI会把第一个的索引信息覆盖
//        {
//            nSEIIndex = i;
//        }
//    }

//    if(nSPSIndex == -1 || nPPSIndex == -1 || nSEIIndex == -1)
//        return false;

//    pSPS->pNalStart = (unsigned char *)pBuf+nSPSIndex;
//    pSPS->dwNalType = CODEC_H264_NAL_SPS;
//    pSPS->dwNalLength = nPPSIndex-nSPSIndex;
//    pPPS->pNalStart = (unsigned char *)pBuf+nPPSIndex;
//    pPPS->dwNalType = CODEC_H264_NAL_PPS;
//    pPPS->dwNalLength = nSEIIndex-nPPSIndex;
//    return true;
//}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetFrameSize
 * Signature: ()Lipc365/app/showmo/jni/JniDataDef/FrameSize;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetFrameSize
(JNIEnv *env, jclass,jbyteArray barr,jint jsize){
    GVM_H264NALINFO sps,pps;
    char* buf=(char*)malloc(sizeof(char)*jsize);
    memset(buf,0,jsize);
    env->GetByteArrayRegion(barr,0,jsize,(jbyte*)buf);
    //LogV(JNITAG, buf);
    if(!getH264Nal(buf, (int)jsize, &sps, &pps)) {
        free(buf);
        LOGE("  getH264Nal_not_found: %d",(int)jsize);
        return NULL;
    }

    GVM_H264SPSINFO stH264Info = {0};
    H264ParseSps( sps.pNalStart+5, sizeof(sps.dwNalLength-5), &stH264Info );
    int nFrameWidth = stH264Info.dwFrameWidth;
    int nFrameHeight = stH264Info.dwFrameHeight;
    free(buf);
    //char temp[128] = {0};
    //sprintf(temp, "PW_1NET_1GetFrameSize wid:%d,hei:%d",nFrameWidth, nFrameHeight);
    //  LogV(JNITAG, temp);
    jclass clazz;
    jobject jobj=jniNewInitObject(env,"ipc365/app/showmo/jni/JniDataDef$FrameSize",&clazz);
    if(jobj==NULL){
        LOGE("  frameSize new obj err ");
        return NULL;
    }
    jniSetIntField(env,clazz,jobj,"nWidth",(jint)nFrameWidth);
    jniSetIntField(env,clazz,jobj,"nHeight",(jint)nFrameHeight);
    return jobj;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Init
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Init
(JNIEnv *, jclass){
    //  LogV("jni","Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Init");
    jboolean bRet= PW_NET_Init();
    if(!bRet){
        printErr("PW_NET_Init");
    }else{

    }
    realplayAdapter.init();

    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetDeviceid
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetDeviceid
(JNIEnv *env, jclass , jstring jstrUuid){
    char* uuid=jstringToCharStr(env,jstrUuid);
   // char msg[150];
    //sprintf(msg,"jstringToCharStr charStr:%s ",uuid);
    // LogV(JNITAG,msg);
    jlong deviceId=PW_NET_GetDeviceid(uuid);
    free(uuid);
    if(deviceId<0){
        printErr("PW_NET_GetDeviceid");
    }
    return deviceId;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Mgr_SignIn
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Mgr_1SignIn
(JNIEnv *, jclass){
    jboolean bRet=PW_NET_Mgr_SignIn();
    if(!bRet){
        printErr("PW_NET_Mgr_SignIn");
    }
    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Mgr_DisConnect
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Mgr_1DisConnect
(JNIEnv *, jclass, jint cameraId){
    jboolean bRet=PW_NET_Mgr_DisConnect(cameraId);
    if(!bRet){
        printErr("PW_NET_Mgr_DisConnect");
    }
    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Login
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ClienteLoginReq;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Login
(JNIEnv *env, jclass, jobject jobj){
    char* str_user_name=jniGetStringFieldCopyToNewChar(env,jobj,"user_name");
    char* str_pass=jniGetStringFieldCopyToNewChar(env,jobj,"pass");
    int net_type=jniGetIntField(env,jobj,"net_type");
    int padding=jniGetIntField(env,jobj,"padding");

    client_2_login_req req={0};
    strcpy(req.user_name,str_user_name);
    strcpy(req.pass,str_pass);
    req.net_type=net_type;
    req.padding=padding;
    //    LOGE("user:%s,psw:%s,netType:%d,padding:%d",req.user_name,req.pass,req.net_type,req.padding);
    bool bRet=PW_NET_Login(&req);
    //  LOGE("PW_NET_Login af");
    if(!bRet){
        printErr("PW_NET_Login");
    }
    free(str_user_name);
    free(str_pass);
    return bRet;

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Logout
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Logout
(JNIEnv *, jclass){
    jboolean bRet=PW_NET_Logout();
    if(!bRet){
        printErr("PW_NET_Logout");
    }
    return bRet;
}

///*
// * Class:     ipc365_app_showmo_jni_JniClient
// * Method:    PW_NET_MgrState
// * Signature: ()Z
// */
//JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1MgrState
//(JNIEnv *, jclass){
//    jboolean bRet=PW_NET_MgrState();
//    if(!bRet){
//        printErr("PW_NET_MgrState");
//    }
//    return bRet;
//}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetLocalVerity
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetLocalVerity
(JNIEnv *, jclass, jint nCamera_id){
    return true;//PW_NET_GetLocalVerity(nCamera_id);
}

///*
// * Class:     ipc365_app_showmo_jni_JniClient
// * Method:    PW_NET_SetNetWork
// * Signature: (I)Z
// */
//JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetNetWork
//(JNIEnv *, jclass, jint netType){
//    return PW_NET_SetNetWork(netType);
//}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetDebugDatacallback
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/OnDebugDataCallbackListener;)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetDebugDatacallback
(JNIEnv *env, jclass, jobject debugCbObj){

    //    const char * path = env->GetStringUTFChars(debugCbObj,NULL);
    //    strcpy(debugpath,path);
    //    env->ReleaseStringUTFChars(debugCbObj,path);

    env->GetJavaVM(&debugjvm);
    obj_DebugCallback = env->NewGlobalRef(debugCbObj);
    jclass debugcbclass=env->GetObjectClass(debugCbObj);
    ID_DebugCallback=env->GetMethodID(debugcbclass,"onDebugDataCallback","(Ljava/lang/String;)V");
    PW_NET_SetDebugCallBack(cpperrordatacallback);
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_StartRealPlay
 * Signature: (ILipc365/app/showmo/jni/JniDataDef/OnRealdataCallBackListener;J)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1StartRealPlay
(JNIEnv *env, jclass, jint cameraId, jobject in_callbackobj, jlong dwuser){
    jclass callbackClass=env->GetObjectClass(in_callbackobj);
    ID_RealDatacallback=env->GetMethodID(callbackClass,"onDataCallBack","([BJJJJ)I");

    obj_RealDatacallback=env->NewGlobalRef(in_callbackobj);
    env->GetJavaVM(&realDatajvm);

    int netType=0;
    //   char mm[100];
    //   sprintf(mm,"**realplay**cameraId:%d",cameraId);
    //   LogV(JNITAG,mm);
    //LOGE("PW_NET_StartRealPlay cameraId:%d",cameraId);
    bool bRet=PW_NET_StartRealPlay(cameraId,&netType);
    //    char * teststr=(char*)malloc(sizeof(char)*200);
    //    memset(teststr,0,sizeof(char)*200);
    //    strcpy(teststr,"###realplaycallbackTest####");
    //    jni_RealDataCallBack(teststr,55,55,55,(long)env);
    //LOGE("PW_NET_StartRealPlay over net:%d ",netType);
    if(!bRet){
        printErr("PW_NET_StartRealPlay");
        return -1;
    }else{
        m_bFoundIFrame = false;
        PW_NET_SetRealDataCallBack(jni_RealDataCallBack,121212);
        return netType;
    }
}

JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetLastError
(JNIEnv *, jclass){
    unsigned long error=0;
    PW_NET_GetLastError(&error);
    jlong err=error;
    return err;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetVerifyCode
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/SendVerifyCodeReq;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetVerifyCode
(JNIEnv *env, jclass, jstring in_Username){
    if(in_Username==NULL){
        return false;
    }
    const char* cUsername=env->GetStringUTFChars(in_Username,NULL);
    LOGE("PW_NET_GetVerifyCode %s",cUsername);
    send_verifycode_req req;
    strcpy(req.user_name,cUsername);
    bool bRet=PW_NET_GetVerifyCode(&req);
    env->ReleaseStringUTFChars(in_Username,cUsername);
    if(!bRet){
        printErr("PW_NET_GetVerifyCode");
    }
    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_ENT_GetAppVersion
 * Signature: (ILipc365/app/showmo/jni/JniDataDef/QueryAppVersionRet;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1ENT_1GetAppVersion
(JNIEnv *env, jclass, jint appType, jobject out_appversion){
    query_app_version_ret ret={0};
    bool bRet =PW_NET_GetAppVersion(appType,&ret);
    if(!bRet){
        printErr("PW_ENT_GetAppVersion");
    }else{
        if(out_appversion==NULL){
            jni_exception_throw(env,"java/lang/IllegalArgumentException","illegal arg out_appversion");
            return false;
        }
        jclass clazz=env->GetObjectClass(out_appversion);
        if(clazz==NULL){
            // LogV(JNITAG,"FindClass QueryAppVersionRet failured");
            bRet=false;
        }else{
            //LOGW("#######get version:%s get feature %s,%d",ret.version,(char*)ret.feature,strlen((char*)ret.feature));
            jniSetStringField(env,clazz,out_appversion,"version",ret.version);
            if(strlen(ret.feature)>0){
                jniSetStringField(env,clazz,out_appversion,"feature",(char*)ret.feature);
            }else{
                jniSetStringField(env,clazz,out_appversion,"feature","");
            }
        }
    }
    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_AddTerminal
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/TerminalDeviceAddReq;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1AddTerminal
(JNIEnv *env, jclass, jobject in_terminalInfo){
    jclass clazz=env->GetObjectClass(in_terminalInfo);
    jfieldID id_user_id=env->GetFieldID(clazz,"user_id","I");
    jfieldID id_dev_uuid=env->GetFieldID(clazz,"dev_uuid","Ljava/lang/String;");
    jfieldID id_dev_model=env->GetFieldID(clazz,"dev_model","Ljava/lang/String;");
    jfieldID id_dev_osversion=env->GetFieldID(clazz,"dev_osversion","Ljava/lang/String;");

    jint userId=env->GetIntField(in_terminalInfo,id_user_id);
    jstring devUuid=(jstring)env->GetObjectField(in_terminalInfo,id_dev_uuid);
    jstring devModel=(jstring)env->GetObjectField(in_terminalInfo,id_dev_model);
    jstring devOsVersion=(jstring)env->GetObjectField(in_terminalInfo,id_dev_osversion);

    terminal_device_add_req req={0};
    char* c_devUuid=jstringToCharStr(env,devUuid);
    char* c_devModel=jstringToCharStr(env,devModel);
    char* c_devOsVersion=jstringToCharStr(env,devOsVersion);
    strcpy(req.dev_uuid,c_devUuid);
    strcpy(req.dev_model,c_devModel);
    strcpy(req.dev_osversion,c_devOsVersion);
    req.user_id=userId;
    bool bret=PW_NET_AddTerminal(&req);
    if(!bret){
        printErr("PW_NET_AddTerminal");
    }
    free(c_devUuid);
    free(c_devModel);
    free(c_devOsVersion);
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_ResetPassword
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ResetPasswordReq;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1ResetPassword
(JNIEnv *env, jclass, jobject in_resetInfo){
    //  LOGE("PW_NET_ResetPassword:%s","in");
    jclass clazz=env->GetObjectClass(in_resetInfo);
    jfieldID id_userName=env->GetFieldID(clazz,"user_name","Ljava/lang/String;");
    jfieldID id_oldpsw=env->GetFieldID(clazz,"oldpass","Ljava/lang/String;");
    jfieldID id_newpsw=env->GetFieldID(clazz,"newpass","Ljava/lang/String;");

    jstring js_userName=(jstring)env->GetObjectField(in_resetInfo,id_userName);
    jstring js_oldpsw=(jstring)env->GetObjectField(in_resetInfo,id_oldpsw);
    jstring js_newpsw=(jstring)env->GetObjectField(in_resetInfo,id_newpsw);


    const char* c_userName=env->GetStringUTFChars(js_userName,NULL);//jstringToCharStr(env,js_userName);
    const char* c_oldpsw=env->GetStringUTFChars(js_oldpsw,NULL);
    const char* c_newpsw=env->GetStringUTFChars(js_newpsw,NULL);
    reset_password_req req={0};
    strcpy(req.user_name,c_userName);
    strcpy(req.oldpass,c_oldpsw);
    strcpy(req.newpass,c_newpsw);
    //  LOGE("user_name:%s,newpass:%s,oldpass:%s",req.user_name,req.newpass,req.oldpass);
    bool bret=PW_NET_ResetPassword(&req);
    if(!bret){
        printErr("PW_NET_ResetPassword");
    }
    env->ReleaseStringUTFChars(js_userName,c_userName);
    env->ReleaseStringUTFChars(js_oldpsw,c_oldpsw);
    env->ReleaseStringUTFChars(js_newpsw,c_newpsw);

    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_ResetPasswordEx
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ResetPasswordByVerifyReq;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1ResetPasswordEx
(JNIEnv *env, jclass, jobject in_forgetInfo){
    jclass clazz=env->GetObjectClass(in_forgetInfo);
    jfieldID id_userName=env->GetFieldID(clazz,"user_name","Ljava/lang/String;");
    jfieldID id_verifycode=env->GetFieldID(clazz,"verifycode","Ljava/lang/String;");
    jfieldID id_newpsw=env->GetFieldID(clazz,"newpass","Ljava/lang/String;");

    jstring js_userName=(jstring)env->GetObjectField(in_forgetInfo,id_userName);
    jstring js_verifycode=(jstring)env->GetObjectField(in_forgetInfo,id_verifycode);
    jstring js_newpsw=(jstring)env->GetObjectField(in_forgetInfo,id_newpsw);
    char* c_userName=jstringToCharStr(env,js_userName);
    char* c_verifycode=jstringToCharStr(env,js_verifycode);
    char* c_newpsw=jstringToCharStr(env,js_newpsw);
    reset_password_byverify_req req={0};
    strcpy(req.user_name,c_userName);
    strcpy(req.verifycode,c_verifycode);
    strcpy(req.newpass,c_newpsw);
    //  LogV(JNITAG,req.user_name);
    //  LogV(JNITAG,req.verifycode);
    //   LogV(JNITAG,req.newpass);
    bool bret=PW_NET_ResetPasswordEx(&req);
    if(!bret){
        printErr("PW_NET_ResetPassword");
    }
    free(c_userName);
    free(c_verifycode);
    free(c_newpsw);
    return bret;

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_BindState
 * Signature: (Ljava/lang/String;)Lipc365/app/showmo/jni/JniDataDef/BindStateInfo;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1BindState
(JNIEnv *env, jclass, jstring uuid){
    const char* c_uuid=env->GetStringUTFChars(uuid,NULL);
    char curUser[64]={0};
    int state=PW_NET_BindState((char*)c_uuid,curUser);
    LOGE("PW_NET_BindState:%d",state);
    env->ReleaseStringUTFChars(uuid,c_uuid);
    if(state == -1){
        return NULL;
    }
    JniObject* bindInfo=new JniObject(env,"ipc365.app.showmo.jni.JniDataDef$BindStateInfo");
    // LOGW("PW_NET_BindState SET OBJ BEGIN");
    bindInfo->getIntField("state")
            ->set(state)
            ->getStringField("deviceCurUser")->set(curUser);
    // LOGW("PW_NET_BindState SET OBJ OVER");
    jobject obj=bindInfo->getObj();
    free(bindInfo);
    return obj;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetWifiValue
 * Signature: (I)Lipc365/app/showmo/jni/JniDataDef/SDK_WIFI_VALUE;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetWifiValue
(JNIEnv *env, jclass, jint cameraId){
    SDK_WIFI_VALUE wifiInfo{0};
    bool bret=PW_NET_GetWifiValue((int)cameraId,&wifiInfo);
    if(!bret){
        printErr("PW_NET_GetWifiValue err:");
        return NULL;
        //        wifiInfo.reserver1=0;
        //        wifiInfo.reserver2=0;
        //        wifiInfo.wifi_db=80;
        //        wifiInfo.local_ip=0xc0a80101;
        //        strcpy(wifiInfo.mac,"DC:07:28:21:11:11");
        //        strcpy(wifiInfo.version,"1.0.0.1");
        //        strcpy(wifiInfo.currentssid,"DC0728211111");
    }
    char ipstr[15]={0};
    in_addr addr{0};
    addr.s_addr=wifiInfo.local_ip;
    strcpy(ipstr, (char*)inet_ntoa(addr));
    JniObject jWifiInfoObj(env,"ipc365.app.showmo.jni.JniDataDef$SDK_WIFI_VALUE");
    jWifiInfoObj.getIntField("reserver1")\
            ->set(wifiInfo.reserver1)\
            ->getIntField("reserver2")\
            ->set(wifiInfo.reserver2)\
            ->getIntField("wifi_db")\
            ->set(wifiInfo.wifi_db)\
            ->getStringField("local_ip")\
            ->set(ipstr)\
            ->getStringField("mac")\
            ->set(wifiInfo.mac)\
            ->getStringField("version")\
            ->set(wifiInfo.version)\
            ->getStringField("currentssid")\
            ->set(wifiInfo.currentssid);
    return jWifiInfoObj.getObj();
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetVolume
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetVolume
(JNIEnv *env, jclass, jint cameraID){
    SDK_VOLUME_VALUE info={0};
    info.options = 0xffffffff;
    bool bres=PW_NET_GetVolume(cameraID,&info);
    if(!bres){
        printErr("PW_NET_GetVolume");
        return -1;
    }
    return info.volume;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetVolume
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetVolume
(JNIEnv *env, jclass, jint cameraID, jint value){
    SDK_VOLUME_VALUE info={0};
    info.options = 0xffffffff;
    info.values =1;
    info.control=0;
    info.volume=value;
    bool bres=PW_NET_SetVolume(cameraID,&info);
    if(!bres){
        printErr("PW_NET_SetVolume");
    }
    return bres;
}


/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetVolumeSwitchs
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetVolumeSwitchs
(JNIEnv *env, jclass, jint cameraID){
    SDK_VOLUME_VALUE info={0};
    info.options = 0xffffff;
    info.values =1;
    info.control=0;
    info.volume=30;
    bool bres=PW_NET_GetVolume(cameraID,&info);
    if(!bres){
        printErr("PW_NET_GetVolume");
        return NULL;
    }
    LOGE("options:%x,values:%x,volume:%d",info.options,info.values,info.volume);
    jclass listClazz;
    jobject obj_list=jniNewInitObject(env,"java/util/ArrayList",&listClazz);
    jmethodID list_add_mid=env->GetMethodID(listClazz,"add","(Ljava/lang/Object;)Z");

    JniObject item_obj_1(env,"ipc365.app.showmo.jni.JniDataDef$DeviceVolumState");
    item_obj_1.getIntField("type")->set(info.options & STREAM_TYPE)
            ->getBooleanField("bSwitch")->set(info.values & STREAM_TYPE );
    env->CallBooleanMethod(obj_list,list_add_mid,item_obj_1.getObj());

    return obj_list;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_OpenVolumeSwitch
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1OpenVolumeSwitch
  (JNIEnv *env, jclass, jint cameraID, jint type){
    SDK_VOLUME_VALUE info={0};
    info.options = type;
    info.values =1;
    info.control=0;
    info.volume=0;
    LOGE("PW_NET_OpenVolumeSwitch options:%x,values:%x,volume:%d",info.options,info.values,info.volume);
    bool bres=PW_NET_SetVolume(cameraID,&info);
    if(!bres){
        printErr("PW_NET_OpenVolumeSwitch");
    }
    return bres;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NETCloseVolumeSwitch
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NETCloseVolumeSwitch
(JNIEnv *, jclass, jint cameraID, jint type){
    SDK_VOLUME_VALUE info={0};
    info.options = type;
    info.values =0;
    info.control=0;
    info.volume=0;
    LOGE("PW_NETCloseVolumeSwitch options:%x,values:%x,volume:%d",info.options,info.values,info.volume);
    bool bres=PW_NET_SetVolume(cameraID,&info);
    if(!bres){
        printErr("PW_NETCloseVolumeSwitch");
    }
    return bres;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_CheckVerifyCode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1CheckVerifyCode
(JNIEnv *env, jclass, jstring username, jstring verifycode){
    const char* cUserName=env->GetStringUTFChars(username,NULL);
    const char* cVerifycode=env->GetStringUTFChars(verifycode,NULL);
    long lret=PW_NET_CheckVerifyCode((char*)cUserName,(char*)cVerifycode);
    if(lret == -1){
        printErr("PW_NET_CheckVerifyCode");
    }
    return lret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_AddDevice
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ClientDeviceAddReq;)Lipc365/app/showmo/jni/JniDataDef/ClientDeviceAddRet;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1AddDevice
(JNIEnv *env, jclass, jobject in_deviceInfo)
{
    jclass in_clazz=env->GetObjectClass(in_deviceInfo);
    jfieldID id_customId=env->GetFieldID(in_clazz,"custom_id","J");
    jfieldID id_deviceSn=env->GetFieldID(in_clazz,"device_sn","Ljava/lang/String;");
    jfieldID id_deviceName=env->GetFieldID(in_clazz,"device_name","Ljava/lang/String;");
    jfieldID id_devicePasswd=env->GetFieldID(in_clazz,"device_passwd","Ljava/lang/String;");
    jlong customId=env->GetLongField(in_deviceInfo,id_customId);
    jstring js_deviceSn=(jstring)env->GetObjectField(in_deviceInfo,id_deviceSn);
    jstring js_deviceName=(jstring)env->GetObjectField(in_deviceInfo,id_deviceName);
    jstring js_devicePasswd=(jstring)env->GetObjectField(in_deviceInfo,id_devicePasswd);
    char* c_deviceSn=jstringToCharStr(env,js_deviceSn);
    char* c_deviceName=jstringToCharStr(env,js_deviceName);
    char* c_devicePasswd=jstringToCharStr(env,js_devicePasswd);

    client_device_add_req req={0};
    req.custom_id=customId;
    strcpy(req.device_sn,c_deviceSn);
    strcpy(req.device_name,c_deviceName);
    strcpy(req.device_passwd,c_devicePasswd);
    client_device_add_ret ret={0};
    bool bret=PW_NET_AddDevice(&req,&ret);
    free(c_deviceSn);
    free(c_deviceName);
    free(c_devicePasswd);

    if(!bret){
        printErr("PW_NET_AddDevice");
        return NULL;
    }
    jclass  out_clazz=env->FindClass("ipc365/app/showmo/jni/JniDataDef$ClientDeviceAddRet");
    if(out_clazz==NULL){
        return NULL;
    }
    jmethodID out_constructM=env->GetMethodID(out_clazz,"<init>","()V");
    jobject out_obj=env->NewObject(out_clazz,out_constructM);

    jfieldID id_cameraId=env->GetFieldID(out_clazz,"camera_id","J");
    jfieldID id_deviceId=env->GetFieldID(out_clazz,"device_id","J");
    jlong cameraID=ret.camera_id;
    jlong deviceId=ret.device_id;
    env->SetLongField(out_obj,id_cameraId,cameraID);
    env->SetLongField(out_obj,id_deviceId,deviceId);
    return out_obj;

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_AddDeviceEx
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ClientDeviceAddReq;)Lipc365/app/showmo/jni/JniDataDef/ClientDeviceAddRet;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1AddDeviceEx
(JNIEnv *env, jclass, jobject in_deviceInfo){
    jclass in_clazz=env->GetObjectClass(in_deviceInfo);
    jfieldID id_customId=env->GetFieldID(in_clazz,"custom_id","J");
    jfieldID id_deviceSn=env->GetFieldID(in_clazz,"device_sn","Ljava/lang/String;");
    jfieldID id_deviceName=env->GetFieldID(in_clazz,"device_name","Ljava/lang/String;");
    jfieldID id_devicePasswd=env->GetFieldID(in_clazz,"device_passwd","Ljava/lang/String;");
    jlong customId=env->GetLongField(in_deviceInfo,id_customId);
    jstring js_deviceSn=(jstring)env->GetObjectField(in_deviceInfo,id_deviceSn);
    jstring js_deviceName=(jstring)env->GetObjectField(in_deviceInfo,id_deviceName);
    jstring js_devicePasswd=(jstring)env->GetObjectField(in_deviceInfo,id_devicePasswd);
    char* c_deviceSn=jstringToCharStr(env,js_deviceSn);
    char* c_deviceName=jstringToCharStr(env,js_deviceName);
    char* c_devicePasswd=jstringToCharStr(env,js_devicePasswd);

    client_device_add_req req={0};
    req.custom_id=customId;
    strcpy(req.device_sn,c_deviceSn);
    strcpy(req.device_name,c_deviceName);
    strcpy(req.device_passwd,c_devicePasswd);
    client_device_add_ret ret={0};
    bool bret=PW_NET_AddDeviceEx(&req,&ret);
    free(c_deviceSn);
    free(c_deviceName);
    free(c_devicePasswd);

    if(!bret){
        printErr("PW_NET_AddDeviceEx");
        return NULL;
    }
    jclass  out_clazz=env->FindClass("ipc365/app/showmo/jni/JniDataDef$ClientDeviceAddRet");
    if(out_clazz==NULL){
        return NULL;
    }
    jmethodID out_constructM=env->GetMethodID(out_clazz,"<init>","()V");
    jobject out_obj=env->NewObject(out_clazz,out_constructM);

    jfieldID id_cameraId=env->GetFieldID(out_clazz,"camera_id","J");
    jfieldID id_deviceId=env->GetFieldID(out_clazz,"device_id","J");
    jlong cameraID=ret.camera_id;
    jlong deviceId=ret.device_id;
    env->SetLongField(out_obj,id_cameraId,cameraID);
    env->SetLongField(out_obj,id_deviceId,deviceId);
    return out_obj;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_DeleteDevice
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1DeleteDevice
(JNIEnv *env, jclass, jstring in_uuid){
    //char* c_uuid=jstringToCharStr(env,in_uuid);
    const char* cUuid=env->GetStringUTFChars(in_uuid,NULL);
    bool bRet=PW_NET_DeleteDevice((char*)cUuid);
    // LogV(JNITAG,c_uuid);
    env->ReleaseStringUTFChars(in_uuid,cUuid);
    if(!bRet){
        printErr("PW_NET_DeleteDevice");
    }
    return bRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_VerityAccount
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1VerityAccount
(JNIEnv *env, jclass, jstring in_uuid){
    char* c_uuid=jstringToCharStr(env,in_uuid);
    long lRet=PW_NET_VerityAccount(c_uuid);
    free(c_uuid);
    if(lRet==-1){
        printErr("PW_NET_VerityAccount");
    }
    return lRet;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_ApplyTestAccount
 * Signature: (I)Lipc365/app/showmo/jni/JniDataDef/SDK_APPLY_ACCOUNTINFO;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1ApplyTestAccount
(JNIEnv *env, jclass, jint apptype){
    //jclass clazz=env->GetObjectClass(In_acountInfo);
    jclass clazz;//=env->FindClass("ipc365/app/showmo/jni/JniDataDef$SDK_APPLY_ACCOUNTINFO");
    jobject account=jniNewInitObject(env,"ipc365/app/showmo/jni/JniDataDef$SDK_APPLY_ACCOUNTINFO",&clazz);
    //    jfieldID id_userName=env->GetFieldID(clazz,"user_name","Ljava/lang/String;");
    //    jfieldID id_userType=env->GetFieldID(clazz,"user_type","I");
    //    jstring js_userName=(jstring)env->GetObjectField(In_acountInfo,id_userName);
    //    jint ji_userType=env->GetIntField(In_acountInfo,id_userType);
    //    char* c_username=jstringToCharStr(env,js_userName);
    SDK_APPLY_ACCOUNTINFO info={0};
    bool bret=PW_NET_ApplyTestAccount(apptype,&info);
    if(!bret){
        printErr("PW_NET_ApplyTestAccount",0);
        return NULL;
    }
    LOGE("PW_NET_ApplyTestAccount:%s,%d",info.user_name,info.user_type);

    jniSetStringField(env,clazz,account,"user_name",info.user_name);
    jniSetIntField(env,clazz,account,"user_type",info.user_type);
    return account;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_DeleteApplyAccount
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/SDK_APPLY_ACCOUNTINFO;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1DeleteApplyAccount
(JNIEnv *env, jclass, jobject In_acountInfo){
    jclass clazz=env->GetObjectClass(In_acountInfo);
    jfieldID id_userName=env->GetFieldID(clazz,"user_name","Ljava/lang/String;");
    jfieldID id_userType=env->GetFieldID(clazz,"user_type","I");
    jstring js_userName=(jstring)env->GetObjectField(In_acountInfo,id_userName);
    jint ji_userType=env->GetIntField(In_acountInfo,id_userType);
    char* c_username=jstringToCharStr(env,js_userName);
    SDK_APPLY_ACCOUNTINFO info={0};
    info.user_type=ji_userType;
    strcpy(info.user_name,c_username);
    bool bret;
    bret=PW_NET_DeleteApplyAccount(&info);
    free(c_username);
    if(!bret){
        printErr("PW_NET_DeleteApplyAccount");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SignUp
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/ClientRegister2Login;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SignUp
(JNIEnv *env, jclass, jobject in_register){
    char* c_userName=jniGetStringFieldCopyToNewChar(env,in_register,"user_name");
    char* c_pass=jniGetStringFieldCopyToNewChar(env,in_register,"pass");
    char* c_verifycode=jniGetStringFieldCopyToNewChar(env,in_register,"verifycode");
    char* c_deviceSn=jniGetStringFieldCopyToNewChar(env,in_register,"device_sn");
    int js_userType=jniGetIntField(env,in_register,"user_type");
    LOGE("c_userName:%s c_pass:%s c_verifycode:%s c_deviceSn:%s",c_userName\
         ,c_pass\
         ,c_verifycode\
         ,c_deviceSn);
    client_register_2_login reg={0};
    reg.user_type=js_userType;
    strcpy(reg.user_name,c_userName);
    strcpy(reg.pass,c_pass);
    strcpy(reg.verifycode,c_verifycode);
    strcpy(reg.device_sn,c_deviceSn);
    free(c_userName);
    free(c_pass);
    free(c_verifycode);
    free(c_deviceSn);
    bool bret;
    bret=PW_NET_SignUp(&reg);
    if(!bret){
        printErr("PW_NET_SignUp");
    }
    return bret;
}


/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_OnLineState
 * Signature: (I)J
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1OnLineState
(JNIEnv *, jclass, jint cameraId){
    bool bret;
    bret=PW_NET_OnLineState(cameraId);
    if(!bret){
        printErr("PW_NET_OnLineState");
    }
    return bret;
}


/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetStreamType
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetStreamType
(JNIEnv *, jclass, jint cameraId, jint streamType){
    bool bret;
    bret=PW_NET_SetStreamType(cameraId,streamType);
    if(!bret){
        printErr("PW_NET_SetStreamType");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Panoinfo
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/PanoInfo;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Panoinfo
(JNIEnv *env, jclass, jobject out_panoparam){
    //    if(out_panoparam==NULL){
    //        jni_exception_throw(env,"java/lang/IllegalArgumentException","illegal arg out_panoparam");
    //        return false;
    //    }

    //    jclass clazz=env->GetObjectClass(out_panoparam);
    //    jfieldID id_width=env->GetFieldID(clazz,"lWidth","J");
    //    jfieldID id_height=env->GetFieldID(clazz,"lHeight","J");
    //    jfieldID id_centerX=env->GetFieldID(clazz,"lCenterX_1024","J");
    //    jfieldID id_centerY=env->GetFieldID(clazz,"lCenterY_1024","J");
    //    jfieldID id_Radius=env->GetFieldID(clazz,"lRadius_1024","J");
    //    jfieldID id_PanoType=env->GetFieldID(clazz,"lPanoType","J");
    //    jfieldID id_PanoTilt=env->GetFieldID(clazz,"lPanoTilt","J");
    //    pano_info panoInfo={0};
    //    bool bret;
    //    bret=PW_NET_Panoinfo(&panoInfo);
    //    if(!bret){
    //        printErr("PW_NET_Panoinfo");
    //    }else{
    //        env->SetLongField(out_panoparam,id_width,(jlong)panoInfo.lWidth);
    //        env->SetLongField(out_panoparam,id_height,(jlong)panoInfo.lHeight);
    //        env->SetLongField(out_panoparam,id_centerX,(jlong)panoInfo.lCenterX_1024);
    //        env->SetLongField(out_panoparam,id_centerY,(jlong)panoInfo.lCenterY_1024);
    //        env->SetLongField(out_panoparam,id_Radius,(jlong)panoInfo.lRadius_1024);
    //        env->SetLongField(out_panoparam,id_PanoTilt,(jlong)panoInfo.lPanoTilt);
    //        env->SetLongField(out_panoparam,id_PanoType,(jlong)panoInfo.lPanoType);
    //    }
    //    return bret;

}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetMsgDataCallBack
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/OnMsgDataCallBackListener;J)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetMsgDataCallBack
(JNIEnv *env, jclass, jobject objCallBack, jlong user){
    if(objCallBack == NULL){
        bool bret = PW_NET_SetMsgDataCallBack(NULL,user);
        if(!bret){
            printErr("PW_NET_SetMsgDataCallBack");
        }
        return bret;
    }
    jclass clazz=env->GetObjectClass(objCallBack);
    glb_class_msg_cb_wraper = clazz;
    glb_object_cb_wraper=env->NewGlobalRef(objCallBack);
    glb_cb_mid = env->GetMethodID(clazz,"onMsgDataCallBack","(Ljava/lang/Object;J)V");
    //env->CallStaticVoidMethod(glb_class_msg_cb_wraper,glb_cb_mid,NULL,123141);
    jclass map_class=NULL;
    jobject map_obj=jniNewInitObject(env,"ipc365/app/showmo/jni/JniObjectInfoMap",&map_class);
    glb_cb_object_map=env->NewGlobalRef(map_obj);
    glb_mid_cb_map_new_class=env->GetMethodID(map_class,"setNewClassName","(Ljava/lang/String;)V");
    glb_mid_cb_map_add_field=env->GetMethodID(map_class,"addField","(Ljava/lang/String;Ljava/lang/String;)V");
    //ID_MsgDatacallback=env->GetMethodID(clazz,"onMsgDataCallBack","(Ljava/lang/Object;J)V");
    //obj_MsgDatacallback=env->NewGlobalRef(objCallBack);
    //global_class_User_2_mgr_disconn_device=env->FindClass("ipc365/app/showmo/jni/JniDataDef$User_2_mgr_disconn_device");
    //global_mid_constr_User_2_mgr_disconn_device=env->GetMethodID(global_class_User_2_mgr_disconn_device,"<init>","()V");
    env->GetJavaVM(&MsgDatajvm);
    bool bret;
    bret = PW_NET_SetMsgDataCallBack(jni_msgDataCallBack,user);
    if(!bret){
        printErr("PW_NET_SetMsgDataCallBack");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetAlarmState
 * Signature: (IIZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetAlarmState
(JNIEnv *, jclass, jint cameraId, jint alarmType, jboolean state){
    bool bret;
    bret = PW_NET_SetAlarmState(cameraId,alarmType,state);
    // LOGW("PW_NET_SetAlarmState:%d,%d",cameraId,bret);
    if(!bret){
        printErr("PW_NET_SetAlarmState");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetAlarmState
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetAlarmState
(JNIEnv *, jclass, jint cameraId, jint alarmType){
    long lret;
    lret = PW_NET_GetAlarmState(cameraId,alarmType);
    if(lret<0){
        printErr("PW_NET_GetAlarmState");
    }

    return lret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetAlarmPic
 * Signature: (IIIIII)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetAlarmPic
(JNIEnv *env, jclass,jstring path,jint recordId,  jint cameraID, jint nalarmtype, jint nalarmcode, jint nccid, jint nstarttime, jint nendtime){

    AlarmImagePath = (jstring)env->NewGlobalRef(path);
    RecordId=recordId;
    long lret;
    lret = PW_NET_GetAlarmPic(cameraID,nalarmtype,nalarmcode,nccid,nstarttime,nendtime);
    if(lret<0){
        printErr("PW_NET_GetAlarmPic");
    }
    return lret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_ReleaseBuf
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1ReleaseBuf
(JNIEnv *, jclass, jbyteArray){

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_StopRealPlay
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1StopRealPlay
(JNIEnv *, jclass, jint cameraId){
    bool bret;
    bret = PW_NET_StopRealPlay(cameraId);
    if(!bret){
        printErr("PW_NET_StopRealPlay");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Audio
 * Signature: (IZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Audio
(JNIEnv *, jclass, jint nCamera_id, jboolean state){
    bool bret;
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetDeviceList
 * Signature: (Ljava/util/Vector;)I
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetDeviceList
(JNIEnv *env, jclass){
    char lDeviceList[4000]={0};
    int Len=0;
    //PW_NET_GetDeviceList(int nDeviceType,int* nDeviceNum,char *lpDeviceList);
    bool bRet=PW_NET_GetDeviceList(1,&Len,lDeviceList);
    if(!bRet){
        //char * exceptionName="ipc365/app/showmo/jni/JniDataDef$GetDeviceFromServerException";
        // jni_exception_throw(env,exceptionName,"GetDeviceFromServerException :PW_NET_GetDeviceList err");
        return NULL;
    }
    client_device_query_ret *temp=(client_device_query_ret *)lDeviceList;
    jclass clazz=env->FindClass("java/util/ArrayList");
    jmethodID id_arr_construct=env->GetMethodID(clazz,"<init>","()V");
    jobject out_deviceList=env->NewObject(clazz,id_arr_construct);
    if(out_deviceList==NULL){
        char * exceptionName="java/lang/IllegalArgumentException";
        jni_exception_throw(env,exceptionName,"illegal arg out_deviceList ");
        return NULL;
    }
    jmethodID id_add=env->GetMethodID(clazz,"add","(Ljava/lang/Object;)Z");
    for(int i=0;i<Len;i++){
        jclass eleClazz=env->FindClass("ipc365/app/showmo/jni/JniDataDef$ClientDeviceQueryRet");
        if(eleClazz==NULL){
            LogV(JNITAG,"FindClass ClientDeviceQueryRet err");
            return out_deviceList;
        }
        jmethodID id_eleConstruct=env->GetMethodID(eleClazz,"<init>","()V");
        jobject obj_ele=env->NewObject(eleClazz,id_eleConstruct);
        jfieldID id_device_id=env->GetFieldID(eleClazz,"device_id","J");
        jfieldID id_deviceSn=env->GetFieldID(eleClazz,"device_sn","Ljava/lang/String;");
        jfieldID id_deviceName=env->GetFieldID(eleClazz,"device_name","Ljava/lang/String;");
        jfieldID id_devicePsw=env->GetFieldID(eleClazz,"device_passwd","Ljava/lang/String;");

        env->SetLongField(obj_ele,id_device_id,(jlong)temp->device[i].device_id);
        jstring js_deviceName=env->NewStringUTF(temp->device[i].device_name);
        env->SetObjectField(obj_ele,id_deviceName,js_deviceName);
        char sn[13]={0};
        strncpy(sn,temp->device[i].device_sn,sizeof(char)*12);
        jstring js_deviceSn=env->NewStringUTF(sn);
        env->SetObjectField(obj_ele,id_deviceSn,js_deviceSn);

        jstring js_devicePsw=env->NewStringUTF(temp->device[i].device_passwd);
        env->SetObjectField(obj_ele,id_devicePsw,js_devicePsw);
       // char msg[300];
       // sprintf(msg,"getdev %s,%s,%s,%d",temp->device[i].device_name,temp->device[i].device_sn,temp->device[i].device_passwd,temp->device[i].device_id);
        if(id_add==NULL){
            LogV(JNITAG,"GetMethodID err");
        }
        env->CallBooleanMethod(out_deviceList,id_add,obj_ele);
        char * exceptionName="java/lang/Exception";
        if(jni_exception_check_and_throw(env,exceptionName,"add to arraylist exception in GetDeviceList")){
            return NULL;
        }
    }
    return out_deviceList;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Log
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Log
(JNIEnv *env, jclass, jboolean flag){
    //    bool bret;
    //    bret = PW_NET_Log(flag);
    //    if(!bret){
    //        printErr("PW_NET_Log");
    //    }
    //    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Upgrade
 * Signature: (ILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Upgrade
(JNIEnv *env, jclass, jint cameraId, jstring version){
    char* str_version=jstringToCharStr(env,version);
    bool bret;
    bret = PW_NET_Upgrade(cameraId,str_version);
    free(str_version);
    if(!bret){
        printErr("PW_NET_Upgrade");
    }
    return bret;

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_BrightCtrl
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1BrightCtrl
(JNIEnv *, jclass, jint cameraId, jint value,jint ctrl){
    bool bret;
    LOGE("PW_NET_BrightCtrl cameraId:%d,value:%d,ctrl:%d",cameraId,value,ctrl);
    bret = PW_NET_BrightCtrl(cameraId,value,ctrl);
    if(!bret){
        printErr("PW_NET_BrightCtrl");
    }
    return bret;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Brightness
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Brightness
(JNIEnv *, jclass, jint cameraId){
    bool bret;
    int value=0;
    bret = PW_NET_Brightness(cameraId,&value);
    if(!bret){
        value=-1;
        printErr("PW_NET_BrightCtrl");
    }
    return value;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SearchRemoteFile
 * Signature: (ILipc365/app/showmo/jni/JniDataDef/SDK_SEARCH;Lipc365/app/showmo/jni/JniDataDef/SDK_REMOTE_FILE;)I
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SearchRemoteFile
(JNIEnv *env, jclass, jint cameraId, jobject findInfo){
    jclass out_arrClass;
    jobject out_arrRetFile=jniNewInitObject(env,"java/util/ArrayList",&out_arrClass);
    if(out_arrRetFile==NULL){
        LogV(JNITAG,"out_arrRetFile==NULL");
        return NULL;
    }
    jmethodID id_out_add=env->GetMethodID(out_arrClass,"add","(Ljava/lang/Object;)Z");

    jclass infoClass=env->GetObjectClass(findInfo);
    //    jfieldID id_fileType=env->GetFieldID(infoClass,"nFileType","I");
    //    jfieldID id_startTime=env->GetFieldID(infoClass,"startTime","Landroid/text/format/Time;");
    //    jfieldID id_endTime=env->GetFieldID(infoClass,"endTime","Landroid/text/format/Time;");
    jobject obj_startTime=jniGetObjectField(env,findInfo,"startTime","Landroid/text/format/Time;");
    jobject obj_endTime=jniGetObjectField(env,findInfo,"endTime","Landroid/text/format/Time;");

    jclass timeClass=env->GetObjectClass(obj_startTime);
    jmethodID id_timeStruct=env->GetMethodID(timeClass,"<init>","()V");

    SDK_SEARCH searchInfo={0};
    searchInfo.nFileType=jniGetIntField(env,findInfo,"nFileType");
    searchInfo.startTime.nYear=jniGetIntField(env,obj_startTime,"year");
    searchInfo.startTime.nMonth=jniGetIntField(env,obj_startTime,"month")+1;
    searchInfo.startTime.nDay=jniGetIntField(env,obj_startTime,"monthDay");
    searchInfo.startTime.nHour=jniGetIntField(env,obj_startTime,"hour");
    searchInfo.startTime.nMinute=jniGetIntField(env,obj_startTime,"minute");
    searchInfo.startTime.nSecond=jniGetIntField(env,obj_startTime,"second");

    searchInfo.endTime.nYear=jniGetIntField(env,obj_endTime,"year");
    searchInfo.endTime.nMonth=jniGetIntField(env,obj_endTime,"month")+1;
    searchInfo.endTime.nDay=jniGetIntField(env,obj_endTime,"monthDay");
    searchInfo.endTime.nHour=jniGetIntField(env,obj_endTime,"hour");
    searchInfo.endTime.nMinute=jniGetIntField(env,obj_endTime,"minute");
    searchInfo.endTime.nSecond=jniGetIntField(env,obj_endTime,"second");

//    char startTimeStr[200];
//    sprintf(startTimeStr,"searchTime %d-%d-%d#%d-%d-%d to  %d-%d-%d#%d-%d-%d",\
//            searchInfo.startTime.nYear,searchInfo.startTime.nMonth,searchInfo.startTime.nDay,
//            searchInfo.startTime.nHour,searchInfo.startTime.nMinute, searchInfo.startTime.nSecond,
//            searchInfo.endTime.nYear,searchInfo.endTime.nMonth,searchInfo.endTime.nDay,
//            searchInfo.endTime.nHour,searchInfo.endTime.nMinute,searchInfo.endTime.nSecond);
    //   LogV(JNITAG,startTimeStr);
    bool bret=false;
    SDK_REMOTE_FILE *data=new SDK_REMOTE_FILE[PW_PHONE_BL_SHOWMO_FINDFILE_MAXCOUNT];
    memset(data,0,PW_PHONE_BL_SHOWMO_FINDFILE_MAXCOUNT*sizeof(SDK_REMOTE_FILE));

    do{
        int findCount=0;
        bret=PW_NET_SearchRemoteFile(cameraId,&searchInfo,(char*)data,&findCount);
        if(!bret){
            printErr("PW_NET_SearchRemoteFile");
            //        char * exceptionName="ipc365/app/showmo/jni/JniDataDef$GetPlayBackFileException";
            //        jni_exception_throw(env,exceptionName,"GetPlayBackFileException :PW_NET_SearchRemoteFile err");
            return NULL;
        }
        for(int i=0;i<findCount;i++){
            //LogV(JNITAG,"get a remote file");
            jobject obj_eleStartTime=env->NewObject(timeClass,id_timeStruct);
            jniSetIntField(env,timeClass,obj_eleStartTime,"year",(jint)data[i].startTime.nYear);
            jniSetIntField(env,timeClass,obj_eleStartTime,"month",(jint)data[i].startTime.nMonth-1);
            jniSetIntField(env,timeClass,obj_eleStartTime,"monthDay",(jint)data[i].startTime.nDay);
            jniSetIntField(env,timeClass,obj_eleStartTime,"hour",(jint)data[i].startTime.nHour);
            jniSetIntField(env,timeClass,obj_eleStartTime,"minute",(jint)data[i].startTime.nMinute);
            jniSetIntField(env,timeClass,obj_eleStartTime,"second",(jint)data[i].startTime.nSecond);
            jobject obj_eleEndTime=env->NewObject(timeClass,id_timeStruct);
            jniSetIntField(env,timeClass,obj_eleEndTime,"year",(jint)data[i].endTime.nYear);
            jniSetIntField(env,timeClass,obj_eleEndTime,"month",(jint)data[i].endTime.nMonth-1);
            jniSetIntField(env,timeClass,obj_eleEndTime,"monthDay",(jint)data[i].endTime.nDay);
            jniSetIntField(env,timeClass,obj_eleEndTime,"hour",(jint)data[i].endTime.nHour);
            jniSetIntField(env,timeClass,obj_eleEndTime,"minute",(jint)data[i].endTime.nMinute);
            jniSetIntField(env,timeClass,obj_eleEndTime,"second",(jint)data[i].endTime.nSecond);

            jclass remote_file_clazz;
            jobject obj_ele_remote=jniNewInitObject(env,"ipc365/app/showmo/jni/JniDataDef$SDK_REMOTE_FILE",&remote_file_clazz);
            jniSetIntField(env,remote_file_clazz,obj_ele_remote,"size",(jint)data[i].size);
            jniSetStringField(env,remote_file_clazz,obj_ele_remote,"sFileName",data[i].sFileName);
            jniSetIntField(env,remote_file_clazz,obj_ele_remote,"nFileType",(jint)data[i].nFileType);
            jniSetObjectField(env,remote_file_clazz,obj_ele_remote,"startTime","Landroid/text/format/Time;",obj_eleStartTime);
            jniSetObjectField(env,remote_file_clazz,obj_ele_remote,"endTime","Landroid/text/format/Time;",obj_eleEndTime);

            //  LOGE("begin hour:%d,end hour:%d,filename:%s",data[i].startTime.nHour,data[i].endTime.nHour,data[i].sFileName);
            env->CallBooleanMethod(out_arrRetFile,id_out_add,obj_ele_remote);

            if(jni_exception_check_and_throw(env,"java/lang/Exception","add to arraylist exception in SearchRemoteFile")){
                env->DeleteLocalRef(obj_ele_remote);
                env->DeleteLocalRef(obj_eleStartTime);
                env->DeleteLocalRef(obj_eleEndTime);
                return NULL;
            }
            env->DeleteLocalRef(obj_ele_remote);
            env->DeleteLocalRef(obj_eleStartTime);
            env->DeleteLocalRef(obj_eleEndTime);
        }
        if(findCount>=PW_PHONE_BL_SHOWMO_FINDFILE_MAXCOUNT){
            searchInfo.startTime.nYear=data[findCount-1].endTime.nYear;
            searchInfo.startTime.nMonth=data[findCount-1].endTime.nMonth;
            searchInfo.startTime.nDay=data[findCount-1].endTime.nDay;
            searchInfo.startTime.nHour=data[findCount-1].endTime.nHour;
            searchInfo.startTime.nMinute=data[findCount-1].endTime.nMinute;
            searchInfo.startTime.nSecond=data[findCount-1].endTime.nSecond+1;
            memset(data,0,PW_PHONE_BL_SHOWMO_FINDFILE_MAXCOUNT*sizeof(SDK_REMOTE_FILE));
            continue;
        }else{
            break;
        }
    }while(1);
    return out_arrRetFile;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_PlayBack
 * Signature: (ILipc365/app/showmo/jni/JniDataDef/SDK_REMOTE_FILE;Lipc365/app/showmo/jni/JniDataDef/OnRealdataCallBackListener;J)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1PlayBack
(JNIEnv *env, jclass, jint cameraId, jobject remoteInfo, jobject dataCallBackListener, jlong userData){
    env->GetJavaVM(&playbackDatajvm);
    obj_PlaybackDatacallback=env->NewGlobalRef(dataCallBackListener);
    jclass playbackClazz=env->GetObjectClass(dataCallBackListener);
    ID_PlaybackDatacallback=env->GetMethodID(playbackClazz,"onDataCallBack","([BJJJJ)I");

    jclass infoClass=env->GetObjectClass(remoteInfo);
    jfieldID id_fileSize=env->GetFieldID(infoClass,"size","I");
    jfieldID id_fileType=env->GetFieldID(infoClass,"nFileType","I");
    jfieldID id_filename=env->GetFieldID(infoClass,"sFileName","Ljava/lang/String;");
    jfieldID id_startTime=env->GetFieldID(infoClass,"startTime","Landroid/text/format/Time;");
    jfieldID id_endTime=env->GetFieldID(infoClass,"endTime","Landroid/text/format/Time;");

    SDK_REMOTE_FILE searchInfo={0};
    searchInfo.size=jniGetIntField(env,remoteInfo,"size");
    searchInfo.nFileType=jniGetIntField(env,remoteInfo,"nFileType");
    char* filename=jniGetStringFieldCopyToNewChar(env,remoteInfo,"sFileName");
    strcpy(searchInfo.sFileName,filename);
    free(filename);
    jobject obj_startTime=jniGetObjectField(env,remoteInfo,"startTime","Landroid/text/format/Time;");
    jobject obj_endTime=jniGetObjectField(env,remoteInfo,"endTime","Landroid/text/format/Time;");

    jclass timeClass=env->GetObjectClass(obj_startTime);
    searchInfo.startTime.nYear=jniGetIntField(env,obj_startTime,"year");
    searchInfo.startTime.nMonth=jniGetIntField(env,obj_startTime,"month")+1;
    searchInfo.startTime.nDay=jniGetIntField(env,obj_startTime,"monthDay");
    searchInfo.startTime.nHour=jniGetIntField(env,obj_startTime,"hour");
    searchInfo.startTime.nMinute=jniGetIntField(env,obj_startTime,"minute");
    searchInfo.startTime.nSecond=jniGetIntField(env,obj_startTime,"second");

    searchInfo.endTime.nYear=jniGetIntField(env,obj_endTime,"year");
    searchInfo.endTime.nMonth=jniGetIntField(env,obj_endTime,"month")+1;
    searchInfo.endTime.nDay=jniGetIntField(env,obj_endTime,"monthDay");
    searchInfo.endTime.nHour=jniGetIntField(env,obj_endTime,"hour");
    searchInfo.endTime.nMinute=jniGetIntField(env,obj_endTime,"minute");
    searchInfo.endTime.nSecond=jniGetIntField(env,obj_endTime,"second");

    //    char startTimeStr[520];
    //    sprintf(startTimeStr,"searchfile: %s \n searchTime %d-%d-%d#%d-%d-%d to  %d-%d-%d#%d-%d-%d",\
    //            searchInfo.sFileName,\
    //            searchInfo.startTime.nYear,searchInfo.startTime.nMonth,searchInfo.startTime.nDay,\
    //            searchInfo.startTime.nHour,searchInfo.startTime.nMinute, searchInfo.startTime.nSecond,\
    //            searchInfo.endTime.nYear,searchInfo.endTime.nMonth,searchInfo.endTime.nDay,\
    //            searchInfo.endTime.nHour,searchInfo.endTime.nMinute,searchInfo.endTime.nSecond);
    //   LogV(JNITAG,startTimeStr);
    bool bret;
    bret=PW_NET_PlayBack(cameraId,&searchInfo,jni_PlaybackCallBack,userData);
    if(!bret){
        printErr("PW_NET_PlayBack");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetPlayBackPos
 * Signature: (ILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetPlayBackPos
(JNIEnv *env, jclass, jint cameraId, jstring filename){
    //    char* str_filename=jstringToCharStr(env,filename);
    //    long pos=PW_NET_GetPlayBackPos(cameraId,str_filename);
    //    free(str_filename);
    return -1;//pos;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_PlayBackControl
 * Signature: (III)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1PlayBackControl
(JNIEnv *, jclass, jint nCamera_id, jint ctrl, jint ctrlvalue){
    bool bret;
    LOGE("PW_NET_PlayBackControl cameraId:%d,ctrl:%d,pos:%d",nCamera_id,ctrl,ctrlvalue);
    bret = PW_NET_PlayBackControl(nCamera_id,ctrl,ctrlvalue);
    if(!bret){
        printErr("PW_NET_PlayBackControl");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_StopPlayBack
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1StopPlayBack
(JNIEnv *env, jclass, jint cameraId){
    bool bret;
    bret = PW_NET_StopPlayBack(cameraId);
    if(!bret){
        printErr("PW_NET_StopPlayBack");
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_Record
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Record
(JNIEnv *env, jclass, jstring filename) {
    //  LogV("JNI", "Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Record 1");
    const char *chFile = env->GetStringUTFChars(filename, NULL);
    m_aviConvert = new AVIConvert(chFile);
    //  LogV("JNI", "Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Record 2");
    if( m_aviConvert->start() < 0 ) {
        //  LogV("JNI", "Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Record 3");
        delete m_aviConvert;
        m_aviConvert = NULL;
        env->ReleaseStringUTFChars(filename, chFile);
        return false;
    }
    // LogV("JNI", "Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1Record 4");
    env->ReleaseStringUTFChars(filename, chFile);
    m_bRecord = true;
    return true;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_StopRecord
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1StopRecord
(JNIEnv *env, jclass) {
    if(!m_bRecord)
        return false;

    m_bRecord = false;
    m_aviConvertMutex.lock();
    if(m_aviConvert) {
        m_aviConvert->stop();
        delete m_aviConvert;
        m_aviConvert = NULL;
    }
    m_aviConvertMutex.unlock();

    return true;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetDevConfig
 * Signature: (JLjava/lang/String;JLjava/lang/String;JI)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetDevConfig
(JNIEnv *, jclass, jlong, jstring, jlong, jstring, jlong, jint){

}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SetDevConfig
 * Signature: (JLjava/lang/String;JLjava/lang/String;JI)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SetDevConfig
(JNIEnv *, jclass, jlong, jstring, jlong, jstring, jlong, jint){

}
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevLogin
(JNIEnv *env, jclass, jstring ip){
    char* str=const_cast<char*>(env->GetStringUTFChars(ip,NULL));
    jlong lret=PW_PRI_DevLogin(str);
    env->ReleaseStringUTFChars(ip,str);
    return lret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevLogout
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevLogout
(JNIEnv *env, jclass, jlong loginId){
    PW_PRI_DevLogout(loginId);
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevSetTimezone
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevSetTimezone
(JNIEnv *, jclass, jlong loginId, jint timezone){
    bool bret;
    bret = PW_PRI_DevSetTimezone(loginId,timezone);
    if(!bret){
        printErr("PW_PRI_DevSetTimezone",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevSetOSD
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevSetOSD
(JNIEnv *env, jclass, jlong loginId, jstring osdName){
    char* osd=const_cast<char*>(env->GetStringUTFChars(osdName,NULL));
    bool bret;
    bret = PW_PRI_DevSetOSD(loginId,osd);
    if(!bret){
        printErr("PW_PRI_DevSetOSD",1);
    }
    env->ReleaseStringUTFChars(osdName,osd);
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevSetWifi
 * Signature: (JLipc365/app/showmo/jni/JniDataDef/LPPW_PHONE_BL_WIFI_CONFIG;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevSetWifi
(JNIEnv * env, jclass, jlong LoginID, jobject wifiConfig){
    jclass wifiConfigClazz=env->GetObjectClass(wifiConfig);
    jfieldID id_bEnable=env->GetFieldID(wifiConfigClazz,"bEnable","Z");
    jfieldID id_sSSID=env->GetFieldID(wifiConfigClazz,"sSSID","[B");
    jfieldID id_nChannel=env->GetFieldID(wifiConfigClazz,"nChannel","I");
    jfieldID id_sNetType=env->GetFieldID(wifiConfigClazz,"sNetType","[B");
    jfieldID id_sEncrypType=env->GetFieldID(wifiConfigClazz,"sEncrypType","[B");
    jfieldID id_sAuth=env->GetFieldID(wifiConfigClazz,"sAuth","[B");
    jfieldID id_nKeyType=env->GetFieldID(wifiConfigClazz,"nKeyType","I");
    jfieldID id_sKeys=env->GetFieldID(wifiConfigClazz,"sKeys","[B");
    jfieldID id_HostIP=env->GetFieldID(wifiConfigClazz,"HostIP","[B");
    jfieldID id_Submask=env->GetFieldID(wifiConfigClazz,"Submask","[B");
    jfieldID id_Gateway=env->GetFieldID(wifiConfigClazz,"Gateway","[B");

    jboolean bEnable=env->GetBooleanField(wifiConfig,id_bEnable);
    jbyteArray arr_ssid=(jbyteArray)env->GetObjectField(wifiConfig,id_sSSID);
    jint nchannel=env->GetIntField(wifiConfig,id_nChannel);
    jbyteArray arr_netType=(jbyteArray)env->GetObjectField(wifiConfig,id_sNetType);
    jbyteArray arr_sEncrypType=(jbyteArray)env->GetObjectField(wifiConfig,id_sEncrypType);
    jbyteArray arr_sAuth=(jbyteArray)env->GetObjectField(wifiConfig,id_sAuth);
    jint nKeyType=env->GetIntField(wifiConfig,id_nKeyType);
    jbyteArray arr_sKeys=(jbyteArray)env->GetObjectField(wifiConfig,id_sKeys);
    jbyteArray arr_HostIP=(jbyteArray)env->GetObjectField(wifiConfig,id_HostIP);
    jbyteArray arr_Submask=(jbyteArray)env->GetObjectField(wifiConfig,id_Submask);
    jbyteArray arr_Gateway=(jbyteArray)env->GetObjectField(wifiConfig,id_Gateway);

    char* ssid=(char*)env->GetByteArrayElements(arr_ssid,0);
    char* netType=(char*)env->GetByteArrayElements(arr_netType,0);
    char* sEncrypType=(char*)env->GetByteArrayElements(arr_sEncrypType,0);
    char* sAuth=(char*)env->GetByteArrayElements(arr_sAuth,0);
    char* sKeys=(char*)env->GetByteArrayElements(arr_sKeys,0);
    char* HostIP=(char*)env->GetByteArrayElements(arr_HostIP,0);
    char* Submask=(char*)env->GetByteArrayElements(arr_Submask,0);
    char* Gateway=(char*)env->GetByteArrayElements(arr_Gateway,0);
    PW_PHONE_BL_WIFI_CONFIG wifi={0};
    wifi.bEnable=bEnable;
    wifi.nChannel=nchannel;
    wifi.nKeyType=nKeyType;
    strcpy((char*)wifi.sSSID,ssid);
    strcpy((char*)wifi.sNetType,netType);
    strcpy((char*)wifi.sEncrypType,sEncrypType);
    strcpy((char*)wifi.sAuth,sAuth);
    strcpy((char*)wifi.sKeys,sKeys);
    strcpy((char*)wifi.HostIP,HostIP);
    strcpy((char*)wifi.Submask,Submask);
    strcpy((char*)wifi.Gateway,Gateway);
    bool bret=PW_PRI_DevSetWifi(LoginID,&wifi);
    if(!bret){
        printErr("PW_PRI_DevSetWifi",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_isDevConnectedInApModel
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1isDevConnectedInApModel
(JNIEnv *, jclass, jlong LoginID){
    bool bret=PW_PRI_isDevConnectedInApModel(LoginID);
    if(!bret){
        printErr("PW_PRI_isDevConnectedInApModel",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_GetConnectedRouterStatus
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1GetConnectedRouterStatus
(JNIEnv *, jclass, jlong LoginID){
    bool bret=PW_PRI_GetConnectedRouterStatus(LoginID);
    if(!bret){
        printErr("PW_PRI_GetConnectedRouterStatus",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevSetDHCP
 * Signature: (JZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevSetDHCP
(JNIEnv *, jclass, jlong LoginID, jboolean bOpen){
    bool bret=PW_PRI_DevSetDHCP(LoginID,bOpen);
    if(!bret){
        printErr("PW_PRI_DevSetDHCP",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevAlive
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevAlive
(JNIEnv *, jclass, jlong LoginID){
    bool bret=PW_PRI_DevAlive(LoginID);
    if(!bret){
        printErr("PW_PRI_DevAlive",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_DevAPSwap
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1DevAPSwap
(JNIEnv *, jclass, jlong LoginID){
    bool bret=PW_PRI_DevAPSwap(LoginID);
    if(!bret){
        printErr("PW_PRI_DevAPSwap",1);
    }
    return bret;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_GetLastError
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1GetLastError
(JNIEnv *, jclass){
    return (jlong)PW_PRI_GetLastError();
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_BeginCycleBroadcastToDev
 * Signature: (Lipc365/app/showmo/jni/JniDataDef/Broadcast_wifi_info;)Z
 */


JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1BeginCycleBroadcastToDev
(JNIEnv *env, jclass, jobject wifiInfoObj){
    int ssidLen=0;
    char* ssid=jniNewCharArrFromStringField(env,wifiInfoObj,"ssid",&ssidLen);
    int pswLen=0;
    char* psw=jniNewCharArrFromStringField(env,wifiInfoObj,"psw",&pswLen);
    int keytypeLen=0;
    char* keytype=jniNewCharArrFromStringField(env,wifiInfoObj,"keytype",&keytypeLen);
    int dataLen=0;
    char* data=jniNewCharArrFromStringField(env,wifiInfoObj,"data",&dataLen);
    //char msg[215];
    //sprintf(msg,"%s##%s##%s##%s",ssid,psw,keytype,data);
    //LOGE("PW_PRI_BroadcastToDevRecycle BEGIN,ssid:%s,psw:%s,keytype:%s,data:%s",ssid,psw,keytype,data);
    //LOGE("PW_PRI_BroadcastToDevRecycle BEGIN,ssidLen:%d,pswLen:%d,keytypeLen:%d,dataLen:%d",ssidLen,pswLen,keytypeLen,dataLen);
    //LOGE("PW_PRI_BroadcastToDevRecycle BEGIN,ssidLen:%d,pswLen:%d,keytypeLen:%d,dataLen:%d",strlen(ssid),strlen(psw),strlen(keytype),strlen(data));
    bool bres=PW_PRI_BroadcastToDevRecycle(ssid,psw,1);
    //LOGE("PW_PRI_BroadcastToDevRecycle OVER result:%d",bres);
    free(ssid);
    free(psw);
    free(keytype);
    free(data);
    return bres;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_StopCycleBroadcastToDev
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1StopCycleBroadcastToDev
(JNIEnv *, jclass){
    PW_PRI_stopBroadcast();
    return true;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_SetPauseCycleBroadcastToDev
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1SetPauseCycleBroadcastToDev
(JNIEnv *, jclass, jboolean bpause){
    PW_PRI_setPauseBroadcast(bpause);
    return true;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_PRI_SearchDevInLan
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1PRI_1SearchDevInLan
(JNIEnv *env, jclass, jint searchTime){
    int maxCount=30;
    char mac[30][32]={0};
    int findCount=0;
    bool bres=PW_PRI_SearchDeviceInLanWithTime(mac,maxCount,&findCount,(int)searchTime);
    if(!bres){
        return NULL;
    }
    jclass listClazz;
    jobject obj_list=jniNewInitObject(env,"java/util/ArrayList",&listClazz);
    jmethodID list_add_mid=env->GetMethodID(listClazz,"add","(Ljava/lang/Object;)Z");
    for(int i=0;i<findCount;i++){
        char ssid[32]={0};
        int index=0;
        for(int j=0;mac[i][j]!='\0';j++){
            if(mac[i][j] != ':'){
                ssid[index]=mac[i][j];
                index++;
            }
        }
        jstring macJstr=env->NewStringUTF(ssid);
        //  LogV(JNITAG,mac[i]);
        env->CallBooleanMethod(obj_list,list_add_mid,macJstr);
        env->DeleteLocalRef(macJstr);
    }
    return obj_list;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_PatternCtrlFingerDown
 * Signature: (I[Lcom/puwell/opengles/Flinger;II)V
 */

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_init
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1init
(JNIEnv *env, jclass,jstring LibPath, jstring CompPath, jstring GLContent){

    if (LibPath == NULL || CompPath == NULL) {
        LOGE("%s: input param error. Try again!!!", __FUNCTION__);
        return;
    }
    const char* libpath = env->GetStringUTFChars(LibPath, NULL);
    const char* comppath = env->GetStringUTFChars(CompPath, NULL);
    const char* glcontent = env->GetStringUTFChars(GLContent, NULL);
    pw_mp_ConfigLibPath(libpath);
    pw_mp_ConfigComPath(comppath);
    //  LOGE("%s: input glconfig is %s", __FUNCTION__, glcontent);
    pw_mp_LoadComponentContext("gl_pattern", glcontent);
    env->ReleaseStringUTFChars(LibPath, libpath);
    env->ReleaseStringUTFChars(CompPath, comppath);
    env->ReleaseStringUTFChars(GLContent, glcontent);
}

//void Uninit(JNIEnv *env, jobject obj) {
//	SDK_CUClear(); //初始化
//	if (avi != NULL) {
//		delete avi;
//		avi = NULL;
//	}
//	if (gppRealStreamAdapter != NULL) {
//		for (int i = 0; i < SIZE(gppRealStreamAdapter); i++) {
//			if (gppRealStreamAdapter[i] != NULL) {
//				delete gppRealStreamAdapter[i];
//				gppRealStreamAdapter[i] = NULL;
//			}
//		}
//		free(gppRealStreamAdapter);
//		gppRealStreamAdapter = NULL;
//	}

//	if (gppRecoderStreamAdapter != NULL) {
//		for (int i = 0; i < SIZE(gppRecoderStreamAdapter); i++) {
//			if (gppRecoderStreamAdapter[i] != NULL) {
//				delete gppRecoderStreamAdapter[i];
//				gppRecoderStreamAdapter[i] = NULL;
//			}
//		}
//		free(gppRecoderStreamAdapter);
//		gppRecoderStreamAdapter = NULL;
//	}
//	glogin_id = 0;
//}

JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1PatternCtrlFingerDown__I_3Lcom_puwell_opengles_Flinger_2II
(JNIEnv *env, jclass, jint index, jobjectArray fingerList, jint fingerCount, jint TickCount){
    GLPOINTINSCREEN *flingerList = NULL;
    jclass cls_Flinger = env->FindClass("com/puwell/opengles/Flinger");
    //	int length = env->GetArrayLength(fingerList);
    flingerList = (GLPOINTINSCREEN *) malloc(sizeof(GLPOINTINSCREEN) * fingerCount);

    for (int i = 0; i < fingerCount; i++) {
        GLPOINTINSCREEN flinger = flingerList[i];
        jobject object = env->GetObjectArrayElement(fingerList, i);
        jfieldID fid_x = env->GetFieldID(cls_Flinger, "x", "I");
        int x = env->GetIntField(object, fid_x);
        flinger.ScreenPoint.x = x;
        jfieldID fid_y = env->GetFieldID(cls_Flinger, "y", "I");
        int y = env->GetIntField(object, fid_y);
        flinger.ScreenPoint.y = y;
        jfieldID fid_width = env->GetFieldID(cls_Flinger, "width", "I");
        jint width = env->GetIntField(object, fid_width);
        flinger.ScreenSize.Wid = width;
        jfieldID fid_height = env->GetFieldID(cls_Flinger, "height", "I");
        jint height = env->GetIntField(object, fid_height);
        flinger.ScreenSize.Hei = height;
        env->DeleteLocalRef(object);
    }
    env->DeleteLocalRef(cls_Flinger);

    realplayAdapter.PatternCtrlFlingerDown(flingerList, fingerCount, TickCount);
    if (flingerList != NULL) {
        free(flingerList);
    }
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_PatternCtrlFingerDown
 * Signature: (I[I[I[I[III)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1PatternCtrlFingerDown__I_3I_3I_3I_3III
(JNIEnv *env, jclass, jint index, jintArray xArr, jintArray yArr, jintArray widthArr, jintArray heightArr, jint count, jint TickCount){
    jboolean isCp;
    int i = 0;
    //   StreamAdapter *pStreamAdapter = &realplayAdapter;
    //    if (PlayType == PLAYER_TYPE_REALPLAY) {
    //        pStreamAdapter = gppRealStreamAdapter[index];
    //    } else if (PlayType == PLAYER_TYPE_PLAYBACK) {
    //        pStreamAdapter = gppRecoderStreamAdapter[index];
    //    }
    GLPOINTINSCREEN *flingerList = (GLPOINTINSCREEN *) malloc(sizeof(GLPOINTINSCREEN) * count);

    jint *xarr = env->GetIntArrayElements(xArr, &isCp);

    for (i = 0; i < count; i++) {
        flingerList[i].ScreenPoint.x = xarr[i];
    }
    env->ReleaseIntArrayElements(xArr, xarr, 0);

    jint *yarr = env->GetIntArrayElements(yArr, &isCp);

    for (i = 0; i < count; i++) {
        flingerList[i].ScreenPoint.y = yarr[i];
    }
    env->ReleaseIntArrayElements(yArr, yarr, 0);

    jint *heightarr = env->GetIntArrayElements(heightArr, &isCp);

    for (i = 0; i < count; i++) {
        flingerList[i].ScreenSize.Hei = heightarr[i];
    }
    env->ReleaseIntArrayElements(heightArr, heightarr, 0);

    jint *widarr = env->GetIntArrayElements(widthArr, &isCp);

    for (i = 0; i < count; i++) {
        flingerList[i].ScreenSize.Wid = widarr[i];
    }
    env->ReleaseIntArrayElements(widthArr, widarr, 0);
    realplayAdapter.PatternCtrlFlingerDown(flingerList, count, TickCount);
    if (flingerList != NULL) {
        free(flingerList);
    }
}
bool ConstructNVRPattern(JNIEnv *env, jobject glObject, PW_PATTERN_OUT* pattern,bool);
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_GetPatternWithTime
 * Signature: (ILjava/lang/String;IIII)I
 */
JNIEXPORT jint JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1GetPatternWithTime
(JNIEnv *env, jclass, jint type, jobject glObject, jint index, jint tickCount, jint height, jint width){
#define PATTERN_CREATE 1
#define PATTERN_CHANAGE 2
#define PATTERN_NOCHANGE 3

    PW_PATTERN_OUT* pattern = NULL;
    bool bInit = true;

    pattern = realplayAdapter.GetPattern(tickCount, height, width);
    if (pattern == NULL) {
        return 0;
    }

    switch (type) {
    case PATTERN_CREATE:
        bInit = true;
        break;
    default:
        bInit = false;
        break;

    }
    // LOGD("%s:---- > pattern %s", __FUNCTION__, "pattern");
    ConstructNVRPattern(env, glObject, pattern, bInit);
    // LOGD("%s:---- > pattern1 %s", __FUNCTION__, "pattern1");
    return 1;
}

long	GetTimer_ms()
{
    GUInt64 dw2Res = 0;
    struct timeval stTim;
    gettimeofday( &stTim, GNull );
    dw2Res = stTim.tv_sec * 1000 + PW_ROUND( stTim.tv_usec * 0.001f );

    return dw2Res;
}

bool ConstructNVRPattern(JNIEnv *env, jobject glObject, PW_PATTERN_OUT* pattern, bool bInit) {
#define DEFCLS(x) "com/puwell/opengles/"#x
#define DEFCLSSIG(x) "Lcom/puwell/opengles/"#x";"
#define DEFCLSARRSIG(x) "[Lcom/puwell/opengles/"#x";"

    bool bOutLog = false;
    long lTime1, lTime2;
    const PWPA_CAM_POSTURE *pstCurCamState;
    const PWPA_POSTURE *pstCurPosture;
    jint m, n, nOffset = 0, nGroupOffset = 0, nMemberTotal = 0, nVertexTotal = 0;
    jint nMemVerOffset = 0, nGrpMemOffset = 0;
    jint nPointCountRow, nPointCountCol, nPointCountLay;
    jfieldID fid_Temp;
    jmethodID mid_TempMethod;
    jfloatArray fVertexArray, fTextureArray, fVertexOriArray;
    jfloatArray fVertexBArray, fTextureBArray;
    //    if (bOutLog)
    //        LOGD("%s:---- > bInit is %d", __FUNCTION__, bInit);
    //    if (bOutLog)
    //        LOGD("%s:---- > message is %s", __FUNCTION__, "ConstructNVRPattern");

    jclass cls_NVRRender = env->GetObjectClass(glObject);
    jfieldID fid_NVRPattern = env->GetFieldID(cls_NVRRender, "pattern", DEFCLSSIG(NVRPattern));
    int len = 0;
    if (pattern == 0) {
        return false;
    }

    for (m = 0; m < pattern->lGroupCount; m++) {
        const PW_PATTERN_GROUP *pstGroup = pattern->pstPatternGroup + m;
        if (0 != pstGroup) {
            nGroupOffset = 0;
            for (n = 0; n < pstGroup->lMemberCount; n++) {
                const PW_PATTERN_MEM *pstPatternMem = pstGroup->pstPatternMember + n;
                if (0 != pstPatternMem) {
                    nPointCountRow = pstPatternMem->dwPointCountRow;
                    nPointCountCol = pstPatternMem->dwPointCountCol;
                    nPointCountLay = pstPatternMem->dwPointCountLay;
                    nOffset = nPointCountRow * nPointCountCol * nPointCountLay;
                    nVertexTotal += nOffset;
                }
            }
        }
        nMemberTotal += pstGroup->lMemberCount;
    }
    fVertexBArray = env->NewFloatArray(6 * 3);
    fTextureBArray = env->NewFloatArray(6 * 2);

    if (true == bInit) {
        fVertexArray = env->NewFloatArray(nVertexTotal * 3);
        fVertexOriArray = env->NewFloatArray(nVertexTotal * 3);
        fTextureArray = env->NewFloatArray(nVertexTotal * 2);
    }

    //    if (bOutLog)
    //        LOGD("%s:---- > nVertexTotal is %d", __FUNCTION__, nVertexTotal);

    jclass cls_NVRPattern = env->FindClass(DEFCLS(NVRPattern));
    jmethodID mid_NVRPattern = env->GetMethodID(cls_NVRPattern, "<init>", "()V");
    jobject obj_NVRPattern = env->NewObject(cls_NVRPattern, mid_NVRPattern);
    //	if (bOutLog)
    //		LOGD("%s:---- > message is %s", __FUNCTION__, "GroupCount");

    jmethodID mid_Reset = env->GetMethodID(cls_NVRPattern, "Reset", "(III)V");
    jmethodID mid_SetCam = env->GetMethodID(cls_NVRPattern, "SetCam", "(IZFFFFFFFFF)V");
    jmethodID mid_SetPos = env->GetMethodID(cls_NVRPattern, "SetPosture", "(IIZFFFZFFFZFFF)V");
    jmethodID mid_SetGrpFlag = env->GetMethodID(cls_NVRPattern, "SetGroupFlag", "(IIII)V");
    jmethodID mid_SetMemFlag = env->GetMethodID(cls_NVRPattern, "SetMemFlag", "(IIIII)V");
    jmethodID mid_SetData = env->GetMethodID(cls_NVRPattern, "SetData", "(I[F[F)V");
    jmethodID mid_SetData_2 = env->GetMethodID(cls_NVRPattern, "SetData_2", "(I[F[F[F)V");
    jmethodID mid_setData_Board = env->GetMethodID(cls_NVRPattern, "SetData_Board", "([F[F)V");
    jmethodID mid_SetVer2Ori = env->GetMethodID(cls_NVRPattern, "SetGroupVer2Ori", "(IF)V");
    jmethodID mid_SetPriority = env->GetMethodID(cls_NVRPattern, "SetPriority", "(II)V");

    jmethodID mid_SetRotate_Board = env->GetMethodID(cls_NVRPattern, "SetRotate_Board", "(IIIFFF)V");
    jmethodID mid_SetTrans_Board = env->GetMethodID(cls_NVRPattern, "SetTrans_Board", "(IIIFFF)V");
    jmethodID mid_SetScale_Board = env->GetMethodID(cls_NVRPattern, "SetScale_Board", "(IIIFFF)V");
    //	public void SetData_Board( float[] pfVertexList, float[] pfTextureList )
    //	public void SetRotate_Board( int iPriX, int iPriY, int iPriZ,  )
    //	public void SetTrans_Board( int iPriX, int iPriY, int iPriZ,  )
    //	public void SetScale_Board( int iPriX, int iPriY, int iPriZ,  )

    //  LOGW("CallVoidMethod mid_Reset BF");
    env->CallVoidMethod(obj_NVRPattern, mid_Reset, pattern->lGroupCount, nMemberTotal, nVertexTotal);
    //  LOGW("CallVoidMethod mid_Reset AF");
    mid_TempMethod = env->GetMethodID(cls_NVRPattern, "SetBackColor", "(FFFF)V");
    env->CallVoidMethod(obj_NVRPattern, mid_TempMethod, pattern->fBackColor[0], pattern->fBackColor[1], pattern->fBackColor[2], pattern->fBackColor[3]);

    pstCurCamState = &pattern->stCamState;
    env->CallVoidMethod(obj_NVRPattern, mid_SetCam, -1, pstCurCamState->bEnable, pstCurCamState->fViewAngle, pstCurCamState->fMinDistance, pstCurCamState->fMaxDistance, pstCurCamState->fPan,
                        pstCurCamState->fTilt, pstCurCamState->fRotate, pstCurCamState->fPosiX, pstCurCamState->fPosiY, pstCurCamState->fPosiZ);

    pstCurPosture = &pattern->stWorldPosture;
    env->CallVoidMethod(obj_NVRPattern, mid_SetPos, -1, -1, pstCurPosture->bNeedRotate, pstCurPosture->fRotate[0], pstCurPosture->fRotate[1], pstCurPosture->fRotate[2], pstCurPosture->bNeedTranslate,
            pstCurPosture->fTranslate[0], pstCurPosture->fTranslate[1], pstCurPosture->fTranslate[2], pstCurPosture->bNeedScale, pstCurPosture->fScale[0], pstCurPosture->fScale[1],
            pstCurPosture->fScale[2]);

    if (true == bInit) {
        nMemVerOffset = 0;
        nGrpMemOffset = 0;

        jfloatArray fTempArray = env->NewFloatArray(3);

        for (m = 0; m < pattern->lGroupCount; m++) {
            const PW_PATTERN_GROUP *pstGroup = pattern->pstPatternGroup + m;
            if (0 != pstGroup) {
                nGroupOffset = 0;
                env->CallVoidMethod(obj_NVRPattern, mid_SetGrpFlag, m, pstGroup->lMemberCount, nGrpMemOffset, nMemVerOffset);

                for (n = 0; n < pstGroup->lMemberCount; n++) {
                    const PW_PATTERN_MEM *pstPatternMem = pstGroup->pstPatternMember + n;
                    if (0 != pstPatternMem) {
                        nPointCountRow = pstPatternMem->dwPointCountRow;
                        nPointCountCol = pstPatternMem->dwPointCountCol;
                        nPointCountLay = pstPatternMem->dwPointCountLay;
                        env->CallVoidMethod(obj_NVRPattern, mid_SetMemFlag, nGrpMemOffset, nPointCountRow, nPointCountCol, nPointCountLay, nMemVerOffset);
                        nOffset = nPointCountRow * nPointCountCol * nPointCountLay;

                        env->SetFloatArrayRegion(fVertexArray, nMemVerOffset * 3, nOffset * 3, pstPatternMem->pfVertexList_3);
                        env->SetFloatArrayRegion(fVertexOriArray, nMemVerOffset * 3, nOffset * 3, pstPatternMem->pfOriVertexList_3);
                        env->SetFloatArrayRegion(fTextureArray, nMemVerOffset * 2, nOffset * 2, pstPatternMem->pfTexCordList_2);

                        nMemVerOffset += nOffset;
                    }
                    nGrpMemOffset++;
                }
            }
        }


    }

    //    if (bOutLog)
    //        LOGD("%s:---- > message is %s", __FUNCTION__, "pstPatternMem");
    lTime1 = GetTimer_ms();
    for (m = 0; m < pattern->lGroupCount; m++) {
        const PW_PATTERN_GROUP *pstGroup = pattern->pstPatternGroup + m;
        if (0 != pstGroup) {

            //			pstCurCamState = &(pstGroup->stCamState);
            //			env->CallVoidMethod(obj_NVRPattern, mid_SetCam, m, pstCurCamState->bEnable, pstCurCamState->fViewAngle, pstCurCamState->fMinDistance, pstCurCamState->fMaxDistance, pstCurCamState->fPan,
            //					pstCurCamState->fTilt, pstCurCamState->fRotate, pstCurCamState->fPosiX, pstCurCamState->fPosiY, pstCurCamState->fPosiZ);
            //			pstCurPosture = &(pstGroup->stGroupPosture);
            //			env->CallVoidMethod(obj_NVRPattern, mid_SetPos, m, -1, pstCurPosture->bNeedRotate, pstCurPosture->fRotate[0], pstCurPosture->fRotate[1], pstCurPosture->fRotate[2],
            //					pstCurPosture->bNeedTranslate, pstCurPosture->fTranslate[0], pstCurPosture->fTranslate[1], pstCurPosture->fTranslate[2], pstCurPosture->bNeedScale, pstCurPosture->fScale[0],
            //					pstCurPosture->fScale[1], pstCurPosture->fScale[2]);
            env->CallVoidMethod(obj_NVRPattern, mid_SetVer2Ori, m, pstGroup->fVerToOri, nGrpMemOffset, nMemVerOffset);
            int nTemp = (int)pstGroup->dwPriority;
            // LOGW("%s:---- > message is %d", __FUNCTION__, nTemp);
            env->CallVoidMethod(obj_NVRPattern, mid_SetPriority, m, nTemp );

            /*			for (n = 0; n < pstGroup->lMemberCount; n++) {
             const PW_PATTERN_MEM *pstPatternMem = pstGroup->pstPatternMember + n;
             if (0 != pstPatternMem) {
             pstCurPosture = &(pstPatternMem->stMemberPosture);
             env->CallVoidMethod(obj_NVRPattern, mid_SetPos, m, n, pstCurPosture->bNeedRotate, pstCurPosture->fRotate[0], pstCurPosture->fRotate[1], pstCurPosture->fRotate[2],
             pstCurPosture->bNeedTranslate, pstCurPosture->fTranslate[0], pstCurPosture->fTranslate[1], pstCurPosture->fTranslate[2], pstCurPosture->bNeedScale,
             pstCurPosture->fScale[0], pstCurPosture->fScale[1], pstCurPosture->fScale[2]);
             }
             }*/
        }
    }

    if( 0 != pattern->pstPatternGroup_2 ){
        env->SetFloatArrayRegion(fVertexBArray, 0, 18, pattern->pstPatternGroup_2->pstPatternMember->pfVertexList_3);
        env->SetFloatArrayRegion(fTextureBArray, 0, 12, pattern->pstPatternGroup_2->pstPatternMember->pfTexCordList_2);

        env->CallVoidMethod(obj_NVRPattern, mid_setData_Board, fVertexBArray, fTextureBArray);
        int iPriX, iPriY, iPriZ;
        iPriX = pattern->pstPatternGroup_2->stGroupPosture.ullPrioRotateX;
        iPriY = pattern->pstPatternGroup_2->stGroupPosture.ullPrioRotateY;
        iPriZ = pattern->pstPatternGroup_2->stGroupPosture.ullPrioRotateZ;
        env->CallVoidMethod(obj_NVRPattern, mid_SetRotate_Board,
                            iPriX, iPriY, iPriZ,
                            pattern->pstPatternGroup_2->stGroupPosture.fRotate[0],
                pattern->pstPatternGroup_2->stGroupPosture.fRotate[1],
                pattern->pstPatternGroup_2->stGroupPosture.fRotate[2]
                );
        iPriX = pattern->pstPatternGroup_2->stGroupPosture.ullPrioTransX;
        iPriY = pattern->pstPatternGroup_2->stGroupPosture.ullPrioTransY;
        iPriZ = pattern->pstPatternGroup_2->stGroupPosture.ullPrioTransZ;
        env->CallVoidMethod(obj_NVRPattern, mid_SetTrans_Board,
                            iPriX, iPriY, iPriZ,
                            pattern->pstPatternGroup_2->stGroupPosture.fTranslate[0],
                pattern->pstPatternGroup_2->stGroupPosture.fTranslate[1],
                pattern->pstPatternGroup_2->stGroupPosture.fTranslate[2]
                );
        iPriX = pattern->pstPatternGroup_2->stGroupPosture.ullPrioScaleX;
        iPriY = pattern->pstPatternGroup_2->stGroupPosture.ullPrioScaleY;
        iPriZ = pattern->pstPatternGroup_2->stGroupPosture.ullPrioScaleZ;
        env->CallVoidMethod(obj_NVRPattern, mid_SetScale_Board,
                            iPriX, iPriY, iPriZ,
                            pattern->pstPatternGroup_2->stGroupPosture.fScale[0],
                pattern->pstPatternGroup_2->stGroupPosture.fScale[1],
                pattern->pstPatternGroup_2->stGroupPosture.fScale[2]
                );
    }
    if (true == bInit) {
        if (bOutLog)
            LOGD("%s:---- > mid_SetData_2 is", __FUNCTION__);

        //	env->CallVoidMethod(obj_NVRPattern, mid_SetData, nMemVerOffset, fVertexArray, fTextureArray);
        env->CallVoidMethod(obj_NVRPattern, mid_SetData_2, nMemVerOffset, fVertexArray, fVertexOriArray, fTextureArray);


    }

    lTime2 = GetTimer_ms();
    if (bOutLog)
        LOGD("%s:---- > Pattern cost is %d", __FUNCTION__, lTime2 - lTime1);

    env->SetObjectField(glObject, fid_NVRPattern, obj_NVRPattern);
    return true;
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_init_opengl
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1init_1opengl
(JNIEnv *, jclass, jint){
    realplayAdapter.InitPlayerGL();
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_uninit_opengl
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1uninit_1opengl
(JNIEnv *, jclass, jint){
    realplayAdapter.UninitPlayerGL();
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_GetTalkState
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1GetTalkState
(JNIEnv *env, jclass, jint cameraid){
    return PW_NET_GetTalkState(cameraid);
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_TalkCtrl
 * Signature: (IZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1TalkCtrl
(JNIEnv *env, jclass, jint cameraId, jboolean bstate){
    return PW_NET_TalkCtrl(cameraId,bstate);
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_SendTalkData
 * Signature: (I[BJ)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1SendTalkData
(JNIEnv *env, jclass, jint cameraId, jbyteArray byteArr, jlong size,jstring path){
    char* data=(char*)malloc(sizeof(char)*size);
    memset(data,0,sizeof(char)*size);
    env->GetByteArrayRegion(byteArr,0,size,(jbyte*)data);
    const char* cpath=env->GetStringUTFChars(path,NULL);
       // LOGW("PATH:%s",cpath);
//        FILE* file= fopen(cpath,"ab+");
//        fwrite(data,size,1,file);
//        fclose(file);
    env->ReleaseStringUTFChars(path,cpath);
 //   LOGE("PW_NET_SendTalkData cameraId:%d",cameraId);
    bool bres=PW_NET_SendTalkData(cameraId,data,size);
    free(data);
    return bres;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    PW_NET_ModifyDevName
 * Signature: (ILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_ipc365_app_showmo_jni_JniClient_PW_1NET_1ModifyDevName
(JNIEnv *env, jclass, jint cameraId, jstring name){
    const char* cname=env->GetStringUTFChars(name,NULL);
    LOGI("PW_NET_ModifyDevName device:%d name:%s",cameraId,cname);
    bool bret= PW_NET_ModifyDevName((int)cameraId,(char*)cname);
    if(!bret){
        printErr("PW_NET_ModifyDevName",0);
    }
    return bret;
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_Stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1Stop(JNIEnv *env, jclass) {
    realplayAdapter.MontagePlayStop();
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_Start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1Start
(JNIEnv *env, jclass) {
    realplayAdapter.MontagePlayStart();

}
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1reset
(JNIEnv *, jclass){
    realplayAdapter.ResetPlayerGl();
}
/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_setPpi
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1setPpi
(JNIEnv *env, jclass, jfloat ppi){
    realplayAdapter.setGlPpi(ppi);
}

/*
 * Class:     ipc365_app_showmo_jni_JniClient
 * Method:    native_mpgl_setPpiXy
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_ipc365_app_showmo_jni_JniClient_native_1mpgl_1setPpiXy
(JNIEnv *env, jclass, jfloat ppix, jfloat ppiy){
    realplayAdapter.setGlPpiXy(ppix,ppiy);
}
#ifdef __cplusplus
}
#endif
