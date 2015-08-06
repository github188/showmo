#include"jniutils.h"

void transJavaClassToJniClass(JNI_IN const char* src, JNI_OUT char* to){
    char* p_s=const_cast<char*>(src);
    char* p_d=to;
    while((*p_s)!='\0'){
        if((*p_s)!='.'){
            (*p_d)=(*p_s);
        }else{
            (*p_d)='/';
        }
        p_s++;
        p_d++;
    }
    (*p_d)='\0';
}
