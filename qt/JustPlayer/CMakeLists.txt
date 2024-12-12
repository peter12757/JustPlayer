cmake_minimum_required(VERSION 3.16)

project(JustPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-g -Wall -stdlib=libc++")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools OpenGLWidgets)


# ffmpeg var
set(AVCODEC  libavcodec)
set(AVDEVICE  libavdevice)
set(AVFILTER libavfilter)
set(AVFORMAT libavformat)
set(AVUTIL libavutil)
set(SWRESAMPLE libswresample)
set(SWSCALE libswscale)


# 使用子项目的话局部变量还是有一些问题的。
# add_subdirectory(ffmpeg)
set(TS_FILES JustPlayer_zh_CN.ts)



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)


file(GLOB_RECURSE SRC ./src/*.cpp ./src/*.h)    #file会把var中的变量重置，GLOB_RECURSE 会递归的把src文件中的所有文件都添加进去
file(GLOB_RECURSE TESTSRC ./src/test/*.cpp ./src/test/*.h)
message("SRC:${SRC}")



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    include(ff_win64.cmake)
    qt_add_executable(JustPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${SRC}
        ${TESTSRC}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET JustPlayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(JustPlayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(JustPlayer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(JustPlayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::OpenGLWidgets)

find_library(AVFILTER_PATH NAMES ${AVFILTER} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/win64/lib)
message("AVFILTER_PATH  ${AVFILTER_PATH}")
target_include_directories(JustPlayer PRIVATE
    ${FFINCLUDE_DIR}
    ./src/)

target_link_libraries(JustPlayer PRIVATE
    ${AVUTIL}
    ${AVCODEC}
    ${AVDEVICE}
    ${AVFILTER_PATH}
    ${AVFORMAT}
    ${SWRESAMPLE}
    ${SWSCALE}
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.JustPlayer)
endif()
set_target_properties(JustPlayer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS JustPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(JustPlayer)
endif()
