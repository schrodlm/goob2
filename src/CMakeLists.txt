add_subdirectory(renderer)
add_subdirectory(image)
add_subdirectory(vector)
add_subdirectory(color)
add_subdirectory(utils)


add_library(goob INTERFACE)
target_link_libraries(goob INTERFACE goob_renderer goob_image goob_vector goob_utils)