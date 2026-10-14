//
// Created by yige on 2026/10/10.
//

#ifndef AS_JNI_PROJECT_JNICALLBACKHELPER_H
#define AS_JNI_PROJECT_JNICALLBACKHELPER_H

#include <jni.h>
#include "utils.h"

class JNiCallBackHelper {

private:
    JavaVM *javaVm_ = nullptr;
    JNIEnv *env_ = nullptr;
    jobject thiz_;
    jmethodID method_id;
public:
    JNiCallBackHelper(JavaVM *javaVm, JNIEnv *env, jobject thiz);
    virtual ~JNiCallBackHelper();

    void prepare(int thread_mode);
};


#endif //AS_JNI_PROJECT_JNICALLBACKHELPER_H
