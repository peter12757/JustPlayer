

SET(LIBNAME FFMPEGPlayer)

message("CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")

message("ANDROID_ABI : ${ANDROID_ABI}")
SET(FFLIB_DIR ${CMAKE_SOURCE_DIR}/../../../libs)

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


AUX_SOURCE_DIRECTORY(player FFMPEGPlayerSrc)
AUX_SOURCE_DIRECTORY(player/data FFMPEGPlayerSrc)

#audio
aux_source_directory(player/audio FFMPEGPlayerSrc)
aux_source_directory(player/decodec FFMPEGPlayerSrc)


#aux_source_directory(../net FFMPEGPlayerSrc)

add_library(${LIBNAME} SHARED ${FFMPEGPlayerSrc})

target_include_directories(${LIBNAME} PUBLIC
        player
        player/data
        ../base/const
        ../base/util/statemachine
        ../base/util/thread
        ../base/util
        player/audio
        player/decodec)

target_include_directories(${LIBNAME} PRIVATE
        ${AVCODEC}
        ${AVDEVICE}
        ${AVFILTER}
        ${AVFORMAT}
        ${AVUTIL}
        ${SWRESAMPLE}
        ${SWSCALE})




MESSAGE("ffplayer dir : ${CMAKE_CURRENT_SOURCE_DIR}")

find_library( # 定位 Android 特定的日志支持库并将其路径存储在 log-lib 中
        log-lib
        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log )

target_link_libraries(${LIBNAME}
        ${AVCODEC}
        ${AVDEVICE}
        ${AVFILTER}
        ${AVFORMAT}
        ${AVUTIL}
        ${SWRESAMPLE}
        ${SWSCALE}
        ${log-lib}
        ${JustBase})



