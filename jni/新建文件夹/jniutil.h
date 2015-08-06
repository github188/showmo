#ifndef JNIUTIL_H
#define JNIUTIL_H

#include<android/log.h>
#define JNITAG ("jnimsg")
//#define NODEBUG

#define LogV(TAG,MSG) __android_log_write(ANDROID_LOG_VERBOSE, TAG, MSG)

#define LOG_TAG "===ShowMoNavite==="

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#ifdef NODEBUG
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGSTART() ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s::START!!!", __FUNCTION__))
#define LOGEND() ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s::END!!!", __FUNCTION__))
#else
#define LOGD(...)
#define LOGSTART()
#define LOGEND()
#define LOGI(...)
//#define LOGW(...)
//#define LOGE(...)
#endif



#endif
