cmake_minimum_required (VERSION 3.12)
project(ClientServer)

add_subdirectory(thirdparty/catch2)
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(tests)