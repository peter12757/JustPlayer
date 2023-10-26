#request project define
cmake_minimum_required(VERSION 3.18.0)

set(LIBNAME JustBase)
project(${LIBNAME})

#使用配置的头文件，否则会使用默认的。
add_definitions(-DHAVE_CONFIG_H)

#src
aux_source_directory(. JustBase_SRC)
aux_source_directory(const JustBase_SRC)
#aux_source_directory(cpptest JustBase_SRC)
aux_source_directory(data JustBase_SRC)
aux_source_directory(net JustBase_SRC)
aux_source_directory(util JustBase_SRC)
aux_source_directory(util/protocol JustBase_SRC)
aux_source_directory(util/statemachine JustBase_SRC)
aux_source_directory(util/thread JustBase_SRC)

#build
add_library(${LIBNAME}  SHARED
        ${JustBase_SRC}
        )

#include
target_include_directories(${LIBNAME} PUBLIC
        ${JustBase_SRC}
        )