#include"JniObject.h"


JniObject::JniObject(JNIEnv* env,jobject obj){
    mEnv=env;
    mObj=obj;
    mClass=env->GetObjectClass(obj);
}

JniObject::JniObject(JNIEnv* env,const char* classname){
    mEnv=env;
    mObj=NULL;
    char *jniclass=(char*)malloc(sizeof(char)*(strlen(classname)));
    memset(jniclass,0,sizeof(char)*(strlen(classname)));
    transJavaClassToJniClass(classname,jniclass);
    LOGI("JniObject::JniObject transJavaClasS: %s",jniclass);
    mClass=env->FindClass(jniclass);
    jmethodID construId=env->GetMethodID(mClass,"<init>","()V");
    mObj=env->NewObject(mClass,construId);
}
JniObject::~JniObject(){
    map<string,JniField*>::const_iterator itrF;// mFieldMap;
    map<string,JniMethod*>::const_iterator itrM;//mMethodMap;
    for(itrF=mFieldMap.begin();itrF!=mFieldMap.end();itrF++){
        delete itrF->second;
    }
    for(itrM=mMethodMap.begin();itrM!=mMethodMap.end();itrM++){
        delete itrM->second;
    }
}

jobject JniObject::getObj(){
    return mObj;
}
JniField* JniObject::findAndStoreField(const char* fieldName,const  char* sig,FIELDTYPE type){
    JniField* field=NULL;
    map<string,JniField*>::const_iterator itr=mFieldMap.find(fieldName);
    if(itr==mFieldMap.end()){
        jfieldID fid =mEnv->GetFieldID(mClass,fieldName,sig);

        switch (type) {
        case IntField:
            field=new JniIntField(this,fid);
            break;
        case StringField:
            field=new JniStringField(this,fid);
            break;
        case ObjectField:
            field=new JniObjectField(this,fid);
            break;
        case LongField:
            field=new JniLongField(this,fid);
            break;
        case FloatField:
            field=new JniFloatField(this,fid);
            break;
        case DoubleField:
            field=new JniDoubleField(this,fid);
            break;
        case ShortField:
            field=new JniShortField(this,fid);
            break;
        case BooleanField:
            field=new JniBooleanField(this,fid);
            break;
        default:
            break;
        }

        if(field!=NULL)
            mFieldMap.insert(make_pair(fieldName,field));
    }else{
        field=itr->second;
        LOGI("########%s is already in map ",fieldName);
    }
    return field;
}
JniMethod* JniObject::findAndStoreMehod(const char* methodName,const char* sig){
    JniMethod *method=NULL;
    map<string,JniMethod*>::const_iterator itr=mMethodMap.find(methodName);
    if(itr==mMethodMap.end()){
        jmethodID mid=mEnv->GetMethodID(mClass,methodName,sig);
        method=new JniMethod(this,mid);
        if(method!=NULL)
            mMethodMap.insert(make_pair(methodName,method));
    }else{
        method=itr->second;
        LOGI("########%s is already in map ",methodName);
    }
    return method;
}

void JniObject::updateEnv(JNIEnv* env){
    mEnv=env;
}
JNIEnv* JniObject::getEnv(){
    return mEnv;
}

JniIntField* JniObject::getIntField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"I",IntField);
    return dynamic_cast<JniIntField*>(field);
}
JniLongField* JniObject::getLongField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"J",LongField);
    return dynamic_cast<JniLongField*>(field);
}

JniStringField* JniObject::getStringField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"Ljava/lang/String;",StringField);
    return dynamic_cast<JniStringField*>(field);
}
JniObjectField* JniObject::getObjectField(const char* fieldName,const char* fieldSig){
    JniField* field=findAndStoreField(fieldName,fieldSig,ObjectField);
    return dynamic_cast<JniObjectField*>(field);
}
JniFloatField* JniObject::getFloatField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"F",FloatField);
    return dynamic_cast<JniFloatField*>(field);
}

JniDoubleField* JniObject::getDoubleField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"D",DoubleField);
    return dynamic_cast<JniDoubleField*>(field);
}

JniShortField* JniObject::getShortField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"S",ShortField);
    return dynamic_cast<JniShortField*>(field);
}
JniBooleanField* JniObject::getBooleanField(const char* fieldName){
    JniField* field=findAndStoreField(fieldName,"Z",BooleanField);
    return dynamic_cast<JniBooleanField*>(field);
}

////杩欓噷閲囩敤浜厓妯″紡锛屼繚瀛樻墍鏈夎幏鍙栬繃鐨凧niField锛屾瀽鏋勭殑鏃跺�欑粺涓�閲婃斁锛屽娆¤幏鍙栦娇鐢↗niField鐨勫悓涓�涓璞�

JniMethod* JniObject::getIntMethod(const char* MethodName,const char* Signal){
    return findAndStoreMehod(MethodName,Signal);
}
