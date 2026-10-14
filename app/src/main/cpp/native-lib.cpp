#include <jni.h>
#include <string>
#include <android/log.h>
#include "YiGePlay.h"
#include "JNiCallBackHelper.h"


#define LOG_TAG "MY_JNI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_as_1jni_1project_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_changeNumber(JNIEnv *env, jobject thiz) {
//    jclass classMainActivity = env->FindClass("com/example/as_jni_project/MainActivity");
    jclass classMainActivity =env->GetObjectClass(thiz);//两种方式获取 class
    jfieldID id = env ->GetFieldID( classMainActivity, "number", "I");
    env ->SetIntField(thiz,id,2);
}
//extern "C"
//JNIEXPORT void JNICALL
////Java_com_example_as_1jni_1project_MainActivity_changeStaticNum(JNIEnv *env, jobject thiz) {
//Java_com_example_as_1jni_1project_MainActivity_changeStaticNum(JNIEnv *env, jclass thiz) {
////    jclass jclassMactivity = env-> FindClass("com/example/as_jni_project/MainActivity");
//    jclass jclassMactivity =env->GetObjectClass(thiz);//两种方式获取 class
//    jfieldID id = env ->GetStaticFieldID(jclassMactivity,"num","Ljava/lang/String;");
//    jstring content = env->NewStringUTF("efgh");
//    env->SetStaticObjectField(jclassMactivity,id,content);
//}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_changeStaticNum(JNIEnv *env, jclass clazz) {
    jfieldID id = env ->GetStaticFieldID(clazz,"num","Ljava/lang/String;");
    jstring content = env->NewStringUTF("efgh");
    env->SetStaticObjectField(clazz,id,content);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_addMethod(JNIEnv *env, jobject thiz) {

    jclass mactivityCls = env ->GetObjectClass(thiz);
    jmethodID addID = env ->GetMethodID(mactivityCls,"add","(II)I");
    env ->CallIntMethod(thiz,addID,1,2);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_printfArr(JNIEnv *env, jobject thiz, jintArray aar) {

    jint *jarr = env ->GetIntArrayElements(aar, nullptr);

    jint length = env ->GetArrayLength(aar);

    for (int i = 0; i < length; ++i) {
        LOGD("jni get %d value before= %d",i, jarr[i])
//            jarr[i] = (i+1000);
            *(jarr + i) = (i+2000);
        LOGD("jni get %d value after= %d",i, jarr[i])
    }

    env ->ReleaseIntArrayElements(aar,jarr,0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_changeContent(JNIEnv *env, jobject thiz,
                                                             jobjectArray aar) {
    jint length = env ->GetArrayLength(aar);
    for (int i = 0; i < length; ++i) {
        auto str = (jstring)env->GetObjectArrayElement(aar, i);
        const char *cha = env ->GetStringUTFChars(str, nullptr);
        LOGD("从 java 读取到的字符 %s \n",cha );
        env->ReleaseStringUTFChars(str,cha);

        jstring newStringUtf = env -> NewStringUTF("h");
        env ->SetObjectArrayElement(aar,i,newStringUtf);

        auto strNew = (jstring)env->GetObjectArrayElement(aar, i);
        const char *chaNew = env ->GetStringUTFChars(strNew, nullptr);
        LOGD("从 java 读取到的字符 %s \n",chaNew )
        env->ReleaseStringUTFChars(strNew,chaNew);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_getStudent(JNIEnv *env, jobject thiz,
                                                          jobject student) {
    jclass studentCls = env->GetObjectClass(student);

    jmethodID studentGetNameId= env->GetMethodID(studentCls,"getName","()Ljava/lang/String;");
    jmethodID studentSetNameId= env->GetMethodID(studentCls,"setName","(Ljava/lang/String;)V");
    jmethodID studentGetAgeId= env->GetMethodID(studentCls,"getAge","()I");
    jmethodID studentSetAgeId= env->GetMethodID(studentCls,"setAge","(I)V");
    if (studentGetNameId == nullptr
        || studentSetNameId == nullptr
        || studentGetAgeId == nullptr
        || studentSetAgeId == nullptr) {
        LOGD("getName method not found");
        return;
    }
//    get student name
    auto  nameStr = (jstring)env->CallObjectMethod(student,studentGetNameId);
    const char *nameStrName = env->GetStringUTFChars(nameStr,nullptr);
    LOGD("get student name : %s \n" , nameStrName)
//    set student name
    jstring newName = env ->NewStringUTF("new name");

    env->CallVoidMethod(student,studentSetNameId,newName);
    env->DeleteLocalRef(newName);
    auto  nameStr1 = (jstring)env->CallObjectMethod(student,studentGetNameId);
    const char *namestrNem1 = env->GetStringUTFChars(nameStr1,nullptr);
    LOGD("set student name : %s \n" , namestrNem1)
//  get student age
    jint age = env->CallIntMethod(student,studentGetAgeId);
    LOGD("get student age : %d \n" , age)

//  set student age
    env->CallVoidMethod(student,studentSetAgeId,20);
    jint setAge = env->CallIntMethod(student,studentGetAgeId);
    LOGD("set student setAge : %d \n" , setAge)

    env ->ReleaseStringUTFChars(nameStr1,namestrNem1);
    namestrNem1 = nullptr;
    env->DeleteLocalRef(nameStr1);
    nameStr1 = nullptr;
    env ->ReleaseStringUTFChars(nameStr,nameStrName);
    nameStrName = nullptr;
    env->DeleteLocalRef(nameStr);
    nameStr = nullptr;
    env->DeleteLocalRef(studentCls);
    studentCls = nullptr;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_initObject(JNIEnv *env, jobject thiz) {

    // 1. 将所有后续需要释放的局部变量提前声明并初始化为 nullptr
    jclass personCls = nullptr;
    jclass studentCls = nullptr;
    jobject personObj = nullptr;
    jobject studentObj = nullptr;

    // 至于 ID 类型，通常不需要在最后 DeleteLocalRef，但为了作用域统一，也可以提上来
    jmethodID personId = nullptr;
    jmethodID studentId = nullptr;
    jmethodID setNameId = nullptr;
    jmethodID setAgeId = nullptr;
    jmethodID personSetId = nullptr;
    jmethodID toStringId = nullptr;

    jstring toString = nullptr;
    jstring newName = nullptr;

    const char *charToString = nullptr;

    personCls = env->FindClass("com/example/as_jni_project/Person");

    if(!personCls) goto end;

    studentCls = env->FindClass("com/example/as_jni_project/Student");

    if(!studentCls) goto end;

    personId = env->GetMethodID(personCls,"<init>", "()V");
    if(!personId) goto end;
     personObj = env->NewObject(personCls,personId);
    if(!personObj) goto end;
    studentId = env->GetMethodID(studentCls,"<init>", "()V");
    if(!studentId) goto end;
    studentObj = env->NewObject(studentCls,studentId);
    if(!studentObj) goto end;
    setNameId = env->GetMethodID(studentCls,"setName", "(Ljava/lang/String;)V");
    if(!setNameId) goto end;
    setAgeId = env->GetMethodID(studentCls,"setAge", "(I)V");
    if(!setAgeId) goto end;
    //设置 student 属性值
    newName = env ->NewStringUTF("哈喽");
    env->CallVoidMethod(studentObj,setNameId,newName);
    env->DeleteLocalRef(newName);
    env->CallVoidMethod(studentObj,setAgeId,100);

    personSetId = env->GetMethodID(personCls,"setStudent","(Lcom/example/as_jni_project/Student;)V");
    if(!personSetId) goto end;
    env->CallVoidMethod(personObj,personSetId,studentObj);

    toStringId = env->GetMethodID(personCls,"toString","()Ljava/lang/String;");
    if(!toStringId) goto end;

    toString = (jstring)env->CallObjectMethod(personObj,toStringId);
    if(!toString) goto end;
    charToString = env->GetStringUTFChars(toString, nullptr);
    LOGD("c++ 创建新对象 ：%s" , charToString)

    env->ReleaseStringUTFChars(toString,charToString);

    end:
        if(toString)env->DeleteLocalRef(toString);
        if(studentObj)env->DeleteLocalRef(studentObj);
        if(studentCls)env->DeleteLocalRef(studentCls);
        if(personObj)env->DeleteLocalRef(personObj);
        if(personCls)env->DeleteLocalRef(personCls);

}


void initDynamic(JNIEnv *env, jobject thiz){
    LOGD("动态注册 c++")
}

/*
typedef struct {
    const char* name;
    const char* signature;
    void*       fnPtr;
} JNINativeMethod;
*/

static JNINativeMethod methods[] = {
        {"initDynamic","()V",(void *)(initDynamic)}
};


JavaVM *vm;

jint JNI_OnLoad(JavaVM *jm , void *args){

    ::vm = jm;
    JNIEnv *env = nullptr;
    jint result;
    result = jm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    if(result != JNI_OK){
        return -1;
    }
    jclass mactivityCls = env->FindClass("com/example/as_jni_project/MainActivity");
    result = env->RegisterNatives(mactivityCls,methods,sizeof methods /sizeof(JNINativeMethod) );
    if(result){
        LOGD("注册失败了")
    }
    return JNI_VERSION_1_6;

}

class MyContext{
public:
    jobject instance = nullptr;
};


void* threadChild(void* args){
    LOGD("C++ Pthread 的 异步线程 启动啦")
    auto *myContext = static_cast<MyContext *>(args);
    JNIEnv *env = nullptr;
    jint r = ::vm->AttachCurrentThread(&env, nullptr);
    if (r) { // 非0 true 失败
        LOGD("C++ Pthread 的 异步线程 失败")
        return nullptr;
    }
    jclass activityCls = env->GetObjectClass(myContext->instance);
    jmethodID updateId = env->GetMethodID(activityCls,"updateActivityUI","()V");
    env->CallVoidMethod(myContext->instance,updateId);

    env->DeleteLocalRef(activityCls);
//    env->DeleteGlobalRef(myContext->instance);
    ::vm->DetachCurrentThread();
//    delete myContext;
//    myContext = nullptr;
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_naitveThread(JNIEnv *env, jobject thiz) {

    auto *myContext = new MyContext;

    myContext ->instance = env->NewGlobalRef(thiz);

    pthread_t pthread;

    pthread_create(&pthread, nullptr, threadChild, myContext);

    pthread_join(pthread, nullptr);

    env->DeleteGlobalRef(myContext->instance);

    delete myContext;
    myContext = nullptr;
}

YiGePlay * mYiGePlay = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity2_nativeFun5(JNIEnv *env, jobject thiz) {
    // TODO: implement nativeFun5()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity2_nativeFun6(JNIEnv *env, jclass clazz) {
    // TODO: implement nativeFun6()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_YiGePlay_onNativeStartPlay(JNIEnv *env, jobject thiz) {
    if(mYiGePlay){
        mYiGePlay->startPlay();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_YiGePlay_onNativeStopPlay(JNIEnv *env, jobject thiz) {
    // TODO: implement onNativeStopPlay()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_YiGePlay_onNativeReleasePlay(JNIEnv *env, jobject thiz) {
    // TODO: implement onNativeReleasePlay()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_YiGePlay_onNativeSetSource(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *data_source_ = env->GetStringUTFChars(data_source,nullptr);
    auto *jniCallBackHelper = new JNiCallBackHelper(vm,env,thiz);
    mYiGePlay = new YiGePlay(data_source_,jniCallBackHelper);
    mYiGePlay->prepare();
    env->ReleaseStringUTFChars(data_source,data_source_);
}