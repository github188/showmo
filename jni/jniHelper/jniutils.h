#ifndef JNIUTILS_H
#define JNIUTILS_H
#include"JniGlobalDef.h"
#include<string.h>
#include<stdlib.h>
#include<android/log.h>
#include<jni.h>
#define TAG "jni"
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
/*
 * 将java类转换为jni的表示方式如java.lang.String   -->java/lang/String
 *
 *
 *
 **/
extern void  transJavaClassToJniClass(JNI_IN const char* src, JNI_OUT char* to);

#endif // JNIUTILS_H

