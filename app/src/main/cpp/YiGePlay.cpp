//
// Created by yige on 2026/10/9.
//

#include "YiGePlay.h"
#include <android/log.h>
#define LOG_TAG "MY_JNI_LOG"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);

YiGePlay::YiGePlay(const char *data_source, JNiCallBackHelper *jniCallBackHelper){
    this->data_source = new char[strlen(data_source)+1];
    strcpy(this->data_source, data_source);
    this->jniCallBackHelper = jniCallBackHelper;
}

YiGePlay::~YiGePlay(){
    if(data_source){
        delete[] data_source;
        data_source = nullptr;
    }
    if(jniCallBackHelper){
        delete jniCallBackHelper;
        jniCallBackHelper = nullptr;
    }
}

void* threadPrepare(void* args){
    auto *yigePlay = static_cast<YiGePlay*>(args);
    yigePlay->prepare_();
    return nullptr;
}

void YiGePlay::prepare() {

    pthread_create(&pid_prepare, nullptr, threadPrepare, this);

}

void YiGePlay::prepare_() {
    af_context = avformat_alloc_context();
    if(!af_context){
        //提示错误信息
        return;
    }
    int r = 0;
    AVDictionary *options = nullptr;
    av_dict_set(&options,"timeout","5000000", 0);
    r = avformat_open_input(&af_context,data_source, nullptr,&options);
    av_dict_free(&options);
    char error[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(r, error, sizeof(error));
    __android_log_print(ANDROID_LOG_ERROR, "YiGePlay",
                        "打开失败：路径=%s，错误=%d，原因=%s",
                        data_source, r, error);
    if(r){
        //提示错误信息
        return;
    }

    r = avformat_find_stream_info(af_context, nullptr);
    if(r < 0){
        //提示错误信息
        return;
    }
    for (int i = 0; i < af_context->nb_streams; ++i) {
        AVStream *streams = af_context->streams[i];
        AVCodecParameters *codecpar = streams->codecpar;

        const AVCodec * av_code = avcodec_find_decoder(codecpar->codec_id);
        AVCodecContext * av_code_context = avcodec_alloc_context3(av_code);
        if(!av_code_context){
            //提示错误信息
            return;
        }
        r = avcodec_parameters_to_context(av_code_context,codecpar);
        if(r < 0){
            //提示错误信息
            return;
        }
        r = avcodec_open2(av_code_context,av_code,nullptr);
        if(r){
            //提示错误信息
            return;
        }
        if(codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_AUDIO){
            LOGD("111111")
            audio_channel = new AudioChannel(i,av_code_context);
        }else if(codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO){
            LOGD("222222")
            video_channel = new VideoChannel(i,av_code_context);
        }
    }// for end
    if(!audio_channel && !video_channel){
        //提示错误信息
        return;
    }

    if(jniCallBackHelper){
        jniCallBackHelper->prepare(THREAD_CHILD);
    }

}

void* thread_start(void *args){
    auto *yigePlay = static_cast<YiGePlay*>(args);
    yigePlay->start_();
    return nullptr;
}

void YiGePlay::startPlay(){
    if (!af_context || (!video_channel && !audio_channel)) {
        return;
    }
    isPlaying = true;
    if(video_channel){
        video_channel->start();
    }
    if(audio_channel){
        audio_channel->start();
    }
    pthread_create(&pid_start, nullptr,thread_start,this);
}

void YiGePlay::start_() {
    while (isPlaying){
        AVPacket *avp = av_packet_alloc();
        if (!avp) {
            // 分配失败：结束读取循环
            break;
        }
        int ret = av_read_frame(af_context,avp);
        if(ret >= 0){
            if(video_channel && video_channel->stream_index == avp->stream_index){
                video_channel->avPackts.insertToQueue(avp);
            }else if(audio_channel && audio_channel->stream_index == avp->stream_index){
//                audio_channel->avPackts.insertToQueue(avp);
            }else {
                av_packet_free(&avp);
            }
        }else{
            av_packet_free(&avp);
            break;
        }

    }
    isPlaying = false;
    if(video_channel) video_channel->stop();
    if(audio_channel) audio_channel->stop();
}
