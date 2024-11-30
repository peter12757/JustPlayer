

SET(LIBNAME CPPTest)

message("ANDROID_ABI : ${ANDROID_ABI}")

aux_source_directory(. SRC)

add_library(${LIBNAME} SHARED ${SRC})

target_include_directories(${LIBNAME} PUBLIC
        ./)

find_library( # 定位 Android 特定的日志支持库并将其路径存储在 log-lib 中
        log-lib
        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log )

target_link_libraries(${LIBNAME}
        ${log-lib})



