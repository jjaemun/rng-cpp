# --testing.

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)


set(
    directories
)

foreach(directory ${directories})

    # --mirroring...

    add_subdirectory(${directory})
endforeach()


add_executable(
    rngtests
    rngtests.cpp
)

target_link_libraries(
    rngtests PRIVATE
    GTest::gtest_main
    rng
)

gtest_discover_tests(rngtests)
