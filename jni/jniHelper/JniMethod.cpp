
#include"JniMethod.h"
#include"JniObject.h"

JniMethod::JniMethod(JniObject* targetObj,jmethodID mid){
    mTargetObj=targetObj;
    mMid=mid;
}

jobject JniMethod::getObj(){
    return mTargetObj->getObj();
}

jmethodID JniMethod::getMid(){
    return mMid;
}
