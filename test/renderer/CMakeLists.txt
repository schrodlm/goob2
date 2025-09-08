add_executable(test_goob_renderer test_goob.cpp)
target_link_libraries(test_goob_renderer PRIVATE goob_renderer Catch2::Catch2WithMain)

# Register tests with CTest
include(Catch)
catch_discover_tests(test_goob_renderer)