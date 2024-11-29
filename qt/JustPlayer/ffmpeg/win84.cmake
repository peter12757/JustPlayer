SET(FFLIB_DIR win64/lib)
SET(FFINCLUDE_DIR win64/include)
message("LIBRARY_OUTPUT_PATH : ${LIBRARY_OUTPUT_PATH}")
SET(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/lib)
message("FFLIB_DIR : ${FFLIB_DIR}")
message("FFINCLUDE_DIR : ${FFINCLUDE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("LIBRARY_OUTPUT_PATH : ${LIBRARY_OUTPUT_PATH}")

set(AVCODEC  libavcodec)
add_library(${AVCODEC} SHARED IMPORTED)
set_target_properties(${AVCODEC}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/avcodec-61.dll)
target_include_directories(${AVCODEC} INTERFACE
        ${FFINCLUDE_DIR}/libavcodec
        )


    set(AVDEVICE  libavdevice)
    add_library(${AVDEVICE} SHARED IMPORTED)
    set_target_properties(${AVDEVICE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avdevice-61.dll)
    target_include_directories(${AVDEVICE} INTERFACE ${FFINCLUDE_DIR}/libavdevice)

    set(AVFILTER libavfilter)
    add_library(${AVFILTER} SHARED IMPORTED)
    set_target_properties(${AVFILTER}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avfilter-10.dll)
    target_include_directories(${AVFILTER} INTERFACE ${FFINCLUDE_DIR}/libavfilter)


    message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
    set(AVFORMAT libavformat)
    add_library(${AVFORMAT} STATIC IMPORTED)
    set_target_properties(${AVFORMAT} PROPERTIES
        IMPORTED_LOCATION   ${FFLIB_DIR}/libavformat.dll.a
        IMPORTED_IMPLIB     ${FFLIB_DIR}/avformat.lib)
    target_include_directories(${AVFORMAT} INTERFACE ${FFINCLUDE_DIR}/libavformat)
    message("AVFORMAT: ${AVFORMAT}")

    set(AVUTIL libavutil)
    add_library(${AVUTIL} SHARED IMPORTED)
    set_target_properties(${AVUTIL}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avutil-59.dll)
    target_include_directories(${AVUTIL} INTERFACE ${FFINCLUDE_DIR}/libavutil)

    set(SWRESAMPLE libswresample)
    add_library(${SWRESAMPLE} SHARED IMPORTED)
    set_target_properties(${SWRESAMPLE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/swresample-5.dll)
    target_include_directories(${SWRESAMPLE} INTERFACE ${FFINCLUDE_DIR}/libswresample)

    set(SWSCALE libswscale)
    add_library(${SWSCALE} SHARED IMPORTED)
    set_target_properties(${SWSCALE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/swscale-8.dll)
    target_include_directories(${SWSCALE} INTERFACE ${FFINCLUDE_DIR}/libswscale)
