//
// Created by yige on 2026/10/9.
//

#ifndef AS_JNI_PROJECT_YIGEPLAY_H
#define AS_JNI_PROJECT_YIGEPLAY_H

#include <string>
#include <pthread.h>
#include "AudioChannel.h"
#include "VideoChannel.h"
#include "JNiCallBackHelper.h"
#include "utils.h"
#include <cstring>

extern "C"{
    #include<libavformat/avformat.h>
    #include<libavcodec/avcodec.h>
}

class YiGePlay{
private:
    char* data_source = nullptr;
    pthread_t pid_prepare;
    pthread_t pid_start;
    AVFormatContext * af_context = nullptr;
    AudioChannel *audio_channel = nullptr;
    VideoChannel *video_channel = nullptr;
    JNiCallBackHelper *jniCallBackHelper = nullptr;
    bool isPlaying;
public:
    YiGePlay(const char *data_source, JNiCallBackHelper *jniCallBackHelper);
    ~YiGePlay();

    void prepare();

    void prepare_();

    void startPlay();

    void start_();
};
#endif //AS_JNI_PROJECT_YIGEPLAY_H
