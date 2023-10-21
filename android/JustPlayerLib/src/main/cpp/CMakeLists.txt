# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("JustApi")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 99)


include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()
add_definitions(-D_ANDROID)
set(CMAKE_C_FLAGS_RELEASE       "${CMAKE_C_FLAGS_RELEASE}   -fvisibility=hidden")
set(CMAKE_CXX_FLAGS_RELEASE     "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility=hidden")
set(CMAKE_C_FLAGS_RELEASE       "${CMAKE_C_FLAGS_RELEASE}   -fvisibility-inlines-hidden")
set(CMAKE_CXX_FLAGS_RELEASE     "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility-inlines-hidden")

set(CMAKE_C_FLAGS_RELEASE       "${CMAKE_C_FLAGS_RELEASE}   -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_RELEASE     "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS_RELEASE     "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections -fdata-sections")

set(CMAKE_MODULE_LINKER_FLAGS   "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--gc-sections")
set(CMAKE_SHARED_LINKER_FLAGS   "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")


add_subdirectory(JUSTPlayer)
add_subdirectory(cpptest)



add_library(JustApi SHARED
        JustApi.cpp
        )

target_include_directories(JustApi PRIVATE
        justplayer/player
        justplayer/player/data
        cpptest
        )

find_library( # 定位 Android 特定的日志支持库并将其路径存储在 log-lib 中
        log-lib
        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log )

target_link_libraries(JustApi
        JUSTPlayer
        CPPTest
        ${log-lib}
)


