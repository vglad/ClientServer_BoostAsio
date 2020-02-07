cmake_minimum_required (VERSION 3.12)
project(Client)

if (WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()

add_executable(client Client.cpp main.cpp)
set_target_properties(client PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES)

find_package(Threads)
target_link_libraries(client PRIVATE Threads::Threads)

if(MINGW)
  target_link_libraries(client PRIVATE ws2_32)
endif()

target_include_directories(client PRIVATE
  ../../thirdparty
  ../../include/detail
  ../../include/lib
)
