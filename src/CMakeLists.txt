add_subdirectory(renderer)
add_subdirectory(image)

add_library(goob INTERFACE)
target_link_libraries(goob INTERFACE goob_renderer goob_image)