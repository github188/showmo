#ifndef JNIWRAPER
#define JNIWRAPER

#include<jni.h>
#include"jniutil.h"
#include<stdlib.h>
#include<stdio.h>
char*   jstringToCharStr(JNIEnv*   env,   jstring   jstr)
{
    char* rtn = NULL;
    jclass   clsstring   =   env->FindClass("java/lang/String");
    jstring   strencode   =   env->NewStringUTF("UTF-8");
    jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
    jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
    jsize   alen   =   env->GetArrayLength(barr);
    jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
    if(alen   >   0)
    {
        rtn   =   (char*)malloc(alen+1);         //new   char[alen+1];
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }

    env->ReleaseByteArrayElements(barr,ba,0);
    return rtn;
}
jstring charStrtoJstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}
bool jni_exception_throw(JNIEnv* env,const char* className,const char* msg){
    jclass exceptionClass=env->FindClass(className);
    if(exceptionClass==NULL){
        return false;
    }
    env->ThrowNew(exceptionClass,msg);
    env->DeleteLocalRef(exceptionClass);
    return true;
}
bool jni_exception_check_and_throw(JNIEnv* env,const char* className,const char* msg){
    jthrowable exc=env->ExceptionOccurred();
    if(exc!=NULL){
       // LogV(JNITAG,"ExceptionOccurred !=NULL ");
        env->ExceptionDescribe();
        env->ExceptionClear();
       // LogV(JNITAG,"jni_exception_throw bf ");
        jni_exception_throw(env,className,msg);
       // LogV(JNITAG,"jni_exception_throw af ");
        return true;
    }
    return false;
}
void jniSetIntField(JNIEnv* env, jclass clazz,jobject obj,char* fieldname,jint value){
    jfieldID fid=env->GetFieldID(clazz,fieldname,"I");
    env->SetIntField(obj,fid,value);
}
void jniSetObjectField(JNIEnv* env, jclass clazz,jobject targetObj,char* fieldname,char* sig,jobject value){
    jfieldID fid=env->GetFieldID(clazz,fieldname,sig);
    env->SetObjectField(targetObj,fid,value);
}

int jniGetIntField(JNIEnv* env,jobject obj,char* fieldname){
    jclass clazz=env->GetObjectClass(obj);
    jfieldID fid=env->GetFieldID(clazz,fieldname,"I");
    return (int)env->GetIntField(obj,fid);
}
jstring jniGetStringField(JNIEnv* env,jobject obj,char* fieldname){
    jclass clazz=env->GetObjectClass(obj);
    jfieldID fid=env->GetFieldID(clazz,fieldname,"Ljava/lang/String;");
    return (jstring)env->GetObjectField(obj,fid);
}
char* jniGetStringFieldCopyToNewChar(JNIEnv* env,jobject obj,char* fieldname,int* length=NULL){
    jclass clazz=env->GetObjectClass(obj);
    jfieldID fid=env->GetFieldID(clazz,fieldname,"Ljava/lang/String;");
     jstring jstr=(jstring)env->GetObjectField(obj,fid);
     const char* cstr=env->GetStringUTFChars(jstr,NULL);
     char* c_char=(char*)malloc(sizeof(char)*(strlen(cstr)+1));
     memset(c_char,0,sizeof(char)*(strlen(cstr)+1));
     strcpy(c_char,cstr);
     if(length!=NULL){
        (*length)=strlen(c_char);
     }
     env->ReleaseStringUTFChars(jstr,cstr);
     return c_char;
}
char* jniNewCharArrFromStringField(JNIEnv* env,jobject obj,char* fieldname,int* len){
    jclass clazz=env->GetObjectClass(obj);
    jfieldID fid=env->GetFieldID(clazz,fieldname,"Ljava/lang/String;");
     jstring jstr=(jstring)env->GetObjectField(obj,fid);

     jclass strclazz=env->FindClass("java/lang/String");
     jmethodID mid=env->GetMethodID(strclazz,"length","()I");
     (*len)=env->CallIntMethod(jstr,mid);
     const char* cstr=env->GetStringUTFChars(jstr,NULL);

     char* des=(char*)malloc(sizeof(char)*((*len)+1));
     memset(des,0,sizeof(char)*(*len));
     strcpy(des,cstr);
     env->ReleaseStringUTFChars(jstr,cstr);
     return des;
}

jobject jniGetObjectField(JNIEnv* env, jobject obj,char* fieldname,char* sig){
    jclass clazz=env->GetObjectClass(obj);
    jfieldID fid=env->GetFieldID(clazz,fieldname,sig);
    return env->GetObjectField(obj,fid);
}
void jniSetStringField(JNIEnv* env, jclass clazz,jobject obj,char* fieldname,char* value){
    jfieldID fid=env->GetFieldID(clazz,fieldname,"Ljava/lang/String;");
    jstring jstr=env->NewStringUTF(value);
    env->SetObjectField(obj,fid,jstr);
    env->DeleteLocalRef(jstr);
}
jobject jniNewInitObjectByOtherObject(JNIEnv* env,jobject obj,jclass* pclazz){
    (*pclazz)=env->GetObjectClass(obj);
    if((*pclazz)==NULL){
        LogV(JNITAG,"(*pclazz)==NULL");
        char msg[256];
        sprintf(msg,"can not find class exception");
        jni_exception_check_and_throw(env,"java/lang/Exception",msg);
        return NULL;
    }else{
        jmethodID conMid=env->GetMethodID((*pclazz),"<init>","()V");
        return env->NewObject((*pclazz),conMid);
    }
}

jobject jniNewInitObject(JNIEnv* env,char* classname,jclass* pclazz){
    //LogV(JNITAG,"FindClass bf");
   // LogV(JNITAG,classname);
    (*pclazz)=env->FindClass(classname);
    //LogV(JNITAG,"FindClass af");

    if((*pclazz)==NULL){
        LogV(JNITAG,"(*pclazz)==NULL");
        char msg[256];
        sprintf(msg,"can not find class exception####classname:%s",classname);
        jni_exception_check_and_throw(env,"java/lang/Exception",msg);
        return NULL;
    }else{
        jmethodID conMid=env->GetMethodID((*pclazz),"<init>","()V");
        return env->NewObject((*pclazz),conMid);
    }
}
#endif // JNIWRAPER

