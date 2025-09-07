cmake_minimum_required(VERSION 3.10)
project(Goob VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GENERATED_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)

file(MAKE_DIRECTORY ${GENERATED_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(sandbox)


