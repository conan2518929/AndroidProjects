//
// Created by yige on 2026/10/10.
//

#ifndef AS_JNI_PROJECT_AUDIOCHANNEL_H
#define AS_JNI_PROJECT_AUDIOCHANNEL_H


#include "BaseChannel.h"

class AudioChannel : public BaseChannel{

public:
    AudioChannel(int stream_index, AVCodecContext *avCodecContext);
    ~AudioChannel();

    void start();

    void stop();
};


#endif //AS_JNI_PROJECT_AUDIOCHANNEL_H
