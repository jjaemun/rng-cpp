# --testing.

add_executable(
    xoshiro_tests
    xoshiro_tests.cpp
)

target_link_libraries(
    xoshiro_tests PRIVATE
    GTest::gtest_main
    rng
)

gtest_discover_tests(xoshiro_tests)
