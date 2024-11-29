SET(FFLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/win64/lib)
SET(FFINCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/win64/include)
# message("LIBRARY_OUTPUT_PATH : ${LIBRARY_OUTPUT_PATH}")


message("FFLIB_DIR : ${FFLIB_DIR}")
message("FFINCLUDE_DIR : ${FFINCLUDE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
# message("LIBRARY_OUTPUT_PATH : ${LIBRARY_OUTPUT_PATH}")


add_library(${AVCODEC} STATIC IMPORTED)
set_target_properties(${AVCODEC}    PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libavcodec.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/avcodec.lib)
# target_include_directories(${AVCODEC} INTERFACE ${FFINCLUDE_DIR}/libavcodec)

add_library(${AVDEVICE} STATIC IMPORTED)
set_target_properties(${AVDEVICE}   PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libavdevice.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/avdevice.lib)
target_include_directories(${AVDEVICE} INTERFACE ${FFINCLUDE_DIR}/libavdevice)

add_library(${AVFILTER} STATIC IMPORTED)
set_target_properties(${AVFILTER}   PROPERTIES
            IMPORTED_LOCATION  ${FFLIB_DIR}/libavfilter.dll.a
            IMPORTED_IMPLIB     ${FFLIB_DIR}/avfilter.lib)
# target_include_directories(${AVFILTER} INTERFACE ${FFINCLUDE_DIR}/libavfilter)


add_library(${AVFORMAT} STATIC IMPORTED)
set_target_properties(${AVFORMAT} PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libavformat.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/avformat.lib)
# target_include_directories(${AVFORMAT} INTERFACE ${FFINCLUDE_DIR}/libavformat)




add_library(${AVUTIL} STATIC IMPORTED)
set_target_properties(${AVUTIL} PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libavutil.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/avutil.lib)
# target_include_directories(${AVUTIL} INTERFACE ${FFINCLUDE_DIR}/libavutil)


add_library(${SWRESAMPLE} STATIC IMPORTED)
set_target_properties(${SWRESAMPLE} PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libswresample.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/swresample.lib)
# target_include_directories(${SWRESAMPLE} INTERFACE ${FFINCLUDE_DIR}/libswresample)


add_library(${SWSCALE} STATIC IMPORTED)
set_target_properties(${SWSCALE}    PROPERTIES
    IMPORTED_LOCATION   ${FFLIB_DIR}/libswscale.dll.a
    IMPORTED_IMPLIB     ${FFLIB_DIR}/swscale.lib)
# target_include_directories(${SWSCALE} INTERFACE ${FFINCLUDE_DIR}/libswscale)

target_include_directories(${SWRESAMPLE} INTERFACE ${FFINCLUDE_DIR})
