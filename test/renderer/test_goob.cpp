#include "goob.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "Test renderer", "[renderer]" ) {
    Goob goob = Goob();
    REQUIRE(goob.generate_hello() == "Hello World!");
}