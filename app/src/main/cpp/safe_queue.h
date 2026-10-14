//
// Created by yige on 2026/10/12.
//

#ifndef AS_JNI_PROJECT_SAFE_QUEUE_H
#define AS_JNI_PROJECT_SAFE_QUEUE_H

#include <queue>
#include <pthread.h>

using namespace std;

template<typename T>

class Safe_queue {
private:
    typedef void(*ReleaseCallback)(T *);
private:
    queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int work{};
    ReleaseCallback releaseCallback;
public:
    Safe_queue(){
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }
    ~Safe_queue(){
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void insertToQueue(T value){
        pthread_mutex_lock(&mutex);

        if(work){
            queue.push(value);
            pthread_cond_signal(&cond);
        }else{
            if(releaseCallback){
                releaseCallback(&value);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    int getQueueAndDel(T &value){
        int ret = 0;
        pthread_mutex_lock(&mutex);

        while (work && queue.empty()){
            pthread_cond_wait(&cond, &mutex);
        }


        if(!queue.empty()){
            value = queue.front();
            queue.pop();
            ret = 1;
        }

        pthread_mutex_unlock(&mutex);
        return ret;
    }


    void setWork(int work){
        pthread_mutex_lock(&mutex);
        this->work = work;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int empty(){
        return queue.empty();
    }

    int size(){
        return queue.size();
    }
    
    void clear(){
        pthread_mutex_lock(&mutex);
        int size = queue.size();
        for (int i = 0; i < size; ++i) {
            T value = queue.front();
            if(releaseCallback){
                releaseCallback(&value);
            }
            queue.pop();
        }
        pthread_mutex_unlock(&mutex);
    }

    void setReleaseCallback(ReleaseCallback callback){
        this->releaseCallback = callback;
    }
};


#endif //AS_JNI_PROJECT_SAFE_QUEUE_H
