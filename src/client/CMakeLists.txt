cmake_minimum_required (VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp)

add_executable(client ${SOURCE_FILES})

if (NOT MSVC)
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)
  target_link_libraries(client PRIVATE Threads::Threads libClientServer)
else()
  target_link_libraries(client PRIVATE libClientServer)
endif()

target_include_directories(client PRIVATE
    ../../include/detail
    ../../include/lib
    )