#ifndef JNIMETHOD
#define JNIMETHOD
#include<jni.h>

//#define M_D(p_target) p_target##->getObj(),p_target##->getMid()
class JniObject;
class JniMethod{
 public:
    JniMethod(JniObject* targetObj,jmethodID mid);
    jobject getObj();
    jmethodID getMid();
private:
    JniObject* mTargetObj;
    jmethodID mMid;
};

#endif // JNIMETHOD

