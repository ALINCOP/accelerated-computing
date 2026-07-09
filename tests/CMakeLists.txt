FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(vector_ops_tests
    vector_ops_tests.cpp
)

target_link_libraries(vector_ops_tests
    PRIVATE
        accelerated_computing
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vector_ops_tests)
