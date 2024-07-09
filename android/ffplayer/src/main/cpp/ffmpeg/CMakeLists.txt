#request project define
cmake_minimum_required(VERSION 3.18.0)

SET(LIBNAME FFPlayer)
project(${LIBNAME})


SET(FFLIB_DIR ${CMAKE_SOURCE_DIR}/../../../libs)
message("FFLIB_DIR : ${FFLIB_DIR}")

set(AVCODEC  libavcodec)
add_library(${AVCODEC} SHARED IMPORTED)
set_target_properties(${AVCODEC}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavcodec.so)
target_include_directories(${AVCODEC} INTERFACE
        ffmpeginc/libavcodec
        )


set(AVDEVICE  libavdevice)
add_library(${AVDEVICE} SHARED IMPORTED)
set_target_properties(${AVDEVICE}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavdevice.so)
target_include_directories(${AVDEVICE} INTERFACE ffmpeginc/libavdevice)

set(AVFILTER libavfilter)
add_library(${AVFILTER} SHARED IMPORTED)
set_target_properties(${AVFILTER}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavfilter.so)
target_include_directories(${AVFILTER} INTERFACE ffmpeginc/libavfilter)

set(AVFORMAT libavformat)
add_library(${AVFORMAT} SHARED IMPORTED)
set_target_properties(${AVFORMAT}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavformat.so)
target_include_directories(${AVFORMAT} INTERFACE ffmpeginc/libavformat)

set(AVUTIL libavutil)
add_library(${AVUTIL} SHARED IMPORTED)
set_target_properties(${AVUTIL}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libavutil.so)
target_include_directories(${AVUTIL} INTERFACE ffmpeginc/libavutil)

set(SWRESAMPLE libswresample)
add_library(${SWRESAMPLE} SHARED IMPORTED)
set_target_properties(${SWRESAMPLE}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libswresample.so)
target_include_directories(${SWRESAMPLE} INTERFACE ffmpeginc/libswresample)

set(SWSCALE libswscale)
add_library(${SWSCALE} SHARED IMPORTED)
set_target_properties(${SWSCALE}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/${ANDROID_ABI}/libswscale.so)
target_include_directories(${SWSCALE} INTERFACE ffmpeginc/libswscale)


AUX_SOURCE_DIRECTORY(player FFlayerSrc)
AUX_SOURCE_DIRECTORY(player/data FFlayerSrc)

#audio c
#aux_source_directory(player/audio FFlayerSrc)
aux_source_directory(player/decodec FFlayerSrc)


#aux_source_directory(../net FFlayerSrc)

add_library(${LIBNAME} SHARED ${FFlayerSrc})

target_include_directories(${LIBNAME} PUBLIC
        ../base/const
        ../base/statemachine
        ../base/thread
        ../base/util
        ../base/log
        ../base/protocol
        player
        player/data
        player/audio
        player/decodec)

target_include_directories(${LIBNAME} PRIVATE
        ${AVCODEC}
        ${AVDEVICE}
        ${AVFILTER}
        ${AVFORMAT}
        ${AVUTIL}
        ${SWRESAMPLE}
        ${SWSCALE}
        player
        player/data
        player/audio
        player/decodec)




MESSAGE("ffplayer dir : ${CMAKE_CURRENT_SOURCE_DIR}")


target_link_libraries(${LIBNAME} PRIVATE
        ${JustBase}
        ${AVCODEC}
        ${AVDEVICE}
        ${AVFILTER}
        ${AVFORMAT}
        ${AVUTIL}
        ${SWRESAMPLE}
        ${SWSCALE}
        log
)



