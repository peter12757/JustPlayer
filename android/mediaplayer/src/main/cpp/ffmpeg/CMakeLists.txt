#request project define
cmake_minimum_required(VERSION 3.18.0)

SET(LIBNAME FFPlayer)
project(${LIBNAME})

SET(FFLIB_DIR ${CMAKE_SOURCE_DIR}/../../../libs)
message("FFLIB_DIR : ${FFLIB_DIR}")

set(FFMPEG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeginc)
message("CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")

# 导入动态库
add_library(avcodec SHARED IMPORTED)
add_library(avformat SHARED IMPORTED)
add_library(avutil SHARED IMPORTED)
add_library(avfilter SHARED IMPORTED)
add_library(swresample SHARED IMPORTED)
add_library(swscale SHARED IMPORTED)

set_target_properties(avcodec PROPERTIES
        IMPORTED_LOCATION ${FFLIB_DIR}/${ANDROID_ABI}/libavcodec.so
)
set_target_properties(avformat PROPERTIES
        IMPORTED_LOCATION ${FFLIB_DIR}/${ANDROID_ABI}/libavformat.so
)
set_target_properties(avutil PROPERTIES
        IMPORTED_LOCATION ${FFLIB_DIR}/${ANDROID_ABI}/libavutil.so
)
set_target_properties(avfilter
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavfilter.so)

set_target_properties(swresample
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libswresample.so)

set_target_properties(swscale
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libswscale.so)

AUX_SOURCE_DIRECTORY(player FFlayerSrc)
AUX_SOURCE_DIRECTORY(player/data FFlayerSrc)
AUX_SOURCE_DIRECTORY(player/stream FFlayerSrc)
AUX_SOURCE_DIRECTORY(player/config FFlayerSrc)


#audio c
#aux_source_directory(player/audio FFlayerSrc)
#aux_source_directory(player/decodec FFlayerSrc)


#aux_source_directory(../net FFlayerSrc)

add_library(${LIBNAME} SHARED ${FFlayerSrc})

target_include_directories(${LIBNAME} PUBLIC
        player)
message("FFMPEG_INCLUDE_DIR : ${FFMPEG_INCLUDE_DIR}")
target_include_directories(${LIBNAME} PRIVATE
        ${FFMPEG_INCLUDE_DIR}
        ../base
        player)





target_link_libraries(${LIBNAME} PRIVATE
        avformat
        avcodec
        avutil
        avfilter
        swresample
        swscale
        ${JustBase}
        log
        android
)



