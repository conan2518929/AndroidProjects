//
// Created by yige on 2026/10/10.
//

#ifndef AS_JNI_PROJECT_VIDEOCHANNEL_H
#define AS_JNI_PROJECT_VIDEOCHANNEL_H

#include "BaseChannel.h"
extern "C"{
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class VideoChannel : public BaseChannel{
private:
    pthread_t pid_video_decode;
    pthread_t pid_video_play;
public:
    VideoChannel(int stream_index,AVCodecContext *avCodeContext);
    ~VideoChannel();

    void start();

    void video_decode();

    void video_play();

    void stop();
};


#endif //AS_JNI_PROJECT_VIDEOCHANNEL_H
