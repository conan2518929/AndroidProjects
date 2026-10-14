//
// Created by yige on 2026/10/12.
//

#ifndef AS_JNI_PROJECT_BASECHANNEL_H
#define AS_JNI_PROJECT_BASECHANNEL_H
extern "C"{
#include <libavcodec/avcodec.h>
}
#include "safe_queue.h"

class BaseChannel{
public:
    Safe_queue<AVPacket*> avPackts;
    Safe_queue<AVFrame*> avFrames;
    bool isPlaying;
    AVCodecContext *codecContext = nullptr;
    int stream_index;

    static void releaseAVPack(AVPacket **avPacket){
        if(avPacket){
            av_packet_free(avPacket);
            *avPacket = nullptr;
        }
    }
    static void releaseAVFrame(AVFrame **avFrames){
        if(avFrames){
            av_frame_free(avFrames);
            *avFrames = nullptr;
        }
    }
    BaseChannel(int stream_index, AVCodecContext *avCodecContext)
    :
    stream_index(stream_index),
    codecContext(avCodecContext){
        avPackts.setReleaseCallback(&releaseAVPack);
        avFrames.setReleaseCallback(&releaseAVFrame);
    }

    virtual ~BaseChannel(){

    }



};
#endif //AS_JNI_PROJECT_BASECHANNEL_H
