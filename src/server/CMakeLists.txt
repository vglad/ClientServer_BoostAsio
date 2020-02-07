cmake_minimum_required (VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES Server.cpp main.cpp)

add_definitions(BOOST_ALL_NO_LIB)
set(PLATFORM_LIBRARIES "" CACHE STRING "Set the platform libraries for your build environment")
add_executable(server ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)

target_link_libraries(server PRIVATE ${PLATFORM_LIBRARIES})

if(MINGW)
  target_link_libraries(server PRIVATE ws2_32)
endif()

target_include_directories(server PRIVATE
  ../../thirdparty
  ../../include/detail
  ../../include/lib
)