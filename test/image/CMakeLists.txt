add_executable(test_goob_image test_tga_image.cpp)
target_compile_definitions(test_goob_image PRIVATE ASSETS_DIR="${ASSETS_PATH}")
target_link_libraries(test_goob_image PRIVATE goob_image Catch2::Catch2WithMain)

# Register tests with CTest
include(Catch)
catch_discover_tests(test_goob_image)