# --testing.

add_executable(
    canon_tests
    canon_tests.cpp
)

target_link_libraries(
    canon_tests PRIVATE
    GTest::gtest_main
    rng
)

gtest_discover_tests(canon_tests)
