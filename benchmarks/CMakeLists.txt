set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(vector_ops_benchmarks
    vector_ops_benchmarks.cpp
)

target_link_libraries(vector_ops_benchmarks
    PRIVATE
        accelerated_computing
        benchmark::benchmark_main
)
