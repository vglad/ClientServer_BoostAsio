cmake_minimum_required (VERSION 3.12)
project(ClientServer)

if (MSVC)
    # warning level 4 and all warnings as errors
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(thirdparty/catch2)
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(tests)