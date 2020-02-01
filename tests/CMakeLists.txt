cmake_minimum_required (VERSION 3.12)
project(AllTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES
    ../src/client/Client.cpp
    ../src/server/Server.cpp
    )

set(TESTS_SOURCE_FILES testMain.cpp
    Client_Tests.cpp
    Server_Tests.cpp
)

add_library(libClientServer STATIC ${LIB_SOURCE_FILES})
add_executable (allTests ${TESTS_SOURCE_FILES})

if (NOT MSVC)
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)
  target_link_libraries(allTests PRIVATE Threads::Threads libClientServer catch2)
else()
  target_link_libraries(allTests PRIVATE libClientServer catch2)
endif()

target_include_directories(allTests PRIVATE
    ../thirdparty
    ../thirdparty/trompeloeil
    ../include/lib
    ../include/detail
)

target_include_directories(libClientServer PRIVATE
    ../thirdparty
    ../include/lib
    ../include/detail
    )

