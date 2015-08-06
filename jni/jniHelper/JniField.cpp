#include"JniField.h"
#include"JniObject.h"
jobject JniField::getTargetObj(){
return mTargetObj->getObj();
}

JniIntField::JniIntField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){
}
int JniIntField::get(){
    int v=mTargetObj->getEnv()->GetIntField(mTargetObj->getObj(),mFid);
    return (int)v;
}
JniObject* JniIntField::set(JNI_IN int value){
    mTargetObj->getEnv()->SetIntField(mTargetObj->getObj(),mFid,value);
    return mTargetObj;
}

JniLongField::JniLongField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){
}
long JniLongField::get(){
    jlong v=mTargetObj->getEnv()->GetLongField(mTargetObj->getObj(),mFid);
    return (long)v;
}
JniObject* JniLongField::set(JNI_IN long value){
    mTargetObj->getEnv()->SetLongField(mTargetObj->getObj(),mFid,(jlong)value);
    return mTargetObj;
}

JniDoubleField::JniDoubleField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){
}
double JniDoubleField::get(){
    jdouble v=mTargetObj->getEnv()->GetDoubleField(mTargetObj->getObj(),mFid);
    return (double)v;
}
JniObject* JniDoubleField::set(JNI_IN double value){
    mTargetObj->getEnv()->SetDoubleField(mTargetObj->getObj(),mFid,(jdouble)value);
    return mTargetObj;
}

JniFloatField::JniFloatField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){
}
float JniFloatField::get(){
    jfloat v=mTargetObj->getEnv()->GetFloatField(mTargetObj->getObj(),mFid);
    return (float)v;
}
JniObject* JniFloatField::set(JNI_IN float value){
    mTargetObj->getEnv()->SetFloatField(mTargetObj->getObj(),mFid,(jfloat)value);
    return mTargetObj;
}
JniBooleanField::JniBooleanField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){

}

bool JniBooleanField::get(){
    bool v=mTargetObj->getEnv()->GetBooleanField(mTargetObj->getObj(),mFid);
    return (bool)v;
}

JniObject* JniBooleanField::set(JNI_IN bool value){
    mTargetObj->getEnv()->SetBooleanField(mTargetObj->getObj(),mFid,(bool)value);
    return mTargetObj;
}
JniShortField::JniShortField(JniObject* targetObj,jfieldID fieldId):JniField(targetObj,fieldId){
}
short JniShortField::get(){
    jshort v=mTargetObj->getEnv()->GetShortField(mTargetObj->getObj(),mFid);
    return (short)v;
}
JniObject* JniShortField::set(JNI_IN short value){
    mTargetObj->getEnv()->SetShortField(mTargetObj->getObj(),mFid,(jshort)value);
    return mTargetObj;
}

JniObjectField::JniObjectField(JniObject* targetObj,jfieldID fid):JniField(targetObj,fid){
}
jobject JniObjectField::get(){
    jobject obj=mTargetObj->getEnv()->GetObjectField(mTargetObj->getObj(),mFid);
    return obj;
}
JniObject* JniObjectField::set(JNI_IN jobject value){
    mTargetObj->getEnv()->SetObjectField(mTargetObj->getObj(),mFid,value);
    return mTargetObj;
}

JniStringField::JniStringField(JniObject* targetObj,jfieldID fid):JniField(targetObj,fid){
}
char* JniStringField::getCharStr(){
    jstring jstr=(jstring)mTargetObj->getEnv()->GetObjectField(mTargetObj->getObj(),mFid);
    const char *cStr= mTargetObj->getEnv()->GetStringUTFChars(jstr,NULL);
    char* value=(char*)malloc(sizeof(char)*(strlen(cStr)+1));
    memset(value,0,sizeof(char)*(strlen(cStr)+1));
    strcpy(value,cStr);
    mTargetObj->getEnv()->ReleaseStringUTFChars(jstr,cStr);
    return value;
}
void JniStringField::releaseChar(char* strField){
    free(strField);
}
jstring  JniStringField::get(){
    jstring value=(jstring)mTargetObj->getEnv()->GetObjectField(mTargetObj->getObj(),mFid);
    return value;
}
JniObject* JniStringField::set(JNI_IN jstring value){
    mTargetObj->getEnv()->SetObjectField(mTargetObj->getObj(),mFid,value);
    return mTargetObj;
}
JniObject* JniStringField::set(JNI_IN char* value){
    char* v=value;
    jstring jstr=mTargetObj->getEnv()->NewStringUTF(v);
    mTargetObj->getEnv()->SetObjectField(mTargetObj->getObj(),mFid,jstr);
    mTargetObj->getEnv()->DeleteLocalRef(jstr);
    return mTargetObj;
}

