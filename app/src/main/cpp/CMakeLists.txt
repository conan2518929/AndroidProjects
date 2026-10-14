# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.

cmake_minimum_required(VERSION 3.22.1)

project("as_jni_project")

# 库目录（静态库 .a 所在）
set(FFMPEG ${CMAKE_CURRENT_LIST_DIR}/ffmpeg)
set(RTMP   ${CMAKE_CURRENT_LIST_DIR}/rtmp)

# 头文件目录
include_directories(${FFMPEG}/include)
include_directories(${RTMP}/include)

# 当前 ABI 对应的库目录
set(FFMPEG_LIB_DIR ${FFMPEG}/libs/${CMAKE_ANDROID_ARCH_ABI})
set(RTMP_LIB_DIR   ${RTMP}/libs/${CMAKE_ANDROID_ARCH_ABI})

# ---- 静态库（.a）必须用 IMPORTED 方式手动声明 ----
add_library(avcodec    STATIC IMPORTED)
add_library(avformat   STATIC IMPORTED)
add_library(avfilter   STATIC IMPORTED)
add_library(avutil     STATIC IMPORTED)
add_library(swresample STATIC IMPORTED)
add_library(swscale    STATIC IMPORTED)

set_target_properties(avcodec    PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.a)
set_target_properties(avformat   PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavformat.a)
set_target_properties(avfilter   PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavfilter.a)
set_target_properties(avutil     PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.a)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.a)
set_target_properties(swscale    PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.a)

add_library(rtmp    STATIC IMPORTED)
add_library(ssl     STATIC IMPORTED)
add_library(crypto  STATIC IMPORTED)

set_target_properties(rtmp   PROPERTIES IMPORTED_LOCATION ${RTMP_LIB_DIR}/librtmp.a)
set_target_properties(ssl    PROPERTIES IMPORTED_LOCATION ${RTMP_LIB_DIR}/libssl.a)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${RTMP_LIB_DIR}/libcrypto.a)

# 收集当前目录下所有 .cpp
file(GLOB file_resouse ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED ${file_resouse})

# 链接顺序很重要：被依赖的库放在后面
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        z
        OpenSLES

        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil

        rtmp
        ssl
        crypto
)
