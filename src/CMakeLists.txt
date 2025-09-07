add_library(goob STATIC goob.cpp)
target_include_directories(goob PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)