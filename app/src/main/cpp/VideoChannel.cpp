//
// Created by yige on 2026/10/10.
//

#include "VideoChannel.h"
#include <android/log.h>
#define LOG_TAG "MY_JNI_LOG"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);

VideoChannel::VideoChannel(int stream_index, AVCodecContext *avCodeContext)
: BaseChannel(stream_index,avCodeContext)
{

}
VideoChannel::~VideoChannel(){

}

void* task_video_decode(void *args){
    auto *videoChannel = static_cast<VideoChannel*>(args);
    videoChannel->video_decode();
    return nullptr;
}

void* task_video_play(void *args){
    auto *videoChannel = static_cast<VideoChannel*>(args);
    videoChannel->video_play();
    return nullptr;
}

void VideoChannel::start() {
    isPlaying = true;
    avPackts.setWork(1);
    avFrames.setWork(1);
    pthread_create(&pid_video_decode, nullptr,task_video_decode,this);
    pthread_create(&pid_video_play, nullptr,task_video_play,this);
}


void VideoChannel::video_decode() {
    AVPacket *avPacket = nullptr;
    while (isPlaying){
        int ret = avPackts.getQueueAndDel(avPacket);

        if(!isPlaying){
            break;
        }

        if(!ret){
            continue;
        }
        ret = avcodec_send_packet(codecContext,avPacket);
        releaseAVPack(&avPacket);
        if(ret){
            break;
        }
        AVFrame *avFrame = av_frame_alloc();
        if (!avFrame) {
            break;
        }
        ret = avcodec_receive_frame(codecContext,avFrame);
        if(ret == AVERROR(EAGAIN)){
            av_frame_free(&avFrame);
            continue;
        }else if(ret != 0){
            av_frame_free(&avFrame);
            break;
        }
        avFrames.insertToQueue(avFrame);
    }
    releaseAVPack(&avPacket);
}

void VideoChannel::video_play() {
    AVFrame * avFrame = nullptr;
    uint8_t *dst_data[4] = {};
    int dst_linesize[4] = {};
    int buffer_size = av_image_alloc(dst_data,dst_linesize,
                                     codecContext->width,codecContext->height,
                                     AV_PIX_FMT_RGBA,1);
    if (buffer_size < 0) {
        return;
    }
    SwsContext *sws_ctx = nullptr;
    while (isPlaying){
        int ret = avFrames.getQueueAndDel(avFrame);
        if(!isPlaying){
            av_frame_free(&avFrame);
            break;
        }
        if(!ret){
            continue;
        }
        sws_ctx = sws_getCachedContext(sws_ctx,
                avFrame->width, avFrame->height,
                static_cast<AVPixelFormat>(avFrame->format),
                codecContext->width, codecContext->height, AV_PIX_FMT_RGBA,
                SWS_BILINEAR, nullptr, nullptr, nullptr);
        if (!sws_ctx) {
            av_frame_free(&avFrame);
            break;
        }
        //YUV转换 RGBA
        sws_scale(sws_ctx,avFrame->data,avFrame->linesize,0,avFrame->height,dst_data,dst_linesize);
        av_frame_free(&avFrame);
    }
    sws_freeContext(sws_ctx);
    av_freep(&dst_data[0]);
}

void VideoChannel::stop() {

}
