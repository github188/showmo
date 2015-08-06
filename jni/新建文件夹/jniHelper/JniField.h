#ifndef JNIFIELD
#define JNIFIELD
#include<jni.h>
#include"JniGlobalDef.h"
class JniObject;
class JniField{
public:
   JniField(JniObject* targetObj,jfieldID fid){
       mTargetObj=targetObj;
       mFid=fid;
   }
   virtual ~JniField(){}
   jobject getTargetObj();
protected:
   JniObject* mTargetObj;
   jfieldID mFid;
};

class JniIntField:public JniField{
public:
    JniIntField(JniObject* targetObj,jfieldID fieldId);
    int get();
    JniObject* set(JNI_IN int value);


};

class JniLongField:public JniField{
public:
    JniLongField(JniObject* targetObj,jfieldID fieldId);
    long get();
    JniObject* set(JNI_IN long value);
};

class JniFloatField:public JniField{
public:
    JniFloatField(JniObject* targetObj,jfieldID fieldId);
    float get();
    JniObject* set(JNI_IN float value);
};

class JniDoubleField:public JniField{
public:
    JniDoubleField(JniObject* targetObj,jfieldID fieldId);
    double get();
    JniObject* set(JNI_IN double value);
};


class JniObjectField:public JniField{
public:
    JniObjectField(JniObject* targetObj,jfieldID fid);
    jobject get();
    JniObject* set(JNI_IN jobject value);//jobject*
};

class JniShortField:public JniField{
public:
    JniShortField(JniObject* targetObj,jfieldID fieldId);
    short get();
    JniObject* set(JNI_IN short value);
};

class JniStringField:public JniField{
public:
    JniStringField(JniObject* targetObj,jfieldID fid);
    jstring get();
    char* getCharStr();//transform jstring field to char* out，you should releaseChar()it or just free it
    void releaseChar(char* strField);
    JniObject* set(JNI_IN char* value);//char*
    JniObject*set(JNI_IN jstring value);
};

#endif // JNIFIELD

