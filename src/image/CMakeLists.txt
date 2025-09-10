add_library(goob_image STATIC tga_image.cpp)

target_include_directories(goob_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(goob_image PUBLIC goob_vector)