//
// Created by yige on 2026/10/10.
//


#include "JNiCallBackHelper.h"

JNiCallBackHelper::JNiCallBackHelper(JavaVM *javaVm, JNIEnv *env, jobject thiz) {
    javaVm_ = javaVm;
    env_ = env;
    thiz_ = env->NewGlobalRef(thiz);
    jclass class1 = env_->GetObjectClass(thiz_);
    method_id = env_->GetMethodID(class1, "onPrapare", "()V");
}
JNiCallBackHelper::~JNiCallBackHelper() {
    javaVm_ = nullptr;
    env_->DeleteGlobalRef(thiz_);
    thiz_ = nullptr;
    env_ = nullptr;
}
void JNiCallBackHelper::prepare(int thread_mode){
    if(thread_mode == THREAD_MAIN){
        //主线程 直接调用
        env_->CallVoidMethod(thiz_,method_id);
    }else if(thread_mode == THREAD_CHILD){
        //子线程
        JNIEnv *child_thread_env = nullptr;
        javaVm_->AttachCurrentThread(&child_thread_env, nullptr);
        child_thread_env->CallVoidMethod(thiz_,method_id);
        javaVm_->DetachCurrentThread();
    }
}