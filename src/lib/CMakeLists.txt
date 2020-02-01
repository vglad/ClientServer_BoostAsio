cmake_minimum_required(VERSION 3.12)
project(LibClientServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    libClientServer.cpp
)

add_library(libClientServer STATIC ${SOURCE_FILES})
target_include_directories(libClientServer PRIVATE
    ../../thirdparty
    ../../include/detail
    ../../include/lib
)
