#ifndef JNIOBJECT
#define JNIOBJECT
#include"jniutils.h"
#include<map>
#include<string>
#include"JniField.h"
#include"JniMethod.h"
using namespace std;
/*
 * @author lss
 * wrapper jni jobject
 *
 */

class JniObject{
private:
    jclass mClass;
    jobject mObj;
    JNIEnv* mEnv;
    map<string,JniField*> mFieldMap;
    map<string,JniMethod*> mMethodMap;

    enum FIELDTYPE{
        IntField,StringField,ObjectField,LongField,DoubleField,ShortField,FloatField
    };
public:
    JniObject(JNIEnv* env,jobject obj);
    JniObject(JNIEnv* env,const char* classname);//
    ~JniObject();
    jobject getObj();
    JniIntField* getIntField(const char* fieldName);
    JniLongField* getLongField(const char* fieldName);
    //杩欓噷閲囩敤浜厓妯″紡锛屼繚瀛樻墍鏈夎幏鍙栬繃鐨凧niField锛屾瀽鏋勭殑鏃跺�欑粺涓�閲婃斁锛屽娆¤幏鍙栦娇鐢↗niField鐨勫悓涓�涓璞�
    JniStringField* getStringField(const char* fieldName);
    JniObjectField* getObjectField(const char* fieldName,const char* fieldSig);
    JniFloatField* getFloatField(const char* fieldName);
    JniDoubleField* getDoubleField(const char* fieldName);
    JniShortField* getShortField(const char* fieldName);
    JniMethod* getIntMethod(const char* MethodName,const char* Signal);

    void updateEnv(JNIEnv* env);
    JNIEnv* getEnv();
private:
    JniField* findAndStoreField(const char* fieldname,const char* sig,FIELDTYPE type);
    JniMethod* findAndStoreMehod(const char* methodName,const char* sig);
};

#endif // JNIOBJECT

